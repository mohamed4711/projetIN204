#ifndef MESH_HPP
#define MESH_HPP

#include "_Generic.hpp"
#include "Triangle.hpp"
#include <vector>
#include <memory>

// Un maillage est une collection de triangles
class Mesh : public hittable {
public:
    std::vector<std::shared_ptr<Triangle>> triangles;

    Mesh() {}
    
    void addTriangle(std::shared_ptr<Triangle> tri) {
        triangles.push_back(tri);
    }

    virtual bool hit(const Ray& r, double* ray_tmin, double* ray_tmax, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        double closest_so_far = *ray_tmax;

        // Tester tous les triangles
        for (const auto& triangle : triangles) {
            if (triangle->hit(r, ray_tmin, &closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }

    
};

#endif
