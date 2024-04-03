#pragma once
#include <vector>
#include "Pricer.h"

class OptimisationImpliedVolatility
{
public :

	// constructeurs

	OptimisationImpliedVolatility();
	OptimisationImpliedVolatility(const double& epsilon, const HestonModel& model, const BlackScholesModel& bs_model);
	OptimisationImpliedVolatility(const OptimisationImpliedVolatility& optim);

	// destructeurs

	~OptimisationImpliedVolatility() = default;

	// Getteurs

	HestonModel* GetModelPtr() const;
	BlackScholesModel* GetBSModelPtr() const;
	double GetEpsilon() const;

	// Setteurs

	void SetEpsilon(const double& epsilon);

	// Pas d'opérateur d'assignation, on veut que le pointeur du modèle soit constant (mais le modèle 
	// va être amener à changer au cours de la calibration)


	// Calcul de la vol implicite (par dichotomie)

	void implied_vol(const double& T, const double& K, const double& C) const;



	// Calcul de la loss function à partir d'une nappe de volatilité

	
	double loss_function(const std::vector<std::vector<double>>& IV_surface) const; //basé sur erreur L2 des vols avec pondération en vega

	double loss_function_bis(const std::vector<std::vector<double>>& IV_surface) const; //basé sur erreur L2 des prix avec pondération en vega^2


	// Optimisation de la loss function (avec Nelder Mead : algo du simplex)

	void calibration(const std::vector<std::vector<double>>& IV_surface) const;

	void calibration_bis(const std::vector<std::vector<double>>& IV_surface) const;

	


private :

	double _epsilon_iv; // erreur tolérée pour le calcul de l'IV 
	HestonModel* const _model_ptr; // pointeur constant vers le modèle dont on cherche à optimiser les paramètres
	BlackScholesModel* const _bs_model_ptr; // pointeur constant vers un modèle BS : calcule de la vol impli


};
