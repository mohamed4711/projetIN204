/*
    Sceneloader.cpp
    JSON scene parser implementation
    Supports spheres, planes, cylinders, cones, triangles and point lights
*/

#include "../hpp/Sceneloader.hpp"

#include "../../objects/hpp/Sphere.hpp"
#include "../../objects/hpp/Cylinder.hpp"
#include "../../objects/hpp/Cone.hpp"
#include "../../objects/hpp/Triangle.hpp"
#include "../../materials/hpp/Lambertian.hpp"
#include "../../materials/hpp/Metal.hpp"
#include "../../materials/hpp/Dielectric.hpp"
#include "../../objects/hpp/Plan.hpp"
#include "../../lights/hpp/PointLight.hpp"

#include <iostream>
#include <fstream>




void SceneLoader::LoadJSON(const std::string& filename, Scene& scene) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR: JSON file not found " << filename << std::endl;
        return;
    }

    try {
        json data = json::parse(file);
        std::cout << "Loading JSON..." << std::endl;

        // parse all objects
        for (const auto& item : data["objects"]) {
            std::string type = item["type"];
            if (type == "sphere") ParseSphereJSON(item, scene);
            else if (type == "plane") ParsePlaneJSON(item, scene);
            else if (type == "cylinder") ParseCylinderJSON(item, scene);
            else if (type == "cone") ParseConeJSON(item, scene);
            else if (type == "triangle") ParseTriangleJSON(item, scene);
        }

        // parse lights if present
        if (data.contains("lights")) {
            for (const auto& item : data["lights"]) {
                std::string type = item["type"];
                if (type == "point") ParsePointLightJSON(item, scene);
            }
        }
        std::cout << "JSON scene loaded!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }
}

// helper to parse Vector3 from JSON array
Vector3 LoadVec3(const json& j) { return Vector3(j[0], j[1], j[2]); }

void SceneLoader::ParseSphereJSON(const json& j, Scene& scene) {
    auto center = LoadVec3(j["center"]);
    double r = j["radius"];
    auto col = LoadVec3(j["color"]);
    int mat = j["material_type"];  // 0=lambertian, 1=metal, 2=dielectric

    std::shared_ptr<Material> m;
    if (mat == 0) m = std::make_shared<Lambertian>(col);
    else if (mat == 1) m = std::make_shared<Metal>(col, 0.1);
    else m = std::make_shared<Dielectric>(1.5);

    scene.AddObject(std::make_shared<sphere>(center, r, m));
}

void SceneLoader::ParsePlaneJSON(const json& j, Scene& scene) {
    auto pt = LoadVec3(j["point"]);
    auto norm = LoadVec3(j["normal"]);
    auto col = LoadVec3(j["color"]);
    int mat = j["material_type"];

    std::shared_ptr<Material> m;
    if (mat == 0) m = std::make_shared<Lambertian>(col);
    else if (mat == 1) m = std::make_shared<Metal>(col, 0.1);
    else m = std::make_shared<Dielectric>(1.5);

    scene.AddObject(std::make_shared<Plan>(pt, norm, m));
}

void SceneLoader::ParsePointLightJSON(const json& j, Scene& scene) {
    auto pos = LoadVec3(j["position"]);
    auto intensity = LoadVec3(j["intensity"]);
    
    scene.AddLight(std::make_shared<PointLight>(pos, intensity));
}

void SceneLoader::ParseCylinderJSON(const json& j, Scene& scene) {
    auto base = LoadVec3(j["base"]);
    auto axis = LoadVec3(j["axis"]);
    double radius = j["radius"];
    double height = j["height"];
    auto col = LoadVec3(j["color"]);
    int mat = j["material_type"];

    std::shared_ptr<Material> m;
    if (mat == 0) m = std::make_shared<Lambertian>(col);
    else if (mat == 1) m = std::make_shared<Metal>(col, 0.1);
    else m = std::make_shared<Dielectric>(1.5);

    scene.AddObject(std::make_shared<Cylinder>(base, axis, radius, height, m));
}

void SceneLoader::ParseConeJSON(const json& j, Scene& scene) {
    auto apex = LoadVec3(j["apex"]);
    auto axis = LoadVec3(j["axis"]);
    double angle_deg = j["angle"];
    double angle = angle_deg * M_PI / 180.0;  // convert degrees to radians
    double height = j["height"];
    auto col = LoadVec3(j["color"]);
    int mat = j["material_type"];

    std::shared_ptr<Material> m;
    if (mat == 0) m = std::make_shared<Lambertian>(col);
    else if (mat == 1) m = std::make_shared<Metal>(col, 0.1);
    else m = std::make_shared<Dielectric>(1.5);

    scene.AddObject(std::make_shared<Cone>(apex, axis, angle, height, m));
}

void SceneLoader::ParseTriangleJSON(const json& j, Scene& scene) {
    auto v0 = LoadVec3(j["v0"]);
    auto v1 = LoadVec3(j["v1"]);
    auto v2 = LoadVec3(j["v2"]);
    auto col = LoadVec3(j["color"]);
    int mat = j["material_type"];

    std::shared_ptr<Material> m;
    if (mat == 0) m = std::make_shared<Lambertian>(col);
    else if (mat == 1) m = std::make_shared<Metal>(col, 0.1);
    else m = std::make_shared<Dielectric>(1.5);

    scene.AddObject(std::make_shared<Triangle>(v0, v1, v2, m));
}
