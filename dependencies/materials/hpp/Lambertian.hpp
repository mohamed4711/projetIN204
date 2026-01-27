/*
    Lambertian.hpp
    Diffuse material with perfect matte surface
    Scatters rays in random directions following cosine distribution
*/

#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "Material.hpp"

class Lambertian : public Material {
public:
    Vector3 albedo;  // surface color (how much of each RGB is reflected)
    
    Lambertian(const Vector3& a);
    
    bool scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, Ray& scattered) const override;
};

#endif