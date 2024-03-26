#pragma once

#include "../ImpliedVolatilitySurface/ImpliedVolatilitySurface.h"
#include <iostream>
using namespace std;

// Abstract class
class Model
{
public:
	Model(const double& init_value);
	// Special members functions
	// 1) default constructor
	// 2) copy constructor
	Model(const Model& model);
	// 3) move constructor 
	Model(Model&& model) noexcept;
	// 4) copy assignment operator
	Model& operator=(const Model& model);
	// 5) move assignment operator
	Model& operator=(Model&& model) noexcept;
	// 6) destructor declared virtual
	virtual ~Model() = default;

	virtual double drift_term(const double& time, const double& asset_price) const = 0;
	virtual double diffusion_term(const double& time, const double& asset_price) const = 0;
	virtual Model* clone() const = 0;

	inline double init_value() const
	{
		return _initial_value;
	}

protected:
	double _initial_value;
};


class BlackScholesModel : public Model
{
public:
	BlackScholesModel(const double& init_value, const double& drift, const double& volatility);

	// special member functions

	// 1) Default constructor

	// 2) Copy constructor
	BlackScholesModel(const BlackScholesModel& model);

	// 3) Move constructor
	BlackScholesModel(BlackScholesModel&& model) noexcept;

	// 4) Copy assignment operator
	BlackScholesModel& operator=(const BlackScholesModel& model);

	// 5) Move assignment operator
	BlackScholesModel& operator=(BlackScholesModel&& model) noexcept;

	// 6) destructor : no need to declare anything

	double drift_term(const double& time, const double& asset_price) const override;
	double diffusion_term(const double& time, const double& asset_price) const override;
	BlackScholesModel* clone() const override;

private:
	double _drift;      // mu
	double _volatility; // sigma
};

class BachelierModel : public Model
{
public:
	// TO DO : spacial member functions

	double drift_term(const double& time, const double& asset_price) const override;
	double diffusion_term(const double& time, const double& asset_price) const override;
	BachelierModel* clone() const override;

private:
	double _drift;      // mu
	double _volatility; // sigma
};

// Dupire Local Volatility Model
class DupireLocalVolatilityModel : public Model
{
public:
	DupireLocalVolatilityModel(double init_value, const ImpliedVolatilitySurface& implied_vol_surface, const double& eps_mat, const double& eps_strike);
	double drift_term(const double& time, const double& asset_price) const override;
	double diffusion_term(const double& time, const double& asset_price) const override;
	DupireLocalVolatilityModel* clone() const override;


private:
	double dupire_local_volatility(const double& time, const double& strike) const;

	double first_order_derivative_impliedvol_maturity(const double& time, const double& strike) const;
	double first_order_derivative_impliedvol_strike(const double& time, const double& strike) const;
	double second_order_derivative_impliedvol_strike(const double& time, const double& strike) const;

	ImpliedVolatilitySurface _implied_volatility_surface;
	double _epsilon_maturity; // dT
	double _epsilon_strike;   // dK
};
