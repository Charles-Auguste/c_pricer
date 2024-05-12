#include "ThomasSolver.h"

int test_ThomasSolver() {
  cout << "Test for Thomas solver" << endl;
  cout << "----------------------" << endl << endl;
  Vector central_diagonal{1, 1, 1, 1};
  Vector lower_diagonal{0, 0, 0, 0};
  Vector upper_diagonal{0, 0, 0, 0};
  Vector right_side{1, 1, 1, 1};

  ThomasSolver solver_instance = ThomasSolver(lower_diagonal, central_diagonal,
                                              upper_diagonal, right_side);
  Vector result = solver_instance.solve();
  for (size_t k = 0; k < result.size(); k++) {
    cout << "Result index " << k << ": \t" << result[k] << endl;
  };
  return 0;
};
