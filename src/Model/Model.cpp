#include "Model.h"

// Model class
Model::Model(const double &init_value) : _initial_value(init_value) {}

Model::Model(const Model &model) : _initial_value(model._initial_value) {}

Model::Model(Model &&model) noexcept
    : _initial_value(std::move(model._initial_value)) {}

Model &Model::operator=(const Model &model) {
  if (this != &model)
    _initial_value = model._initial_value;

  return *this;
}

Model &Model::operator=(Model &&model) noexcept {
  if (this != &model)
    _initial_value = std::move(model._initial_value);

  return *this;
}

// BlackScholesModel class
BlackScholesModel::BlackScholesModel(const double &init_value, const double &r,
                                     const double &volatility)
    : Model(init_value), _r(r), _volatility(volatility) {}

BlackScholesModel::BlackScholesModel(const BlackScholesModel &model)
    : Model(model), _r(model._r), _volatility(model._volatility) {}

BlackScholesModel::BlackScholesModel(BlackScholesModel &&model) noexcept
    : Model(std::move(model)), _r(std::move(model._r)),
      _volatility(std::move(model._volatility)) {}

BlackScholesModel &
BlackScholesModel::operator=(const BlackScholesModel &model) {
  if (this != &model) {
    Model::operator=(model);
    _r = model._r;
    _volatility = model._volatility;
  }
  return *this;
}

BlackScholesModel &
BlackScholesModel::operator=(BlackScholesModel &&model) noexcept {
  if (this != &model) {
    Model::operator=(std::move(model));
    _r = std::move(model._r);
    _volatility = std::move(model._volatility);
  }
  return *this;
}

double BlackScholesModel::drift_term(const double &time,
                                     const double &asset_price) const {
  return _r * asset_price;
}

double BlackScholesModel::diffusion_term(const double &time,
                                         const double &asset_price) const {
  return _volatility * asset_price;
}

BlackScholesModel *BlackScholesModel::clone() const {
  return new BlackScholesModel(*this);
}

// Bachelier class
BachelierModel::BachelierModel(const double &init_value, const double &r,
                               const double &volatility)
    : Model(init_value), _r(r), _volatility(volatility) {}

BachelierModel::BachelierModel(const BachelierModel &model)
    : Model(model), _r(model._r), _volatility(model._volatility) {}

BachelierModel::BachelierModel(BachelierModel &&model) noexcept
    : Model(std::move(model)), _r(std::move(model._r)),
      _volatility(std::move(model._volatility)) {}

BachelierModel &BachelierModel::operator=(const BachelierModel &model) {
  if (this != &model) {
    Model::operator=(model);
    _r = model._r;
    _volatility = model._volatility;
  }
  return *this;
}

BachelierModel &BachelierModel::operator=(BachelierModel &&model) noexcept {
  if (this != &model) {
    Model::operator=(std::move(model));
    _r = std::move(model._r);
    _volatility = std::move(model._volatility);
  }
  return *this;
}

double BachelierModel::drift_term(const double &time,
                                  const double &asset_price) const {
  return _r;
}

double BachelierModel::diffusion_term(const double &time,
                                      const double &asset_price) const {
  return _volatility;
}

BachelierModel *BachelierModel::clone() const {
  return new BachelierModel(*this);
}

// Dupire Model
DupireLocalVolatilityModel::DupireLocalVolatilityModel(
    double init_value, const ImpliedVolatilitySurface &implied_vol_surface,
    const double &eps_mat, const double &eps_strike, const double &r)
    : Model(init_value), _implied_volatility_surface(implied_vol_surface),
      _epsilon_maturity(eps_mat), _epsilon_strike(eps_strike), _r(r) {}

double DupireLocalVolatilityModel::drift_term(const double &time,
                                              const double &asset_price) const {
  return _r * asset_price;
}

double
DupireLocalVolatilityModel::diffusion_term(const double &time,
                                           const double &asset_price) const {
  return dupire_local_volatility(time, asset_price) * asset_price;
}

DupireLocalVolatilityModel *DupireLocalVolatilityModel::clone() const {
  return new DupireLocalVolatilityModel(*this);
}

double DupireLocalVolatilityModel::first_order_derivative_impliedvol_maturity(
    const double &time, const double &strike) const {
  double time_up = time + _epsilon_maturity;
  double time_down = time - _epsilon_maturity;

  double sigma_up =
      _implied_volatility_surface.implied_volatility(time_up, strike);
  double sigma_down =
      _implied_volatility_surface.implied_volatility(time_down, strike);
  double deriv = 0.5 * (sigma_up - sigma_down) / _epsilon_maturity;

  return deriv;
}

double DupireLocalVolatilityModel::first_order_derivative_impliedvol_strike(
    const double &time, const double &strike) const {
  double strike_up = strike + _epsilon_strike;
  double strike_down = strike - _epsilon_strike;

  double sigma_up =
      _implied_volatility_surface.implied_volatility(time, strike_up);
  double sigma_down =
      _implied_volatility_surface.implied_volatility(time, strike_down);
  double deriv = 0.5 * (sigma_up - sigma_down) / _epsilon_maturity;

  return deriv;
}

double DupireLocalVolatilityModel::second_order_derivative_impliedvol_strike(
    const double &time, const double &strike) const {
  double strike_up = strike + _epsilon_strike;
  double strike_down = strike - _epsilon_strike;
  double d_sigma_up = first_order_derivative_impliedvol_strike(time, strike_up);
  double d_sigma_down =
      first_order_derivative_impliedvol_strike(time, strike_down);
  double deriv = 0.5 * (d_sigma_up - d_sigma_down) / _epsilon_maturity;

  return deriv;
}

double DupireLocalVolatilityModel::d1(const double &time,
                                      const double &strike) const {
  double sigma = _implied_volatility_surface.implied_volatility(time, strike);
  double numerator =
      log(_initial_value / strike) + time * (_r + pow(sigma, 2.) / 2);
  double denominator = sigma * sqrt(time);
  return numerator / denominator;
};

double DupireLocalVolatilityModel::d2(const double &time,
                                      const double &strike) const {
  double d1_value = d1(time, strike);
  double sigma = _implied_volatility_surface.implied_volatility(time, strike);
  return (d1_value - sigma * sqrt(time));
};

double DupireLocalVolatilityModel::dupire_local_volatility(
    const double &time, const double &strike) const {
  double r = _r;
  double sigma = _implied_volatility_surface.implied_volatility(time, strike);
  double d_sigma_k = first_order_derivative_impliedvol_strike(time, strike);
  double d_sigma_T = first_order_derivative_impliedvol_maturity(time, strike);
  double d2_sigma_k = second_order_derivative_impliedvol_strike(time, strike);
  double d1_value = d1(time, strike);
  double d2_value = d2(time, strike);

  double numerator = 1 + (2 * time / sigma) * (d_sigma_T + r * d2_sigma_k);
  double denominator =
      1 + 2 * d1_value * strike * d_sigma_k * sqrt(time) +
      d1_value * d2_value * pow(strike * d2_sigma_k, 2.0) * time +
      pow(strike, 2.0) * time * sigma * d2_sigma_k;
  double sigma_dupire_square = pow(sigma, 2.0) * (numerator / denominator);
  double sigma_dupire = sqrt(sigma_dupire_square);
  return sigma_dupire;
}

// MdModel class
MdModel::MdModel(const Vector &initial_asset_vector)
    : _initial_asset_vector(initial_asset_vector) {}

MdModel::MdModel(const MdModel &model)
    : _initial_asset_vector(model._initial_asset_vector) {}

MdModel &MdModel::operator=(const MdModel &model) {
  if (this != &model) {
    _initial_asset_vector = model._initial_asset_vector;
  }
  return *this;
}

// Heston
HestonModel::HestonModel(const Vector &initial_asset_vector,
                         const double &kappa, const double &sigma_vol,
                         const double &theta, const double &r)
    : MdModel(initial_asset_vector), _kappa(kappa), _sigma_vol(sigma_vol),
      _theta(theta), _r(r) {}

HestonModel::HestonModel(const HestonModel &model)
    : MdModel(model), _kappa(model._kappa), _sigma_vol(model._sigma_vol),
      _theta(model._theta), _r(model._r) {}

HestonModel &HestonModel::operator=(const HestonModel &model) {
  if (this != &model) {
    MdModel::operator=(model);
    _kappa = model._kappa;
    _sigma_vol = model._sigma_vol;
    _theta = model._theta;
    _r = model._r;
  }
  return *this;
}

Vector HestonModel::drift_vector_term(const double &time,
                                      const Vector &asset_vector) const {
  Vector drift_vector_term(2, 0.0);
  drift_vector_term[0] = _r - max(asset_vector[1], 0.0) * 0.5;
  drift_vector_term[1] = _kappa * (_theta - max(asset_vector[1], 0.0));
  return drift_vector_term;
}

Vector HestonModel::diffusion_vector_term(const double &time,
                                          const Vector &asset_vector) const {
  Vector diffusion_term_vector(2, 0.0);
  diffusion_term_vector[0] = sqrt(max(asset_vector[1], 0.0)) * asset_vector[0];
  diffusion_term_vector[1] = _sigma_vol * sqrt(max(asset_vector[1], 0.0));
  return diffusion_term_vector;
}

HestonModel *HestonModel::clone() const { return new HestonModel(*this); }

double HestonModel::sigma_vol() const { return _sigma_vol; }

double HestonModel::kappa() const { return _kappa; }

double HestonModel::theta() const { return _theta; }

double HestonModel::r() const { return _r; }