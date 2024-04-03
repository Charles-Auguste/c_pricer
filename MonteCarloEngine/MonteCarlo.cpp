#include "MonteCarlo.h"

MonteCarlo::MonteCarlo(size_t nb_sims, const PathSimulator& path_sim, const Payoff& payoff)
	: _number_simulations(nb_sims), _path_simulator_ptr(path_sim.clone()), _payoff_ptr(payoff.clone())
{
}

double MonteCarlo::price() const
{
	double price = 0.;

	for (size_t sim_idx = 0; sim_idx < _number_simulations; ++sim_idx)
	{
		double result = _payoff_ptr->payoff(_path_simulator_ptr->path());
		price += result;
		cout << "Simulation " << sim_idx << " : Price = " << result << " , Mean = " << price/(sim_idx + 1) << endl;
	}
	price /= _number_simulations;
	return price;
}
