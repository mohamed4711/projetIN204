#ifndef LIBS_HPP
#define LIBS_HPP

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <random>


// C++ Std Usings

using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

#include <random>

inline double random_double() {
    thread_local std::mt19937 generator(std::random_device{}()); 
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}
inline int random_int(int min, int max) {
    return static_cast<int>(random_double(min, max + 1));
}

inline Vector3 random_in_unit_sphere() {
    while (true) {
        Vector3 p(random_double(-1,1), random_double(-1,1), random_double(-1,1));
        if (p.lengthSquared() >= 1) continue;
        return p;
    }
}

// normalized random direction for diffuse scattering
inline Vector3 random_unit_vector() { return unit_vector(random_in_unit_sphere()); }
// Common Headers

#include "utils/hpp/Color.hpp"
#include "camera/hpp/Ray.hpp"
#include "utils/hpp/Vector3.hpp"
#include "utils/hpp/interval.hpp"

#endif
