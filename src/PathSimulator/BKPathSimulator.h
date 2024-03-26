#pragma once
#include <vector>
#include "MDModel.h"

using Vector = std::vector<double>;
using VectorVector = std::vector<Vector>;

class BKPathSimulator
{

public:

	BKPathSimulator(const MDModel* model_ptr, const Vector& time_points); // copy constructor

	BKPathSimulator& operator=(const BKPathSimulator& path); // copy assignment operator

	// Next Step to go from V_t to V_{t + Delta t} :
	double generate_chi_squared(int d) const;
	double chi_prime(int d, double lambda) const ;
	double var_next_step(size_t current_time_idx, double current_value) const;

	//Sampling from integral of V(s)ds between [u,t] given Vt and Vu :
	double integral_sampling(size_t current_time_index, double var_current_value) const; // estimation of the integral of V(s)ds between t and t+dt given V_t and V_{t+dt} (by var_next_step)


	// Next Step to go from S_t to S_{t + Delta t} :
	double asset_next_step(size_t current_time_idx, const Vector current_asset_log_value) const;


	// method to create a path
	VectorVector path() const;

	~BKPathSimulator(); // not equal default because of the pointer we use : if this method is not declared as follow one may notice leaking memory

	BKPathSimulator* clone();


private:
	const MDModel* _model_ptr;
	Vector _time_points; // {t_0, t_1, ... , t_M = T}
};


