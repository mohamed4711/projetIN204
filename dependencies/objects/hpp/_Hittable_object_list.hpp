/*
    _Hittable_object_list.hpp
    Container for scene objects
    Finds the closest intersection among all objects
*/

#ifndef HITTABLE_OBJECT_LIST_HPP
#define HITTABLE_OBJECT_LIST_HPP

#include "_Generic.hpp"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class hittable_list : public hittable {
  public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    // tests all objects, returns closest hit
    bool hit(const Ray& r, double *ray_tmin, double *ray_tmax, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        double closest_so_far = *ray_tmax;

        for (const auto& object : objects) {
            if (object->hit(r, ray_tmin, &closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
    
        return hit_anything;
    }

    aabb bounding_box() const override {
        if (objects.empty()) return aabb(interval::empty, interval::empty, interval::empty);

        aabb temp_box;
        bool first_box = true;

        for (const auto& object : objects) {
            if (first_box) {
                temp_box = object->bounding_box();
                first_box = false;
            } else {
                temp_box = aabb(temp_box, object->bounding_box()); // Union des boîtes
            }
        }
        return temp_box;
    }
};

#endif