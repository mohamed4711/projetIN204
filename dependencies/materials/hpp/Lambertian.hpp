/*
    Lambertian.hpp
    Diffuse material with perfect matte surface
    Scatters rays in random directions following cosine distribution
*/

#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include <memory>
#include "Material.hpp"
#include "Texture.hpp"

class Lambertian : public Material {
public:
    std::shared_ptr<Texture> albedo;  // texture-based albedo

    Lambertian(const Vector3& a);
    Lambertian(std::shared_ptr<Texture> texture);
    
    bool scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, Ray& scattered) const override;
};

#endif