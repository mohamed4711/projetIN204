/*
    SpotLight.cpp
    Spotlight implementation with cone falloff
*/

#include "../hpp/SpotLight.hpp"
#include "materials/hpp/Material.hpp"
#include <cmath>

SpotLight::SpotLight() : direction(0, -1, 0), inner_angle(0.3), outer_angle(0.5) {}

SpotLight::SpotLight(Vector3 pos, Vector3 dir, Vector3 col, double inner_deg, double outer_deg) 
    : Light(pos, col), direction(dir.normalize()) {
    inner_angle = inner_deg * M_PI / 180.0;
    outer_angle = outer_deg * M_PI / 180.0;
}

bool SpotLight::computeIllumination(hit_record &rec, const hittable_list &world, Vector3 &outColor) const {
    Vector3 light_dir = (position - rec.p).normalize();
    double distance_to_light = (position - rec.p).length();
    
    // check if point is within spotlight cone
    double cos_angle = dot(-light_dir, direction);
    double cos_inner = cos(inner_angle);
    double cos_outer = cos(outer_angle);
    
    // outside the outer cone = no light
    if (cos_angle < cos_outer) {
        return false;
    }
    
    // shadow test
    Ray shadow_ray(rec.p + rec.normal * 0.001, light_dir);
    hit_record shadow_rec;
    double t_min = 0.001;

    if (world.hit(shadow_ray, &t_min, &distance_to_light, shadow_rec)) {
        return false;
    }

    // compute spotlight falloff (smooth transition between inner and outer cone)
    double spot_factor = 1.0;
    if (cos_angle < cos_inner) {
        spot_factor = (cos_angle - cos_outer) / (cos_inner - cos_outer);
        spot_factor = std::max(0.0, std::min(1.0, spot_factor));
    }

    // Lambert shading
    double cos_theta = std::max(0.0, dot(rec.normal, light_dir));
    
    // get material color
    Vector3 material_color(1.0, 1.0, 1.0);
    if (rec.mat_ptr != nullptr) {
        material_color = rec.mat_ptr->baseColor();
    }
    
    // apply spotlight falloff to final color
    outColor = material_color * intensity * cos_theta * spot_factor;
    return true;
}
