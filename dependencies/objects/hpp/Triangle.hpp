/*
    Triangle.hpp
    Triangle primitive defined by 3 vertices
    Uses Moller-Trumbore algorithm for intersection
*/

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "_Generic.hpp"
#include "utils/hpp/Vector3.hpp"
#include "materials/hpp/Material.hpp"
#include <memory>

class Triangle : public hittable {
public:
    Point3 v0, v1, v2;   // the 3 vertices
    Vector3 normal;       // precomputed face normal
    std::shared_ptr<Material> mat_ptr;

    Triangle() {}
    
    Triangle(Point3 v0_, Point3 v1_, Point3 v2_, std::shared_ptr<Material> m)
        : v0(v0_), v1(v1_), v2(v2_), mat_ptr(m) {
        // compute normal via cross product: (v1-v0) x (v2-v0)
        Vector3 edge1 = v1 - v0;
        Vector3 edge2 = v2 - v0;
        normal = edge1.cross(edge2).normalize();
    }

    virtual bool hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const override;
};

#endif
