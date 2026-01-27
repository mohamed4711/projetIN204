#ifndef AABB_HPP
#define AABB_HPP

#include "utils/hpp/Vector3.hpp"
#include "utils/hpp/interval.hpp"
#include "camera/hpp/Ray.hpp"

class aabb {
public:
    interval x, y, z;

    aabb() {} 
    aabb(const interval& x, const interval& y, const interval& z) : x(x), y(y), z(z) {}

    // creates the smallest AABB that contains both a and b
    aabb(const Point3& a, const Point3& b) {
        x = interval(fmin(a.x, b.x), fmax(a.x, b.x));
        y = interval(fmin(a.y, b.y), fmax(a.y, b.y));
        z = interval(fmin(a.z, b.z), fmax(a.z, b.z));
    }

    // creates the smallest AABB that contains both box0 and box1
    aabb(const aabb& box0, const aabb& box1) {
        x = interval(box0.x, box1.x);
        y = interval(box0.y, box1.y);
        z = interval(box0.z, box1.z);
    }

    const interval& axis(int n) const {
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }

    // Ray-AABB intersection test using the "slab" method
    bool hit(const Ray& r, interval ray_t) const {
        for (int a = 0; a < 3; a++) {
            auto invD = 1.0 / r.direction()[a];
            auto orig = r.origin()[a];

            auto t0 = (axis(a).min - orig) * invD;
            auto t1 = (axis(a).max - orig) * invD;

            if (invD < 0) std::swap(t0, t1);

            if (t0 > ray_t.min) ray_t.min = t0;
            if (t1 < ray_t.max) ray_t.max = t1;

            if (ray_t.max <= ray_t.min) return false;
        }
        return true;
    }
};
#endif