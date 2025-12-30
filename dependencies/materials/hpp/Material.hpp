#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <cmath>
#include <random>
#include <cstdlib>                     // <--- ADD THIS for rand()
#include "camera/hpp/Ray.hpp"
#include "../../utils/hpp/Vector3.hpp"

inline double random_double() { return rand() / (RAND_MAX + 1.0); }
inline double random_double(double min, double max) { return min + (max-min)*random_double(); }
inline Vector3 random_in_unit_sphere() {
    while (true) {
        Vector3 p(random_double(-1,1), random_double(-1,1), random_double(-1,1));
        if (p.lengthSquared() >= 1) continue;
        return p;
    }
}
inline Vector3 random_unit_vector() { return unit_vector(random_in_unit_sphere()); }

class Material {
public:
    virtual ~Material() = default;
    virtual bool scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, Ray& scattered) const = 0;
};
#endif