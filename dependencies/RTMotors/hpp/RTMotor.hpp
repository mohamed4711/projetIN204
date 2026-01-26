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
    Renderer() : max_depth(50), samples_per_pixel(10) {} // Mettre samples_per_pixel à 50 ou 100 pour haute qualité
    virtual void Render(const Scene& scene, Image& image) = 0;
    
    // Setters pour configuration
    void SetMaxDepth(int depth) { max_depth = depth; }
    void SetSamplesPerPixel(int samples) { samples_per_pixel = samples; }

protected:
    Vector3 RayColor(const Ray& r, const hittable_list& world, int depth){
            if (depth <= 0) return Vector3(0,0,0);

        hit_record rec;
        double t_min = 0.001;
        double t_max = std::numeric_limits<double>::infinity();

        if (world.hit(r, &t_min, &t_max, rec)) {
            Ray scattered;
            Vector3 attenuation;
            if (rec.mat_ptr != nullptr && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
                return attenuation * RayColor(scattered, world, depth-1);
            return Vector3(0,0,0);
        }

        Vector3 unit_direction = unit_vector(r.direction());
        auto t = 0.5*(unit_direction.y + 1.0);
        return (1.0-t)*Vector3(1.0, 1.0, 1.0) + t*Vector3(0.5, 0.7, 1.0);
    }
    int max_depth;
    int samples_per_pixel;
};

// Forward declaration for DefaultRenderer
class DefaultRenderer : public Renderer {
public:
    DefaultRenderer();
    DefaultRenderer(const DefaultRenderer& other);
    void Render(const Scene& scene, Image& image) override;
    
private:
    Vector3 RayColor(const Ray& r, const hittable_list& world, const class Light_list& lights, int depth);
};

#endif