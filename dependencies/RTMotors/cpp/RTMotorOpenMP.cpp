#include "RTMotors/hpp/RTMotor.hpp"

class RendererOpenMP : public Renderer {
public:
    RendererOpenMP() : Renderer() {}
    RendererOpenMP(const RendererOpenMP& other) : Renderer(other) {}
    
    Vector3 RayColor(const Ray& r, const hittable_list& world, int depth) { return Renderer::RayColor(r, world, depth); }

    void Render(const Scene& scene, Image& image) override {
    int nx = image.GetXsize();
    int ny = image.GetYsize();
    const auto& camera = scene.GetCamera();
    const auto& world = scene.GetObjects();

    std::cout << "Rendering with OpenMP..." << std::endl;

    // Cette directive parallélise la boucle FOR juste en dessous
    // schedule(dynamic, 10) : donne des paquets de 10 lignes aux threads disponibles
    #pragma omp parallel for schedule(dynamic, 10)
    for (int j = 0; j < ny; ++j) {
        
        // Attention : std::cout n'est pas thread-safe, l'affichage sera mélangé
        // On l'affiche moins souvent ou uniquement par le thread principal (tid==0)
        // if (j % 50 == 0) std::cout << "Line " << j << " done." << std::endl; 

        for (int i = 0; i < nx; ++i) {
            Vector3 pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                // random_double() est maintenant thread-safe !
                auto u = (i + random_double()) / (nx-1);
                auto v = (ny-1 - j + random_double()) / (ny-1);
                Ray r = camera.GenerateRay(u, v);
                pixel_color += RayColor(r, world, max_depth);
            }
            
            // Correction Gamma & Écriture
            // Important : image.SetPixel doit être thread-safe.
            // Comme chaque thread écrit dans un pixel (i,j) unique, 
            // il n'y a pas de conflit d'écriture mémoire ici.
            auto r_ = sqrt(pixel_color.x / samples_per_pixel);
            auto g_ = sqrt(pixel_color.y / samples_per_pixel);
            auto b_ = sqrt(pixel_color.z / samples_per_pixel);

            image.SetPixel(i, j, r_ * 255.99, g_ * 255.99, b_ * 255.99);
        }
    }
    std::cout << "Render Complete." << std::endl;
}
};