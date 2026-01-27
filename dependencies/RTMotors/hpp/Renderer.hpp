/*
    Renderer.hpp
    Abstract base class for all ray tracing renderers
    Defines the common interface and basic ray color computation
*/

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <limits>
#include <iostream>
#include "../../scene/hpp/scene.hpp"
#include "../../utils/hpp/Image.hpp"
#include "../../utils/hpp/Vector3.hpp"
#include "../../materials/hpp/Material.hpp"

// Abstract base class for ray tracing renderers
class Renderer {
public:
    Renderer() : max_depth(50), samples_per_pixel(10) {}
    virtual ~Renderer() = default;
    
    // Pure virtual: each renderer must implement this
    virtual void Render(const Scene& scene, Image& image) = 0;
    
    // Configuration setters
    void SetMaxDepth(int depth) { max_depth = depth; }
    void SetSamplesPerPixel(int samples) { samples_per_pixel = samples; }
    
    // Configuration getters
    int GetMaxDepth() const { return max_depth; }
    int GetSamplesPerPixel() const { return samples_per_pixel; }

protected:
    // Basic ray color without lighting (sky gradient background)
    Vector3 RayColorBasic(const Ray& r, const hittable_list& world, int depth) {
        // Max bounces reached -> black
        if (depth <= 0) return Vector3(0, 0, 0);

        hit_record rec;
        double t_min = 0.001;
        double t_max = std::numeric_limits<double>::infinity();

        if (world.hit(r, &t_min, &t_max, rec)) {
            Ray scattered;
            Vector3 attenuation;
            // If material scatters, continue tracing
            if (rec.mat_ptr != nullptr && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
                return attenuation * RayColorBasic(scattered, world, depth - 1);
            }
            return Vector3(0, 0, 0);
        }

        // No hit: sky gradient (blue to white)
        Vector3 unit_direction = unit_vector(r.direction());
        auto t = 0.5 * (unit_direction.y + 1.0);
        return (1.0 - t) * Vector3(1.0, 1.0, 1.0) + t * Vector3(0.5, 0.7, 1.0);
    }

    int max_depth;          // Maximum ray bounce depth
    int samples_per_pixel;  // Antialiasing samples per pixel
};

#endif
