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

    bool computeIllumination( hit_record &hitPoint, hittable_list Objects, const std::shared_ptr<hittable>& CurrentObject) override {
        for (const auto& light : Lights_list) {
            if (light->computeIllumination(hitPoint, Objects, CurrentObject)) {
                return true;
            }
        }
        return false;
    }
};

#endif