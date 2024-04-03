#pragma once

#include "../Model/Model.h"

#include <random>
#include <vector>
using Vector = std::vector<double>;
using Matrix = std::vector<Vector>;
#include <iostream>
#include <utility>
#include <cmath>
#include <cassert>
using namespace std;

// Abstract class 
class PathSimulator
{
public:
	// 1) constructor
	PathSimulator(const Model* model_ptr, const double& T, const size_t& nb_steps);
	PathSimulator(const Model& model, const double& T, const size_t& nb_steps);
	PathSimulator(const PathSimulator& path_simulator);
	PathSimulator(PathSimulator&& path_simulator) noexcept;
	PathSimulator& operator=(const PathSimulator& path_simulator);

	// 2) destructor declared virtual
	virtual ~PathSimulator();

	virtual double next_step(const size_t& current_time_idx, const double& current_price) const =0;
	Vector path() const;

	virtual PathSimulator* clone() const = 0;

protected:
	void build_time();
	Vector _time_points; // {t_0, t_1, ... , t_M = T}

	Model* _model_ptr;
	double _T;
	size_t _nb_steps;
};


class MdPathSimulator
{
public:
	// 1) constructor
	MdPathSimulator(const MdModel* model_ptr, const double& T, const size_t& nb_steps, const double& correlation);
	MdPathSimulator(const MdModel& model, const double& T, const size_t& nb_steps, const double& correlation);
	MdPathSimulator(const MdPathSimulator& path_simulator);
	MdPathSimulator(MdPathSimulator&& path_simulator) noexcept;
	MdPathSimulator& operator=(const MdPathSimulator& path_simulator);

	// 2) destructor declared virtual
	virtual ~MdPathSimulator();

	virtual Vector next_step(size_t current_time_idx, Vector current_price) const =0;
	Matrix path() const;

	virtual MdPathSimulator* clone() const = 0;

protected:
	void build_time();
	Vector _time_points; // {t_0, t_1, ... , t_M = T}

	const MdModel* _model_ptr;
	double _T;
	size_t _nb_steps;
	double _correlation;
};


class EulerPathSimulator : public PathSimulator
{
public:
	EulerPathSimulator(const Model* model_ptr, const double& T, const size_t& nb_steps);
	EulerPathSimulator* clone() const override;
	double next_step(const size_t& current_time_idx, const double& current_price) const override;
};


class MdEulerPathSimulator : public MdPathSimulator
{
public:
	MdEulerPathSimulator(const MdModel* model_ptr, const double& T, const size_t& nb_steps, const double& correlation); // copy constructor
	Vector next_step(size_t current_time_idx, Vector current_asset_value) const override;
	MdEulerPathSimulator* clone() const override;
};


class BroadieKayaPathSimulator: public MdPathSimulator
{
public:
	BroadieKayaPathSimulator(const MdModel* model_ptr, const double& T, const size_t& nb_steps, const double& correlation); // copy constructor
	Vector next_step(size_t current_time_idx, Vector current_asset_value) const override;
	BroadieKayaPathSimulator* clone() const override;
private:
	double generate_chi_squared(int d) const;
	double chi_prime(int d, double lambda) const ;

	//Sampling from integral of V(s)ds between [u,t] given Vt and Vu :
	double integral_sampling(size_t current_time_index, double var_current_value) const; // estimation of the integral of V(s)ds between t and t+dt given V_t and V_{t+dt} (by var_next_step)

	// Next Step to go from S_t to S_{t + Delta t} :
	double var_next_step(size_t current_time_idx, double current_value) const;
	double asset_next_step(size_t current_time_idx, const Vector current_asset_log_value) const;
};