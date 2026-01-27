/*
    DirectionalLight.hpp
    Directional light source (like the sun)
    All rays are parallel, no position - only direction
*/

#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP

#include "Light.hpp"

class DirectionalLight : public Light {
public:
    Vector3 direction;  // direction FROM the light (towards the scene)

    DirectionalLight();
    DirectionalLight(Vector3 dir, Vector3 col);

    bool computeIllumination(hit_record &rec, const hittable_list &world, Vector3 &outColor) const override;
};

#endif
