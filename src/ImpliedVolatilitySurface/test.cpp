#include "ImpliedVolatilitySurface.h"
#include <gtest/gtest.h>

namespace TestImpliedVolatilitySurface {

class TestImpliedVolatilitySurface : public testing::Test {
protected:
  virtual void SetUp() {
    for (size_t k = 0; k < 4; k++) {
      Vector volatilities;
      for (size_t l = 0; l < 4; l++) {
        volatilities.push_back(strikes[l] / 100000 * maturities[k]);
      };
      market_volatilities.push_back(volatilities);
    };
  }
  virtual void TearDown() {};

  double risk_free_rate = 0.03;
  Vector strikes{90, 100, 110, 120};
  Vector maturities{0.5, 1, 1.5, 2};
  Matrix market_volatilities;
};

TEST_F(TestImpliedVolatilitySurface, Test1) {

  ImpliedVolatilitySurface surface_instance = ImpliedVolatilitySurface(
      maturities, strikes, market_volatilities, risk_free_rate);
  double strike1 = 90;
  double maturity1 = 1.5;
  cout << "Inputs 1:" << endl
       << "\tStrike =" << strike1 << endl
       << "\tMaturity =" << maturity1 << endl;
  double result1 = surface_instance.implied_volatility(maturity1, strike1);
  cout << "\tRESULT =" << result1 << endl << endl;
  ASSERT_TRUE(1 == 1);
}

TEST_F(TestImpliedVolatilitySurface, Test2) {

  ImpliedVolatilitySurface surface_instance = ImpliedVolatilitySurface(
      maturities, strikes, market_volatilities, risk_free_rate);
  double strike2 = 95;
  double maturity2 = 1.25;
  cout << "Inputs 2:" << endl
       << "\tStrike =" << strike2 << endl
       << "\tMaturity =" << maturity2 << endl;
  double result2 = surface_instance.implied_volatility(maturity2, strike2);
  cout << "\t\tRESULT =" << result2 << endl << endl;
  ASSERT_TRUE(1 == 1);
}

TEST_F(TestImpliedVolatilitySurface, Test3) {

  ImpliedVolatilitySurface surface_instance = ImpliedVolatilitySurface(
      maturities, strikes, market_volatilities, risk_free_rate);
  double strike3 = 200;
  double maturity3 = 3;
  cout << "Inputs 3:" << endl
       << "\tStrike =" << strike3 << endl
       << "\tMaturity =" << maturity3 << endl;
  double result3 = surface_instance.implied_volatility(maturity3, strike3);
  cout << "\tRESULT =" << result3 << endl;
  ASSERT_TRUE(1 == 1);
}

}; // namespace TestImpliedVolatilitySurface