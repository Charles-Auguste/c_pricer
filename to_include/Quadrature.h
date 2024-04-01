#pragma once
# include <cfloat>
# include <cmath>
# include <cstdlib>
# include <cstring>
# include <ctime>
# include <fstream>
# include <iomanip>
# include <iostream>
# include <vector>



std::vector<double> gauss_hermite_points(const int& degree); //renvoie le vecteur de points


std::vector<double> gauss_hermite_weights(const int& degree); //renvoie le vecteur de poids