/*
    SimpleRenderer.cpp
    Single-threaded renderer with direct + indirect lighting
    Best for debugging and small images
*/

#include "../hpp/SimpleRenderer.hpp"
#include <limits>

const double infinity = std::numeric_limits<double>::infinity();

SimpleRenderer::SimpleRenderer() : Renderer() {}

SimpleRenderer::SimpleRenderer(const SimpleRenderer& other) : Renderer(other) {}

// Computes color for a ray with full lighting model
Vector3 SimpleRenderer::RayColor(const Ray& r, const hittable_list& world, const Light_list& lights, int depth) {
    // Too many bounces -> return black
    if (depth <= 0) {
        return Vector3(0, 0, 0);
    }

    hit_record rec;
    double t_min = 0.001;
    double t_max = infinity;
    
    // No hit -> background color (black for now)
    if (!world.hit(r, &t_min, &t_max, rec)) {
        return Vector3(0, 0, 0);
    }

    if (rec.mat_ptr == nullptr) {
        return Vector3(0, 0, 0);
    }

    Ray scattered;
    Vector3 attenuation;
    
    // If material scatters the ray
    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
        // Direct lighting from light sources
        Vector3 direct_illumination(0, 0, 0);
        lights.computeIllumination(rec, world, direct_illumination);
        
        // Indirect lighting (recursive bounces)
        Vector3 indirect_illumination = RayColor(scattered, world, lights, depth - 1);
        
        // Combine direct + indirect weighted by material color
        return direct_illumination + attenuation * indirect_illumination;
    }

    // Material absorbs all light
    return Vector3(0, 0, 0);
}

void SimpleRenderer::Render(const Scene& scene, Image& image) {
    int nx = image.GetXsize();
    int ny = image.GetYsize();

    // Get scene components
    const auto& camera = scene.GetCamera();
    const auto& world = scene.GetObjects();
    const auto& lights = scene.GetLights();

    std::cout << "SimpleRenderer: Starting render (" << nx << "x" << ny << ")..." << std::endl;
    std::cout << "  Samples: " << samples_per_pixel << ", Max depth: " << max_depth << std::endl;
    
    for (int j = 0; j < ny; ++j) {
        // Progress indicator every 10 lines
        if (j % 10 == 0) {
            std::cout << "  Progress: " << (j * 100 / ny) << "% (line " << j << "/" << ny << ")" << std::endl;
        }
        
        for (int i = 0; i < nx; ++i) {
            Vector3 pixel_color(0, 0, 0);
            
            // Antialiasing: average multiple samples per pixel
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (nx - 1);
                auto v = (ny - 1 - j + random_double()) / (ny - 1);
                Ray r = camera.GenerateRay(u, v);
    
                pixel_color += RayColor(r, world, lights, max_depth);
            }
            
            // Gamma correction (gamma = 2.0) and averaging
            auto r_ = sqrt(pixel_color.x / samples_per_pixel);
            auto g_ = sqrt(pixel_color.y / samples_per_pixel);
            auto b_ = sqrt(pixel_color.z / samples_per_pixel);

            image.SetPixel(i, j, r_ * 255.99, g_ * 255.99, b_ * 255.99);
        }
    }
    std::cout << "SimpleRenderer: Done." << std::endl;
}
