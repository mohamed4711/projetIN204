/*
    scene.cpp
    Scene constructor with default camera setup
*/

#include "../hpp/scene.hpp"
#include "objects/hpp/Sphere.hpp"
#include "materials/hpp/Material.hpp"
#include "materials/hpp/Lambertian.hpp"
#include "materials/hpp/Metal.hpp"
#include "materials/hpp/Dielectric.hpp"




Scene::Scene() {
   
    
    // default camera configuration (can be loaded from JSON later)
    Point3 lookfrom(20,0,3);
    Point3 lookat(0,0,0);
    Vector3 vup(0,0,1);
    auto dist_to_focus = 20.0;
    auto aperture = 0.1;
    s_camera.Setup(lookfrom, lookat, vup, 20, 1.0, aperture, dist_to_focus);

}