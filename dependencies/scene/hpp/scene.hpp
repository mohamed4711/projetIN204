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

    // Accesseurs
    const Camera& GetCamera() const { return s_camera; }
    const hittable_list& GetObjects() const { return s_ObjectList; }
    const Light_list& GetLights() const { return s_Lights; }
    
   
    // Permet d'ajouter un objet dynamiquement
    void AddObject(std::shared_ptr<hittable> object) { 
        s_ObjectList.add(object); 
    }
    void AddLight(std::shared_ptr<Light> light) { 
        s_Lights.add(light); 
    }

    // Permet de vider la scène 
    void Clear() {
        s_ObjectList.clear();
        s_Lights.clear();
    }

   

private:
    Camera s_camera;
    hittable_list s_ObjectList;
    Light_list s_Lights;
};

#endif