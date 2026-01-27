/*
    SpotLight.hpp
    Spotlight source with cone angle
    Light intensity falls off outside the cone
*/

#ifndef SPOTLIGHT_HPP
#define SPOTLIGHT_HPP

#include "Light.hpp"

class SpotLight : public Light {
public:
    Vector3 direction;    // spotlight direction (normalized)
    double inner_angle;   // inner cone angle (full intensity) in radians
    double outer_angle;   // outer cone angle (falloff to zero) in radians

    SpotLight();
    SpotLight(Vector3 pos, Vector3 dir, Vector3 col, double inner_deg, double outer_deg);

    bool computeIllumination(hit_record &rec, const hittable_list &world, Vector3 &outColor) const override;
};

#endif
