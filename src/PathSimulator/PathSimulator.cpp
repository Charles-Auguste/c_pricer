#include "PathSimulator.h"

#include <random>

PathSimulator::PathSimulator(const Model* model_ptr)
	: _model_ptr(model_ptr->clone())
{
}

PathSimulator::PathSimulator(const Model& model)
	: _model_ptr(model.clone())
{
}

std::vector<double> PathSimulator::path() const
{
	std::vector<double> path; // size = 0
	path.push_back(_model_ptr->init_value()); // size = 1

	for (size_t idx = 0; idx < _time_points.size() - 1; ++idx) 
		path.push_back(next_step(idx, path[idx])); // size idx + 2

	return path;
}

PathSimulator::~PathSimulator()
{
	delete _model_ptr;
}

EulerPathSimulator* EulerPathSimulator::clone() const
{
	return new EulerPathSimulator(*this);
}

double EulerPathSimulator::next_step(size_t current_time_idx, double current_price) const
{
	std::random_device rd;
	std::mt19937 gen(rd());
	
	// exercice: place the seed generation elsewhere
	
	std::normal_distribution<double> distribution(0, 1.);
	// N(0,1)
	double randomVariable = distribution(gen);

	double delta_t = _time_points[current_time_idx + 1] - _time_points[current_time_idx];
	double next_price = current_price + _model_ptr->drift_term(_time_points[current_time_idx], current_price) * delta_t
		+ _model_ptr->diffusion_term(_time_points[current_time_idx], current_price) * std::sqrt(delta_t) * randomVariable;

	return next_price;
}
