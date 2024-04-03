#include "Pricer.h"
#include "Calibration.h"
#include <iostream>
#include <vector>



int main() {


	// nappe de vol issue du drive
	std::vector<std::vector<double>> IV_surface;
	IV_surface = { { 0., 20., 40., 60., 80., 100., 120., 140., 160.,	180. },
				{0.25,	0.39,	0.31,	0.24,	0.22,	0.16,	0.19,	0.23,	0.29,	0.38},
				{0.5,	0.44,	0.36,	0.27,	0.21,	0.17,	0.21,	0.27,	0.35,	0.4},
				{0.75,	0.45,	0.3,	0.25,	0.21,	0.18,	0.22,	0.29,	0.37,	0.45},
				{1.,	0.48,	0.42,	0.34,	0.28,	0.2,	0.26,	0.31,	0.42,	0.5},
				{2.,	0.52,	0.43,	0.34,	0.26,	0.21,	0.27,	0.38,	0.45,	0.55},
				{3.,	0.54,	0.46,	0.34,	0.27,	0.23,	0.28,	0.36,	0.49,	0.58},
				{4.,	0.57,	0.5,	0.46,	0.35,	0.25,	0.32,	0.45,	0.54,	0.6},
				{5.,	0.6,	0.52,	0.41,	0.31,	0.26,	0.34,	0.4,	0.55,	0.62}
	};

	double S0 = 100; // Spot initial
	double r = 0.05; // taux en convention continue



	double kappa = 0.133229; // force de retour à la moyenne
	double theta = 0.891253; //pow(IV_surface[IV_surface.size() - 1][5],2); // variance asymptotique
	double sigma = 0.591923; // Vol de vol
	double rho = -0.732991;  // Correlation spot_vol
	double v0 = 0; // Variance initiale

	// Création d'une instance HestonModel et BlackScholesModel
	HestonModel model_test(kappa, theta, sigma, rho, v0, S0, r);
	BlackScholesModel model_bs(S0, r, 0.5);

	// Création d'une instance OptimisationImpliedVolatility
	OptimisationImpliedVolatility optim(0.000001, model_test, model_bs);

	
	// test de l'optimisation

	optim.calibration_bis(IV_surface);
	std::cout << std::endl;

	
	// On sort la nappe de vol issue du modèle calibré

	std::vector<std::vector<double>> IV_surface_model;
	IV_surface_model = { { 0., 20., 40., 60., 80., 100., 120., 140., 160.,	180. },
				{0.25,	0.39,	0.31,	0.24,	0.22,	0.16,	0.19,	0.23,	0.29,	0.38},
				{0.5,	0.44,	0.36,	0.27,	0.21,	0.17,	0.21,	0.27,	0.35,	0.4},
				{0.75,	0.45,	0.3,	0.25,	0.21,	0.18,	0.22,	0.29,	0.37,	0.45},
				{1.,	0.48,	0.42,	0.34,	0.28,	0.2,	0.26,	0.31,	0.42,	0.5},
				{2.,	0.52,	0.43,	0.34,	0.26,	0.21,	0.27,	0.38,	0.45,	0.55},
				{3.,	0.54,	0.46,	0.34,	0.27,	0.23,	0.28,	0.36,	0.49,	0.58},
				{4.,	0.57,	0.5,	0.46,	0.35,	0.25,	0.32,	0.45,	0.54,	0.6},
				{5.,	0.6,	0.52,	0.41,	0.31,	0.26,	0.34,	0.4,	0.55,	0.62}
	};


	for (int i = 1; i<IV_surface_model.size(); i++) {

		double T = IV_surface_model[i][0];

		for (int j = 1; j<IV_surface_model[0].size();  j++) {

			double K = IV_surface_model[0][j];
			double call = optim.GetModelPtr()->CallPrice(K, T);
			optim.implied_vol(T, K, call);
			IV_surface_model[i][j] = optim.GetBSModelPtr()->GetSigma();
		}

	}

	for (int i = 0; i<IV_surface_model.size(); i++) {

		for (int j = 0; j<IV_surface_model[0].size(); j++) {

			std::cout << IV_surface_model[i][j]<< " ";
		}
		std::cout<<std::endl;


	}

	std::cout << "Kappa " << optim.GetModelPtr()->GetKappa() << std::endl;
	std::cout << "Theta " << optim.GetModelPtr()->GetTheta() << std::endl;
	std::cout << "Sigma " << optim.GetModelPtr()->GetSigma() << std::endl;
	std::cout << "Rho " << optim.GetModelPtr()->GetRho() << std::endl;
	std::cout << "V0 " << optim.GetModelPtr()->GetV0() << std::endl;

	double test = optim.GetModelPtr()->CallPrice(60., 0.25);
	std::cout << test << std::endl;
	optim.implied_vol(0.25, 60., test);
	std::cout << optim.GetBSModelPtr()->GetSigma() << std::endl;


	
	return 0;
}