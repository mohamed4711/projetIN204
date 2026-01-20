#ifndef SCENE_H
#define SCENE_H

#include "../../camera/hpp/Camera.hpp"
#include "../../objects/hpp/_Hittable_object_list.hpp"
#include <memory>
#include <vector>

class Scene {
public:
    Scene(); 

    // Accesseurs
    const Camera& GetCamera() const { return s_camera; }
    const hittable_list& GetObjects() const { return s_ObjectList; }
    
   
    // Permet d'ajouter un objet dynamiquement
    void AddObject(std::shared_ptr<hittable> object) { 
        s_ObjectList.add(object); 
    }

    // Permet de vider la scène 
    void Clear() {
        s_ObjectList.clear();
    }
   

private:
    Camera s_camera;
    hittable_list s_ObjectList;
};

#endif