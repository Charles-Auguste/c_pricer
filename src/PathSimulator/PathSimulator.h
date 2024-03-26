#ifndef PATHSIMULATOR_H
#define PATHSIMULATOR_H

#include <vector>
#include "Model.h"

// Abstract class 
class PathSimulator
{
public:
	PathSimulator(const Model* model_ptr);
	PathSimulator(const Model& model);

	// TO DO : copy constructor, copy assignment operator

	// Next Step to go from S_t to S_{t + Delta t}
	virtual double next_step(size_t current_time_idx, double current_price) const =0;

	// TO DO : method to create a path
	std::vector<double> path() const;

	virtual ~PathSimulator();

	virtual PathSimulator* clone() const = 0;

protected:
	const Model* _model_ptr;
	std::vector<double> _time_points; // {t_0, t_1, ... , t_M = T}

};

class EulerPathSimulator : public PathSimulator
{
public:
	EulerPathSimulator* clone() const override;

	double next_step(size_t current_time_idx, double current_price) const override;
};

#endif
