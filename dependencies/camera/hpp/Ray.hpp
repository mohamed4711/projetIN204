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

    Point3 at(double t) const {
        return orig + t*dir;
    }

  private:
    Point3 orig;
    Vector3 dir;
};

#endif