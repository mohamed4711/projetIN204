/*
    Sceneloader.cpp
    JSON scene parser implementation
    Supports spheres, planes, cylinders, cones, triangles and point lights
*/

#include "../hpp/Sceneloader.hpp"
#include "objects/hpp/_bvh_node.hpp"
#include "objects/hpp/Sphere.hpp"
#include "objects/hpp/Cylinder.hpp"
#include "objects/hpp/Cone.hpp"
#include "objects/hpp/Triangle.hpp"
#include "objects/hpp/Parallepiped.hpp"
#include "materials/hpp/Lambertian.hpp"
#include "materials/hpp/Metal.hpp"
#include "materials/hpp/Dielectric.hpp"
#include "materials/hpp/Texture.hpp"
#include "objects/hpp/Plan.hpp"
#include "lights/hpp/PointLight.hpp"
#include "lights/hpp/DirectionalLight.hpp"
#include "lights/hpp/SpotLight.hpp"

#include <iostream>
#include <fstream>




void SceneLoader::LoadJSON(const std::string& filename, Scene& scene, double aspect_ratio) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR: JSON file not found " << filename << std::endl;
        return;
    }

    try {
        json data = json::parse(file);
        std::cout << "Loading JSON..." << std::endl;

        // parse camera configuration if present
        if (data.contains("camera")) {
            ParseCameraJSON(data["camera"], scene, aspect_ratio);
        }

        // parse all objects
        for (const auto& item : data["objects"]) {
            std::string type = item["type"];
            if (type == "sphere") ParseSphereJSON(item, scene);
            else if (type == "plane") ParsePlaneJSON(item, scene);
            else if (type == "cylinder") ParseCylinderJSON(item, scene);
            else if (type == "cone") ParseConeJSON(item, scene);
            else if (type == "triangle") ParseTriangleJSON(item, scene);
            else if (type == "parallepiped" || type == "box") ParseParallelepipedJSON(item, scene);
        }

        // parse lights if present
        if (data.contains("lights")) {
            for (const auto& item : data["lights"]) {
                std::string type = item["type"];
                if (type == "point") ParsePointLightJSON(item, scene);
                else if (type == "directional" || type == "sun") ParseDirectionalLightJSON(item, scene);
                else if (type == "spot" || type == "spotlight") ParseSpotLightJSON(item, scene);
            }
        }
        std::cout << "JSON scene loaded!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }
}


void SceneLoader::LoadJSONBVH(const std::string& filename, Scene& scene, double aspect_ratio) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR: JSON file not found " << filename << std::endl;
        return;
    }

    try {
        json data = json::parse(file);
        std::cout << "Loading JSON..." << std::endl;

        if (data.contains("camera")) {
            ParseCameraJSON(data["camera"], scene, aspect_ratio);
        }

        // 1. Chargement des primitives
        for (const auto& item : data["objects"]) {
            std::string type = item["type"];
            if (type == "sphere") ParseSphereJSON(item, scene);
            else if (type == "plane") ParsePlaneJSON(item, scene);
            else if (type == "cylinder") ParseCylinderJSON(item, scene);
            else if (type == "cone") ParseConeJSON(item, scene);
            else if (type == "triangle") ParseTriangleJSON(item, scene);
            else if (type == "parallepiped" || type == "box") ParseParallelepipedJSON(item, scene);
        }

        // 2. OPTIMISATION : Construction du BVH
        auto& world = const_cast<hittable_list&>(scene.GetObjects());
        if (!world.objects.empty()) {
            std::cout << "Building BVH for " << world.objects.size() << " objects..." << std::endl;
            auto bvh_root = std::make_shared<bvh_node>(world);
            world.clear();
            world.add(bvh_root);
            std::cout << "BVH hierarchy constructed." << std::endl;
        }

        // 3. Chargement des lumières
        if (data.contains("lights")) {
            for (const auto& item : data["lights"]) {
                std::string type = item["type"];
                if (type == "point") ParsePointLightJSON(item, scene);
                else if (type == "directional") ParseDirectionalLightJSON(item, scene);
                else if (type == "spot") ParseSpotLightJSON(item, scene);
            }
        }
        std::cout << "JSON scene loaded!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }
}
// helper to parse Vector3 from JSON array
Vector3 LoadVec3(const json& j) { return Vector3(j[0], j[1], j[2]); }

std::shared_ptr<Texture> CreateTextureFromJSON(const json& j) {
    if (!j.contains("texture")) {
        return nullptr;
    }

    const auto& tex = j["texture"];
    std::string type = tex.value("type", "solid");

    if (type == "checker") {
        Vector3 c1 = tex.contains("color1") ? LoadVec3(tex["color1"]) : Vector3(0.2, 0.3, 0.1);
        Vector3 c2 = tex.contains("color2") ? LoadVec3(tex["color2"]) : Vector3(0.9, 0.9, 0.9);
        double scale = tex.value("scale", 5.0);
        return std::make_shared<CheckerTexture>(
            std::make_shared<SolidColor>(c1),
            std::make_shared<SolidColor>(c2),
            scale);
    }

    if (type == "stripes") {
        Vector3 c1 = tex.contains("color1") ? LoadVec3(tex["color1"]) : Vector3(0.1, 0.2, 0.8);
        Vector3 c2 = tex.contains("color2") ? LoadVec3(tex["color2"]) : Vector3(0.9, 0.9, 0.9);
        Vector3 axis = tex.contains("axis") ? LoadVec3(tex["axis"]) : Vector3(1, 0, 0);
        double scale = tex.value("scale", 8.0);
        return std::make_shared<StripeTexture>(
            std::make_shared<SolidColor>(c1),
            std::make_shared<SolidColor>(c2),
            axis,
            scale);
    }

    return nullptr;
}

std::shared_ptr<Material> CreateMaterialFromJSON(const json& j) {
    int mat = j.value("material_type", 0);  // 0=lambertian, 1=metal, 2=dielectric
    Vector3 col = j.contains("color") ? LoadVec3(j["color"]) : Vector3(0.8, 0.8, 0.8);

    if (mat == 0) {
        auto tex = CreateTextureFromJSON(j);
        if (tex) {
            return std::make_shared<Lambertian>(tex);
        }
        return std::make_shared<Lambertian>(col);
    }

    if (mat == 1) {
        return std::make_shared<Metal>(col, 0.1);
    }

    return std::make_shared<Dielectric>(1.5);
}

void SceneLoader::ParseSphereJSON(const json& j, Scene& scene) {
    auto center = LoadVec3(j["center"]);
    double r = j["radius"];
    std::shared_ptr<Material> m = CreateMaterialFromJSON(j);

    scene.AddObject(std::make_shared<sphere>(center, r, m));
}

void SceneLoader::ParsePlaneJSON(const json& j, Scene& scene) {
    auto pt = LoadVec3(j["point"]);
    auto norm = LoadVec3(j["normal"]);
    std::shared_ptr<Material> m = CreateMaterialFromJSON(j);

    scene.AddObject(std::make_shared<Plan>(pt, norm, m));
}

void SceneLoader::ParsePointLightJSON(const json& j, Scene& scene) {
    auto pos = LoadVec3(j["position"]);
    auto intensity = LoadVec3(j["intensity"]);
    
    scene.AddLight(std::make_shared<PointLight>(pos, intensity));
}

void SceneLoader::ParseDirectionalLightJSON(const json& j, Scene& scene) {
    auto dir = LoadVec3(j["direction"]);
    auto intensity = LoadVec3(j["intensity"]);
    
    scene.AddLight(std::make_shared<DirectionalLight>(dir, intensity));
}

void SceneLoader::ParseSpotLightJSON(const json& j, Scene& scene) {
    auto pos = LoadVec3(j["position"]);
    auto dir = LoadVec3(j["direction"]);
    auto intensity = LoadVec3(j["intensity"]);
    double inner_angle = j.value("inner_angle", 20.0);  // default 20 degrees
    double outer_angle = j.value("outer_angle", 30.0);  // default 30 degrees
    
    scene.AddLight(std::make_shared<SpotLight>(pos, dir, intensity, inner_angle, outer_angle));
}

void SceneLoader::ParseCylinderJSON(const json& j, Scene& scene) {
    auto base = LoadVec3(j["base"]);
    auto axis = LoadVec3(j["axis"]);
    double radius = j["radius"];
    double height = j["height"];
    std::shared_ptr<Material> m = CreateMaterialFromJSON(j);

    scene.AddObject(std::make_shared<Cylinder>(base, axis, radius, height, m));
}

void SceneLoader::ParseConeJSON(const json& j, Scene& scene) {
    auto apex = LoadVec3(j["apex"]);
    auto axis = LoadVec3(j["axis"]);
    double angle_deg = j["angle"];
    double angle = angle_deg * M_PI / 180.0;  // convert degrees to radians
    double height = j["height"];
    std::shared_ptr<Material> m = CreateMaterialFromJSON(j);

    scene.AddObject(std::make_shared<Cone>(apex, axis, angle, height, m));
}

void SceneLoader::ParseTriangleJSON(const json& j, Scene& scene) {
    auto v0 = LoadVec3(j["v0"]);
    auto v1 = LoadVec3(j["v1"]);
    auto v2 = LoadVec3(j["v2"]);
    std::shared_ptr<Material> m = CreateMaterialFromJSON(j);

    scene.AddObject(std::make_shared<Triangle>(v0, v1, v2, m));
}

void SceneLoader::ParseParallelepipedJSON(const json& j, Scene& scene) {
    auto p_min = LoadVec3(j["p_min"]);
    auto p_max = LoadVec3(j["p_max"]);
    std::shared_ptr<Material> m = CreateMaterialFromJSON(j);

    scene.AddObject(std::make_shared<Parallepiped>(p_min, p_max, m));
}

void SceneLoader::ParseCameraJSON(const json& j, Scene& scene, double aspect_ratio) {
    // camera position (required)
    Point3 lookfrom = j.contains("position") ? LoadVec3(j["position"]) : Point3(0, -10, 2);
    
    // target point (required)
    Point3 lookat = j.contains("lookat") ? LoadVec3(j["lookat"]) : Point3(0, 0, 0);
    
    // up vector (optional, default Y-up)
    Vector3 vup = j.contains("up") ? LoadVec3(j["up"]) : Vector3(0, 0, 1);
    
    // field of view in degrees (optional, default 60)
    double vfov = j.value("fov", 60.0);
    
    // aperture for depth of field (optional, default 0 = no DOF)
    double aperture = j.value("aperture", 0.0);
    
    // focus distance (optional, default = distance to lookat)
    double focus_dist = j.value("focus_distance", (lookfrom - lookat).length());
    
    scene.SetupCamera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, focus_dist);
    
    std::cout << "Camera configured: pos(" << lookfrom.x << "," << lookfrom.y << "," << lookfrom.z 
              << ") -> target(" << lookat.x << "," << lookat.y << "," << lookat.z << ")" << std::endl;
}
