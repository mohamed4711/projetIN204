#ifndef GENERIC_OBJECT_HPP
#define GENERIC_OBJECT_HPP

#include "Ray.hpp"

class hit_record {
  public:
    Point3 p;
    Vector3 normal;
    double t;
    bool front_face;

    void set_face_normal(const Ray& r, const Vector3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
  public:
    virtual ~hittable() = default;

    virtual bool hit(const Ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};

#endif