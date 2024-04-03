cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

# Test executable
add_executable(run test.cpp MonteCarlo/MonteCarlo.cpp ../Payoff/Payoff.cpp ../PathSimulator/PathSimulator.cpp ../Model/Model.cpp ../ImpliedVolatilitySurface/ImpliedVolatilitySurface.cpp ../ThomasSolver/ThomasSolver.cpp)

