#include "../hpp/scene.hpp"
#include "objects/hpp/Sphere.hpp"
#include "materials/hpp/Material.hpp"
#include "materials/hpp/Lambertian.hpp"
#include "materials/hpp/Metal.hpp"
#include "materials/hpp/Dielectric.hpp"

Scene::Scene() {
    // 1. Sol
    auto ground_material = std::make_shared<Lambertian>(Vector3(0.5, 0.5, 0.5));
    s_ObjectList.add(std::make_shared<sphere>(Point3(0,-1000,0), 1000, ground_material));

    // 2. Petites sphères aléatoires
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) { // Diffuse
                    auto albedo = Vector3(random_double(), random_double(), random_double()) * Vector3(random_double(), random_double(), random_double());
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    s_ObjectList.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) { // Metal
                    auto albedo = Vector3(random_double(0.5, 1), random_double(0.5, 1), random_double(0.5, 1));
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    s_ObjectList.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else { // Glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    s_ObjectList.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    // 3. Grosses sphères
    auto material1 = std::make_shared<Dielectric>(1.5);
    s_ObjectList.add(std::make_shared<sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Vector3(0.4, 0.2, 0.1));
    s_ObjectList.add(std::make_shared<sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Vector3(0.7, 0.6, 0.5), 0.0);
    s_ObjectList.add(std::make_shared<sphere>(Point3(4, 1, 0), 1.0, material3));

    // 4. Configuration Caméra
    Point3 lookfrom(13,2,3);
    Point3 lookat(0,0,0);
    Vector3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    s_camera.Setup(lookfrom, lookat, vup, 20, 16.0/9.0, aperture, dist_to_focus);
}