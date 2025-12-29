#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "_Generic.hpp"
#include "Vector3.hpp"

class sphere : public hittable {
  public:
    sphere(){};
    sphere(const Point3& center, double radius) : center(center), radius(std::fmax(0,radius)) {}

    bool hit(const Ray& r, double *ray_tmin, double *ray_tmax, hit_record& rec) const override {
        Vector3 oc = center - r.origin();
        auto a = r.direction().lengthSquared();
        auto h = dot(r.direction(), oc);
        auto c = oc.lengthSquared() - radius*radius;
        auto discriminant = h*h - a*c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto t1 = (h - sqrtd) / a;
        auto t2 = (h + sqrtd) / a;
         double  root=0.0; 
        if( t1<t2){
          root = t1 ; 
        }else {
          root = t2 ; 
        }
        if( root>*ray_tmax){
          *ray_tmax=root;
        }else if (root<*ray_tmin)
        {
          *ray_tmin=root;
        }
        

        rec.t = root;
        rec.p = r.at(rec.t);

        Vector3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);

        return true;
    }

  private:
    Point3 center;
    double radius;
};

#endif