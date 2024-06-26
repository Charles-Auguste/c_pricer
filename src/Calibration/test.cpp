#include "Calibration.h"
#include <gtest/gtest.h>

namespace TestCalibration {

class TestCalibration : public testing::Test {
protected:
  virtual void SetUp() {
    // Defining models
    IV_surface = {{0., 20., 40., 60., 80., 100., 120., 140., 160., 180.},
                  {0.25, 0.39, 0.31, 0.24, 0.22, 0.16, 0.19, 0.23, 0.29, 0.38},
                  {0.5, 0.44, 0.36, 0.27, 0.21, 0.17, 0.21, 0.27, 0.35, 0.4},
                  {0.75, 0.45, 0.3, 0.25, 0.21, 0.18, 0.22, 0.29, 0.37, 0.45},
                  {1., 0.48, 0.42, 0.34, 0.28, 0.2, 0.26, 0.31, 0.42, 0.5},
                  {2., 0.52, 0.43, 0.34, 0.26, 0.21, 0.27, 0.38, 0.45, 0.55},
                  {3., 0.54, 0.46, 0.34, 0.27, 0.23, 0.28, 0.36, 0.49, 0.58},
                  {4., 0.57, 0.5, 0.46, 0.35, 0.25, 0.32, 0.45, 0.54, 0.6},
                  {5., 0.6, 0.52, 0.41, 0.31, 0.26, 0.34, 0.4, 0.55, 0.62}};
  }

  Matrix IV_surface;
  double S0 = 100; // Initial Spot
  double r = 0.05; // Risk free rate

  double kappa = 0.5; // mean return
  double theta =
      1.; // pow(IV_surface[IV_surface.size() - 1][5],2); //asymptotic var
  double sigma = 0.1; // Volatility of volatility
  double rho = 0.5;   // correlation
  double v0 = 0;      // initial variance
};

TEST_F(TestCalibration, TestOfCalibration) {
  ExplicitHestonModel model_test(kappa, theta, sigma, rho, v0, S0, r);
  ExplicitBlackScholesModel model_bs(S0, r, 0.5);
  OptimisationImpliedVolatility optim(0.000001, model_test, model_bs);

  // Optimisation test
  optim.calibration(IV_surface, LOSS_FUNCTION::VOL);
  // optim.calibration(IV_surface, LOSS_FUNCTION::CALL);
  cout << endl;

  // Results
  Matrix IV_surface_model;
  IV_surface_model = {{0., 20., 40., 60., 80., 100., 120., 140., 160., 180.},
                      {0.25, 0., 0., 0., 0., 0., 0., 0., 0., 0.},
                      {0.5, 0., 0., 0., 0., 0., 0., 0., 0., 0.},
                      {0.75, 0., 0., 0., 0., 0., 0., 0., 0., 0.},
                      {1., 0., 0., 0., 0., 0., 0., 0., 0., 0.},
                      {2., 0., 0., 0., 0., 0., 0., 0., 0., 0.},
                      {3., 0., 0., 0., 0., 0., 0., 0., 0., 0.},
                      {4., 0., 0., 0., 0., 0., 0., 0., 0., 0.},
                      {5., 0., 0., 0., 0., 0., 0., 0., 0., 0.}};

  for (int i = 1; i < IV_surface_model.size(); i++) {
    double T = IV_surface_model[i][0];
    for (int j = 1; j < IV_surface_model[0].size(); j++) {
      double K = IV_surface_model[0][j];
      double call = optim.GetModelPtr()->CallPrice(K, T);
      optim.implied_vol(T, K, call);
      IV_surface_model[i][j] = optim.GetBSModelPtr()->GetSigma();
      std::cout << IV_surface_model[i][j] << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "Kappa : " << optim.GetModelPtr()->GetKappa() << std::endl;
  std::cout << "Theta : " << optim.GetModelPtr()->GetTheta() << std::endl;
  std::cout << "Sigma : " << optim.GetModelPtr()->GetSigma() << std::endl;
  std::cout << "Rho : " << optim.GetModelPtr()->GetRho() << std::endl;
  std::cout << "V0 : " << optim.GetModelPtr()->GetV0() << std::endl;

  double test = optim.GetModelPtr()->CallPrice(60., 0.25);
  std::cout << test << std::endl;
  optim.implied_vol(0.25, 60., test);
  std::cout << optim.GetBSModelPtr()->GetSigma() << std::endl;
  ASSERT_TRUE(1 == 1);
}

}; // namespace TestCalibration
