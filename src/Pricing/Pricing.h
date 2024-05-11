#pragma once
#include <random>
#include <vector>
using Vector = std::vector<double>;
using Matrix = std::vector<Vector>;
#include <iostream>
#include <cmath>
#include <cassert>
#include <Eigen/Dense> //  Eigen pour la régression linéaire
using namespace std;

enum class CALL_PUT
{
	CALL,
	PUT
};


class Pricing
{
public:
	virtual double price(Matrix asset_paths) const = 0;
	virtual Pricing* clone() const = 0;
	virtual ~Pricing() = default;
};


class EuropeanOptionPricing : public Pricing
{
public:
	EuropeanOptionPricing(double strike, CALL_PUT call_put, double r, double T);
	double price(Matrix asset_paths) const override;

	EuropeanOptionPricing* clone() const override;

private:
	double _strike;
	CALL_PUT _call_put;
	double _r;
	double _T;
};


class AmericanOptionPricing: public Pricing
{
public:
	AmericanOptionPricing(double strike, CALL_PUT call_put, double r, double T);
	double price(Matrix asset_paths) const override;

	AmericanOptionPricing* clone() const override;
private:
	Eigen::VectorXd genlaguerre(int k, const Eigen::VectorXd& x) const;
	Eigen::MatrixXd laguerre_basis(const Eigen::VectorXd& x, int n) const;

	double _strike;
	CALL_PUT _call_put;
	double _r;
	double _T;
};


// Arithmetic mean for asian option
class AsianOptionPricing: public Pricing
{
public:
	AsianOptionPricing(double strike, CALL_PUT call_put, double r, double T);
	double price(Matrix asset_paths) const override;

	AsianOptionPricing* clone() const override;
private:
	double _strike;
	CALL_PUT _call_put;
	double _r;
	double _T;
};