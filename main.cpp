#include "MDModel.h"
#include "HestonModel.h"
#include "EulerPathSimulator.h"
#include <iostream>


// 

// On a une matrice de vol (voir XML)




// method to create a time sequence
std::vector<double> linspace(double start, double end, int num_points) {
	std::vector<double> time;
	double step = (end - start) / (num_points - 1);

	for (int i = 0; i < num_points; ++i) {
		time.push_back(start + i * step);
	}

	return time;
}

int main() {

	// Test

	// parameters : 
	Vector initial_asset_vector{ 100, 0. };
	double rho = -0.5;
	double kappa = 0.2;
	double sigma_vol = 0.01;
	double theta = 0.2;
	double interest_rate = 0.05;
	Vector time_points(linspace(0., 1., 100));
	int N = 100;
	//std::vector<HestonModel> heston_vec(N);

	for (int k = 0; k < N; ++k) {

		HestonModel model_test(initial_asset_vector, rho, kappa, sigma_vol, theta, interest_rate);
		//std::cout << model_test.init_value()[1] << std::endl;
		HestonModel* model_ptr(&model_test);
		//Vector time_points{ 0., 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0 };
		EulerPathSimulator simu(model_ptr, time_points);
		VectorVector path(simu.path());
		std::cout << path[0].back() << std::endl;
		//std::cout << "Asset price" << std::endl;
		/*for (size_t idx = 0; idx < time_points.size() - 1; ++idx) {
			std::cout << path[0][idx] << std::endl;
		}
		std::cout << "Volatility spot" << std::endl;
		for (size_t idx = 0; idx < time_points.size() - 1; ++idx) {
			std::cout << path[1][idx] << std::endl;
		}*/

	}
}