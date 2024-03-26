#pragma once

#include <vector>


/*enum CALL_PUT_ENUM
{
	CALL, // 0
	PUT   // 1
};*/


enum class CALL_PUT
{
	CALL,
	PUT
};


// abstract class

// TO DO : include the discounting as part of the payoff/path_price

class Payoff
{
public:

	// maybe rename path_price
	virtual double payoff(std::vector<double> asset_path) const = 0;

	virtual Payoff* clone() const = 0;

	virtual ~Payoff() = default;
};

class EuropeanOptionPayoff : public Payoff
{
public:
	EuropeanOptionPayoff(double strike, CALL_PUT call_put);
	double payoff(std::vector<double> asset_path) const override;

	EuropeanOptionPayoff* clone() const override;

private:
	double _strike;
	CALL_PUT _call_put;
};

// Derived class of EuropeanOptionPayoff with Barriers

// American payoffs

// Asian payoffs