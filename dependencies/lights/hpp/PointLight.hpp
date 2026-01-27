/*
    PointLight.hpp
    Point light source with shadow casting
    Uses Lambert shading model
*/

#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "Light.hpp"
#include "materials/hpp/Material.hpp"

class PointLight : public Light {
public:
    using Light::Light;

    bool computeIllumination(hit_record &rec, const hittable_list &world, Vector3 &outColor) const override {
        Vector3 light_dir = (position - rec.p).normalize();
        Ray shadow_ray(rec.p + rec.normal * 0.001, light_dir); // offset to avoid self-intersection (shadow acne)
        
        hit_record shadow_rec;
        double t_min = 0.001;
        double distance_to_light = (position - rec.p).length();

        // check if something blocks the light
        if (world.hit(shadow_ray, &t_min, &distance_to_light, shadow_rec)) {
            return false; 
        }

        // Lambert diffuse shading
        double cos_theta = std::max(0.0, dot(rec.normal, light_dir));
        
        // get material albedo color
        Vector3 material_color(1.0, 1.0, 1.0); // default white
        if (rec.mat_ptr != nullptr) {
            Ray dummy_ray;
            Vector3 attenuation;
            Ray scattered;
            if (rec.mat_ptr->scatter(dummy_ray, rec, attenuation, scattered)) {
                material_color = attenuation;
            }
        }
        
        // final color = material * light intensity * cosine factor
        outColor = material_color * intensity * cos_theta;
        return true;
    }
};

#endif