/*
    Plan.hpp
    Infinite plane defined by a point and normal vector
*/

#ifndef PLAN_HPP
#define PLAN_HPP

#include "_Generic.hpp"
#include "utils/hpp/Vector3.hpp"

class Plan : public hittable {
public:
    Point3 point;    // any point on the plane
    Vector3 normal;  // plane normal (perpendicular to surface)
    std::shared_ptr<Material> mat_ptr;

    Plan(Point3 p, Vector3 n, std::shared_ptr<Material> m) 
        : point(p), normal(unit_vector(n)), mat_ptr(m) {}

    virtual bool hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const override;

   
    aabb bounding_box() const override {
    double limit = 1e8; 
    return aabb(Point3(-limit, -limit, -limit), Point3(limit, limit, limit));
}
};

#endif