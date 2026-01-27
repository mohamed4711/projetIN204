/*
    Cylinder.hpp
    Cylinder primitive with base center, axis, radius and height
    Includes top and bottom cap intersections
*/

#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "_Generic.hpp"
#include "utils/hpp/Vector3.hpp"
#include "materials/hpp/Material.hpp"
#include <memory>

class Cylinder : public hittable {
public:
    Point3 base;      // center of bottom cap
    Vector3 axis;     // cylinder axis (normalized)
    double radius;
    double height;
    std::shared_ptr<Material> mat_ptr;

    Cylinder() {}
    
    Cylinder(Point3 base_, Vector3 axis_, double r, double h, std::shared_ptr<Material> m)
        : base(base_), axis(axis_.normalize()), radius(r), height(h), mat_ptr(m) {}

    virtual bool hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const override;

    aabb bounding_box() const override {
    Point3 top = base + axis * height;
    
    // simplified bounding box around cylinder 
    double min_x = fmin(base.x, top.x) - radius;
    double min_y = fmin(base.y, top.y) - radius;
    double min_z = fmin(base.z, top.z) - radius;

    double max_x = fmax(base.x, top.x) + radius;
    double max_y = fmax(base.y, top.y) + radius;
    double max_z = fmax(base.z, top.z) + radius;

    return aabb(Point3(min_x, min_y, min_z), Point3(max_x, max_y, max_z));
}
};

#endif
