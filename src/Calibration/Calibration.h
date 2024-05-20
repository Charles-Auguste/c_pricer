#pragma once
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numbers>
#include <numeric>
#include <vector>
using Vector = std::vector<double>;
using Matrix = std::vector<std::vector<double>>;
#include <complex>
using Complex = std::complex<double>;
using namespace std;

// Useful constant
const Complex i(0, 1);
const double pi = 2 * acos(0.0);

enum class LOSS_FUNCTION { VOL, CALL };

class ExplicitModel {
public:
  ExplicitModel();
  ExplicitModel(const double &S, const double &r);
  ExplicitModel(const ExplicitModel &model);

  virtual ~ExplicitModel() = default;

  ExplicitModel &operator=(const ExplicitModel &model);

  double GetS() const;
  double GetR() const;
  void SetS(const double &S);
  void SetR(const double &r);

protected:
  double _S; // Initial price
  double _r; // risk free rate
};

class ExplicitHestonModel : public ExplicitModel {
public:
  ExplicitHestonModel();
  ExplicitHestonModel(const double &kappa, const double &theta,
                      const double &sigma, const double &rho, const double &v0,
                      const double &S, const double &r);
  ExplicitHestonModel(const ExplicitHestonModel &other);

  double GetKappa() const;
  double GetTheta() const;
  double GetSigma() const;
  double GetRho() const;
  double GetV0() const;

  void SetKappa(const double &kappa);
  void SetTheta(const double &theta);
  void SetSigma(const double &sigma);
  void SetRho(const double &rho);
  void SetV0(const double &v0);

  ~ExplicitHestonModel() override = default;

  ExplicitHestonModel &operator=(const ExplicitHestonModel &model);

  ExplicitHestonModel *clone() const;

  double CallPrice(const double &K, const double &T) const;
  double PutPrice(const double &K, const double &T) const;

private:
  Complex C1(const double &T, const double &omega) const;
  Complex C2(const double &T, const double &omega) const;
  Complex D1(const double &T, const double &omega) const;
  Complex D2(const double &T, const double &omega) const;
  Complex phi1(const double &T, const double &omega) const;
  Complex phi2(const double &T, const double &omega) const;
  Vector P_1_2(const double &K, const double &T) const;

  Vector gauss_hermite_points() const;  // point vector
  Vector gauss_hermite_weights() const; // weight vector

  double _kappa;
  double _theta;
  double _sigma;
  double _rho;
  double _v0;
};

class ExplicitBlackScholesModel : public ExplicitModel {
public:
  ExplicitBlackScholesModel();
  ExplicitBlackScholesModel(const double &S, const double &r,
                            const double &sigma);
  ExplicitBlackScholesModel(const ExplicitBlackScholesModel &model);

  ~ExplicitBlackScholesModel() override = default;

  ExplicitBlackScholesModel &operator=(const ExplicitBlackScholesModel &model);

  ExplicitBlackScholesModel *clone() const;

  double GetSigma() const;
  void SetSigma(const double &sigma);

  double CallPrice(const double &K, const double &T) const;
  double Vega(const double &K, const double &T, const double &sigma_mkt) const;

private:
  double normalCDF(const double &x) const;
  double _sigma;
};

class OptimisationImpliedVolatility {
public:
  OptimisationImpliedVolatility();
  OptimisationImpliedVolatility(const double &epsilon,
                                const ExplicitHestonModel &model,
                                const ExplicitBlackScholesModel &bs_model);
  OptimisationImpliedVolatility(const OptimisationImpliedVolatility &optim);

  ~OptimisationImpliedVolatility() = default;

  ExplicitHestonModel *GetModelPtr() const;
  ExplicitBlackScholesModel *GetBSModelPtr() const;
  double GetEpsilon() const;

  void SetEpsilon(const double &epsilon);
  // No assignment operator, we want the model pointer to be constant (but the
  // model will change during calibration)

  // Calculation of implicit volatility (by dichotomy)
  void implied_vol(const double &T, const double &K, const double &C) const;

  // Calculation of the loss function from a volatility surface
  double loss_function(const Matrix &IV_surface)
      const; // L2 error on volatility weighted with vega
  double loss_function_square(const Matrix &IV_surface)
      const; // L2 error on volatility weighted with vega2

  // Optimization of the loss function (with Nelder Mead: simplex algorithm)
  void calibration(const Matrix &IV_surface, LOSS_FUNCTION func_loss) const;

private:
  double _epsilon_iv; // error tolerated for calculating the IV
  ExplicitHestonModel *const _model_ptr; // constant pointer to the model whose
                                         // parameters we seek to optimize
  ExplicitBlackScholesModel
      *const _bs_model_ptr; // constant pointer to a BS model: calculation of
                            // implied volatility
};