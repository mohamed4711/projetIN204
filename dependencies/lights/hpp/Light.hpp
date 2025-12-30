#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "utils/hpp/Vector3.hpp"

// Classe de base générique
class Light {
public:
    Vector3 position;
    Vector3 intensity; // Couleur * Puissance

    Light(Vector3 pos, Vector3 col) : position(pos), intensity(col) {}
    virtual ~Light() = default;
};

#endif