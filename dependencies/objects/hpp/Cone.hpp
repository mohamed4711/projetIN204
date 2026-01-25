#ifndef CONE_HPP
#define CONE_HPP

#include "_Generic.hpp"
#include "utils/hpp/Vector3.hpp"
#include "materials/hpp/Material.hpp"
#include <memory>

class Cone : public hittable {
public:
    Point3 apex;           // Sommet du cône
    Vector3 axis;          // Axe du cône (pointe vers la base, normalisé)
    double angle;          // Angle du cône (en radians)
    double height;         // Hauteur du cône
    std::shared_ptr<Material> mat_ptr;

    Cone() {}
    
    Cone(Point3 apex_, Vector3 axis_, double angle_, double h, std::shared_ptr<Material> m)
        : apex(apex_), axis(axis_.normalize()), angle(angle_), height(h), mat_ptr(m) {}

    virtual bool hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const override;
};

#endif
