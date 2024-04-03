#include "Payoff.h"

EuropeanOptionPayoff::EuropeanOptionPayoff(double strike, CALL_PUT call_put)
	: _strike(strike), _call_put(call_put)
{
}

double EuropeanOptionPayoff::payoff(Vector asset_path) const
{
	double price_at_maturity = asset_path[asset_path.size() - 1];
	double multpilicative_factor = _call_put == CALL_PUT::CALL ? 1. : -1.;

	double payoff = max(multpilicative_factor * (price_at_maturity - _strike), 0.);
	return payoff;
}

EuropeanOptionPayoff* EuropeanOptionPayoff::clone() const
{
	return new EuropeanOptionPayoff(*this);
}

AmericanPayoff::AmericanPayoff(double strike, CALL_PUT call_put)
	: _strike(strike), _call_put(call_put)
{
}

double AmericanPayoff::payoff(Vector asset_path) const
{
	double price_at_maturity;
	double multpilicative_factor;

	if (_call_put == CALL_PUT::CALL){
		price_at_maturity = *max_element(begin(asset_path), end(asset_path));
		multpilicative_factor = 1.;
	}
	else{
		price_at_maturity = *min_element(begin(asset_path), end(asset_path));
		multpilicative_factor = -1.;
	}
	
	double payoff = max(multpilicative_factor * (price_at_maturity - _strike), 0.);
	return payoff;
}

AmericanPayoff* AmericanPayoff::clone() const
{
	return new AmericanPayoff(*this);
}

AsianPayoff::AsianPayoff(double strike, CALL_PUT call_put)
	: _strike(strike), _call_put(call_put)
{
}

double AsianPayoff::payoff(Vector asset_path) const
{
	double price_at_maturity = reduce(begin(asset_path), end(asset_path), 0.0) / asset_path.size();

	double multpilicative_factor = _call_put == CALL_PUT::CALL ? 1. : -1.;

	double payoff = max(multpilicative_factor * (price_at_maturity - _strike), 0.);
	return payoff;
}

AsianPayoff* AsianPayoff::clone() const
{
	return new AsianPayoff(*this);
}

