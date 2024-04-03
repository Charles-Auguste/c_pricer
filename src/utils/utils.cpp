#include "Calibration.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <numeric>
#include <numbers>


const double pi = 2 * acos(0.0);


// Constructeurs

OptimisationImpliedVolatility::OptimisationImpliedVolatility() :

	_epsilon_iv(0.005),
	_model_ptr(new HestonModel),
	_bs_model_ptr(new BlackScholesModel)
{}

OptimisationImpliedVolatility::OptimisationImpliedVolatility(const double& epsilon, const HestonModel& model, const BlackScholesModel& bs_model) :

	_epsilon_iv(epsilon),
	_model_ptr(model.clone()),
	_bs_model_ptr(bs_model.clone())

{};


OptimisationImpliedVolatility::OptimisationImpliedVolatility(const OptimisationImpliedVolatility& optim):
	
	_epsilon_iv(optim._epsilon_iv),
	_model_ptr(optim._model_ptr),
	_bs_model_ptr(optim._bs_model_ptr)

{};


// Getteurs

HestonModel* OptimisationImpliedVolatility::GetModelPtr() const {

	return _model_ptr;
}

BlackScholesModel* OptimisationImpliedVolatility::GetBSModelPtr() const {

	return _bs_model_ptr;
}


double OptimisationImpliedVolatility::GetEpsilon() const {

	return _epsilon_iv;
}

// Setteurs


void OptimisationImpliedVolatility::SetEpsilon(const double& epsilon) {

	_epsilon_iv = epsilon;
}

// Calcul de la vol impli


void OptimisationImpliedVolatility::implied_vol(const double& T, const double& K, const double& C) const {


	double a = 0.;
	double b = 3;

	double mid = 0.5*(a+b);
	_bs_model_ptr->SetSigma(mid);
	double BS = _bs_model_ptr->CallPrice(K, T);

	while (abs(C - BS) >= _epsilon_iv) {

		if (C > BS) {

			a = mid;
			mid = 0.5 * (a + b);
			_bs_model_ptr->SetSigma(mid);
			BS = _bs_model_ptr->CallPrice(K, T);
		}

		else {

			b = mid;
			mid = 0.5 * (a + b);
			_bs_model_ptr->SetSigma(mid);
			BS = _bs_model_ptr->CallPrice(K, T);
		}

	}

}





// Calcul de la Loss function


double OptimisationImpliedVolatility::loss_function(const std::vector<std::vector<double>>& IV_surface) const {

	// Le premier élément du tableau est vide (ou zero)
	// La première colonne contient les maturités (à partir de la deuxième ligne)
	// La première ligne contient les strikes (à partie de la première colonne)
	// Ainsi l'élement à la i-ème ligne et j-ème colonne du tableau (i>=1 j>=1) 
	// contient la vol impli pour le prix C(K_j,T_i).

	double loss = 0.;
	int maturity_size = IV_surface.size();
	int strike_size = IV_surface[0].size();
	double T;
	double K;
	double call;
	double somme_poids=0.;

	for (int i = 1; i < maturity_size; i++) {

		T = IV_surface[i][0];
		for (int j = 1; j < strike_size; j++) {
			K = IV_surface[0][j];
			call = _model_ptr->CallPrice(K, T); // prix avec heston
			implied_vol(T, K, call);
            loss += pow(_bs_model_ptr->GetSigma() - IV_surface[i][j], 2) * _bs_model_ptr->Vega(K, T, IV_surface[i][j]);
			somme_poids += _bs_model_ptr->Vega(K, T, IV_surface[i][j]);

		}
		
	}
	return loss/somme_poids;


}

double OptimisationImpliedVolatility::loss_function_bis(const std::vector<std::vector<double>>& IV_surface) const {

	double loss = 0.;
	int maturity_size = IV_surface.size();
	int strike_size = IV_surface[0].size();
	double T;
	double K;
	double call_model;
	double call_market;


	for (int i = 1; i < maturity_size; i++) {

		T = IV_surface[i][0];
		for (int j = 1; j < strike_size; j++) {

			K = IV_surface[0][j];
			call_model = _model_ptr->CallPrice(K, T); // prix avec heston
			_bs_model_ptr->SetSigma(IV_surface[i][j]);
			call_market = _bs_model_ptr->CallPrice(K, T);

			
			loss += pow(call_model - call_market, 2); ///(_bs_model_ptr->Vega(K,T,IV_surface[i][j]));
		
		}

	}
	return loss;


}


void OptimisationImpliedVolatility::calibration(const std::vector<std::vector<double>>& IV_surface) const {

	//Utilisation descente de gradient

	const int N = 5; // Nombre de paramètres à calibrer (kappa, theta, sigma, rho, v0)
	const int nb_iter = 1000;
	const double dx = 1.0E-5;
	const double rate = 1.0E-8;
	const double tol = 1.0E-5; // Critère d'arret pour l'optim
	bool arret = true; //True si on continue l'optim et false sinon

	std::vector<double> gradient(N, 0.);
	double loss;
	double loss_dx;
	std::vector<double> params(N, 0.);


	// Initialisation du vecteur de paramètres

	params[0] = _model_ptr->GetKappa();
	params[1] = _model_ptr->GetTheta();
	params[2] = _model_ptr->GetSigma();
	params[3] = _model_ptr->GetRho();
	params[4] = _model_ptr->GetV0();


	int k = 0;


	while (k < nb_iter and arret) {

		//// Calcul du gradient 

		loss = loss_function(IV_surface);
		std::cout << "Fonction d'erreur :" << loss << std::endl;

		// Première coordonée


		_model_ptr->SetKappa(params[0] + dx);
		loss_dx = loss_function(IV_surface);
		_model_ptr->SetKappa(params[0]);

		gradient[0] = (loss_dx - loss) / dx;
		//std::cout << "gradient 0 " << gradient[0] << std::endl;



		// Deuxième 

		_model_ptr->SetTheta(params[1] + dx);
		loss_dx = loss_function(IV_surface);
		_model_ptr->SetTheta(params[1]);

		gradient[1] = (loss_dx - loss) / dx;


		// Troisème 

		_model_ptr->SetSigma(params[2] + dx);
		loss_dx = loss_function(IV_surface);
		_model_ptr->SetSigma(params[2]);

		gradient[2] = (loss_dx - loss) / dx;


		// Quatrième

		_model_ptr->SetRho(params[3] + dx);
		loss_dx = loss_function(IV_surface);
		_model_ptr->SetRho(params[3]);

		gradient[3] = (loss_dx - loss) / dx;


		// Cinquième

		_model_ptr->SetV0(params[4] + dx);
		loss_dx = loss_function(IV_surface);
		_model_ptr->SetV0(params[4]);

		gradient[4] = (loss_dx - loss) / dx;


		/// Mise à jour de params

		for (int i = 0; i < N; i++) {

			params[i] = params[i] - rate * gradient[i];


			if (i == 3) {

				params[i] = std::max(-1., std::min(params[i], 1.));

			}
			else {

				params[i] = std::max(0., params[i]);
			}


		}

		/// Mise à jour des attributs du modèle

		_model_ptr->SetKappa(params[0]);
		_model_ptr->SetTheta(params[1]);
		_model_ptr->SetSigma(params[2]);
		_model_ptr->SetRho(params[3]);
		_model_ptr->SetV0(params[4]);


		// Incrémentation de k
		k++;

		// Mise à jour du bool


		loss_dx = loss_function_bis(IV_surface);

		arret = abs(((loss - loss_dx) / loss)) > tol;


	}

}


void OptimisationImpliedVolatility::calibration_bis(const std::vector<std::vector<double>>& IV_surface) const {

	//Utilisation descente de gradient

	const int N = 5; // Nombre de paramètres à calibrer (kappa, theta, sigma, rho, v0)
	const int nb_iter = 5000; //Nombres itérations maximum
	const double dx = 1.0E-3;
	const double rate = 1.0E-6; // pas d'apprentissage
	const double tol = 1.0E-10; // Critère d'arret pour l'optim
	bool arret = true; //True si on continue l'optim et false sinon

	std::vector<double> gradient(N, 0.);
	double loss;
	double loss_dx;
	std::vector<double> params(N, 0.);


	// Initialisation du vecteur de paramètres

	params[0] = _model_ptr->GetKappa();
	params[1] = _model_ptr->GetTheta();
	params[2] = _model_ptr->GetSigma();
	params[3] = _model_ptr->GetRho();
	params[4] = _model_ptr->GetV0();


	int k = 0;


	while (k < nb_iter and arret) {

		//// Calcul du gradient 

		loss = loss_function_bis(IV_surface);
		std::cout << "Fonction d'erreur :"<<loss << std::endl;

		// Première coordonée


		_model_ptr->SetKappa(params[0] + dx);
		loss_dx = loss_function_bis(IV_surface);
		_model_ptr->SetKappa(params[0]);

		gradient[0] = (loss_dx - loss) / dx;
		//std::cout << "gradient 0 " << gradient[0] << std::endl;



		// Deuxième 

		_model_ptr->SetTheta(params[1] + dx);
		loss_dx = loss_function_bis(IV_surface);
		_model_ptr->SetTheta(params[1]);

		gradient[1] = (loss_dx - loss) / dx;


		// Troisème 

		_model_ptr->SetSigma(params[2] + dx);
		loss_dx = loss_function_bis(IV_surface);
		_model_ptr->SetSigma(params[2]);

		gradient[2] = (loss_dx - loss) / dx;


		// Quatrième

		_model_ptr->SetRho(params[3] + dx);
		loss_dx = loss_function_bis(IV_surface);
		_model_ptr->SetRho(params[3]);

		gradient[3] = (loss_dx - loss) / dx;


		// Cinquième

		_model_ptr->SetV0(params[4] + dx);
		loss_dx = loss_function_bis(IV_surface);
		_model_ptr->SetV0(params[4]);

		gradient[4] = (loss_dx - loss) / dx;


		/// Mise à jour de params

		for (int i = 0; i < N; i++) {

			params[i] = params[i] - rate * gradient[i];


			if (i == 3) {

				params[i] = std::max(-1., std::min(params[i], 1.));

			}
			else {

				params[i] = std::max(0., params[i]);
			}


		}



		/// Mise à jour des attributs du modèle

		_model_ptr->SetKappa(params[0]);
		_model_ptr->SetTheta(params[1]);
		_model_ptr->SetSigma(params[2]);
		_model_ptr->SetRho(params[3]);
		_model_ptr->SetV0(params[4]);


		// Incrémentation de k
		k++;

		// Mise à jour du bool


		loss_dx = loss_function_bis(IV_surface);

		arret = abs(((loss - loss_dx) / loss)) > tol;

		



	}

}

    