#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include<SDL2/SDL.h>
#include "utils/hpp/Image.hpp"
#include "camera/hpp/Camera.hpp"
#include "objects/hpp/Sphere.hpp"
#include"./objects/hpp/_Hittable_object_list.hpp"
#include "./lights/hpp/Light_list.hpp"


class Scene {
public:
    Scene();
    bool Render(Image& image);

private:
    Camera s_camera;
    hittable_list s_ObjectList ; 
    Light_list s_Lights;  

};
;

#endif 