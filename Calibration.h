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

	// Pas d'op�rateur d'assignation, on veut que le pointeur du mod�le soit constant (mais le mod�le 
	// va �tre amener � changer au cours de la calibration)


	// Calcul de la vol implicite (par dichotomie)

	void implied_vol(const double& T, const double& K, const double& C) const;



	// Calcul de la loss function � partir d'une nappe de volatilit�

	
	double loss_function(const std::vector<std::vector<double>>& IV_surface) const; //bas� sur erreur L2 des vols avec pond�ration en vega

	double loss_function_bis(const std::vector<std::vector<double>>& IV_surface) const; //bas� sur erreur L2 des prix avec pond�ration en vega^2


	// Optimisation de la loss function (avec Nelder Mead : algo du simplex)

	void calibration(const std::vector<std::vector<double>>& IV_surface) const;

	void calibration_bis(const std::vector<std::vector<double>>& IV_surface) const;

	


private :

	double _epsilon_iv; // erreur tol�r�e pour le calcul de l'IV 
	HestonModel* const _model_ptr; // pointeur constant vers le mod�le dont on cherche � optimiser les param�tres
	BlackScholesModel* const _bs_model_ptr; // pointeur constant vers un mod�le BS : calcule de la vol impli


};
