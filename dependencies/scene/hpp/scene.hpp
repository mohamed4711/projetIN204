/*
    scene.hpp
    Scene container holding camera, objects and lights
    Central structure for the ray tracer
*/

#ifndef SCENE_H
#define SCENE_H

#include "../../camera/hpp/Camera.hpp"
#include "../../objects/hpp/_Hittable_object_list.hpp"
#include "../../lights/hpp/Light_list.hpp"
#include <memory>
#include <vector>

class Scene {
public:
    Scene(); 

    // getters
    const Camera& GetCamera() const { return s_camera; }
    const hittable_list& GetObjects() const { return s_ObjectList; }
    const Light_list& GetLights() const { return s_Lights; }
    
   
    // add objects and lights to the scene
    void AddObject(std::shared_ptr<hittable> object) { 
        s_ObjectList.add(object); 
    }
    void AddLight(std::shared_ptr<Light> light) { 
        s_Lights.add(light); 
    }

    // reset scene to empty state
    void Clear() {
        s_ObjectList.clear();
        s_Lights.clear();
    }

   

private:
    Camera s_camera;            // scene camera
    hittable_list s_ObjectList; // all objects in scene
    Light_list s_Lights;        // all light sources
};

#endif