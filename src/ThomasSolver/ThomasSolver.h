#pragma once

#include <vector>
using Vector = std::vector<double>;
#include <iostream>
using namespace std;

class ThomasSolver {
public:
  // Constructor with parameters
  ThomasSolver(const Vector &lower_diag, const Vector &central_diag,
               const Vector &upper_diag, const Vector &rhs);

  // returns [X_1,..., X_N]
  Vector solve() const;

private:
  Vector _lower_diagonal;   // [a_2, ..., a_N]
  Vector _central_diagonal; // [b_1, ..., b_N]
  Vector _upper_diagonal;   // [c_1, ..., c_{N-1}]
  Vector _right_hand_side;  // [R_1, ..., R_N]
                            //
};
