#include "RTMotors/hpp/RTMotor.hpp"
#include "../dependencies/materials/hpp/Material.hpp"
#include <iostream>
#include <limits>

Renderer::Renderer() : max_depth(50), samples_per_pixel(10) {} // Mettre samples_per_pixel à 50 ou 100 pour haute qualité

Vector3 Renderer::RayColor(const Ray& r, const hittable_list& world, int depth) {
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

void Renderer::Render(const Scene& scene, Image& image) {
    int nx = image.GetXsize();
    int ny = image.GetYsize();
    const auto& camera = scene.GetCamera();
    const auto& world = scene.GetObjects();

    std::cout << "Rendering..." << std::endl;

    for (int j = 0; j < ny; ++j) {
        if (j % 10 == 0) std::cout << "Lines remaining: " << (ny - j) << std::endl;
        for (int i = 0; i < nx; ++i) {
            Vector3 pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (nx-1);
                auto v = (ny-1 - j + random_double()) / (ny-1);
                Ray r = camera.GenerateRay(u, v);
                pixel_color += RayColor(r, world, max_depth);
            }
            
            // Gamma Correction & Averaging
            auto r_ = sqrt(pixel_color.x / samples_per_pixel);
            auto g_ = sqrt(pixel_color.y / samples_per_pixel);
            auto b_ = sqrt(pixel_color.z / samples_per_pixel);

            image.SetPixel(i, j, r_ * 255.99, g_ * 255.99, b_ * 255.99);
        }
    }
    std::cout << "Done." << std::endl;
}