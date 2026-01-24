#include "RTMotors/hpp/RTMotor.hpp"
#include "lights/hpp/Light_list.hpp"
#include <limits>

const double infinity = std::numeric_limits<double>::infinity();

class DefaultRenderer : public Renderer {
public:
    DefaultRenderer() : Renderer() {}
    DefaultRenderer(const DefaultRenderer& other) : Renderer(other) {}

    
    Vector3 RayColor(const Ray& r, const hittable_list& world, const Light_list& lights, int depth) {
        // Sécurité : trop de rebonds → noir
        if (depth <= 0) {
            return Vector3(0, 0, 0);
        }

        hit_record rec;
        double t_min = 0.001;
        double t_max = infinity;
        
        // Si le rayon ne touche rien -> couleur du ciel (dégradé bleu/blanc)
        if (!world.hit(r, &t_min, &t_max, rec)) {
            return Vector3(0,0,0);
        }

        // Récupérer le matériau
        if (rec.mat_ptr == nullptr) {
            return Vector3(0, 0, 0);
        }

        Ray scattered;
        Vector3 attenuation;
        
        // Si le matériau disperse le rayon
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            // Calculer l'illumination directe (lumières)
            Vector3 direct_illumination(0, 0, 0);
            lights.computeIllumination(rec, world, direct_illumination);
            
            // Calculer l'illumination indirecte (réflexions/réfractions récursives)
            Vector3 indirect_illumination = RayColor(scattered, world, lights, depth - 1);
            
            // Combiner : couleur directe + réflexion pondérée par le matériau
            return direct_illumination + attenuation * indirect_illumination ; // 30 pourcent de reflexion
        }

        // Matériau absorbe tout
        return Vector3(0, 0, 0);
    }
    
    void Render(const Scene& scene, Image& image) {
        int nx = image.GetXsize();
        int ny = image.GetYsize();

        //charger le secne 
        const auto& camera = scene.GetCamera();
        const auto& world = scene.GetObjects();
        const auto& lights = scene.GetLights();

        std::cout << "Rendering with multiple lights..." << std::endl;
        for (int j = 0; j < ny; ++j) {
            if (j % 10 == 0) std::cout << "Lines remaining: " << (ny - j) << std::endl;
            for (int i = 0; i < nx; ++i) {
                Vector3 pixel_color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel; ++s) {
                    auto u = (i + random_double()) / (nx-1);
                    auto v = (ny-1 - j + random_double()) / (ny-1);
                    Ray r = camera.GenerateRay(u, v);
        
                    pixel_color += RayColor(r, world, lights, max_depth);
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