# include "MonteCarloEngine/test.hpp"
# include "src/PathSimulator/test.hpp"
# include "src/Model/test.hpp"
# include "src/ImpliedVolatilitySurface/test.hpp"
# include "src/ThomasSolver/test.hpp"
# include "src/Calibration/test.hpp"
# include "src/Pricing/test.hpp"

int main() {
	cout <<endl << endl << "=============== TEST PROCEDURE ===============" << endl << endl;
	test_ThomasSolver();
	test_ImpliedVolatilitySurface();
	test_Model();
	test_PathSimulator();
	test_Pricing();
	test_Calibration();
	test_MonteCarlo();
	cout << endl << endl << "===============  END OF TESTS  ===============" << endl << endl;
	return 0;
}