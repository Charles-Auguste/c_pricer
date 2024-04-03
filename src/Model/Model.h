#pragma once

#include "../ImpliedVolatilitySurface/ImpliedVolatilitySurface.h"

#include <vector>
using Vector = std::vector<double>;
using Matrix = std::vector<Vector>;
#include <iostream>
#include <utility>
#include <cmath>
using namespace std;


// Abstract class
class Model
{
public:
// 1) constructor
	Model(const double& init_value);
	Model(const Model& model);
	Model(Model&& model) noexcept;
	Model& operator=(const Model& model);
	Model& operator=(Model&& model) noexcept;
	// 2) destructor declared virtual
	virtual ~Model() = default;

	// 3) pure virtual functions
	virtual double  drift_term(const double& time, const double& asset_price) const = 0;
	virtual double  diffusion_term(const double& time, const double& asset_price) const = 0;
	virtual Model* clone() const = 0;

	inline double init_value() const
	{
		return _initial_value;
	}

protected:
	// 4) member variables
	double _initial_value;
};


class BlackScholesModel : public Model
{
public:
	// 1) constructor
	BlackScholesModel(const double& init_value, const double& drift, const double& volatility);
	BlackScholesModel(const BlackScholesModel& model);
	BlackScholesModel(BlackScholesModel&& model) noexcept;
	BlackScholesModel& operator=(const BlackScholesModel& model);
	BlackScholesModel& operator=(BlackScholesModel&& model) noexcept;

	// 2) public member functions
	double drift_term(const double& time, const double& asset_price) const override;
	double diffusion_term(const double& time, const double& asset_price) const override;
	BlackScholesModel* clone() const override;

private:
	// 3) member variables
	double _drift;      // mu
	double _volatility; // sigma
};


class BachelierModel : public Model
{
public:
	// 1) constructor
	BachelierModel(const double& init_value, const double& drift, const double& volatility);
	BachelierModel(const BachelierModel& model);
	BachelierModel(BachelierModel&& model) noexcept;
	BachelierModel& operator=(const BachelierModel& model);
	BachelierModel& operator=(BachelierModel&& model) noexcept;

	// 2) public member functions
	double drift_term(const double& time, const double& asset_price) const override;
	double diffusion_term(const double& time, const double& asset_price) const override;
	BachelierModel* clone() const override;

private:
	// 3) member variables
	double _drift;      // mu
	double _volatility; // sigma
};


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
	double d1(const double& time, const double& strike) const;
	double d2(const double& time, const double& strike) const;

	ImpliedVolatilitySurface _implied_volatility_surface;
	double _epsilon_maturity; // dT
	double _epsilon_strike;   // dK
};

// +1D models
// =========
// Abstract class
class MdModel
{
public:
	// Constructor with parameters
	MdModel(const Vector& initial_asset_vector);
	MdModel(const MdModel& model);
	MdModel& operator=(const MdModel& model);

	// pure virtual methods so MdModel class is an abstract class
	virtual Vector drift_vector_term(const double& time, const Vector& asset_vector) const = 0;
	virtual Vector diffusion_vector_term(const double& time, const Vector& asset_vector) const = 0;
	virtual MdModel* clone() const = 0;

	inline Vector init_value() const
	{
		return _initial_asset_vector;
	}

	virtual double sigma_vol() const = 0;
	virtual double kappa() const = 0;
	virtual double theta() const = 0;
	virtual double interest_rate() const = 0;

protected:
	Vector _initial_asset_vector; // (S_0, V_0)
};


class HestonModel : public MdModel
{
public:
	HestonModel(const Vector& initial_asset_vector,const double& kappa, const double& sigma_vol, const double& theta, const double& interest_rate); // parameters constructor
	HestonModel(const HestonModel& model); // copy constructor
	HestonModel& operator=(const HestonModel& model);
	virtual ~HestonModel() = default;

	Vector drift_vector_term(const double& time, const Vector& asset_vector) const override;
	Vector diffusion_vector_term(const double& time, const Vector& asset_vector) const override;

	HestonModel* clone() const override;

	virtual double sigma_vol() const override;
	virtual double kappa() const override;
	virtual double theta() const override;
	virtual double interest_rate() const override;


private:
	//double _corr_rate;
	double _kappa;
	double _sigma_vol;
	double _theta;
	double _interest_rate;
};