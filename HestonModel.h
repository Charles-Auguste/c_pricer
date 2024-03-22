#pragma once
#include "MDModel.h"

class HestonModel : public MDModel
{
	using Vector = std::vector<double>;

public:
	HestonModel(const Vector& initial_asset_vector, const double& correlation_rate,
		const double& kappa, const double& sigma_vol, const double& theta, 
		const double& interest_rate); // parameters constructor
	HestonModel(const HestonModel& model); // copy constructor
	HestonModel& operator=(const HestonModel& model);
	virtual ~HestonModel() = default;

	Vector drift_vector_term(const double& time, const Vector& asset_vector) const override;
	Vector diffusion_vector_term(const double& time, const Vector& asset_vector) const override;

	HestonModel* clone() const override;

	virtual double sigma_vol() const override;
	virtual double kappa() const override;
	virtual double theta() const override;
	virtual double interest_rate() const override;


private:
	//double _corr_rate;
	double _kappa;
	double _sigma_vol;
	double _theta;
	double _interest_rate;
};

