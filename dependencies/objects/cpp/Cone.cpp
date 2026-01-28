/*
    Cone.cpp
    Ray-cone intersection with base cap
    Fixed: finds closest hit among side and base cap
*/

#include "../hpp/Cone.hpp"
#include <cmath>

bool Cone::hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const {
    bool hit_anything = false;
    double closest_so_far = *ray_tmax;
    hit_record temp_rec;
    
    Vector3 oc = r.origin() - apex;
    
    double cos_angle = cos(angle);
    double sin_angle = sin(angle);
    double cos2 = cos_angle * cos_angle;
    
    // --- Check infinite cone (side) ---
    double dot_axis_dir = dot(axis, r.direction());
    double dot_axis_oc = dot(axis, oc);
    
    double a = dot_axis_dir * dot_axis_dir - cos2;
    double b = 2.0 * (dot_axis_dir * dot_axis_oc - dot(r.direction(), oc) * cos2);
    double c = dot_axis_oc * dot_axis_oc - dot(oc, oc) * cos2;
    
    double discriminant = b * b - 4 * a * c;
    if (discriminant >= 0) {
        double sqrt_d = sqrt(discriminant);
        
        for (int i = 0; i < 2; i++) {
            double t = (i == 0) ? (-b - sqrt_d) / (2.0 * a) : (-b + sqrt_d) / (2.0 * a);
            
            if (t >= *ray_tmin && t < closest_so_far) {
                Point3 p = r.at(t);
                double h_check = dot(p - apex, axis);
                
                if (h_check >= 0 && h_check <= height) {
                    temp_rec.t = t;
                    temp_rec.p = p;
                    
                    // Compute surface normal
                    Vector3 axis_point = apex + axis * h_check;
                    Vector3 radial = (p - axis_point).normalize();
                    Vector3 outward_normal = (radial * cos_angle + axis * sin_angle).normalize();
                    
                    temp_rec.set_face_normal(r, outward_normal);
                    temp_rec.mat_ptr = mat_ptr;
                    
                    closest_so_far = t;
                    rec = temp_rec;
                    hit_anything = true;
                }
            }
        }
    }
    
    // --- Check base cap intersection ---
    Point3 base_center = apex + axis * height;
    double denom = dot(axis, r.direction());
    
    if (std::abs(denom) > 1e-6) {
        double t_base = dot(base_center - r.origin(), axis) / denom;
        if (t_base >= *ray_tmin && t_base < closest_so_far) {
            Point3 p = r.at(t_base);
            Vector3 v = p - base_center;
            double base_radius = height * tan(angle);
            
            if (dot(v, v) <= base_radius * base_radius) {
                temp_rec.t = t_base;
                temp_rec.p = p;
                temp_rec.set_face_normal(r, axis);
                temp_rec.mat_ptr = mat_ptr;
                
                closest_so_far = t_base;
                rec = temp_rec;
                hit_anything = true;
            }
        }
    }
    
    return hit_anything;
}
