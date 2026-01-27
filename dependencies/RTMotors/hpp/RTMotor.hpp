/*
    RTMotor.hpp
    Base renderer class for ray tracing
    Defines the rendering interface and ray color computation
*/

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <limits>
#include <iostream>
#include "../dependencies/scene/hpp/scene.hpp"
#include "../dependencies/utils/hpp/Image.hpp"
#include "../dependencies/utils/hpp/Vector3.hpp"
#include "../dependencies/materials/hpp/Material.hpp"

class Renderer {
public:
    Renderer() : max_depth(50), samples_per_pixel(10) {} // increase samples for better quality
    virtual void Render(const Scene& scene, Image& image) = 0;
    
    void SetMaxDepth(int depth) { max_depth = depth; }
    void SetSamplesPerPixel(int samples) { samples_per_pixel = samples; }

protected:
    // recursively traces ray and computes color
    Vector3 RayColor(const Ray& r, const hittable_list& world, int depth){
            // max bounces reached, return black
            if (depth <= 0) return Vector3(0,0,0);

        hit_record rec;
        double t_min = 0.001;
        double t_max = std::numeric_limits<double>::infinity();

        if (world.hit(r, &t_min, &t_max, rec)) {
            Ray scattered;
            Vector3 attenuation;
            // if material scatters, continue tracing
            if (rec.mat_ptr != nullptr && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
                return attenuation * RayColor(scattered, world, depth-1);
            return Vector3(0,0,0);
        }

        // no hit: sky gradient (blue to white)
        Vector3 unit_direction = unit_vector(r.direction());
        auto t = 0.5*(unit_direction.y + 1.0);
        return (1.0-t)*Vector3(1.0, 1.0, 1.0) + t*Vector3(0.5, 0.7, 1.0);
    }
    int max_depth;          // max ray bounce depth
    int samples_per_pixel;  // antialiasing samples
};

// renderer with direct lighting support
class DefaultRenderer : public Renderer {
public:
    DefaultRenderer();
    DefaultRenderer(const DefaultRenderer& other);
    void Render(const Scene& scene, Image& image) override;
    
private:
    // version with light computation
    Vector3 RayColor(const Ray& r, const hittable_list& world, const class Light_list& lights, int depth);
};

#endif