#pragma once
# include <cfloat>
# include <cmath>
# include <cstdlib>
# include <cstring>
# include <ctime>
# include <fstream>
# include <iomanip>
# include <iostream>
#include <vector>
using Vector = std::vector<double>;
#include <complex>
using Complex = std::complex<double>;
using namespace std;


class ExplicitModel
{
public :
    ExplicitModel();
    ExplicitModel(const double& S, const double& r);
    ExplicitModel(const ExplicitModel& model);

    virtual ~ExplicitModel()=default;

    ExplicitModel& operator=(const Model& model);

protected:
    double _S; // Initial price
    double _r; // risk free rate


class ExplicitHestonModel : public ExplicitModel
{
public :
    ExplicitHestonModel();
    ExplicitHestonModel(const double& kappa, const double& theta, const double& sigma, const double& rho, const double& v0, const double& S, const double& r);
    ExplicitHestonModel(const ExplicitHestonModel& other);

    double GetKappa() const;
    double GetTheta() const ;
    double GetSigma() const ;
    double GetRho() const ;
    double GetV0() const ;
    double GetS() const ;
    double GetR() const ;

    void SetKappa(const double& kappa);
    void SetTheta(const double& theta);
    void SetSigma(const double& sigma);
    void SetRho(const double& rho);
    void SetV0(const double& v0);
    void SetS(const double& S);
    void SetR(const double& r);

    ~ExplicitHestonModel() override = default;

    ExplicitHestonModel& operator = (const ExplicitHestonModel& model);

    ExplicitHestonModel* clone() const;
    
    double CallPrice(const double& K, const double& T) const ;
    double PutPrice(const double& K, const double& T) const ;

private :
    Complex C1(const double& T, const double& omega) const;
    Complex C2(const double& T, const double& omega) const ;
    Complex D1(const double& T, const double& omega) const ;
    Complex D2(const double& T, const double& omega) const ;
    Complex phi1(const double& T, const double& omega) const ;
    Complex phi2(const double& T, const double& omega) const ;
    Complex P_1_2(const double& K, const double& T) const ;

    double _kappa;
    double _theta;
    double _sigma;
    double _rho;
    double _v0;
};


class ExplicitBlackScholesModel : public Model
{
public :
    ExplicitBlackScholesModel();
    ExplicitBlackScholesModel(const double& S, const double& r, const double& sigma);
    ExplicitBlackScholesModel(const ExplicitBlackScholesModel& model);

    ~ExplicitBlackScholesModel() override = default ;

    ExplicitBlackScholesModel& operator=(const ExplicitBlackScholesModel& model);

    ExplicitBlackScholesModel* clone() const;

    double GetS() const;
    double GetR() const;
    double GetSigma() const;

    void SetS(const double& S);
    void SetR(const double& r);
    void SetSigma(const double& sigma);

    double CallPrice(const double& K, const double& T) const;
    double Vega(const double&K, const double& T, const double& sigma_mkt) const;

private : 
   double _sigma;
};


class OptimisationImpliedVolatility
{
public :
	OptimisationImpliedVolatility();
	OptimisationImpliedVolatility(const double& epsilon, const ExplicitHestonModel& model, const ExplicitBlackScholesModel& bs_model);
	OptimisationImpliedVolatility(const OptimisationImpliedVolatility& optim);

	~OptimisationImpliedVolatility() = default;

	ExplicitHestonModel* GetModelPtr() const;
	ExplicitBlackScholesModel* GetBSModelPtr() const;
	double GetEpsilon() const;

	void SetEpsilon(const double& epsilon);
	// No assignment operator, we want the model pointer to be constant (but the model will change during calibration)

	// Calculation of implicit volatility (by dichotomy)
	void implied_vol(const double& T, const double& K, const double& C) const;

	// Calculation of the loss function from a volatility surface
	double loss_function(const std::vector<std::vector<double>>& IV_surface) const; //bas� sur erreur L2 des vols avec pond�ration en vega
	double loss_function_bis(const std::vector<std::vector<double>>& IV_surface) const; //bas� sur erreur L2 des prix avec pond�ration en vega^2

	// Optimization of the loss function (with Nelder Mead: simplex algorithm)
	void calibration(const std::vector<std::vector<double>>& IV_surface) const;
	void calibration_bis(const std::vector<std::vector<double>>& IV_surface) const;

private :
	double _epsilon_iv; // error tolerated for calculating the IV
	ExplicitHestonModel* const _model_ptr; // constant pointer to the model whose parameters we seek to optimize
	ExplicitBlackScholesModel* const _bs_model_ptr; // constant pointer to a BS model: calculation of implied volatility
};

// Quadrature
Vector gauss_hermite_points(const int& degree); // point vector
Vector gauss_hermite_weights(const int& degree); //weight vector