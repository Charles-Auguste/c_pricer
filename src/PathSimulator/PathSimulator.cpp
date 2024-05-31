#include "PathSimulator.h"

void MdPathSimulator::build_time() {
  _time_points.push_back(0);
  double delta_t = _T / _nb_steps;
  for (size_t idx = 0; idx < _nb_steps; ++idx)
    _time_points.push_back(_time_points[idx] + delta_t);
}

void PathSimulator::build_time() {
  _time_points.push_back(0);
  double delta_t = _T / _nb_steps;                          // timestep
  for (size_t idx = 0; idx < _nb_steps; ++idx)
    _time_points.push_back(_time_points[idx] + delta_t);
}

// PathSimulator class
PathSimulator::PathSimulator(const Model *model_ptr, const double &T,
                             const size_t &nb_steps)
    : _model_ptr(model_ptr->clone()), _T(T), _nb_steps(nb_steps) {
  build_time();
}

PathSimulator::PathSimulator(const Model &model, const double &T,
                             const size_t &nb_steps)
    : _model_ptr(model.clone()), _T(T), _nb_steps(nb_steps) {
  build_time();
}

PathSimulator::PathSimulator(const PathSimulator &path_simulator)
    : _model_ptr(path_simulator._model_ptr->clone()), _T(path_simulator._T),
      _nb_steps(path_simulator._nb_steps) {
  _time_points = path_simulator._time_points;
}

PathSimulator::PathSimulator(PathSimulator &&path_simulator) noexcept
    : _model_ptr(std::move(path_simulator._model_ptr)), _T(path_simulator._T),
      _nb_steps(path_simulator._nb_steps) {
  _time_points = std::move(path_simulator._time_points);
}

PathSimulator &PathSimulator::operator=(const PathSimulator &path_simulator) {
  if (this != &path_simulator) {
    delete _model_ptr;
    _model_ptr = path_simulator._model_ptr->clone();
    _T = path_simulator._T;
    _nb_steps = path_simulator._nb_steps;
    _time_points = path_simulator._time_points;
  }

  return *this;
}

Vector PathSimulator::path() const {
  Vector _path; // size = 0

  _path.push_back(_model_ptr->init_value()); // size = 1

  for (size_t idx = 1; idx < _time_points.size(); ++idx) {
    double next = next_step(idx, _path[idx-1]);
    _path.push_back(next); // size idx + 2
  };

  return _path;
}

PathSimulator::~PathSimulator() { delete _model_ptr; }

// EulerPathSimulator class
EulerPathSimulator *EulerPathSimulator::clone() const {
  return new EulerPathSimulator(*this);
}

EulerPathSimulator::EulerPathSimulator(const Model *model_ptr, const double &T,
                                       const size_t &nb_steps)
    : PathSimulator(model_ptr, T, nb_steps) {}

double EulerPathSimulator::next_step(const size_t &current_time_idx,
                                     const double &current_price) const {
  std::random_device rd;
  std::mt19937 gen(rd());       // random numbers generation

  std::normal_distribution<double> distribution(0, 1.);
  // N(0,1)
  double randomVariable = distribution(gen);    // random number

  double delta_t =
      _time_points[current_time_idx] - _time_points[current_time_idx -1];

  double next_price =
      current_price +
      _model_ptr->drift_term(_time_points[current_time_idx], current_price) *
          delta_t +
      _model_ptr->diffusion_term(_time_points[current_time_idx],
                                 current_price) *
          std::sqrt(delta_t) * randomVariable;

  return next_price;
}

// MdPathSimulator class
MdPathSimulator::MdPathSimulator(const MdModel *model_ptr, const double &T,
                                 const size_t &nb_steps,
                                 const double &correlation)
    : _model_ptr(model_ptr->clone()), _T(T), _nb_steps(nb_steps),
      _correlation(correlation) {
  assert(model_ptr->init_value().size() == 2);
  build_time();
}

MdPathSimulator::MdPathSimulator(const MdModel &model, const double &T,
                                 const size_t &nb_steps,
                                 const double &correlation)
    : _model_ptr(model.clone()), _T(T), _nb_steps(nb_steps),
      _correlation(correlation) {
  assert(model.init_value().size() == 2);
  build_time();
}

MdPathSimulator::MdPathSimulator(const MdPathSimulator &path_simulator)
    : _model_ptr(path_simulator._model_ptr->clone()), _T(path_simulator._T),
      _nb_steps(path_simulator._nb_steps),
      _correlation(path_simulator._correlation) {
  _time_points = path_simulator._time_points;
}

MdPathSimulator::MdPathSimulator(MdPathSimulator &&path_simulator) noexcept
    : _model_ptr(std::move(path_simulator._model_ptr)), _T(path_simulator._T),
      _nb_steps(path_simulator._nb_steps),
      _correlation(path_simulator._correlation) {
  _time_points = std::move(path_simulator._time_points);
}

MdPathSimulator &
MdPathSimulator::operator=(const MdPathSimulator &path_simulator) {
  if (this != &path_simulator) {
    delete _model_ptr;
    _model_ptr = path_simulator._model_ptr->clone();
    _T = path_simulator._T;
    _correlation = path_simulator._correlation;
    _nb_steps = path_simulator._nb_steps;
    _time_points = path_simulator._time_points;
  }

  return *this;
}

Vector MdPathSimulator::path() const {
  Matrix _path;
  int dim = _model_ptr->init_value().size();

  // initialize the matrix
  for (size_t k = 0; k < dim; k++) {
    Vector _path_el{_model_ptr->init_value()[k]};
    _path.push_back(_path_el);
  }

  // Run the simulation
  for (size_t idx = 0; idx < _time_points.size() - 1; ++idx) {
    // Build current value vector
    Vector current_asset_value;
    for (size_t k = 0; k < dim; k++)
      current_asset_value.push_back(_path[k][idx]);

    // Getting the next step
    Vector next_asset_value = next_step(idx, current_asset_value);

    // apending to our _path vector
    for (size_t k = 0; k < dim; k++)
      _path[k].push_back(next_asset_value[k]);
  }

  return _path[0];
}

MdPathSimulator::~MdPathSimulator() { delete _model_ptr; }

// MdEulerPathSimulator
MdEulerPathSimulator::MdEulerPathSimulator(const MdModel *model_ptr,
                                           const double &T,
                                           const size_t &nb_steps,
                                           const double &correlation)
    : MdPathSimulator(model_ptr, T, nb_steps, correlation) {}

Vector MdEulerPathSimulator::next_step(size_t current_time_idx,
                                       Vector current_asset_value) const {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::normal_distribution<double> distribution(0, 1.); // N(0,1)

  // generating a two-dimensional brownian motion
  double g1 = distribution(gen);
  double g_int = distribution(gen);
  // correlated brownian motion
  double g2 = _correlation * g1 + sqrt(1 - pow(_correlation, 2)) * g_int;

  double delta_t =
      _time_points[current_time_idx + 1] - _time_points[current_time_idx];

  double next_log_price;
  double next_var;

  next_log_price = current_asset_value[0] +
                   _model_ptr->drift_vector_term(_time_points[current_time_idx],
                                                 current_asset_value)[0] *
                       delta_t +
                   _model_ptr->diffusion_vector_term(
                       _time_points[current_time_idx], current_asset_value)[0] *
                       sqrt(delta_t) * g1;
  next_var = current_asset_value[1] +
             _model_ptr->drift_vector_term(_time_points[current_time_idx],
                                           current_asset_value)[1] *
                 delta_t +
             _model_ptr->diffusion_vector_term(_time_points[current_time_idx],
                                               current_asset_value)[1] *
                 sqrt(delta_t) * g2;

  Vector next_asset_value{next_log_price, next_var};

  return next_asset_value;
}

MdEulerPathSimulator *MdEulerPathSimulator::clone() const {
  return new MdEulerPathSimulator(*this);
}

// BroadieKayaPathSimulator
BroadieKayaPathSimulator::BroadieKayaPathSimulator(const HestonModel *model_ptr,
                                                   const double &T,
                                                   const size_t &nb_steps,
                                                   const double &correlation)
    : MdPathSimulator(model_ptr, T, nb_steps, correlation) {}

BroadieKayaPathSimulator *BroadieKayaPathSimulator::clone() const {
  return new BroadieKayaPathSimulator(*this);
}

Vector BroadieKayaPathSimulator::next_step(size_t current_time_idx,
                                           Vector current_asset_value) const {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::normal_distribution<double> distribution(0, 1.); // N(0,1)

  // generating a two-dimensional brownian motion
  double g1 = distribution(gen);
  double g_int = distribution(gen);
  // correlated brownian motion
  double g2 = _correlation * g1 + sqrt(1 - pow(_correlation, 2)) * g_int;

  double delta_t =
      _time_points[current_time_idx + 1] - _time_points[current_time_idx];

  double next_log_price;
  double next_var;

  next_log_price = current_asset_value[0] +
                   _model_ptr->drift_vector_term(_time_points[current_time_idx],
                                                 current_asset_value)[0] *
                       delta_t +
                   _model_ptr->diffusion_vector_term(
                       _time_points[current_time_idx], current_asset_value)[0] *
                       sqrt(delta_t) * g1;
  next_var = current_asset_value[1] +
             _model_ptr->drift_vector_term(_time_points[current_time_idx],
                                           current_asset_value)[1] *
                 delta_t +
             _model_ptr->diffusion_vector_term(_time_points[current_time_idx],
                                               current_asset_value)[1] *
                 sqrt(delta_t) * g2;

  Vector next_asset_value{next_log_price, next_var};

  return next_asset_value;
}

double BroadieKayaPathSimulator::generate_chi_squared(int d) const {
  std::random_device rd;
  std::mt19937 gen(rd());
  // Convert d into double for the division
  double alpha = static_cast<double>(d) / 2.0;
  // Gamma distribution with alpha = d/2 & beta = 2
  std::gamma_distribution<double> distribution(alpha, 2.0);
  // Generate a sample of gamma distribution
  double chi_squared = distribution(gen);

  return chi_squared;
};

double BroadieKayaPathSimulator::chi_prime(int d, double lambda) const
// noncentral chi-squared random variable with d degrees of freedom,
// and noncentrality parameter lambda may be expressed using an ordinary
//  chi-squared and an independent normal when d > 1
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::normal_distribution<double> distribution(0, 1.); // N(0,1)
  double g = distribution(gen);
  double res = pow(g + std::sqrt(lambda), 2) + generate_chi_squared(d);
  return res;
};

double
BroadieKayaPathSimulator::integral_sampling(size_t current_time_index,
                                            double var_current_value) const {
  double var_next_value = var_next_step(current_time_index, var_current_value);
  return 0.0;
};

double BroadieKayaPathSimulator::var_next_step(size_t current_time_idx,
                                               double current_value) const {
  double delta_t =
      _time_points[current_time_idx + 1] - _time_points[current_time_idx];
  double pre_factor = (pow(_model_ptr->sigma_vol(), 2) *
                       (1 - std::exp(-_model_ptr->kappa() * delta_t))) /
                      (4 * _model_ptr->kappa());
  int degree = int(4 * _model_ptr->kappa() * _model_ptr->theta() /
                   pow(_model_ptr->sigma_vol(), 2));
  double next_var =
      pre_factor *
      chi_prime(degree, current_value *
                            std::exp(-_model_ptr->kappa() * delta_t) /
                            pre_factor); // where f is loi du chi 2 � d degr�s
                                         // de libert� d�riv�
  return next_var;
};

double BroadieKayaPathSimulator::asset_next_step(
    size_t current_time_idx, const Vector current_asset_log_value) const {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::normal_distribution<double> distribution(0, 1.); // N(0,1)
  double g = distribution(gen);
  double delta_t =
      _time_points[current_time_idx + 1] - _time_points[current_time_idx];
  double integral =
      integral_sampling(current_time_idx, current_asset_log_value[1]);
  double next_asset_log_value =
      current_asset_log_value[0] + _model_ptr->r() * delta_t +
      _correlation / _model_ptr->sigma_vol() *
          (var_next_step(current_time_idx, current_asset_log_value[1]) -
           current_asset_log_value[1] -
           _model_ptr->kappa() * _model_ptr->theta() * delta_t) +
      (_model_ptr->kappa() * _correlation / _model_ptr->sigma_vol() - 0.5) *
          integral +
      sqrt(max(integral, 0.0) * (1 - pow(_correlation, 2))) *
          g; // gaussian distribution

  return next_asset_log_value;
};
