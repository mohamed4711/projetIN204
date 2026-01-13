#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "Material.hpp"

class Lambertian : public Material {
public:
    Vector3 albedo;
    Lambertian(const Vector3& a) : albedo(a) {}
    bool scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, Ray& scattered) const override {
        Vector3 scatter_direction = rec.normal + random_unit_vector();
        if (fabs(scatter_direction.x) < 1e-8 && fabs(scatter_direction.y) < 1e-8 && fabs(scatter_direction.z) < 1e-8)
            scatter_direction = rec.normal;
        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
};
#endif