cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

# Test executable
add_executable(run main.cpp src/utils/utils.cpp MonteCarloEngine/MonteCarlo.cpp src/Payoff/Payoff.cpp src/PathSimulator/PathSimulator.cpp src/Model/Model.cpp src/ImpliedVolatilitySurface/ImpliedVolatilitySurface.cpp src/ThomasSolver/ThomasSolver.cpp)

