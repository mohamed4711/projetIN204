#include "../hpp/Cylinder.hpp"
#include <cmath>

bool Cylinder::hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const {
    // Cylindre infini d'abord, puis on coupe par la hauteur
    Vector3 oc = r.origin() - base;
    
    // Projections sur l'axe et le plan perpendiculaire
    Vector3 ray_dir_perp = r.direction() - axis * dot(r.direction(), axis);
    Vector3 oc_perp = oc - axis * dot(oc, axis);
    
    // Équation quadratique pour l'intersection avec le cylindre infini
    double a = dot(ray_dir_perp, ray_dir_perp);
    double b = 2.0 * dot(oc_perp, ray_dir_perp);
    double c = dot(oc_perp, oc_perp) - radius * radius;
    
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) return false;
    
    double sqrt_d = sqrt(discriminant);
    double t = (-b - sqrt_d) / (2.0 * a);
    
    // Essayer les deux solutions
    for (int i = 0; i < 2; i++) {
        if (i == 1) t = (-b + sqrt_d) / (2.0 * a);
        
        if (t < *ray_tmin || t > *ray_tmax) continue;
        
        Point3 p = r.at(t);
        double h_check = dot(p - base, axis);
        
        // Vérifier si le point est dans les limites de hauteur
        if (h_check >= 0 && h_check <= height) {
            rec.t = t;
            rec.p = p;
            
            // Calculer la normale (perpendiculaire à l'axe)
            Point3 axis_point = base + axis * h_check;
            Vector3 outward_normal = (p - axis_point).normalize();
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            
            return true;
        }
    }
    
    // Vérifier les intersections avec les capuchons (bases circulaires)
    // Base inférieure
    double denom = dot(axis, r.direction());
    if (std::abs(denom) > 1e-6) {
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
        
        // Base supérieure
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
