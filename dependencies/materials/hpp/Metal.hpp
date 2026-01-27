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
    
    Metal(const Vector3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}
    
    bool scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, Ray& scattered) const override {
        // perfect reflection direction
        Vector3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        // add fuzz perturbation for rough metals
        scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        // only scatter if reflected ray goes outward
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};
#endif