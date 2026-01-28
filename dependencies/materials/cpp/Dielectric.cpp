/*
    Dielectric.cpp
    Transparent material implementation
    Handles refraction with Schlick approximation and total internal reflection
*/

#include "../hpp/Dielectric.hpp"

Dielectric::Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

bool Dielectric::scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, Ray& scattered) const {
    attenuation = Vector3(1.0, 1.0, 1.0);  // Glass absorbs nothing
    
    // Ratio depends on whether we're entering or exiting the material
    double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;
    
    Vector3 unit_direction = unit_vector(r_in.direction());
    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    
    // Check for total internal reflection
    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    Vector3 direction;
    
    // Use Schlick approximation for reflectance at steep angles
    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
        direction = reflect(unit_direction, rec.normal);
    else
        direction = refract(unit_direction, rec.normal, refraction_ratio);
        
    scattered = Ray(rec.p, direction);
    return true;
}