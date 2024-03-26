#pragma once

#include <vector>
using Vector = std::vector<double>;
using Matrix = std::vector<Vector>;
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

class ImpliedVolatilitySurface
{
public:
	ImpliedVolatilitySurface(const Vector& maturities, const Vector& strikes, const Matrix& market_implied_volatilities, const double& risk_free_rate);

	// returns sigma*(T,K) for any maturity T and any strike K
	double implied_volatility(const double& maturity, const double& strike) const;

	inline double risk_free_rate() const
	{
		return _risk_free_rate;
	}

private:
	// Checking methods called during object construction
	bool check_ordered_vectors() const;
	bool check_dimensions() const;
	bool check_vols_positivity() const;

	// Method that does intermediary computations : should be called once only during construction
	void evaluate_cubic_spline_coefficients(); // NON-CONST because modifies _alpha_coefficients, _beta_coefficients ,etc...

	// For a given market maturity T_i, compute the interpolation/extrapolation along strikes
	double smile_implied_volatility(const size_t& maturity_index, const double& strike) const;

	// size of strikes vector
	inline size_t strike_size() const
	{
		return _strikes.size();
	}

	// size of maturites vector
	inline size_t maturity_size() const
	{
		return _maturities.size();
	}

	// Maturities [T_1, ..., T_M]
	Vector _maturities;

	// Strikes [K_1, ..., K_N]
	Vector _strikes;

	Vector _delta_strikes;

	// Market Implied Volatility Matrix
	// [sigma*(T_1,K_1), .... sigma*(T_1, K_N)]
	// ........................................
	// [sigma*(T_M,K_1), .... sigma*(T_M, K_N)]
	Matrix _market_implied_volatilities;

	double _risk_free_rate; // needed for linear interpolation in variance towards maturities

	// Intermediary computations

	// alpha_1, ..., alpha_{N-1} [T_1]
	// ...........................
	// alpha_1, ..., alpha_{N-1} [T_M]

	Matrix _alpha_coefficients;
	Matrix _beta_coefficients;
	Matrix _gamma_coefficients;

};
