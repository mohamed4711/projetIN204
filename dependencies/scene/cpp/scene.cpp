#include "../hpp/scene.hpp"
#include "objects/hpp/Sphere.hpp"
#include "materials/hpp/Material.hpp"
#include "materials/hpp/Lambertian.hpp"
#include "materials/hpp/Metal.hpp"
#include "materials/hpp/Dielectric.hpp"




Scene::Scene() {
   
    
    // Configuration Caméra par défaut (on peux charger ca  apartir du fichier json plus tard )
    Point3 lookfrom(13,2,3);
    Point3 lookat(0,0,0);
    Vector3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    s_camera.Setup(lookfrom, lookat, vup, 20, 1.0, aperture, dist_to_focus);

}