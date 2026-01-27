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
    
    Dielectric(double index_of_refraction) : ir(index_of_refraction) {}
    
    bool scatter(const Ray& r_in, const hit_record& rec, Vector3& attenuation, Ray& scattered) const override {
        attenuation = Vector3(1.0, 1.0, 1.0);  // glass absorbs nothing
        
        // ratio depends on whether we're entering or exiting the material
        double refraction_ratio = rec.front_face ? (1.0/ir) : ir;
        
        Vector3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
        
        // check for total internal reflection
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vector3 direction;
        
        // use Schlick approximation for reflectance at steep angles
        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);
            
        scattered = Ray(rec.p, direction);
        return true;
    }
};
#endif