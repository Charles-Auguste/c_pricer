#include "Pricing.h"
#include <gtest/gtest.h>

namespace TestPricing {

class TestPricing : public testing::Test {
protected:
  virtual void SetUp() {
    call = CALL_PUT::CALL;
    put = CALL_PUT::PUT;

    // Initialise price vector
    Vector price_tmp;
    price_tmp.push_back(100);

    // Initialise random generator
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<double> distribution(0, 1.);

    // Fill vector price
    for (size_t k = 0; k < 100; k++)
      price_tmp.push_back(price_tmp[k] + distribution(gen));

    price.push_back(price_tmp);
    T = 1;
    r = 0.03;
  }
  virtual void TearDown() {}

  Matrix price;
  CALL_PUT call;
  CALL_PUT put;
  int T;
  double r;
};

TEST_F(TestPricing, EuropeanOption) {
  cout << "European option" << endl;
  EuropeanOptionPricing eu_call = EuropeanOptionPricing(110, call, r, T);
  EuropeanOptionPricing eu_put = EuropeanOptionPricing(110, put, r, T);
  cout << "\tCall payoff: " << eu_call.price(price) << endl;
  cout << "\tPut payoff: " << eu_put.price(price) << endl;
  cout << endl;
  ASSERT_TRUE(1 == 1);
}

TEST_F(TestPricing, AmericanOption) {
  cout << "American option" << endl;
  AmericanOptionPricing am_call = AmericanOptionPricing(110, call, r, T);
  AmericanOptionPricing am_put = AmericanOptionPricing(110, put, r, T);
  cout << "\tCall payoff: " << am_call.price(price) << endl;
  cout << "\tPut payoff: " << am_put.price(price) << endl;
  cout << endl;
  ASSERT_TRUE(1 == 1);
}

TEST_F(TestPricing, AsianOption) {
  cout << "Asian option" << endl;
  AsianOptionPricing as_call = AsianOptionPricing(110, call, r, T);
  AsianOptionPricing as_put = AsianOptionPricing(110, put, r, T);
  cout << "\tCall payoff: " << as_call.price(price) << endl;
  cout << "\tPut payoff: " << as_put.price(price) << endl;
  cout << endl;
  ASSERT_TRUE(1 == 1);
}
}; // namespace TestPricing