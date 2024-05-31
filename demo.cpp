// Ce script suit point par point les étapes du projet

#include <MonteCarloEngine/MonteCarlo.h>
#include <Calibration/Calibration.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv) {
     cout << endl << "ETAPE 1 ----> Black Scholes et Volatilité Implicite" << endl;
     // Parameters of the model
     double S0 = 100.; // Spot
     double r = 0.03; // Risk free rate
     double sigma = 0.2; // Volatilty

     // Parameters for the option
     double T = 1.;   // Expiry
     double K = 105.; // Strike
     CALL_PUT type_option = CALL_PUT::CALL;
     
     // Parameters for Monte Carlo Engine
     size_t nb_simulation = 1000;
     size_t timestep = 300;

     // Explicit formula
     ExplicitBlackScholesModel model_bs_explicit(S0, r, sigma);
     double call_price_bs_explicit = model_bs_explicit.CallPrice(K, T);
     
     // MonteCarlo
     BlackScholesModel bs_model(S0, r, sigma);
     EulerPathSimulator bs_path(&bs_model, T, timestep);

     EuropeanOptionPricing pricing_eu_bs = EuropeanOptionPricing(K, type_option, r, T);

     MonteCarlo bs_simulation_eu(nb_simulation, bs_path, pricing_eu_bs);

     double call_price_bs_eu = bs_simulation_eu.price();

     cout << "Call Européen" << endl;
     cout << "Prix Explicite : " << call_price_bs_explicit << " | Prix Monte Carlo : " << call_price_bs_eu << endl;

     cout << endl << "ETAPE 2 ----> Matrice de volatilité, interpolation, extrapolation" << endl;

     Matrix IV_SURFACE = {
          {0., 20., 40., 60., 80., 100., 120., 140., 160., 180.},
          {0.25, 0.39, 0.31, 0.24, 0.22, 0.16, 0.19, 0.23, 0.29, 0.38},
          {0.5, 0.44, 0.36, 0.27, 0.21, 0.17, 0.21, 0.27, 0.35, 0.4},
          {0.75, 0.45, 0.3, 0.25, 0.21, 0.18, 0.22, 0.29, 0.37, 0.45},
          {1., 0.48, 0.42, 0.34, 0.28, 0.2, 0.26, 0.31, 0.42, 0.5},
          {2., 0.52, 0.43, 0.34, 0.26, 0.21, 0.27, 0.38, 0.45, 0.55},
          {3., 0.54, 0.46, 0.34, 0.27, 0.23, 0.28, 0.36, 0.49, 0.58},
          {4., 0.57, 0.5, 0.46, 0.35, 0.25, 0.32, 0.45, 0.54, 0.6},
          {5., 0.6, 0.52, 0.41, 0.31, 0.26, 0.34, 0.4, 0.55, 0.62}
     };

     Vector strikes;
     for (size_t k = 1; k < IV_SURFACE[0].size(); k++)
          strikes.push_back(IV_SURFACE[0][k]);

     Vector maturities;
     for (size_t k = 1; k < IV_SURFACE.size(); k++)
          maturities.push_back(IV_SURFACE[k][0]);

     Matrix market_volatilities;
     for (size_t i = 1; i < IV_SURFACE.size(); i++)
     {
          Vector row;
          for (size_t j = 1; j < IV_SURFACE[0].size(); j++)
          {
               row.push_back(IV_SURFACE[i][j]);
          }
          market_volatilities.push_back(row);
     }

     ImpliedVolatilitySurface surface_instance = ImpliedVolatilitySurface(maturities, strikes, market_volatilities, r);

     Vector strikes_tests{10,90,110,150,200};
     Vector test_maturities{0.10,0.6,1.5,4,6};

     for (size_t i = 0; i < strikes_tests.size(); i++)
     {
          double K_test = strikes_tests[i];
          for (size_t j = 0; j < test_maturities.size(); j++)
          {
               double T_test = test_maturities[j];
               double imp_vol = surface_instance.implied_volatility(T_test, K_test);
               cout << "Strike K : " << K_test << " |  Maturité T : " << T_test << " | Volatilité implicite : "<< imp_vol << endl;
          }
     }

     cout << endl << "ETAPE 3 ----> Modèle de Dupire" << endl;

     double epsilon_maturity = 0.0001;
     double epsilon_strike = 0.0001;

     DupireLocalVolatilityModel du_model = DupireLocalVolatilityModel(
          S0, surface_instance, epsilon_maturity, epsilon_strike, r
     );

     // MonteCarlo
     EulerPathSimulator du_path(&du_model, T, timestep);
     MonteCarlo du_simulation_eu(nb_simulation, du_path, pricing_eu_bs);

     AsianOptionPricing pricing_as_du = AsianOptionPricing(K, type_option, r, T);
     MonteCarlo du_simulation_as(nb_simulation, du_path, pricing_as_du);

     double call_price_du_eu = du_simulation_eu.price();
     double call_price_du_as = du_simulation_as.price();

     cout << "Call Européen : " << call_price_du_eu << " | Call Asiatique : " << call_price_du_as << endl;

     cout << endl << "ETAPE 4 ----> Modèle de Heston" << endl;

     return 0;
}