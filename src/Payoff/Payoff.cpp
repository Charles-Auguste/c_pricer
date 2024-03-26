#include "Payoff.h"

EuropeanOptionPayoff::EuropeanOptionPayoff(double strike, CALL_PUT call_put)
	: _strike(strike), _call_put(call_put)
{
}

double EuropeanOptionPayoff::payoff(std::vector<double> asset_path) const
{
	double price_at_maturity = asset_path[asset_path.size() - 1];
	double multpilicative_factor = _call_put == CALL_PUT::CALL ? 1. : -1.;

	double payoff = std::max(multpilicative_factor * (price_at_maturity - _strike), 0.);
	return payoff;
}

EuropeanOptionPayoff* EuropeanOptionPayoff::clone() const
{
	return new EuropeanOptionPayoff(*this);
}
