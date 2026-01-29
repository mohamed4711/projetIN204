/*
    Metal.hpp
    Reflective metal material with optional fuzziness
    Uses specular reflection with perturbation for roughness
*/

#ifndef METAL_HPP
#define METAL_HPP

#include "Material.hpp"

class Metal : public Material {
public:
    Vector3 albedo;  // metal tint color
    double fuzz;     // roughness (0 = mirror, 1 = very rough)
    
    Metal(const Vector3& a, double f);
    
    Vector3 baseColor() const override { return albedo; }
    bool scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, Ray& scattered) const override;
};

#endif
