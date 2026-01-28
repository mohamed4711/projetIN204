
/*
    RayTracerApp.hpp
    Main application class using SDL2 and ImGui
    Handles window, events and rendering loop
*/

#ifndef RAYTRACERAPP_HPP
#define RAYTRACERAPP_HPP

#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include "../dependencies/utils/hpp/Image.hpp"
#include "../dependencies/scene/hpp/scene.hpp"
#include "../dependencies/RTMotors/hpp/Renderer.hpp"
#include "../dependencies/RTMotors/hpp/SimpleRenderer.hpp"
#include "../dependencies/RTMotors/hpp/ParallelRenderer.hpp"


#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "../dependencies/objects/hpp/Sphere.hpp"
#include "../dependencies/materials/hpp/Lambertian.hpp"
#include "../dependencies/materials/hpp/Metal.hpp"
#include "../dependencies/materials/hpp/Dielectric.hpp"
#include "../dependencies/lights/hpp/PointLight.hpp"

class RayTracerApp {
public:
    RayTracerApp();
    void ApplyEditorStyle(); // custom ImGui style
    int OnExecute();    // main loop entry point
    bool OnInit();      // initialize SDL and ImGui
    void OnEvent(SDL_Event* event);  // handle input events
    void OnLoop();      // update logic
    void OnRender();    // draw frame
    void OnExit();      // cleanup resources

private:
    bool isRunning;
    SDL_Window* pWindow;
    SDL_Renderer* pRenderer;
    
    // rendering engine and scene
    Image m_image;
    Scene m_scene;
    
    // Polymorphic renderer (can switch at runtime)
    std::shared_ptr<Renderer> m_renderer;
    int m_lastMotorType = -1;
    
    void CreateRenderer();
    void LoadScene();
    void SaveImage(const std::string& format);
    void CreateDefaultScene();

    // UI state (sliders)
    int m_samples = 5;
    int m_depth = 5;
    bool m_renderRequested = false;
    double m_lastRenderTime = 0.0;
    
    // Motor selection (0 = Default, 1 = OpenMP)
    int m_motorType = 1;
    
    // Loader selection (0 = Default, 1 = BVH)
    int m_loaderType = 1;
    int m_lastLoaderType = -1;
    
    // Scene selection (0 = Default, 1 = Upload custom)
    int m_sceneType = 0;
    
    // Save format (0 = PNG, 1 = PPM)
    int m_saveFormat = 1;
    
    // Log capture for UI display
    std::string m_renderLog;
    
    // path to loaded JSON scene file
    std::string m_jsonFilePath = "../SceneFromJson/Scene01.json";
    
};

#endif