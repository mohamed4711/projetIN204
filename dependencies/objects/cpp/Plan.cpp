/*
    Plan.cpp
    Ray-plane intersection implementation
*/

#include "../hpp/Plan.hpp"
#include <cmath>

bool Plan::hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const {
    // ray-plane intersection: t = (point - origin) . normal / (direction . normal)
    auto denom = dot(normal, r.direction());

    // ray parallel to plane
    if (std::abs(denom) < 1e-6) return false;

    auto t = dot(point - r.origin(), normal) / denom;

    if (t < *ray_tmin || t > *ray_tmax) return false;

    // fill hit record
    rec.t = t;
    rec.p = r.at(t);
    rec.set_face_normal(r, normal);
    rec.mat_ptr = mat_ptr;

    return true;
}