#ifndef CAMERA_H
#define CAMERA_H

#include "../../utils/hpp/Point3.hpp"
#include "../../utils/hpp/Vector3.hpp"
#include "Ray.hpp"
#include <cmath>

class Camera {
public:
    Point3 origin;
    Point3 lower_left_corner;
    Vector3 horizontal;
    Vector3 vertical;
    Vector3 u, v, w;
    double lens_radius;

    Camera() {}

    // Configuration flexible (Position, Cible, FOV, Ouverture, Focus)
    void Setup(Point3 lookfrom, Point3 lookat, Vector3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist) {
        auto theta = vfov * M_PI / 180.0;
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(vup.cross(w));
        v = w.cross(u);

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

        lens_radius = aperture / 2;
    }

    Ray GenerateRay(double s, double t) const {
        Vector3 rd = lens_radius * random_in_unit_disk();
        Vector3 offset = u * rd.x + v * rd.y;

        return Ray(
            origin + offset,
            lower_left_corner + s*horizontal + t*vertical - origin - offset
        );
    }

private:
    static Vector3 random_in_unit_disk() {
        while (true) {
            auto p = Vector3(random_double(-1,1), random_double(-1,1), 0);
            if (p.lengthSquared() >= 1) continue;
            return p;
        }
    }
    
    static double random_double(double min = 0.0, double max = 1.0) {
        return min + (max-min)*((double)rand()/RAND_MAX);
    }
};

#endif