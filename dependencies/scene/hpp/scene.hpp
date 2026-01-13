#ifndef SCENE_H
#define SCENE_H

#include "camera/hpp/Camera.hpp"
#include "objects/hpp/_Hittable_object_list.hpp"
#include <memory>

class Scene {
public:
    Scene(); // Construit la scène aléatoire

    // Accesseurs pour le moteur de rendu
    const Camera& GetCamera() const { return s_camera; }
    const hittable_list& GetObjects() const { return s_ObjectList; }

private:
    Camera s_camera;
    hittable_list s_ObjectList;
};

#endif