#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "_Generic.hpp"
#include "utils/hpp/Vector3.hpp"
#include "materials/hpp/Material.hpp"
#include <memory>

class Cylinder : public hittable {
public:
    Point3 base;           // Centre de la base
    Vector3 axis;          // Axe du cylindre (normalisé)
    double radius;
    double height;
    std::shared_ptr<Material> mat_ptr;

    Cylinder() {}
    
    Cylinder(Point3 base_, Vector3 axis_, double r, double h, std::shared_ptr<Material> m)
        : base(base_), axis(axis_.normalize()), radius(r), height(h), mat_ptr(m) {}

    virtual bool hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const override;
};

#endif
