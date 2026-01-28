/*
    ParallelRenderer.cpp
    OpenMP parallelized renderer for multi-core CPUs
    Significantly faster than SimpleRenderer on multi-core systems
*/

#include "../hpp/ParallelRenderer.hpp"
#include <limits>
#include <atomic>
#include <omp.h>


ParallelRenderer::ParallelRenderer() : Renderer() {}

ParallelRenderer::ParallelRenderer(const ParallelRenderer& other) : Renderer(other) {}

// Thread-safe ray color computation with full lighting
Vector3 ParallelRenderer::RayColor(const Ray& r, const hittable_list& world, const Light_list& lights, int depth) {
    // Too many bounces -> return black
    if (depth <= 0) {
        return Vector3(0, 0, 0);
    }

    hit_record rec;
    double t_min = 0.001;
    double t_max = infinity;
    
    // No hit -> background color
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
        
        // Combine: direct light * material color + indirect bounces * material color
        return attenuation * (direct_illumination + indirect_illumination);
    }

    return Vector3(0, 0, 0);
}

void ParallelRenderer::Render(const Scene& scene, Image& image) {
    int nx = image.GetXsize();
    int ny = image.GetYsize();
    
    const auto& camera = scene.GetCamera();
    const auto& world = scene.GetObjects();
    const auto& lights = scene.GetLights();

    int num_threads = omp_get_max_threads();
    std::cout << "ParallelRenderer: Starting render (" << nx << "x" << ny << ")..." << std::endl;
    std::cout << "  Threads: " << num_threads << ", Samples: " << samples_per_pixel << ", Max depth: " << max_depth << std::endl;

    std::atomic<int> rows_done(0);

    // Parallelize outer loop with dynamic scheduling for load balancing
    #pragma omp parallel for schedule(dynamic, 10)
    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            Vector3 pixel_color(0, 0, 0);
            
            for (int s = 0; s < samples_per_pixel; ++s) {
                // random_double() must be thread-safe (uses thread_local)
                auto u = (i + random_double()) / (nx - 1);
                auto v = (ny - 1 - j + random_double()) / (ny - 1);
                Ray r = camera.GenerateRay(u, v);
                pixel_color += RayColor(r, world, lights, max_depth);
            }
            
            // Gamma correction and pixel write
            // Each thread writes to unique pixel -> no race condition
            auto r_ = sqrt(pixel_color.x / samples_per_pixel);
            auto g_ = sqrt(pixel_color.y / samples_per_pixel);
            auto b_ = sqrt(pixel_color.z / samples_per_pixel);

            image.SetPixel(i, j, r_ * 255.99, g_ * 255.99, b_ * 255.99);
        }

        int done = ++rows_done;
        if (done % 10 == 0 || done == ny) {
            int percent = done * 100 / ny;
            #pragma omp critical
            {
                std::cout << "  Progress: " << percent << "% (line " << done << "/" << ny << ")" << std::endl;
            }
        }
    }
    std::cout << "ParallelRenderer: Done." << std::endl;
}
