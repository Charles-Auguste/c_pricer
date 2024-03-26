#pragma once

#include "PathSimulator.h"
#include "Payoff.h"

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
