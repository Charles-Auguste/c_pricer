##################################################################################################
#					                    C++ project 
#################################################################################################
#
# Author: Charles-Auguste GOURIO
#
# Last modification date: 12/05/2024
# 
#################################################################################################

cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

if(POLICY CMP0079 )
  cmake_policy(SET CMP0079 NEW)
endif()


# GLOBAL PARAMETERS
#################################################################################################

SET(PROJECT_NAME "C++ pricer")
SET(BINARY_NAME "pricer")
SET(PROJECT_DESCRIPTION "C++ project - ENPC - Master MFD")

INCLUDE(GNUInstallDirs)

project("${PROJECT_NAME}" DESCRIPTION "${PROJECT_DESCRIPTION}")

SET(LIBRARY_BASE_PATH "${PROJECT_SOURCE_DIR}/src")
message("Library Base path : " ${LIBRARY_BASE_PATH})

INCLUDE_DIRECTORIES (
	"${LIBRARY_BASE_PATH}"
)


# LIBRARY PARAMETERS
#################################################################################################
set(EIGEN_PATH_INCLUDE "/usr/local/include") 
# ^ default path for c++ libraries (include ==> headers)


# SOURCE & HEADER FILES
#################################################################################################

SET(PUBLIC_SOURCES_FILES
	"${LIBRARY_BASE_PATH}/MonteCarloEngine/MonteCarlo.cpp"
	"${LIBRARY_BASE_PATH}/PathSimulator/PathSimulator.cpp"
	"${LIBRARY_BASE_PATH}/Model/Model.cpp"
	"${LIBRARY_BASE_PATH}/ImpliedVolatilitySurface/ImpliedVolatilitySurface.cpp"
	"${LIBRARY_BASE_PATH}/ThomasSolver/ThomasSolver.cpp"
    "${LIBRARY_BASE_PATH}/Calibration/Calibration.cpp"
    "${LIBRARY_BASE_PATH}/Pricing/Pricing.cpp"
)

SET(PUBLIC_HEADERS_FILES
    "${LIBRARY_BASE_PATH}/MonteCarloEngine/MonteCarlo.h"
    "${LIBRARY_BASE_PATH}/PathSimulator/PathSimulator.h"
    "${LIBRARY_BASE_PATH}/Model/Model.h"
    "${LIBRARY_BASE_PATH}/ImpliedVolatilitySurface/ImpliedVolatilitySurface.h"
    "${LIBRARY_BASE_PATH}/ThomasSolver/ThomasSolver.h"
    "${LIBRARY_BASE_PATH}/Calibration/Calibration.h"
    "${LIBRARY_BASE_PATH}/Pricing/Pricing.h"
)

set(TEST_SOURCE_FILES
    "${LIBRARY_BASE_PATH}/MonteCarloEngine/test.cpp"
    "${LIBRARY_BASE_PATH}/PathSimulator/test.cpp"
    "${LIBRARY_BASE_PATH}/Model/test.cpp"
    "${LIBRARY_BASE_PATH}/ImpliedVolatilitySurface/test.cpp"
    "${LIBRARY_BASE_PATH}/ThomasSolver/test.cpp"
    "${LIBRARY_BASE_PATH}/Pricing/test.cpp"
    "${LIBRARY_BASE_PATH}/Calibration/test.cpp"
)

# Ajoutez une source à l'exécutable de ce projet.
ADD_LIBRARY (
	${BINARY_NAME} STATIC "${PUBLIC_HEADERS_FILES}" "${PUBLIC_SOURCES_FILES}" 
)

SET_TARGET_PROPERTIES (
	${BINARY_NAME} PROPERTIES
	VERSION		${LIBRARY_VERSION_STRING}
	SOVERSION	${LIBRARY_VERSION_MAJOR}
	PUBLIC_HEADER  "${PUBLIC_HEADERS_FILES}"
    LINKER_LANGUAGE CXX
)


# Test
#################################################################################################

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test main.cpp
    "${TEST_SOURCE_FILES}"
    "${PUBLIC_HEADERS_FILES}"
)

target_link_libraries(test PUBLIC ${BINARY_NAME} ${GTEST_LIBRARIES} pthread)

set_target_properties(test PROPERTIES LINKER_LANGUAGE CXX) 

# Presentation
#################################################################################################

add_executable(demo demo.cpp
    "${PUBLIC_HEADERS_FILES}"
)

target_link_libraries(demo PUBLIC ${BINARY_NAME})

set_target_properties(demo PROPERTIES LINKER_LANGUAGE CXX) 


# ADDITIONNAL LIBRARIES
#################################################################################################

# Eigen
if (EXISTS "${EIGEN_PATH_INCLUDE}/Eigen")
    include_directories("${EIGEN_PATH_INCLUDE}")
else()
	message("Cannot find Eigen. Make sure you install it correctly")
endif()

