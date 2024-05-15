#pragma once

#include "PathSimulator/PathSimulator.h"
#include "Pricing/Pricing.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
// #include <Eigen/Dense> //  Eigen pour la régression linéaire
// using namespace Eigen;
using namespace std;

/*
double normalCDF(double value) {
    return 0.5 * (1 + std::erf(value / std::sqrt(2)));  // CDF de N(0,1)
}
*/

class MonteCarlo {
public:
  MonteCarlo(size_t nb_sims, const PathSimulator &path_sim,
             const Pricing &Pricing);
  double price() const;

private:
  Matrix generate_paths() const;
  size_t _number_simulations;
  const PathSimulator *_path_simulator_ptr;
  const Pricing *_pricing_ptr;
};

class MdMonteCarlo {
public:
  MdMonteCarlo(size_t nb_sims, const MdPathSimulator &path_sim,
               const Pricing &Pricing);
  double price() const;

private:
  Matrix generate_paths() const;
  size_t _number_simulations;
  const MdPathSimulator *_path_simulator_ptr;
  const Pricing *_pricing_ptr;
};

/*
VectorXd genlaguerre(int k, const VectorXd& x) {  // application coordonnée par
coordonnée du polynôme de Laguerre d'ordre k au vecteur x VectorXd V(x.size());
        for (int i = 0; i < x.size(); ++i) {
            V(i) = std::laguerre(k, x(i));
    }
    return V;
}

MatrixXd laguerre_basis(const VectorXd& x, int n) {  // Renvoie une matrice M
avec m_ij = Laguerre_j(x_i) MatrixXd basis(x.size(), n); for (int i = 0; i < n;
++i) { basis.col(i) = genlaguerre(i, x);
    }
    return basis;
}


int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> distribution(0, 1); // Génération de
nombres aléatoires



    double K = 130;         // Strike
    double T = 1;         // Temps d'expiration
    double sig = 0.8;       // Volatilité
    double r = 0.05;        // Taux d'intérêt
    double S0 = 110;        // Prix Spot
    int N_echeances = 200;  // Nombre d'échéances =  N pas de discrétisation ici
    double dt = T / N_echeances;
    double rdt = r * dt;
    double sdt = sig * std::sqrt(dt);
    double der = rdt - sdt * sdt / 2;
    int N_regressors = 10;  // Nombre de régresseurs pour la régression de
Laguerre int N_sim = 25000;      // Nombre de simulations int N_step = 1; //
Nombre de pas pour chaque échéance, fixé à 1 dans l'idée d'une option américaine

    double d1 = (std::log(S0 / K) + r * T) / (sig * std::sqrt(T)) + sig *
std::sqrt(T) / 2.0; double d2 = d1 - sig * std::sqrt(T); double BS = -K *
std::exp(-r * T) * normalCDF(d2) + S0 * normalCDF(d1);

    // Initialisation des 2*N_sim trajectoires Milstein. (Variable antithétique)

    vector<vector<double> > trajectoires(2*N_sim,
vector<double>(N_echeances+1,0.0)); for (size_t i = 0; i < 2*N_sim; ++i) {
        trajectoires[i][0] = S0;
    }
    for (size_t i = 1; i < N_echeances+1; ++i) {
        for (size_t m = 0; m < N_sim; ++m) {
            double g = distribution(gen);
            trajectoires[m][i]= trajectoires[m][i-1]* (1 + sdt * g + der + (sdt
* g) * (sdt * g) / 2.0);  // Schéma de Milstein 1D trajectoires[2*N_sim-m-1][i]
= trajectoires[2*N_sim-m-1][i - 1] * (1 - sdt * g + der + (sdt * g) * (sdt * g)
/ 2.0);       // Ajout de la trajectoire antithétique
        }
    }


    vector<vector<double>> cashflows = trajectoires;        // Calcul des
cashflows pour toutes les dates


    for (size_t i = 0; i < trajectoires.size(); ++i) {
        for (int k = 1; k <= N_echeances; ++k) {
            cashflows[i][k] = std::max(trajectoires[i][k] - K, 0.0);  // cash
flows
        }
        cashflows[i][0] = 0.0;
    }

    vector<double> eu_cf(2*N_sim, 0.0);                 // European Style
Cashflows for (size_t i = 0; i < cashflows.size(); ++i) { eu_cf[i] =
cashflows[i][N_echeances];
    }
        double sum_eu_call = accumulate(begin(eu_cf), end(eu_cf), 0);
        double eu_call = exp(-r * T) * sum_eu_call / (2*N_sim);
    cout << "EU Style Price MC = " << eu_call << endl;
    cout << "EU BS Formula= " << BS << endl;

    // Backward Monte Carlo par moindre carrés :

    for (int k = N_echeances - 1; k > 0; --k) {                // Identification
des chemins in the money à la date précédente : vector<double> in_the_money; for
(size_t i = 0; i < trajectoires.size(); ++i) { if (trajectoires[i][k] > K)  {
                in_the_money.push_back(i);
            }
        }

        MatrixXd X(in_the_money.size(), N_regressors);         // Préparation de
la régression linéaire visant à expliquer le cashflow futur actualisé (noté y)
                                                               // par la valeur
spot sur les chemins ITM (noté  X_)

        vector<double> y;
        VectorXd X_(in_the_money.size());
        for (size_t j = 0; j < in_the_money.size(); ++j) {
            int idx = in_the_money[j];
            X_(j) = trajectoires[idx][k];


            for (int u = k + 1; u <= N_echeances; ++u) { // Repérage sur chaque
trajectoire ou est le prochain cashflow non nul if (cashflows[idx][u] > 0 || u
== N_echeances) { y.push_back(exp(-r * (u - k) * T / N_echeances) *
cashflows[idx][u]); break;
                }
            }

        }
        X = laguerre_basis(X_, N_regressors);


        // Régression linéaire par moindre carrés sur base de polynômes de
Legendre


        VectorXd y_vec(y.size());
        for (size_t i = 0; i < y.size(); ++i) {
            y_vec(i) = y[i];
        }
        VectorXd regression = (X.transpose() * X).ldlt().solve(X.transpose() *
y_vec);  // Régression linéaire via le module Eigen VectorXd estimated_cashflows
= X * regression; for (size_t j = 0; j < in_the_money.size(); ++j) { int idx =
in_the_money[j]; if (cashflows[idx][k] < estimated_cashflows(j)) {
                cashflows[idx][k] = 0.0;
            }
        }
    }


    vector<double> mc;
    for (const auto& cashflow : cashflows) {
        for (size_t i = 1; i < cashflow.size(); ++i) {
            if (cashflow[i] > 0) {
                mc.push_back(cashflow[i] * exp(-r * i * T / N_echeances));
                break;
            }
        }
    }

    // Calcul du prix
        double sum_mc = accumulate(begin(mc), end(mc), 0);
        double price = sum_mc / (2*N_sim);
    cout << "American Style Price = " << price << endl;
}
*/
