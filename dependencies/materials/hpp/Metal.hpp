#ifndef METAL_HPP
#define METAL_HPP

#include "Material.hpp"


class Metal : public Material {
public:
    Vector3 albedo;
    double fuzz;
    Metal(const Vector3& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}
    bool scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, Ray& scattered) const override {
        Vector3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};
#endif