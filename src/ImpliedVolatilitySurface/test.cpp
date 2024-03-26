#include "ImpliedVolatilitySurface.h"

int main(){
    cout << "Test for Implied Volatility surface" <<endl;
    cout << "-----------------------------------" <<endl << endl;

    Vector strikes {90, 100, 110, 120};
    Vector maturities {0.5, 1, 1.5, 2};
    Matrix market_volatilities;
    for (size_t k = 0; k < 4; k++){
        Vector volatilities;
        for (size_t l = 0; l < 4; l++){
            volatilities.push_back(strikes[l]/100000 * maturities[k]);
        };
        market_volatilities.push_back(volatilities);
    };
    double risk_free_rate = 0.03;

    ImpliedVolatilitySurface surface_instance = ImpliedVolatilitySurface(maturities, strikes, market_volatilities, risk_free_rate);

    double strike1 = 90;
    double maturity1 = 1.5;
    cout << "Inputs 1:" << endl << "Strike =" << strike1 << endl << "Maturity =" << maturity1 << endl;
    double result1 = surface_instance.implied_volatility(maturity1, strike1);
    cout << "RESULT ="<< result1 <<endl << endl;

    double strike2 = 95;
    double maturity2 = 1.25;
    cout << "Inputs 2:" << endl << "Strike =" << strike2 << endl << "Maturity =" << maturity2 << endl;
    double result2 = surface_instance.implied_volatility(maturity2, strike2);
    cout << "RESULT ="<< result2 <<endl << endl;

    double strike3 = 200;
    double maturity3 = 3;
    cout << "Inputs 3:" << endl << "Strike =" << strike3 << endl << "Maturity =" << maturity3 << endl;
    double result3 = surface_instance.implied_volatility(maturity3, strike3);
    cout << "RESULT ="<< result3 <<endl;

    return 0;
};
