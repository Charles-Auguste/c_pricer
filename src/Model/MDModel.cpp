#include "MDModel.h"

MDModel::MDModel(const Vector& initial_asset_vector, const double& correlation_rate):
	_initial_asset_vector(initial_asset_vector), _correlation_rate(correlation_rate)
{
}

MDModel::MDModel(const MDModel& model):
	_initial_asset_vector(model._initial_asset_vector), _correlation_rate(model._correlation_rate)
{
}

MDModel& MDModel::operator=(const MDModel& model)
{
	if (this != &model) {
		_initial_asset_vector = model._initial_asset_vector;
		_correlation_rate = model._correlation_rate;
	}
	return *this;
}


