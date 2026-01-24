#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "Light.hpp"
#include "materials/hpp/Material.hpp"

class PointLight : public Light {
public:
    using Light::Light;

    bool computeIllumination(hit_record &rec, const hittable_list &world, Vector3 &outColor) const override {
        Vector3 light_dir = (position - rec.p).normalize();
        Ray shadow_ray(rec.p + rec.normal * 0.001, light_dir); // Anti-acné
        
        hit_record shadow_rec;
        double t_min = 0.001;
        double distance_to_light = (position - rec.p).length();

        // si un objet est entre le point et la lumière
        if (world.hit(shadow_ray, &t_min, &distance_to_light, shadow_rec)) {
            return false; 
        }

        // Calcul Lambert avec couleur du matériau
        double cos_theta = std::max(0.0, dot(rec.normal, light_dir));
        
        // Récupérer la couleur du matériau (albedo)
        Vector3 material_color(1.0, 1.0, 1.0); // Blanc par défaut
        if (rec.mat_ptr != nullptr) {
            Ray dummy_ray;
            Vector3 attenuation;
            Ray scattered;
            if (rec.mat_ptr->scatter(dummy_ray, rec, attenuation, scattered)) {
                material_color = attenuation;
            }
        }
        
        outColor = material_color * intensity * cos_theta;
        return true;
    }
};

#endif