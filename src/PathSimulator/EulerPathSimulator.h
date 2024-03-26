#pragma once
#include <vector>
#include "MDModel.h"

using Vector = std::vector<double>;
using VectorVector = std::vector<Vector>;

class EulerPathSimulator
{
public:

	EulerPathSimulator(const MDModel* model_ptr, const Vector& time_points ); // copy constructor

	EulerPathSimulator& operator=(const EulerPathSimulator& path); // copy assignment operator

	// Next Step to go from S_t to S_{t + Delta t}
	Vector next_step(size_t current_time_idx, Vector current_asset_value) const;

	// method to create a path
	VectorVector path() const;

	~EulerPathSimulator(); // not equal default because of the pointer we use : if this method is not declared as follow one may notice leaking memory

	EulerPathSimulator* clone();
	

private :
	const MDModel* _model_ptr;
	Vector _time_points; // {t_0, t_1, ... , t_M = T}

};
