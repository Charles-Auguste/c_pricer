#pragma once

#include <random>
#include <vector>
using Vector = std::vector<double>;
using Matrix = std::vector<Vector>;
#include <iostream>
#include <utility>
#include <cmath>
#include <cassert>
using namespace std;

enum class CALL_PUT
{
	CALL,
	PUT
};

class Payoff
{
public:
	virtual double payoff(Vector asset_path) const = 0;
	virtual Payoff* clone() const = 0;
	virtual ~Payoff() = default;
};

class EuropeanOptionPayoff : public Payoff
{
public:
	EuropeanOptionPayoff(double strike, CALL_PUT call_put);
	double payoff(Vector asset_path) const override;

	EuropeanOptionPayoff* clone() const override;

private:
	double _strike;
	CALL_PUT _call_put;
};

class AmericanPayoff: public Payoff
{
public:
	AmericanPayoff(double strike, CALL_PUT call_put);
	double payoff(Vector asset_path) const override;

	AmericanPayoff* clone() const override;

private:
	double _strike;
	CALL_PUT _call_put;
};

class AsianPayoff: public Payoff
{
public:
	AsianPayoff(double strike, CALL_PUT call_put);
	double payoff(Vector asset_path) const override;

	AsianPayoff* clone() const override;
private:
	double _strike;
	CALL_PUT _call_put;
};