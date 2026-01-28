/*
    Metal.cpp
    Metal material implementation
    Specular reflection with optional fuzz for rough metals
*/

#include "../hpp/Metal.hpp"

Metal::Metal(const Vector3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

bool Metal::scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, Ray& scattered) const {
    // Perfect reflection direction
    Vector3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    
    // Add fuzz perturbation for rough metals
    scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
    attenuation = albedo;
    
    // Only scatter if reflected ray goes outward
    return (dot(scattered.direction(), rec.normal) > 0);
}