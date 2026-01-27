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

    aabb bounding_box() const override {
    // Calcule une boîte englobant l'apex et la base du cône
    Point3 base_center = apex + axis * height;
    double base_radius = height * std::tan(angle); // angle en radians

    double min_x = std::fmin(apex.x, base_center.x) - base_radius;
    double min_y = std::fmin(apex.y, base_center.y) - base_radius;
    double min_z = std::fmin(apex.z, base_center.z) - base_radius;

    double max_x = std::fmax(apex.x, base_center.x) + base_radius;
    double max_y = std::fmax(apex.y, base_center.y) + base_radius;
    double max_z = std::fmax(apex.z, base_center.z) + base_radius;

    return aabb(Point3(min_x, min_y, min_z), Point3(max_x, max_y, max_z));
}
};

#endif
