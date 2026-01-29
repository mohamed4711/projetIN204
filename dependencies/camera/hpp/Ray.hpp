/*
    Ray.hpp
    Represents a ray with origin and direction
    Core structure for ray tracing
*/

#ifndef RAY_H
#define RAY_H

#include "../../utils/hpp/Point3.hpp"
#include "../../utils/hpp/Vector3.hpp"

class Ray {
  public:
    Ray() {}

    Ray(const Point3& origin, const Vector3& direction) : orig(origin), dir(direction) {}
    
    const Point3& origin() const  { return orig; }
    const Vector3& direction() const { return dir; }

    // returns point along the ray at parameter t
    // P(t) = origin + t * direction
    Point3 at(double t) const {
        return orig + t*dir;
    }

  private:
    Point3 orig;   // ray starting point
    Vector3 dir;   // ray direction (not necessarily normalized)
};

#endif