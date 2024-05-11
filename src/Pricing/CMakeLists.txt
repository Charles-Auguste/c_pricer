cmake_minimum_required(VERSION 3.22)

project(Pricing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(EIGEN_ROOT_DIR "/usr/local/include/eigen/3.4.0_1/include/eigen3/")

include_directories(
    ${EIGEN_ROOT_DIR}
)

# Test executable
add_executable(run test.cpp Pricing.cpp)
