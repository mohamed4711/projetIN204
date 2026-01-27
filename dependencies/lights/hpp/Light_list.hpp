/*
    Light_list.hpp
    Container for multiple light sources
    Accumulates contributions from all lights
*/

#ifndef LIGHT_LIST_HPP
#define LIGHT_LIST_HPP


#include "Light.hpp"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr; 

class Light_list : public Light {
  public:
    std::vector<shared_ptr<Light>> Lights_list;

    Light_list() {}
    Light_list(shared_ptr<Light> light) { add(light); } 

    void clear() { Lights_list.clear(); }

    void add(shared_ptr<Light> light) {
        Lights_list.push_back(light);
    }

    // iterates over all lights and sums their contributions
    bool computeIllumination(hit_record &hitPoint, const hittable_list &Objects, Vector3 &outColor) const override {
        Vector3 tempColor(0, 0, 0);
        outColor = Vector3(0, 0, 0); // reset output color 
        bool isAnyLightHitting = false;

        for (const auto& light : Lights_list) {
            // accumulate intensity from each light
            if (light->computeIllumination(hitPoint, Objects, tempColor)) { 
                outColor += tempColor; 
                isAnyLightHitting = true; 
            }
        }
        return isAnyLightHitting; 
    }
};

#endif