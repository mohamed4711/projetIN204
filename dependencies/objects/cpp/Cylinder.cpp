/*
    Cylinder.cpp
    Ray-cylinder intersection with top and bottom caps
*/

#include "../hpp/Cylinder.hpp"
#include <cmath>

bool Cylinder::hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const {
    // start with infinite cylinder, then clamp by height
    Vector3 oc = r.origin() - base;
    
    // project onto plane perpendicular to axis
    Vector3 ray_dir_perp = r.direction() - axis * dot(r.direction(), axis);
    Vector3 oc_perp = oc - axis * dot(oc, axis);
    
    // quadratic equation for infinite cylinder
    double a = dot(ray_dir_perp, ray_dir_perp);
    double b = 2.0 * dot(oc_perp, ray_dir_perp);
    double c = dot(oc_perp, oc_perp) - radius * radius;
    
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) return false;
    
    double sqrt_d = sqrt(discriminant);
    double t = (-b - sqrt_d) / (2.0 * a);
    
    // try both solutions
    for (int i = 0; i < 2; i++) {
        if (i == 1) t = (-b + sqrt_d) / (2.0 * a);
        
        if (t < *ray_tmin || t > *ray_tmax) continue;
        
        Point3 p = r.at(t);
        double h_check = dot(p - base, axis);
        
        // check height bounds
        if (h_check >= 0 && h_check <= height) {
            rec.t = t;
            rec.p = p;
            
            // normal perpendicular to axis
            Point3 axis_point = base + axis * h_check;
            Vector3 outward_normal = (p - axis_point).normalize();
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            
            return true;
        }
    }
    
    // check cap intersections
    double denom = dot(axis, r.direction());
    if (std::abs(denom) > 1e-6) {
        // bottom cap
        double t_base = dot(base - r.origin(), axis) / denom;
        if (t_base >= *ray_tmin && t_base <= *ray_tmax) {
            Point3 p = r.at(t_base);
            Vector3 v = p - base;
            if (dot(v, v) <= radius * radius) {
                rec.t = t_base;
                rec.p = p;
                rec.set_face_normal(r, -axis);
                rec.mat_ptr = mat_ptr;
                return true;
            }
        }
        
        // top cap
        Point3 top = base + axis * height;
        double t_top = dot(top - r.origin(), axis) / denom;
        if (t_top >= *ray_tmin && t_top <= *ray_tmax) {
            Point3 p = r.at(t_top);
            Vector3 v = p - top;
            if (dot(v, v) <= radius * radius) {
                rec.t = t_top;
                rec.p = p;
                rec.set_face_normal(r, axis);
                rec.mat_ptr = mat_ptr;
                return true;
            }
        }
    }
    
    return false;
}
