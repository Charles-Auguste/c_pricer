#include "PathSimulator.h"


int main(){
    cout << "Test PathSimulator" <<endl;
    cout << "------------------" <<endl << endl;
    
    // 1) Parameters
    double init_value = 100; // Initial spot price
    // For B&S and Bachelier
    double drift = 0.05;
    double volatility = 0.2;
    // For heston
    double r = 0.0319;     // Risk-free rate
    double v_0 = 0.010201; // Initial volatility 
    double rho = -0.7;     // Correlation of asset and volatility
    double kappa = 6.21;   // Mean-reversion rate
    double theta = 0.019;  // Long run average volatility
    double xi = 0.61;      // "Vol of vol"
    // For simulation
    double T = 1.00;       // One year until expiry
    double nb_plot = 10;
    
    // 2) Generate a path for black & scholes model
    cout << "Black & Scholes model" << endl;
    BlackScholesModel bs_model(init_value, drift, volatility);
    cout << "BS model address: " << &bs_model << endl<< endl;


    EulerPathSimulator bs_path = EulerPathSimulator(&bs_model, T, nb_plot);
    Vector bs_path_vect = bs_path.path();
    
    for (size_t idx = 0; idx < bs_path_vect.size(); idx++){
        cout << bs_path_vect[idx] << ", ";
    };
    cout << endl << endl;


    // 3) Generate a path for Heston model
    cout << "Heston model" << endl;
    Vector init_values_heston {init_value, v_0};
    HestonModel he_model(init_values_heston, kappa, xi, theta, r);
    cout << "Heston model address: " << &he_model << endl<< endl;


    MdEulerPathSimulator he_path = MdEulerPathSimulator(&he_model, T, nb_plot, rho);
    Vector he_path_matrix = he_path.path();
    
    for (size_t idx = 0; idx < he_path_matrix.size(); idx++){
        cout << he_path_matrix[idx] << ", ";
    };
    cout << endl << endl;

    return 0;
};