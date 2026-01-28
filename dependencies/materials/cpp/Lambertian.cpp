/*
    Lambertian.cpp
    Diffuse material implementation
    Perfect matte surface with cosine-weighted random scattering
*/

#include "../hpp/Lambertian.hpp"

Lambertian::Lambertian(const Vector3& a)
    : albedo(std::make_shared<SolidColor>(a)) {}

Lambertian::Lambertian(std::shared_ptr<Texture> texture)
    : albedo(std::move(texture)) {}

bool Lambertian::scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, Ray& scattered) const {
    // Scatter direction = normal + random unit vector (cosine-weighted)
    Vector3 scatter_direction = rec.normal + random_unit_vector();
    
    // Catch degenerate case where random vector cancels normal
    if (fabs(scatter_direction.x) < 1e-8 && fabs(scatter_direction.y) < 1e-8 && fabs(scatter_direction.z) < 1e-8)
        scatter_direction = rec.normal;
        
    scattered = Ray(rec.p, scatter_direction);
    attenuation = albedo->value(rec.p);
    return true;
}
