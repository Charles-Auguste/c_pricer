#include "HestonModel.h"
#include <cmath>

using Vector = std::vector<double>;

HestonModel::HestonModel(const Vector& initial_asset_vector, const double& correlation_rate, const double& kappa, const double& sigma_vol, const double& theta, const double& interest_rate):
	MDModel(initial_asset_vector, correlation_rate), _kappa(kappa), _sigma_vol(sigma_vol), _theta(theta), _interest_rate(interest_rate)
{
}

HestonModel::HestonModel(const HestonModel& model):
	MDModel(model), _kappa(model._kappa), _sigma_vol(model._sigma_vol), _theta(model._theta), _interest_rate(model._interest_rate)
{
}

HestonModel& HestonModel::operator=(const HestonModel& model)
{
	if (this != &model) {
		MDModel::operator=(model);
		_kappa = model._kappa;
		_sigma_vol = model._sigma_vol;
		_theta = model._theta;
		_interest_rate = model._interest_rate;
	}
	return *this;
}

Vector HestonModel::drift_vector_term(const double& time, const Vector& asset_vector) const
{
	Vector drift_vector_term(2, 0.0);
	drift_vector_term[0] = _interest_rate - std::max(asset_vector[1], 0.0)*0.5;
	drift_vector_term[1] = _kappa * (_theta - std::max(asset_vector[1], 0.0));
	return drift_vector_term;
}

Vector HestonModel::diffusion_vector_term(const double& time, const Vector& asset_vector) const
{
	Vector diffusion_term_vector(2, 0.0);
	diffusion_term_vector[0] = std::sqrt(std::max(asset_vector[1],0.0));
	diffusion_term_vector[1] = _sigma_vol * std::sqrt(std::max(asset_vector[1], 0.0));
	return diffusion_term_vector;
}

HestonModel* HestonModel::clone() const
{
	return new HestonModel(*this);
}

double HestonModel::sigma_vol() const
{
	return _sigma_vol;
}

double HestonModel::kappa() const
{
	return _kappa;
}

double HestonModel::theta() const
{
	return _theta;
}

double HestonModel::interest_rate() const
{
	return _interest_rate;
}
