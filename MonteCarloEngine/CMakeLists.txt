cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

# Test executable
add_executable(run test.cpp MonteCarlo.cpp ../src/Calibration/Calibration.cpp ../src/Pricing/Pricing.cpp ../src/PathSimulator/PathSimulator.cpp ../src/Model/Model.cpp ../src/ImpliedVolatilitySurface/ImpliedVolatilitySurface.cpp ../src/ThomasSolver/ThomasSolver.cpp)

