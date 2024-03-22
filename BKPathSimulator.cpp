#include "BKPathSimulator.h"
#include <random>
#include <iostream>
#include "HestonModel.h"



BKPathSimulator::BKPathSimulator(const MDModel* model_ptr, const Vector& time_points) :
	_model_ptr(model_ptr->clone()), _time_points(time_points)
{
}

BKPathSimulator& BKPathSimulator::operator=(const BKPathSimulator& path)
{
	if (this != &path) {
		_model_ptr = path._model_ptr;
		_time_points = path._time_points;
	}
	return *this;
}

// 
double BKPathSimulator::generate_chi_squared(int d) const
{
	std::random_device rd;
	std::mt19937 gen(rd());
	// Convertir d en double pour la division
	double alpha = static_cast<double>(d) / 2.0;
	// Distribution gamma avec alpha = d/2 et beta = 2
	std::gamma_distribution<double> distribution(alpha, 2.0);
	// Générer un échantillon de la distribution gamma
	double chi_squared = distribution(gen);

	return chi_squared;
}

double BKPathSimulator::chi_prime(int d, double lambda) const
// noncentral chi-squared random variable with d degrees of freedom, 
// and noncentrality parameter lambda may be expressed using an ordinary
//  chi-squared and an independent normal when d > 1
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution<double> distribution(0, 1.); // N(0,1)
	double g = distribution(gen);
	double res = pow(g + std::sqrt(lambda), 2) + generate_chi_squared(d);
	return res;
}

double BKPathSimulator::var_next_step(size_t current_time_idx, double current_value) const
{
	double delta_t = _time_points[current_time_idx + 1] - _time_points[current_time_idx];
	double pre_factor = (pow(_model_ptr->sigma_vol(), 2) * (1 - std::exp(-_model_ptr->kappa() * delta_t))) / (4 * _model_ptr->kappa());
	int degree = int(4 * _model_ptr->kappa() * _model_ptr->theta() / pow(_model_ptr->sigma_vol(),2));
	double next_var = pre_factor * chi_prime(degree, current_value * std::exp(-_model_ptr->kappa() * delta_t) / pre_factor); // where f is loi du chi 2 à d degrés de liberté dérivé
	return next_var;
}

double BKPathSimulator::integral_sampling(size_t current_time_index, double var_current_value) const
{
	double var_next_value = var_next_step(current_time_index, var_current_value);
	return 0.0;
}


double BKPathSimulator::asset_next_step(size_t current_time_idx, Vector current_asset_log_value) const // current_asset_log_value = [ln(S_t), V_t]
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution<double> distribution(0, 1.); // N(0,1)
	double g = distribution(gen);
	double delta_t = _time_points[current_time_idx + 1] - _time_points[current_time_idx];
	double integral = integral_sampling(current_time_idx, current_asset_log_value[1]);
	double next_asset_log_value = current_asset_log_value[0]
								  + _model_ptr->interest_rate() * delta_t
								  + _model_ptr->correlation() / _model_ptr->sigma_vol() * (var_next_step(current_time_idx, current_asset_log_value[1]) - current_asset_log_value[1] - _model_ptr->kappa()*_model_ptr->theta() * delta_t)
								  + (_model_ptr->kappa()*_model_ptr->correlation()/_model_ptr->sigma_vol() - 0.5) * integral
								  + std::sqrt(std::max(integral, 0.0) * (1 - pow(_model_ptr->correlation(),2))) * g; // gaussian distribution

	return next_asset_log_value;
}

VectorVector BKPathSimulator::path() const
{
	VectorVector bidim_log_path(2, Vector(_time_points.size()));

	bidim_log_path[0][0] = std::log(_model_ptr->init_value()[0]); // S[0]
	bidim_log_path[1][0] = _model_ptr->init_value()[1]; // V[0]

	for (size_t idx = 0; idx < _time_points.size() - 1; ++idx) {
		Vector current_asset_value{ bidim_log_path[0][idx], bidim_log_path[1][idx] };
		bidim_log_path[1][idx + 1] = var_next_step(idx, current_asset_value[1]); // adding the next volatility spot to the volatility spot path
		bidim_log_path[0][idx + 1] = asset_next_step(idx, current_asset_value); // adding the next asset price to the asset price path
	}

	// returning the asset price path
	VectorVector bidim_path(2, Vector(_time_points.size()));
	for (size_t idx = 0; idx < _time_points.size() - 1; ++idx) {
		bidim_path[0][idx] = std::exp(bidim_log_path[0][idx]);
	}
	bidim_path[1] = bidim_log_path[1];

	return bidim_path;
}

BKPathSimulator::~BKPathSimulator()
{
	delete _model_ptr;
	std::cout << "Broadie-Kaya destructor operated" << std::endl;
}

BKPathSimulator* BKPathSimulator::clone()
{
	return new BKPathSimulator(*this);
}