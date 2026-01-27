/*
    Cone.cpp
    Ray-cone intersection with base cap
*/

#include "../hpp/Cone.hpp"
#include <cmath>

bool Cone::hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const {
    Vector3 oc = r.origin() - apex;
    
    double cos_angle = cos(angle);
    double cos2 = cos_angle * cos_angle;
    
    // infinite cone equation
    double dot_axis_dir = dot(axis, r.direction());
    double dot_axis_oc = dot(axis, oc);
    
    double a = dot_axis_dir * dot_axis_dir - cos2;
    double b = 2.0 * (dot_axis_dir * dot_axis_oc - dot(r.direction(), oc) * cos2);
    double c = dot_axis_oc * dot_axis_oc - dot(oc, oc) * cos2;
    
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) return false;
    
    double sqrt_d = sqrt(discriminant);
    double t = (-b - sqrt_d) / (2.0 * a);
    
    // try both solutions
    for (int i = 0; i < 2; i++) {
        if (i == 1) t = (-b + sqrt_d) / (2.0 * a);
        
        if (t < *ray_tmin || t > *ray_tmax) continue;
        
        Point3 p = r.at(t);
        double h_check = dot(p - apex, axis);
        
        // check height bounds
        if (h_check >= 0 && h_check <= height) {
            rec.t = t;
            rec.p = p;
            
            // compute surface normal
            double r_at_h = h_check * tan(angle);
            Vector3 axis_point = apex + axis * h_check;
            Vector3 radial = (p - axis_point).normalize();
            Vector3 outward_normal = (radial * cos_angle + axis * sin(angle)).normalize();
            
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            
            return true;
        }
    }
    
    // check base cap intersection
    Point3 base_center = apex + axis * height;
    double denom = dot(axis, r.direction());
    
    if (std::abs(denom) > 1e-6) {
        double t_base = dot(base_center - r.origin(), axis) / denom;
        if (t_base >= *ray_tmin && t_base <= *ray_tmax) {
            Point3 p = r.at(t_base);
            Vector3 v = p - base_center;
            double base_radius = height * tan(angle);
            
            if (dot(v, v) <= base_radius * base_radius) {
                rec.t = t_base;
                rec.p = p;
                rec.set_face_normal(r, axis);
                rec.mat_ptr = mat_ptr;
                return true;
            }
        }
    }
    
    return false;
}
