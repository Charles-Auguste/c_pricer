#include "Pricing.h"

EuropeanOptionPricing::EuropeanOptionPricing(double strike, CALL_PUT call_put, double r, double T)
	: _strike(strike), _call_put(call_put), _r(r), _T(T)
{
}

double EuropeanOptionPricing::price(Matrix asset_paths) const
{
	Vector prices;
	double multpilicative_factor = _call_put == CALL_PUT::CALL ? 1. : -1.;
	for (size_t k=0; k<asset_paths.size(); k++){
		double price_at_maturity =  asset_paths[k][asset_paths[k].size() -1];
		double payoff = max(multpilicative_factor * (price_at_maturity - _strike), 0.);
		payoff = payoff * exp(-_r * _T);
		prices.push_back(payoff);
	}

	double price = accumulate(begin(prices), end(prices), 0);
	price = price / prices.size();

	return price;
}

EuropeanOptionPricing* EuropeanOptionPricing::clone() const
{
	return new EuropeanOptionPricing(*this);
}


AmericanOptionPricing::AmericanOptionPricing(double strike, CALL_PUT call_put, double r, double T)
	: _strike(strike), _call_put(call_put), _r(r), _T(T)
{
}

Eigen::VectorXd AmericanOptionPricing::genlaguerre(int k, const Eigen::VectorXd& x) const{  
    Eigen::VectorXd V(x.size());
        for (int i = 0; i < x.size(); ++i) {
            V(i) = laguerre(k, x(i));
    }
    return V;
}

Eigen::MatrixXd AmericanOptionPricing::laguerre_basis(const Eigen::VectorXd& x, int n) const { 
    Eigen::MatrixXd basis(x.size(), n);
    for (int i = 0; i < n; ++i) {
        basis.col(i) = genlaguerre(i, x);
    }
    return basis;
}

double AmericanOptionPricing::price(Matrix asset_paths) const
{
	double multpilicative_factor = _call_put == CALL_PUT::CALL ? 1. : -1.;
	size_t N_regressors = 10;
	size_t N_echeances = asset_paths[0].size();

	// Cash flows for all dates
	Matrix cashflows = asset_paths;
	// Loop over paths
    for (size_t i = 0; i < asset_paths.size(); ++i){
		// Build Cashflows
        for (size_t k = 0; k < asset_paths[i].size(); ++k) {
            cashflows[i][k] = max(multpilicative_factor * (asset_paths[i][k] - _strike), 0.); 
        }
    }

    // Backward Monte Carlo least square error
	// loop over assest prices 
    for (int k = asset_paths[0].size() - 1; k > 0; --k) {              
        Vector in_the_money;
		// Loop over paths
        for (size_t i = 0; i < asset_paths.size(); ++i) {
            if (cashflows[i][k] > 0){
                in_the_money.push_back(i);
            }
        }

        Eigen::MatrixXd X(in_the_money.size(), N_regressors);         
                                                               		
        Vector y;
        Eigen::VectorXd X_(in_the_money.size());
        for (size_t j = 0; j < in_the_money.size(); ++j) {
            int idx = in_the_money[j];
            X_(j) = asset_paths[idx][k];
    
            for (int u = k + 1; u <= N_echeances; ++u) {                       
                if (cashflows[idx][u] > 0 || u == N_echeances) {
                    y.push_back(exp(-_r * (u - k) * _T / N_echeances) * cashflows[idx][u]);
                    break;
                }
            }
        }
        X = laguerre_basis(X_, N_regressors);

        Eigen::VectorXd y_vec(y.size());
        for (size_t i = 0; i < y.size(); ++i) {
            y_vec(i) = y[i];
        }
        Eigen::VectorXd regression = (X.transpose() * X).ldlt().solve(X.transpose() * y_vec);  
    	Eigen::VectorXd estimated_cashflows = X * regression;
        for (size_t j = 0; j < in_the_money.size(); ++j) {
            int idx = in_the_money[j];
            if (cashflows[idx][k] < estimated_cashflows(j)) {
                cashflows[idx][k] = 0.0;
            }
        }
    }

    Vector mc;
    for (const auto& cashflow : cashflows) {
        for (size_t i = 1; i < cashflow.size(); ++i) {
            if (cashflow[i] > 0) {
                mc.push_back(cashflow[i] * exp(-_r * i * _T / N_echeances));
                break;
            }
        }
    }

    double price = reduce(begin(mc), end(mc), 0.0) / mc.size();
    cout << "American Style Price = " << price << endl;
	return price;
}

AmericanOptionPricing* AmericanOptionPricing::clone() const
{
	return new AmericanOptionPricing(*this);
}


AsianOptionPricing::AsianOptionPricing(double strike, CALL_PUT call_put, double r, double T)
	: _strike(strike), _call_put(call_put), _r(r), _T(T)
{
}

double AsianOptionPricing::price(Matrix asset_paths) const
{
	Vector prices;
	double multpilicative_factor = _call_put == CALL_PUT::CALL ? 1. : -1.;

	for (size_t k=0; k<asset_paths.size(); k++){
		double mean_price = reduce(begin(asset_paths[k]), end(asset_paths[k]), 0.0) / asset_paths[k].size();
		double payoff = max(multpilicative_factor * (mean_price - _strike), 0.);
		payoff = payoff * exp(-_r * _T);
		prices.push_back(payoff);
	}

	double price = accumulate(begin(prices), end(prices), 0);
	price = price / prices.size();

	return price;
}

AsianOptionPricing* AsianOptionPricing::clone() const
{
	return new AsianOptionPricing(*this);
}

