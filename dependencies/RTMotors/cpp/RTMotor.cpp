#include "RTMotors/hpp/RTMotor.hpp"

class DefaultRenderer : public Renderer {
public:
    DefaultRenderer() : Renderer() {}
    DefaultRenderer(const DefaultRenderer& other) : Renderer(other) {}

    Vector3 RayColor(const Ray& r, const hittable_list& world, int depth) { return Renderer::RayColor(r, world, depth); }
    
    void Render(const Scene& scene, Image& image) {
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
};