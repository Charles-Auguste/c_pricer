#include "EulerPathSimulator.h"
#include <random>
#include <iostream>

EulerPathSimulator::EulerPathSimulator(const MDModel* model_ptr, const Vector& time_points):
	_model_ptr(model_ptr->clone()), _time_points(time_points)
{
}

EulerPathSimulator& EulerPathSimulator::operator=(const EulerPathSimulator& path)
{
	if (this != &path) {
		_model_ptr = path._model_ptr;
		_time_points = path._time_points;
	}
	return *this;
}

Vector EulerPathSimulator::next_step(size_t current_time_idx, Vector current_asset_value) const
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::normal_distribution<double> distribution(0, 1.); // N(0,1)
	
	// generating a two-dimensional brownian motion
	double g1 = distribution(gen);
	double g2 = distribution(gen);
	//std::cout << "g1 = " << g1 << std::endl;
	//std::cout << "g2 = " << g2 << std::endl;
	// correlated brownian motion 
	double g3 = _model_ptr->correlation() * g1 + std::sqrt(1 - pow(_model_ptr->correlation(),2)) * g2;

	double delta_t = _time_points[current_time_idx + 1] - _time_points[current_time_idx];
	double next_log_price;
	double next_var;
	// current_asset_value[0] stands for asset_price
	// current_asset_value[1] stands for vol_spot
	next_log_price = current_asset_value[0] 
					 + _model_ptr->drift_vector_term(_time_points[current_time_idx], current_asset_value)[0] * delta_t
					 + _model_ptr->diffusion_vector_term(_time_points[current_time_idx], current_asset_value)[0] * std::sqrt(delta_t) * g1;
	next_var       = current_asset_value[1] 
					 + _model_ptr->drift_vector_term(_time_points[current_time_idx], current_asset_value)[1] * delta_t
					 + _model_ptr->diffusion_vector_term(_time_points[current_time_idx], current_asset_value)[1] * std::sqrt(delta_t) * g3;;
	Vector next_asset_value{ next_log_price, next_var };

	return next_asset_value;
}
// il faut modifier un peu le code de façon à créer un path bi-dimensionnel (juste un vecteur à deux dimensions quoi)
/*VectorVector EulerPathSimulator::path() const
{
	VectorVector bidim_path; // size = 0 ; 2-dimensional path
	// size = 1
	bidim_path[0].push_back(_model_ptr->init_value()[0]); // S[0]
	bidim_path[1].push_back(_model_ptr->init_value()[1]); // V[0]

	for (size_t idx = 0; idx < _time_points.size() - 1; ++idx) {
		bidim_path[0].push_back(next_step(idx, bidim_path[idx])[0]); // adding the next asset price to the asset price path
		bidim_path[1].push_back(next_step(idx, bidim_path[idx])[1]); // // adding the next volatility spot to the volatility spot path
	}
	return bidim_path;
}*/

VectorVector EulerPathSimulator::path() const
{
	VectorVector bidim_log_path(2, Vector(_time_points.size()));

	bidim_log_path[0][0] = std::log(_model_ptr->init_value()[0]); // S[0]
	bidim_log_path[1][0] = _model_ptr->init_value()[1]; // V[0]

	for (size_t idx = 0; idx < _time_points.size() - 1; ++idx) {
		Vector current_asset_value{bidim_log_path[0][idx], bidim_log_path[1][idx]};
		bidim_log_path[0][idx + 1] = next_step(idx, current_asset_value)[0]; // adding the next asset price to the asset price path
		bidim_log_path[1][idx + 1] = next_step(idx, current_asset_value)[1]; // adding the next volatility spot to the volatility spot path
	}

	// returning the asset price path
	VectorVector bidim_path(2, Vector(_time_points.size()));
	for (size_t idx = 0; idx < _time_points.size() - 1; ++idx) {
		bidim_path[0][idx] = std::exp(bidim_log_path[0][idx]);
	}
	bidim_path[1] = bidim_log_path[1];

	return bidim_path;
}

EulerPathSimulator::~EulerPathSimulator()
{
	delete _model_ptr;
	std::cout << "Euler path destructor operated" << std::endl;
}

EulerPathSimulator* EulerPathSimulator::clone()
{
	return new EulerPathSimulator(*this);
}


