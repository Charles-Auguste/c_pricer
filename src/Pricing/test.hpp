#include "Pricing.h"


int test_Pricing(){
    cout << "Test Pricing" <<endl;
    cout << "------------" <<endl << endl;

    /*
    CALL_PUT call = CALL_PUT::CALL;
    CALL_PUT put = CALL_PUT::PUT;

    // Initialise price vector
    Vector price;
    price.push_back(100);

    // Initialise random generator
    random_device rd;
	mt19937 gen(rd());
    normal_distribution<double> distribution(0, 1.);
	
    // Fill vector price
    for (size_t k=0; k < 100; k++)
        price.push_back(price[k] + distribution(gen));
	 
    // 1) European
    cout << "European option" << endl;
    EuropeanOptionPayoff eu_call = EuropeanOptionPayoff(110, call);
    EuropeanOptionPayoff eu_put = EuropeanOptionPayoff(110, put);
    cout << "\tCall payoff: " << eu_call.payoff(price) << endl;
    cout << "\tPut payoff: " << eu_put.payoff(price) << endl;
    cout << endl;

    // 2) American
    cout << "American option" << endl;
    AmericanPayoff am_call = AmericanPayoff(110, call);
    AmericanPayoff am_put = AmericanPayoff(110, put);
    cout << "\tCall payoff: " << am_call.payoff(price) << endl;
    cout << "\tPut payoff: " << am_put.payoff(price) << endl;
    cout << endl;

    // 3) Asian
    cout << "Asian option" << endl;
    AsianPayoff as_call = AsianPayoff(110, call);
    AsianPayoff as_put = AsianPayoff(110, put);
    cout << "\tCall payoff: " << as_call.payoff(price) << endl;
    cout << "\tPut payoff: " << as_put.payoff(price) << endl;
    cout << endl;

    */
    return 0;
};
