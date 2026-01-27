/*
    Triangle.cpp
    Ray-triangle intersection using Moller-Trumbore algorithm
*/

#include "../hpp/Triangle.hpp"
#include <cmath>

bool Triangle::hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const {
    // Moller-Trumbore intersection algorithm
    const double EPSILON = 1e-8;
    
    Vector3 edge1 = v1 - v0;
    Vector3 edge2 = v2 - v0;
    Vector3 h = r.direction().cross(edge2);
    double a = dot(edge1, h);
    
    // ray parallel to triangle
    if (std::abs(a) < EPSILON) return false;
    
    double f = 1.0 / a;
    Vector3 s = r.origin() - v0;
    double u = f * dot(s, h);  // barycentric coord u
    
    // intersection outside triangle
    if (u < 0.0 || u > 1.0) return false;
    
    Vector3 q = s.cross(edge1);
    double v = f * dot(r.direction(), q);  // barycentric coord v
    
    // intersection outside triangle
    if (v < 0.0 || u + v > 1.0) return false;
    
    // compute t to find intersection point
    double t = f * dot(edge2, q);
    
    if (t < *ray_tmin || t > *ray_tmax) return false;
    
    // valid intersection
    rec.t = t;
    rec.p = r.at(t);
    rec.set_face_normal(r, normal);
    rec.mat_ptr = mat_ptr;
    
    return true;
}

