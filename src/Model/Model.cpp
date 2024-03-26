#include "Model.h"
#include <utility>

Model::Model(const double& init_value)
	: _initial_value(init_value)
{
}

Model::Model(const Model& model)
	: _initial_value(model._initial_value)
{
}

Model::Model(Model&& model) noexcept
	: _initial_value(std::move(model._initial_value))
{
}

Model& Model::operator=(const Model& model)
{
	if (this != &model)
		_initial_value = model._initial_value;
	
	return *this;
}

Model& Model::operator=(Model&& model) noexcept
{
	if (this != &model)
		_initial_value = std::move(model._initial_value);

	return *this;
}

BlackScholesModel::BlackScholesModel(const double& init_value, const double& drift, const double& volatility)
	: Model(init_value), _drift(drift), _volatility(volatility)
{
}

BlackScholesModel::BlackScholesModel(const BlackScholesModel& model)
	: Model(model), _drift(model._drift), _volatility(model._volatility)
{
}

BlackScholesModel::BlackScholesModel(BlackScholesModel&& model) noexcept
	: Model(std::move(model)), _drift(std::move(model._drift)), _volatility(std::move(model._volatility))
{
}

BlackScholesModel& BlackScholesModel::operator=(const BlackScholesModel& model)
{
	if (this != &model)
	{
		Model::operator=(model);
		_drift = model._drift;
		_volatility = model._volatility;
	}
	return *this;
}

BlackScholesModel& BlackScholesModel::operator=(BlackScholesModel&& model) noexcept
{
	if (this != &model)
	{
		Model::operator=(std::move(model));
		_drift = std::move(model._drift);
		_volatility = std::move(model._volatility);
	}
	return *this;
}

double BlackScholesModel::drift_term(const double& time, const double& asset_price) const
{
	return _drift * asset_price;
}

double BlackScholesModel::diffusion_term(const double& time, const double& asset_price) const
{
	return _volatility * asset_price;
}

BlackScholesModel* BlackScholesModel::clone() const
{
	return new BlackScholesModel(*this);
}

double BachelierModel::drift_term(const double& time, const double& asset_price) const
{
	return 0.0;
}

double BachelierModel::diffusion_term(const double& time, const double& asset_price) const
{
	return 0.0;
}

BachelierModel* BachelierModel::clone() const
{
	return new BachelierModel(*this);
}

DupireLocalVolatilityModel::DupireLocalVolatilityModel(double init_value, const ImpliedVolatilitySurface& implied_vol_surface, const double& eps_mat, const double& eps_strike)
	: Model(init_value), _implied_volatility_surface(implied_vol_surface), _epsilon_maturity(eps_mat), _epsilon_strike(eps_strike)
{
}

double DupireLocalVolatilityModel::drift_term(const double& time, const double& asset_price) const
{
	return _implied_volatility_surface.risk_free_rate() * asset_price;
}

double DupireLocalVolatilityModel::diffusion_term(const double& time, const double& asset_price) const
{
	return 0.0;
}

DupireLocalVolatilityModel* DupireLocalVolatilityModel::clone() const
{
	return new DupireLocalVolatilityModel(*this);
}

double DupireLocalVolatilityModel::first_order_derivative_impliedvol_maturity(const double& time, const double& strike) const
{
	double time_up = time + _epsilon_maturity;
	double time_down = time - _epsilon_maturity;

	double sigma_up = _implied_volatility_surface.implied_volatility(time_up, strike);
	double sigma_down = _implied_volatility_surface.implied_volatility(time_down, strike);
	double deriv = 0.5 * (sigma_up - sigma_down) / _epsilon_maturity;
	
	return deriv;
}

double DupireLocalVolatilityModel::first_order_derivative_impliedvol_strike(const double& time, const double& strike) const
{
	return 0.0;
}

double DupireLocalVolatilityModel::second_order_derivative_impliedvol_strike(const double& time, const double& strike) const
{
	return 0.0;
}

double DupireLocalVolatilityModel::dupire_local_volatility(const double& time, const double& strike) const
{
	return 0.0;
}
