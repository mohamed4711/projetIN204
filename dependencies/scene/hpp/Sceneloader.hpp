/*
    Sceneloader.hpp
    Loads scene from JSON file
    Parses objects, materials and lights
*/

#ifndef SCENELOADER_HPP
#define SCENELOADER_HPP

#include <string>
#include "scene.hpp"
#include "../../utils/hpp/json.hpp" 

using json = nlohmann::json;

class SceneLoader {
public:
    
    // loads scene from JSON file
    static void LoadJSON(const std::string& filename, Scene& scene);

private:
    
    // parsers for each object type
    static void ParseSphereJSON(const json& j, Scene& scene);
    static void ParsePlaneJSON(const json& j, Scene& scene);
    static void ParseCylinderJSON(const json& j, Scene& scene);
    static void ParseConeJSON(const json& j, Scene& scene);
    static void ParseTriangleJSON(const json& j, Scene& scene);
    static void ParsePointLightJSON(const json& j, Scene& scene);
};

#endif