#include "Model.h"


int main(){
    cout << "Test Model" <<endl;
    cout << "----------" <<endl << endl;
    
    // 1) Parameters
    double init_value = 100;
    // For B&S and Bachelier
    double drift = 0.05;
    double volatility = 0.2;
    // For Dupire
    double epsilon_maturity = 0.0001;
    double epsilon_strike = 100;
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

    // 2) BlackScholesModel
    cout << "BlackScholesModel" << endl;
    BlackScholesModel bs_model = BlackScholesModel(init_value, drift, volatility);
    cout << "Initial value: " << bs_model.init_value() << endl;
    cout << "Drift term: " << bs_model.drift_term(0, 100) << endl;
    cout << "Diffusion term: " << bs_model.diffusion_term(0, 100) << endl;
    cout << endl;

    // 3) BachelierModel
    cout << "BachelierModel" << endl;
    BachelierModel ba_model = BachelierModel(init_value, drift, volatility);
    cout << "Initial value: " << ba_model.init_value() << endl;
    cout << "Drift term: " << ba_model.drift_term(0, 100) << endl;
    cout << "Diffusion term: " << ba_model.diffusion_term(0, 100) << endl;
    cout << endl;

    // 3) DupireLocalVolatilityModel
    cout << "DupireLocalVolatilityModel" << endl;
    DupireLocalVolatilityModel du_model = DupireLocalVolatilityModel(init_value, surface_instance, epsilon_maturity, epsilon_strike);
    cout << "Initial value: " << du_model.init_value() << endl;
    cout << "Drift term: " << du_model.drift_term(1, 100) << endl;
    cout << "Diffusion term: " << du_model.diffusion_term(1, 100) << endl;
    cout << endl;

    return 0;
};
