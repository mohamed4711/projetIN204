/*
    Parallepiped.hpp
    Axis-aligned box (AABB) defined by two corner points
    Uses slab method for ray intersection
*/

#ifndef PARALLEPIPED_HPP
#define PARALLEPIPED_HPP

#include "_Generic.hpp"
#include "utils/hpp/Vector3.hpp"
#include "materials/hpp/Material.hpp"
#include <memory>
#include <cmath>
#include <algorithm>

class Parallepiped : public hittable {
public:
    Point3 p_min;   // corner with smallest coordinates
    Point3 p_max;   // corner with largest coordinates
    std::shared_ptr<Material> mat_ptr;

    Parallepiped() {}
    
    // constructor with two opposite corners
    Parallepiped(Point3 a, Point3 b, std::shared_ptr<Material> m) : mat_ptr(m) {
        // ensure p_min < p_max for each axis
        p_min = Point3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
        p_max = Point3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));
    }

    virtual bool hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const override {
        // slab method: find intersection intervals for each axis
        double t_min = *ray_tmin;
        double t_max = *ray_tmax;
        
        // store which axis gave us the entry point (for normal calculation)
        int hit_axis = -1;
        bool hit_min_side = true;
        
        for (int axis = 0; axis < 3; axis++) {
            double origin, dir, box_min, box_max;
            
            if (axis == 0) {
                origin = r.origin().x; dir = r.direction().x;
                box_min = p_min.x; box_max = p_max.x;
            } else if (axis == 1) {
                origin = r.origin().y; dir = r.direction().y;
                box_min = p_min.y; box_max = p_max.y;
            } else {
                origin = r.origin().z; dir = r.direction().z;
                box_min = p_min.z; box_max = p_max.z;
            }
            
            double inv_d = 1.0 / dir;
            double t0 = (box_min - origin) * inv_d;
            double t1 = (box_max - origin) * inv_d;
            
            bool swapped = false;
            if (inv_d < 0.0) {
                std::swap(t0, t1);
                swapped = true;
            }
            
            if (t0 > t_min) {
                t_min = t0;
                hit_axis = axis;
                hit_min_side = !swapped;
            }
            if (t1 < t_max) {
                t_max = t1;
            }
            
            // no intersection if intervals don't overlap
            if (t_max <= t_min) return false;
        }
        
        if (t_min < *ray_tmin || t_min > *ray_tmax) return false;
        
        // fill hit record
        rec.t = t_min;
        rec.p = r.at(t_min);
        rec.mat_ptr = mat_ptr;
        
        // compute normal based on which face was hit
        Vector3 outward_normal(0, 0, 0);
        if (hit_axis == 0) outward_normal.x = hit_min_side ? -1.0 : 1.0;
        else if (hit_axis == 1) outward_normal.y = hit_min_side ? -1.0 : 1.0;
        else outward_normal.z = hit_min_side ? -1.0 : 1.0;
        
        rec.set_face_normal(r, outward_normal);
        
        return true;
    }
};

#endif
