/*
    RTMotorOpenMP.cpp
    Parallelized renderer using OpenMP
    Distributes scanlines across multiple threads
*/

#include "RTMotors/hpp/RTMotor.hpp"
#include "lights/hpp/Light_list.hpp"

Vector3 RendererOpenMP::RayColor(const Ray& r, const hittable_list& world, const class Light_list& lights, int depth) {
    // too many bounces -> black
    if (depth <= 0) {
        return Vector3(0, 0, 0);
    }

    hit_record rec;
    double t_min = 0.001;
    double t_max = std::numeric_limits<double>::infinity();
    
    // no hit -> background color
    if (!world.hit(r, &t_min, &t_max, rec)) {
        return Vector3(0,0,0);
    }

    if (rec.mat_ptr == nullptr) {
        return Vector3(0, 0, 0);
    }

    Ray scattered;
    Vector3 attenuation;
    
    // if material scatters the ray
    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
        // direct lighting from light sources
        Vector3 direct_illumination(0, 0, 0);
        lights.computeIllumination(rec, world, direct_illumination);
        
        // indirect lighting (recursive bounces)
        Vector3 indirect_illumination = RayColor(scattered, world, lights, depth - 1);
        
        // combine direct + indirect weighted by material
        return direct_illumination + attenuation * indirect_illumination;
    }

    // material absorbs everything
    return Vector3(0, 0, 0);
}

void RendererOpenMP::Render(const Scene& scene, Image& image) {
    int nx = image.GetXsize();
    int ny = image.GetYsize();
    const auto& camera = scene.GetCamera();
    const auto& world = scene.GetObjects();
    const auto& lights = scene.GetLights();

    std::cout << "Rendering with OpenMP..." << std::endl;

    // parallelize outer loop, dynamic scheduling for load balancing
    #pragma omp parallel for collapse(2) schedule(dynamic, 1)
    for (int j = 0; j < ny; ++j) {
        
        for (int i = 0; i < nx; ++i) {
            Vector3 pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                // random_double() is thread-safe (uses thread_local)
                auto u = (i + random_double()) / (nx-1);
                auto v = (ny-1 - j + random_double()) / (ny-1);
                Ray r = camera.GenerateRay(u, v);
                pixel_color += RayColor(r, world, lights, max_depth);
            }
            
            // gamma correction and pixel write
            // each thread writes to unique pixel, no race condition
            auto r_ = sqrt(pixel_color.x / samples_per_pixel);
            auto g_ = sqrt(pixel_color.y / samples_per_pixel);
            auto b_ = sqrt(pixel_color.z / samples_per_pixel);

            image.SetPixel(i, j, r_ * 255.99, g_ * 255.99, b_ * 255.99);
        }
    }
    std::cout << "Render Complete." << std::endl;
}
