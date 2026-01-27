/*
    Light.hpp
    Base class for all light sources in the scene
*/

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "utils/hpp/Vector3.hpp"
#include "objects/hpp/_Generic.hpp"              
#include "objects/hpp/_Hittable_object_list.hpp" 
#include <memory>

// Abstract base class for lights
class Light {
public:
    Vector3 position;   // light position in world space
    Vector3 intensity;  // color * power

    Light() : position(0,0,0), intensity(0,0,0) {}
    Light(Vector3 pos, Vector3 col) : position(pos), intensity(col) {}
    virtual ~Light() = default;

    // computes lighting contribution at hit point, returns false if in shadow
    virtual bool computeIllumination(hit_record &hitPoint, const hittable_list &Objects, Vector3 &outColor) const = 0;
};

#endif