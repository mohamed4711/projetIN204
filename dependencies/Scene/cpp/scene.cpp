#include "../hpp/scene.hpp"
#include"./objects/hpp/Sphere.hpp"
#include "./lights/hpp/Light.hpp"
#include <iostream>

Scene::Scene() {
    // Configuration de la caméra
    s_camera = Camera();
    
    // On place la sphère à (0,0,0) avec un rayon de 4.0
    // Note : Si votre caméra est à (0,-10,0), elle regarde vers l'origine.
    s_ObjectList.add( std::make_shared<sphere>(Vector3(0.0, 0.0, 0.0), 4.0) );
    s_Lights.add(std::make_shared<Light>()); 
}

// Helper pour l'affichage (optionnel)
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    for (const auto& x : v)
        os << x << " ";
    return os;
}

bool Scene::Render(Image& image) {
    int m_Xsize = image.GetXsize();
    int m_Ysize = image.GetYsize();
    
    // Calcul du ratio d'aspect pour éviter la déformation "cylindrique"
    double aspectRatio = static_cast<double>(m_Xsize) / static_cast<double>(m_Ysize);
    
    double RayMin = 0.0; 
    double RayMax = 1e10; 
    

    for (int z = 0; z < m_Ysize; z++) {
        for (int x = 0; x < m_Xsize; x++) {
            
            // 1. Coordonnées normalisées entre -1 et 1
            // On multiplie 'u' par aspectRatio pour compenser l'étirement horizontal
            double u = (2.0 * ((x + 0.5) / static_cast<double>(m_Xsize)) - 1.0) * aspectRatio;
            double v = (1.0 - 2.0 * ((z + 0.5) / static_cast<double>(m_Ysize)));

            // 2. Génération du rayon via la caméra
            Ray RayFromPosition = s_camera.GenerateRay(u, v);
            
            hit_record record;
            bool hit_anything = false;
            double closest_so_far = RayMax;
            
            for (const auto& object : s_ObjectList.objects) {
                if (object->hit(RayFromPosition, &RayMin, &closest_so_far, record)) {
                    hit_anything = true;
                    closest_so_far = record.t;
                }
            }
            
            if (hit_anything) {
                for (const auto& light : s_Lights.Lights_list) {
                    bool illuminated = light->computeIllumination(record, s_ObjectList, nullptr); // assuming CurrentObject not needed or pass the object, but since multiple, perhaps nullptr
                    if (illuminated) {
                        

                        image.SetPixel(x, z, record.LocalColor.x * record.ColorIntensity, record.LocalColor.y * record.ColorIntensity, record.LocalColor.z * record.ColorIntensity);
                        
                        break; // since only one light or to set once
                    }
                }
            } else {
                // Sinon, fond noir
                image.SetPixel(x, z, 0.0, 0.0, 0.0);
            }
        }
    }
    
    
    
    return true;
}