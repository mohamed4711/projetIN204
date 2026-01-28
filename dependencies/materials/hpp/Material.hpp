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
#include "../../camera/hpp/Ray.hpp"
#include "../../utils/hpp/Vector3.hpp"
#include "../../objects/hpp/_Generic.hpp"
#include "../../libs.hpp"  // Provides random_double(), random_in_unit_sphere(), random_unit_vector()

// abstract base class for materials
class Material {
public:
    virtual ~Material() = default;
    // base (albedo/tint) color used for direct lighting
    virtual Vector3 baseColor() const = 0;
    // computes scattered ray and attenuation, returns false if ray is absorbed
    virtual bool scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, Ray& scattered) const = 0;
};
#endif
