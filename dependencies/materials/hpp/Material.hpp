/*
    Material.hpp
    Base class for all materials
    Defines how rays interact with surfaces
*/

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <cmath>
#include <random>
#include <cstdlib>
#include "camera/hpp/Ray.hpp"
#include "../../utils/hpp/Vector3.hpp"

// thread-safe random number generator
inline double random_double() { 
    // thread_local: each thread gets its own generator instance
    static thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
 }
 
inline double random_double(double min, double max) { return min + (max-min)*random_double(); }

// generates random point inside unit sphere (rejection sampling)
inline Vector3 random_in_unit_sphere() {
    while (true) {
        Vector3 p(random_double(-1,1), random_double(-1,1), random_double(-1,1));
        if (p.lengthSquared() >= 1) continue;
        return p;
    }
}

// normalized random direction for diffuse scattering
inline Vector3 random_unit_vector() { return unit_vector(random_in_unit_sphere()); }

// abstract base class for materials
class Material {
public:
    virtual ~Material() = default;
    // computes scattered ray and attenuation, returns false if ray is absorbed
    virtual bool scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, Ray& scattered) const = 0;
};
#endif