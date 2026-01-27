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

    // configure camera from parameters
    void SetupCamera(Point3 lookfrom, Point3 lookat, Vector3 vup, 
                     double vfov, double aspect_ratio, double aperture, double focus_dist) {
        s_camera.Setup(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, focus_dist);
    }

    // direct camera access for setup
    Camera& GetCameraMutable() { return s_camera; }

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