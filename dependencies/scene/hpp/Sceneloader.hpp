#ifndef SCENELOADER_HPP
#define SCENELOADER_HPP

#include <string>
#include "scene.hpp"
#include "../../utils/hpp/json.hpp" 

using json = nlohmann::json;

class SceneLoader {
public:
    
    static void LoadJSON(const std::string& filename, Scene& scene); // pour charger depuis le fichier json 

private:
    

    //méthodes pour le JSON
    static void ParseSphereJSON(const json& j, Scene& scene);
    static void ParsePlaneJSON(const json& j, Scene& scene);
    static void ParseCylinderJSON(const json& j, Scene& scene);
    static void ParseConeJSON(const json& j, Scene& scene);
    static void ParseTriangleJSON(const json& j, Scene& scene);
    static void ParsePointLightJSON(const json& j, Scene& scene);
};

#endif