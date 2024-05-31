// Ce script suit point par point les étapes du projet

#include <MonteCarloEngine/MonteCarlo.h>
#include <Calibration/Calibration.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv) {
     cout << endl << "ETAPE 1 ----> Black Scholes et Volatilité Implicite" << endl;
     // Parameters of the model
     double S0 = 100.; // Spot
     double r = 0.03; // Risk free rate
     double sigma = 0.2; // Volatilty

     // Parameters for the option
     double T = 1.;   // Expiry
     double K = 105.; // Strike
     CALL_PUT type_option = CALL_PUT::CALL;
     
     // Parameters for Monte Carlo Engine
     size_t nb_simulation = 50000;
     size_t timestep = 300;

     // Explicit formula
     ExplicitBlackScholesModel model_bs_explicit(S0, r, sigma);
     double call_price_bs_explicit = model_bs_explicit.CallPrice(K, T);
     
     // MonteCarlo
     BlackScholesModel bs_model(S0, r, sigma);
     EulerPathSimulator bs_path(&bs_model, T, timestep);

     EuropeanOptionPricing pricing_eu_bs = EuropeanOptionPricing(K, type_option, r, T);

     MonteCarlo bs_simulation_eu(nb_simulation, bs_path, pricing_eu_bs);

     double call_price_bs_eu = bs_simulation_eu.price();

     cout << "European Call" << endl;
     cout << "Explicit price : " << call_price_bs_explicit << " | MonteCarlo price : " << call_price_bs_eu << endl;

     return 0;
}