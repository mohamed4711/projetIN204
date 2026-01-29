/*
    _Generic.hpp
    Base class for all hittable objects
    Defines hit_record structure and hittable interface
*/

#ifndef GENERIC_OBJECT_HPP
#define GENERIC_OBJECT_HPP

#include "camera/hpp/Ray.hpp"
#include "objects/hpp/_AABB.hpp"
#include <memory>

class Material;

// stores information about a ray-object intersection
class hit_record {
  public:
    Point3 p;          // hit point position
    Vector3 normal;    // surface normal at hit point
    double t;          // ray parameter (p = origin + t*direction)
    bool front_face;   // true if ray hits front surface
    std::shared_ptr<Material> mat_ptr;
    Vector3 LocalColor; 
    double ColorIntensity; 

    // determines if we hit front or back face and adjusts normal accordingly
    void set_face_normal(const Ray& r, const Vector3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

// abstract base class for anything a ray can hit
class hittable {
  public:
    virtual ~hittable() = default;
    // returns true if ray hits object within [ray_tmin, ray_tmax]
    virtual bool hit(const Ray& r, double *ray_tmin, double* ray_tmax, hit_record& rec) const = 0;

    // return the bounding box of the object
    virtual aabb bounding_box() const = 0;
};

#endif