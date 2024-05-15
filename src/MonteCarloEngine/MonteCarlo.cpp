#include "MonteCarlo.h"

MonteCarlo::MonteCarlo(size_t nb_sims, const PathSimulator &path_sim,
                       const Pricing &pricing)
    : _number_simulations(nb_sims), _path_simulator_ptr(path_sim.clone()),
      _pricing_ptr(pricing.clone()) {}

double MonteCarlo::price() const {
  Matrix paths = generate_paths();
  double price_mc = _pricing_ptr->price(paths);
  return price_mc;
}

Matrix MonteCarlo::generate_paths() const {
  Matrix result;
  for (size_t sim_idx = 0; sim_idx < _number_simulations; ++sim_idx) {
    Vector path = _path_simulator_ptr->path();
    result.push_back(path);
  }
  return result;
}

MdMonteCarlo::MdMonteCarlo(size_t nb_sims, const MdPathSimulator &path_sim,
                           const Pricing &pricing)
    : _number_simulations(nb_sims), _path_simulator_ptr(path_sim.clone()),
      _pricing_ptr(pricing.clone()) {}

double MdMonteCarlo::price() const {
  Matrix paths = generate_paths();
  double price_mc = _pricing_ptr->price(paths);
  return price_mc;
}

Matrix MdMonteCarlo::generate_paths() const {
  Matrix result;
  for (size_t sim_idx = 0; sim_idx < _number_simulations; ++sim_idx) {
    Vector path = _path_simulator_ptr->path();
    result.push_back(path);
  }
  return result;
}
