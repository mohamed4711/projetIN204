#ifndef SCENELOADER_HPP
#define SCENELOADER_HPP

#include <string>
#include "scene.hpp"
#include "../../utils/hpp/json.hpp" 

using json = nlohmann::json;

class SceneLoader {
public:
    static void Load(const std::string& filename, Scene& scene);
    static void LoadJSON(const std::string& filename, Scene& scene); // Nouvelle méthode

private:
    // Méthodes existantes pour le TXT
    static void ParseSphere(std::stringstream& ss, Scene& scene);
    static void ParsePlane(std::stringstream& ss, Scene& scene);

    // Nouvelles méthodes pour le JSON
    static void ParseSphereJSON(const json& j, Scene& scene);
    static void ParsePlaneJSON(const json& j, Scene& scene);
};

#endif