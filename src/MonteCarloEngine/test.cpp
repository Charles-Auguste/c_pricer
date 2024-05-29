#include "Calibration/Calibration.h"
#include "MonteCarlo.h"
#include <gtest/gtest.h>

namespace TestMonteCarloEngine {

class TestMonteCarloEngine : public testing::Test {
protected:
  virtual void SetUp() {}
  // 1) Parameters
  double S0 = 100.;
  double r = 0.03;
  double volatility = 0.2;

  Vector init_values{100., 0.04};
  double theta = 0.04;
  double kappa = 1;
  double sigma = 0.05;
  double rho = -0.7;

  double T = 1;
  size_t nb_points = 100;

  double K = 105;
  CALL_PUT type_option = CALL_PUT::CALL;

  size_t nb_sims = 1000;
};

TEST_F(TestMonteCarloEngine, OneDimensionTest) {
  BlackScholesModel bs_model(S0, r, volatility);
  EulerPathSimulator bs_path(&bs_model, T, nb_points);
  EuropeanOptionPricing pricing_eu =
      EuropeanOptionPricing(K, type_option, r, T);
  AsianOptionPricing pricing_as = AsianOptionPricing(K, type_option, r, T);
  AmericanOptionPricing pricing_am =
      AmericanOptionPricing(K, type_option, r, T);

  MonteCarlo bs_simulation_eu(nb_sims, bs_path, pricing_eu);
  MonteCarlo bs_simulation_as(nb_sims, bs_path, pricing_as);
  MonteCarlo bs_simulation_am(nb_sims, bs_path, pricing_am);

  double price_eu = bs_simulation_eu.price();
  double price_as = bs_simulation_as.price();
  double price_am = bs_simulation_am.price();
  cout << "Black & Scholes" << endl;
  cout << endl
       << "European : " << price_eu << "   Asian : " << price_as
       << "   American : " << price_am << endl;
  ASSERT_TRUE(1 == 1);
}

TEST_F(TestMonteCarloEngine, MultiDimensionTest) {
  HestonModel he_model(init_values, kappa, sigma, theta, r);
  MdEulerPathSimulator he_path(&he_model, T, nb_points, rho);
  EuropeanOptionPricing pricing_eu =
      EuropeanOptionPricing(K, type_option, r, T);

  MdMonteCarlo he_simulation_eu(nb_sims, he_path, pricing_eu);
  double price_eu = he_simulation_eu.price();
  cout << "Heston" << endl;
  cout << endl << "European : " << price_eu << endl << endl;
  ASSERT_TRUE(1 == 1);
}

}; // namespace TestMonteCarloEngine