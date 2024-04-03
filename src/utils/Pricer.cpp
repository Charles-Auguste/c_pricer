#include "Pricer.h"
#include "Quadrature.h"
#include <cmath>
#include <vector>
#include <complex>
#include <numbers>
#include <algorithm>


// CDF Gaussian

double normalCDF(double x)
{
	return std::erfc(-x / std::sqrt(2)) / 2;
}


// Paramètres utiles pour Heston

const std::complex<double> i(0, 1);
const double pi = 2 * acos(0.0);

const std::vector<double> points = gauss_hermite_points(60);
const std::vector<double> weights = gauss_hermite_weights(60);


//// Classe Model

// Constructeurs

Model::Model():
	_S(100.),
	_r(0.05)
{}

Model::Model(const double& S, const double& r):

	_S(S),
	_r(r)
{}

Model::Model(const Model& model):

	_S(model._S),
	_r(model._r)
{}

// Opérateur d'assignation

Model& Model::operator=(const Model& model) {

	if (this != &model) {

		_S = model._S;
		_r = model._r;

	}
	return *this;
}


//// Classe HestonModel

// Constructeurs

HestonModel::HestonModel() 
	:	
		Model(),
		_kappa(1),
		_theta(0.2),
		_sigma(0.2),
		_rho(0),
		_v0(0.2)
{}


HestonModel::HestonModel(const double& kappa, const double& theta, const double& sigma, const double& rho, const double& v0, const double& S, const double& r)
	:
	
	Model(S,r),
	_kappa(kappa),
	_theta(theta),
	_sigma(sigma),
	_rho(rho),
	_v0(v0)
{};


HestonModel::HestonModel(const HestonModel& other)
	:
	
	Model(other),
	_kappa(other._kappa),
	_theta(other._theta),
	_sigma(other._sigma),
	_rho(other._rho),
	_v0(other._v0)
{};


// Operateur d'assignation


HestonModel& HestonModel::operator=(const HestonModel& model){

	if (this != &model) {

		Model::operator=(model);
		_kappa = model._kappa;
		_theta = model._theta;
		_sigma = model._sigma;
		_rho = model._rho;
		_v0 = model._v0;
	}
	return *this;
}

// Méthode clone

HestonModel* HestonModel::clone() const {

	return new HestonModel(*this);
}

// Getteurs 

double HestonModel::GetKappa() const {
	return _kappa;
}

double HestonModel::GetTheta() const {
	return _theta;
}

double HestonModel::GetSigma() const {
	return _sigma;
}

double HestonModel::GetRho() const {
	return _rho;
}

double HestonModel::GetV0() const  {
	return _v0;
}

double HestonModel::GetS() const {
	return _S;
}

double HestonModel::GetR() const {
	return _r;
}


// Setteurs


void HestonModel::SetKappa(const double& kappa) {
	_kappa=kappa;
}

void HestonModel::SetTheta(const double& theta) {
	_theta = theta;
}

void HestonModel::SetSigma(const double& sigma) {
	_sigma = sigma;
}

void HestonModel::SetRho(const double& rho) {
	_rho = rho;
}

void HestonModel::SetV0(const double& v0) {
	_v0 = v0;
}

void HestonModel::SetS(const double& S) {
	_S = S;
}

void HestonModel::SetR(const double& r) {
	_r = r;
}


// Méthodes servant au pricing close formula


std::complex<double> HestonModel::C1(const double& T, const double& omega) const{

	double u = -1.0;
	std::complex<double> y = i * omega - 1.0;

	std::complex<double> a = _kappa - _rho * _sigma * y;
	std::complex<double> b = sqrt(a * a + _sigma * _sigma * (u * i * omega + omega * omega));

	std::complex<double> g = (a - b) / (a + b);

	std::complex<double> C = _r * i * omega * 0.5 * T * T + ((_kappa * _theta) / (_sigma * _sigma)) * ((a - b) * T - 2.0 * log(  ( 1.0-g*exp(-b*T) )   /  (1.0-g)  ));

	return C;


}

std::complex<double> HestonModel::C2(const double& T, const double& omega) const {

	double u = 1.0;
	std::complex<double> y = i * omega;

	std::complex<double> a = _kappa - _rho * _sigma * y;
	std::complex<double> b = sqrt(a * a + _sigma * _sigma * (u * i * omega + omega * omega));

	std::complex<double> g = (a - b) / (a + b);

	std::complex<double> C = _r * i * omega * 0.5 * T * T + ((_kappa * _theta) / (_sigma * _sigma)) * ((a - b) * T - 2.0 * log((1.0 - g * exp(-b * T)) / (1.0 - g)));

	return C;


}

std::complex<double> HestonModel::D1(const double& T, const double& omega) const {

	double u = -1.0;
	std::complex<double> y = i * omega - 1.0;

	std::complex<double> a = _kappa - _rho * _sigma * y;
	std::complex<double> b = sqrt(a * a + _sigma * _sigma * (u * i * omega + omega * omega));

	std::complex<double> g = (a - b) / (a + b);

	std::complex<double> D = ((a - b) / (_sigma * _sigma)) * ((1.0 - exp(-b * T)) / (1.0 - g * exp(-b * T)));

	return D;
}

std::complex<double> HestonModel::D2(const double& T, const double& omega) const {

	double u = 1.0;
	std::complex<double> y = i * omega;

	std::complex<double> a = _kappa - _rho * _sigma * y;
	std::complex<double> b = sqrt(a * a + _sigma * _sigma * (u * i * omega + omega * omega));

	std::complex<double> g = (a - b) / (a + b);

	std::complex<double> D = ((a - b) / (_sigma * _sigma)) * ((1.0 - exp(-b * T)) / (1.0 - g * exp(-b * T)));

	return D;
}


std::complex<double> HestonModel::phi1(const double& T, const double& omega) const {
	double x = log(_S);
	std::complex<double> phi = exp(C1(T, omega) + D1(T, omega) * _v0 + i * omega * x);
	return phi;
}


std::complex<double> HestonModel::phi2(const double& T, const double& omega) const {
	double x = log(_S);
	std::complex<double> phi = exp(C2(T, omega) + D2(T, omega) * _v0 + i * omega*x);
	return phi;
}

std::vector<double> HestonModel::P_1_2(const double& K, const double& T) const {

	double omega;
	double integrale1 = 0.;
	double integrale2 = 0.;
	double logK = log(K);
	std::vector<double> P(2,0.);

	int N = points.size();

	for (int k = 0; k < N; k++) {

		omega = points[k];
		integrale1 += weights[k] * ((phi1(T, omega) * exp((omega * omega) - i * omega * logK) / (i * omega))).real();
		integrale2 += weights[k] * ((phi2(T, omega) * exp((omega * omega) - i * omega * logK) / (i * omega))).real();


	}
	
	P[0] = 0.5 + (1 / (2 * pi)) * integrale1;
	P[1] = 0.5 + (1 / (2 * pi)) * integrale2;

	return P;
}


double HestonModel::CallPrice(const double& K, const double& T) const {

	double P1 = P_1_2(K, T)[0];
	double P2 = P_1_2(K, T)[1];


	double price = _S * P1 - K * exp(-_r * T) * P2;

	double inf = std::max(_S - K * exp(-_r * T), 0.);
	double sup = _S;



	return std::max(inf,std::min(price,sup)); // On cap et floor avec les bornes d'arbitrage
}


double HestonModel::PutPrice(const double& K, const double& T) const {

	double call = CallPrice(K,T);
	double price = call - (_S - K * exp(-_r * T));
	return price;

}


//// Classe BlackScholesModel


// Constructeurs

BlackScholesModel::BlackScholesModel() :

	Model(),
	_sigma(0.2)
{}

BlackScholesModel::BlackScholesModel(const double& S, const double& r,const double& sigma):

	Model(S,r),
	_sigma(sigma)
{}

BlackScholesModel::BlackScholesModel(const BlackScholesModel& model):

	Model(model),
	_sigma(model._sigma)
{}

// Opérateur d'assignation

BlackScholesModel& BlackScholesModel::operator=(const BlackScholesModel& model) {

	if (this != &model) {

		Model::operator=(model);
		_sigma = model._sigma;

	}
	return *this;
}


// Méthode Clone

BlackScholesModel* BlackScholesModel::clone() const {

	return new BlackScholesModel(*this);

}

// Getteurs

double BlackScholesModel::GetS() const {

	return _S;
}

double BlackScholesModel::GetR() const {

	return _r;
}

double BlackScholesModel::GetSigma() const {

	return _sigma;
}
// Setteurs

void BlackScholesModel::SetS(const double& S) {

	_S = S;
}

void BlackScholesModel::SetR(const double& r) {

	_r = r;
}

void BlackScholesModel::SetSigma(const double& sigma) {

	_sigma = sigma;
}

// Méthode de pricing


double BlackScholesModel::CallPrice(const double& K, const double& T) const {

	double d1 = (log(_S/K) + (_r + 0.5*_sigma*_sigma)*T)/ (_sigma * pow(T, 0.5));
	double d2 = d1 - _sigma * pow(T, 0.5);

	return _S * normalCDF(d1) - K * exp(-_r * T) * normalCDF(d2);
}


double BlackScholesModel::Vega(const double& K, const double& T, const double& sigma_mkt) const {

	double d1 = (log(_S / K) + (_r + 0.5 * sigma_mkt * sigma_mkt) * T) / (sigma_mkt * pow(T, 0.5));

	return 0.5*_S*pow(T,0.5)*exp(-0.5*d1*d1)/pi;
}