#include "../hpp/Sceneloader.hpp"

#include "../../objects/hpp/Sphere.hpp"
#include "../../materials/hpp/Lambertian.hpp"
#include "../../materials/hpp/Metal.hpp"
#include "../../materials/hpp/Dielectric.hpp"
#include "../../objects/hpp/Plan.hpp"

#include <iostream>
#include <fstream>




void SceneLoader::LoadJSON(const std::string& filename, Scene& scene) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERREUR JSON: Fichier introuvable " << filename << std::endl;
        return;
    }

    try {
        json data = json::parse(file);
        std::cout << "Chargement JSON..." << std::endl;

        for (const auto& item : data["objects"]) {
            std::string type = item["type"];
            if (type == "sphere") ParseSphereJSON(item, scene);
            else if (type == "plane") ParsePlaneJSON(item, scene);
        }
        std::cout << "Scene JSON chargee !" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Erreur parsing JSON: " << e.what() << std::endl;
    }
}

Vector3 LoadVec3(const json& j) { return Vector3(j[0], j[1], j[2]); }

void SceneLoader::ParseSphereJSON(const json& j, Scene& scene) {
    auto center = LoadVec3(j["center"]);
    double r = j["radius"];
    auto col = LoadVec3(j["color"]);
    int mat = j["material_type"];

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