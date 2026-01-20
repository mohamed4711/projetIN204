#include "../hpp/Plan.hpp"
#include <cmath>

bool Plan::hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const {
    // intersection Rayon-Plan
    auto denom = dot(normal, r.direction());

    
    if (std::abs(denom) < 1e-6) return false;

    auto t = dot(point - r.origin(), normal) / denom;

    if (t < *ray_tmin || t > *ray_tmax) return false;

    // On a touché , On remplit les infos
    rec.t = t;
    rec.p = r.at(t);
    
    // Pour un plan, la normale est toujours la même 
    Vector3 outward_normal = normal;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}