#include "../src/Calibration/Calibration.h"
#include "MonteCarlo.h"

int test_MonteCarlo() {
  cout << "Test MonteCarloEngine" << endl;
  cout << "---------------------" << endl << endl;

  // 1) Parameters
  double S0 = 100.;
  double r = 0.03;
  double volatility = 0.2;

  double T = 1;
  size_t nb_points = 100;

  double K = 105;
  CALL_PUT type_option = CALL_PUT::CALL;

  size_t nb_sims = 1000;

  EuropeanOptionPricing pricing_eu(K, type_option, r, T);
  AsianOptionPricing pricing_as(K, type_option, r, T);

  // 2) Model, PathSimulator & Payoff
  BlackScholesModel bs_model(S0, r, volatility);
  EulerPathSimulator bs_path(&bs_model, T, nb_points);

  // 3) Simulations
  MonteCarlo bs_simulation_eu(nb_sims, bs_path, pricing_eu);
  MonteCarlo bs_simulation_as(nb_sims, bs_path, pricing_as);
  double price_eu = bs_simulation_eu.price();
  double price_as = bs_simulation_as.price();
  cout << "Black & Scholes" << endl;
  cout << endl
       << "European : " << price_eu << "   Asian : " << price_as << endl;

  // 4)  Parameters
  Vector init_values{100., 0.04};
  double theta = 0.04;
  double kappa = 1;
  double sigma = 0.05;
  double rho = -0.7;

  // 5) Heston model
  HestonModel he_model(init_values, kappa, sigma, theta, r);
  MdEulerPathSimulator he_path(&he_model, T, nb_points, rho);

  // 6) Simulations
  MdMonteCarlo he_simulation_eu(nb_sims, he_path, pricing_eu);
  MdMonteCarlo he_simulation_as(nb_sims, he_path, pricing_as);
  price_eu = he_simulation_eu.price();
  price_as = he_simulation_as.price();
  cout << "Heston" << endl;
  cout << endl
       << "European : " << price_eu << "   Asian : " << price_as << endl
       << endl;
  return 0;
};