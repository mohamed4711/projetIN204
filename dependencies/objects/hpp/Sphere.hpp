// dependencies/objects/hpp/Sphere.hpp
#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "_Generic.hpp"
#include "utils/hpp/Vector3.hpp"
#include "materials/hpp/Material.hpp"
#include <cmath>
#include <memory>

class sphere : public hittable {
  public:
    sphere() {}
    // Ensure this 3-argument constructor is saved and visible to the compiler
    sphere(const Point3& center, double radius, std::shared_ptr<Material> m) 
        : center(center), radius(std::fmax(0,radius)), mat_ptr(m) {}

    bool hit(const Ray& r, double *ray_tmin, double *ray_tmax, hit_record& rec) const override {
        // ... (your existing hit logic)
        Vector3 oc = center - r.origin();
        auto a = r.direction().lengthSquared();
        auto h = dot(r.direction(), oc);
        auto c = oc.lengthSquared() - radius*radius;
        auto discriminant = h*h - a*c;
        
        if (discriminant < 0) return false;
        auto sqrtd = std::sqrt(discriminant);

        // Find nearest root in acceptable range
        auto root = (h - sqrtd) / a;
        if (root <= *ray_tmin || *ray_tmax <= root) {
            root = (h + sqrtd) / a;
            if (root <= *ray_tmin || *ray_tmax <= root)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        rec.mat_ptr = mat_ptr; // Ensure material is assigned
        Vector3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        return true;
    }

  public:
    Point3 center;
    double radius;
    std::shared_ptr<Material> mat_ptr; // Ensure this member exists
};
#endif