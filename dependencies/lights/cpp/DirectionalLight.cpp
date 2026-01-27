/*
    DirectionalLight.cpp
    Directional light implementation (sun-like)
*/

#include "../hpp/DirectionalLight.hpp"
#include "materials/hpp/Material.hpp"

DirectionalLight::DirectionalLight() : direction(0, -1, 0) {}

DirectionalLight::DirectionalLight(Vector3 dir, Vector3 col) 
    : Light(Vector3(0,0,0), col), direction(dir.normalize()) {}

bool DirectionalLight::computeIllumination(hit_record &rec, const hittable_list &world, Vector3 &outColor) const {
    // light direction is reversed (from surface towards light)
    Vector3 light_dir = -direction;
    
    // shadow ray towards infinity
    Ray shadow_ray(rec.p + rec.normal * 0.001, light_dir);
    
    hit_record shadow_rec;
    double t_min = 0.001;
    double t_max = 1e10;  // very far (sun is at infinity)

    // check for shadows
    if (world.hit(shadow_ray, &t_min, &t_max, shadow_rec)) {
        return false;
    }

    // Lambert shading
    double cos_theta = std::max(0.0, dot(rec.normal, light_dir));
    
    // get material color
    Vector3 material_color(1.0, 1.0, 1.0);
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
