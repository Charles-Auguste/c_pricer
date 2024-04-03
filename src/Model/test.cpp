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
    // For Heston
    Vector initial_asset_vector {100, 0.2};
    double kappa = 0.5;
    double sigma_vol = 0.2;
    double theta = 0.2;
    double interest_rate = 0.03;

    // 2) BlackScholesModel
    cout << "BlackScholesModel" << endl;
    BlackScholesModel bs_model = BlackScholesModel(init_value, drift, volatility);
    cout << "\tInitial value: " << bs_model.init_value() << endl;
    cout << "\tDrift term: " << bs_model.drift_term(1, bs_model.init_value()) << endl;
    cout << "\tDiffusion term: " << bs_model.diffusion_term(1, bs_model.init_value()) << endl;
    cout << endl;

    // 3) BachelierModel
    cout << "BachelierModel" << endl;
    BachelierModel ba_model = BachelierModel(init_value, drift, volatility);
    cout << "\tInitial value: " << ba_model.init_value() << endl;
    cout << "\tDrift term: " << ba_model.drift_term(1, ba_model.init_value()) << endl;
    cout << "\tDiffusion term: " << ba_model.diffusion_term(1, ba_model.init_value()) << endl;
    cout << endl;

    // 3) DupireLocalVolatilityModel
    cout << "DupireLocalVolatilityModel" << endl;
    DupireLocalVolatilityModel du_model = DupireLocalVolatilityModel(init_value, surface_instance, epsilon_maturity, epsilon_strike);
    cout << "\tInitial value: " << du_model.init_value() << endl;
    cout << "\tDrift term: " << du_model.drift_term(1, du_model.init_value()) << endl;
    cout << "\tDiffusion term: " << du_model.diffusion_term(1, du_model.init_value()) << endl;
    cout << endl;

    // 4) HestonModel
    cout << "HestonModel" << endl;
    HestonModel he_model = HestonModel(initial_asset_vector, kappa, sigma_vol, theta, interest_rate);
    cout << "\tInitial value: " << he_model.init_value()[0] << ", " << he_model.init_value()[1] << endl;
    Vector drift_vector = he_model.drift_vector_term(1, initial_asset_vector);
    Vector diffusion_vector = he_model.diffusion_vector_term(1, initial_asset_vector);
    cout << "\tDrift term: " << drift_vector[0] << ", " << drift_vector[1] << endl;
    cout << "\tDiffusion term: " << diffusion_vector[0] << ", " << diffusion_vector[1] << endl;
    cout << endl;

    return 0;
};
