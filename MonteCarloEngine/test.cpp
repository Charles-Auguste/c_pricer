#include "MonteCarlo.h"


double normalCDF(double x)
{
	return std::erfc(-x / std::sqrt(2)) / 2;
}

double BsCallPrice(
    const double& K, 
    const double& T, 
    const double& r, 
    const double& sigma,
    const double& S
) {

	double d1 = (log(S/K) + (r + 0.5*sigma*sigma)*T)/ (sigma * pow(T, 0.5));
	double d2 = d1 - sigma * pow(T, 0.5);

	return S * normalCDF(d1) - K * exp(-r * T) * normalCDF(d2);
}

int main(){
    cout << "Test MonteCarloEngine" <<endl;
    cout << "---------------------" <<endl << endl;

    // 1) Parameters
    double S0 = 100.;
    double drift = 0.03;
    double volatility = 0.1;

    double T= 1;
    size_t nb_points = 1;

    double K = 105;
    CALL_PUT type_option = CALL_PUT::CALL;

    size_t nb_sims = 100000;

    // 2) Model, PathSimulator & Payoff
    BlackScholesModel bs_model(S0, drift, volatility);
    EulerPathSimulator bs_path(&bs_model,T,nb_points);
    EuropeanOptionPayoff bs_payoff(K,type_option);

    // 3) Simulations
    MonteCarlo simulation(nb_sims,bs_path, bs_payoff);
    double price = simulation.price();
    double expected_result = BsCallPrice(K,T,drift,volatility, S0);
    cout << endl << "Expected result :" << expected_result << "       Real result : " << price << endl;
    return 0;
};
