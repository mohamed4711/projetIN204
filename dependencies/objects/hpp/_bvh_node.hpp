#ifndef BVH_HPP
#define BVH_HPP

#include "libs.hpp"         
#include "objects/hpp/_Generic.hpp"
#include "objects/hpp/_AABB.hpp"
#include "objects/hpp/_Hittable_object_list.hpp"
#include <algorithm>

class bvh_node : public hittable {
  public:
    bvh_node(const hittable_list& list) : bvh_node(list.objects, 0, list.objects.size()) {}

    bvh_node(const std::vector<std::shared_ptr<hittable>>& src_objects, size_t start, size_t end) {
        auto objects = src_objects; // mutable copy for sorting

        // Choose a random axis to split objects
        int axis = random_int(0, 2); 
        auto comparator = (axis == 0) ? box_x_compare
                        : (axis == 1) ? box_y_compare
                                      : box_z_compare;

        size_t object_span = end - start;

        if (object_span == 1) {
            left = right = objects[start];
        } else if (object_span == 2) {
            if (comparator(objects[start], objects[start+1])) {
                left = objects[start];
                right = objects[start+1];
            } else {
                left = objects[start+1];
                right = objects[start];
            }
        } else {
            // Sort objects and split list in two
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            auto mid = start + object_span / 2;
            left = std::make_shared<bvh_node>(objects, start, mid);
            right = std::make_shared<bvh_node>(objects, mid, end);
        }

        bbox = aabb(left->bounding_box(), right->bounding_box());
    }

    // BVH intersection: the key optimization step
    bool hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const override {
        // If the ray doesn't hit the node's bounding box, ignore all contents
        if (!bbox.hit(r, interval(*ray_tmin, *ray_tmax)))
            return false;

        // Otherwise, test children
        bool hit_left = left->hit(r, ray_tmin, ray_tmax, rec);
        
        // For the right side, if we hit on the left, restrict t_max to the left hit distance
        double new_tmax = hit_left ? rec.t : *ray_tmax;
        bool hit_right = right->hit(r, ray_tmin, &new_tmax, rec);

        return hit_left || hit_right;
    }

    aabb bounding_box() const override { return bbox; }

  private:
    std::shared_ptr<hittable> left;
    std::shared_ptr<hittable> right;
    aabb bbox;

    // Comparators for spatial sorting
    static bool box_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b, int axis_index) {
        return a->bounding_box().axis(axis_index).min < b->bounding_box().axis(axis_index).min;
    }

    static bool box_x_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) { return box_compare(a, b, 0); }
    static bool box_y_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) { return box_compare(a, b, 1); }
    static bool box_z_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) { return box_compare(a, b, 2); }
};

#endif