#pragma once

#include "../src/PathSimulator/PathSimulator.h"
#include "../src/Payoff/Payoff.h"

#include <algorithm>
using namespace std;

class MonteCarlo
{
public:
	MonteCarlo(size_t nb_sims, const PathSimulator& path_sim, const Payoff& payoff);
	double price() const;

private:
	size_t _number_simulations;
	const PathSimulator* _path_simulator_ptr;
	const Payoff* _payoff_ptr;
};
