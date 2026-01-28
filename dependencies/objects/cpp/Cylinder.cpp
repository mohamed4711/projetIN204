/*
    Cylinder.cpp
    Ray-cylinder intersection with top and bottom caps
    Fixed: finds closest hit among side and caps
*/

#include "../hpp/Cylinder.hpp"
#include <cmath>

bool Cylinder::hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const {
    bool hit_anything = false;
    double closest_so_far = *ray_tmax;
    hit_record temp_rec;
    
    // --- Check infinite cylinder (side) ---
    Vector3 oc = r.origin() - base;
    Vector3 ray_dir_perp = r.direction() - axis * dot(r.direction(), axis);
    Vector3 oc_perp = oc - axis * dot(oc, axis);
    
    double a = dot(ray_dir_perp, ray_dir_perp);
    double b = 2.0 * dot(oc_perp, ray_dir_perp);
    double c = dot(oc_perp, oc_perp) - radius * radius;
    
    double discriminant = b * b - 4 * a * c;
    if (discriminant >= 0) {
        double sqrt_d = sqrt(discriminant);
        
        // Try both solutions for cylinder side
        for (int i = 0; i < 2; i++) {
            double t = (i == 0) ? (-b - sqrt_d) / (2.0 * a) : (-b + sqrt_d) / (2.0 * a);
            
            if (t >= *ray_tmin && t < closest_so_far) {
                Point3 p = r.at(t);
                double h_check = dot(p - base, axis);
                
                // Check height bounds
                if (h_check >= 0 && h_check <= height) {
                    temp_rec.t = t;
                    temp_rec.p = p;
                    
                    Point3 axis_point = base + axis * h_check;
                    Vector3 outward_normal = (p - axis_point).normalize();
                    temp_rec.set_face_normal(r, outward_normal);
                    temp_rec.mat_ptr = mat_ptr;
                    
                    closest_so_far = t;
                    rec = temp_rec;
                    hit_anything = true;
                }
            }
        }
    }
    
    // --- Check bottom cap ---
    double denom = dot(axis, r.direction());
    if (std::abs(denom) > 1e-6) {
        double t_base = dot(base - r.origin(), axis) / denom;
        if (t_base >= *ray_tmin && t_base < closest_so_far) {
            Point3 p = r.at(t_base);
            Vector3 v = p - base;
            double dist_sq = dot(v, v) - pow(dot(v, axis), 2);  // distance from axis
            if (dist_sq <= radius * radius) {
                temp_rec.t = t_base;
                temp_rec.p = p;
                temp_rec.set_face_normal(r, -axis);
                temp_rec.mat_ptr = mat_ptr;
                
                closest_so_far = t_base;
                rec = temp_rec;
                hit_anything = true;
            }
        }
        
        // --- Check top cap ---
        Point3 top = base + axis * height;
        double t_top = dot(top - r.origin(), axis) / denom;
        if (t_top >= *ray_tmin && t_top < closest_so_far) {
            Point3 p = r.at(t_top);
            Vector3 v = p - top;
            double dist_sq = dot(v, v) - pow(dot(v, axis), 2);  // distance from axis
            if (dist_sq <= radius * radius) {
                temp_rec.t = t_top;
                temp_rec.p = p;
                temp_rec.set_face_normal(r, axis);
                temp_rec.mat_ptr = mat_ptr;
                
                closest_so_far = t_top;
                rec = temp_rec;
                hit_anything = true;
            }
        }
    }
    
    return hit_anything;
}
