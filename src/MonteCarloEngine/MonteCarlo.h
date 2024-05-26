#pragma once

#include "PathSimulator/PathSimulator.h"
#include "Pricing/Pricing.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

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