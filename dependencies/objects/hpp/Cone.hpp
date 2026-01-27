/*
    Cone.hpp
    Cone primitive with apex, axis, angle and height
    Includes base cap intersection
*/

#ifndef CONE_HPP
#define CONE_HPP

#include "_Generic.hpp"
#include "utils/hpp/Vector3.hpp"
#include "materials/hpp/Material.hpp"
#include <memory>

class Cone : public hittable {
public:
    Point3 apex;      // tip of the cone
    Vector3 axis;     // cone axis direction (normalized, points to base)
    double angle;     // half-angle in radians
    double height;    // cone height from apex to base
    std::shared_ptr<Material> mat_ptr;

    Cone() {}
    
    Cone(Point3 apex_, Vector3 axis_, double angle_, double h, std::shared_ptr<Material> m)
        : apex(apex_), axis(axis_.normalize()), angle(angle_), height(h), mat_ptr(m) {}

    virtual bool hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const override;
};

#endif
