#ifndef PLAN_HPP
#define PLAN_HPP

#include "_Generic.hpp"
#include "utils/hpp/Vector3.hpp"

class Plan : public hittable {
public:
    Point3 point;   
    Vector3 normal; 
    std::shared_ptr<Material> mat_ptr;

    Plan(Point3 p, Vector3 n, std::shared_ptr<Material> m) 
        : point(p), normal(unit_vector(n)), mat_ptr(m) {}

    virtual bool hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const override;
};

#endif