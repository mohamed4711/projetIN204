#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "utils/hpp/Vector3.hpp"
#include "objects/hpp/_Generic.hpp"              
#include "objects/hpp/_Hittable_object_list.hpp" 
#include <memory>

// Classe de base générique
class Light {
public:
    Vector3 position;
    Vector3 intensity; // Couleur * Puissance

    Light() : position(0,0,0), intensity(0,0,0) {}
    Light(Vector3 pos, Vector3 col) : position(pos), intensity(col) {}
    virtual ~Light() = default;

    virtual bool computeIllumination(hit_record &hitPoint, const hittable_list &Objects, Vector3 &outColor) const = 0;
};

#endif