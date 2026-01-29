/*
    Dielectric.hpp
    Transparent material (glass, water, etc.)
    Handles refraction and total internal reflection
*/

#ifndef DIELECTRIC_HPP
#define DIELECTRIC_HPP

#include "Material.hpp"

class Dielectric : public Material {
public:
    double ir;  // index of refraction (glass ~1.5, water ~1.33)
    
    Dielectric(double index_of_refraction);
    
    Vector3 baseColor() const override { return Vector3(1.0, 1.0, 1.0); }
    bool scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, Ray& scattered) const override;
};

#endif
