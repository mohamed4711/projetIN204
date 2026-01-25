#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "_Generic.hpp"
#include "utils/hpp/Vector3.hpp"
#include "materials/hpp/Material.hpp"
#include <memory>

class Triangle : public hittable {
public:
    Point3 v0, v1, v2;     // Les 3 sommets du triangle
    Vector3 normal;        // Normale du triangle
    std::shared_ptr<Material> mat_ptr;

    Triangle() {}
    
    Triangle(Point3 v0_, Point3 v1_, Point3 v2_, std::shared_ptr<Material> m)
        : v0(v0_), v1(v1_), v2(v2_), mat_ptr(m) {
        // Calculer la normale (v1-v0) x (v2-v0)
        Vector3 edge1 = v1 - v0;
        Vector3 edge2 = v2 - v0;
        normal = edge1.cross(edge2).normalize();
    }

    virtual bool hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const override;
};

#endif
