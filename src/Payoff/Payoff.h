#pragma once

#include <vector>
using Vector = std::vector<double>;
using Matrix = std::vector<Vector>;
#include <iostream>
#include <utility>
#include <cmath>
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
	double _discount_factor;
};

// Derived class of EuropeanOptionPayoff with Barriers

// American payoffs

// Asian payoffs