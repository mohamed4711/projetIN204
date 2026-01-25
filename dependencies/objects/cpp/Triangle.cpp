#include "../hpp/Triangle.hpp"
#include <cmath>

bool Triangle::hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const {
    // Algorithme de Möller-Trumbore pour l'intersection rayon-triangle
    const double EPSILON = 1e-8;
    
    Vector3 edge1 = v1 - v0;
    Vector3 edge2 = v2 - v0;
    Vector3 h = r.direction().cross(edge2);
    double a = dot(edge1, h);
    
    // Le rayon est parallèle au triangle
    if (std::abs(a) < EPSILON) return false;
    
    double f = 1.0 / a;
    Vector3 s = r.origin() - v0;
    double u = f * dot(s, h);
    
    // Le point d'intersection est hors du triangle
    if (u < 0.0 || u > 1.0) return false;
    
    Vector3 q = s.cross(edge1);
    double v = f * dot(r.direction(), q);
    
    // Le point d'intersection est hors du triangle
    if (v < 0.0 || u + v > 1.0) return false;
    
    // Calculer t pour savoir où le point d'intersection se trouve sur la ligne
    double t = f * dot(edge2, q);
    
    if (t < *ray_tmin || t > *ray_tmax) return false;
    
    // Il y a une intersection
    rec.t = t;
    rec.p = r.at(t);
    rec.set_face_normal(r, normal);
    rec.mat_ptr = mat_ptr;
    
    return true;
}
