/*
    RayTracerApp.hpp
    Main application class using SDL2 and ImGui
    Handles window, events and rendering loop
*/

#ifndef RAYTRACERAPP_HPP
#define RAYTRACERAPP_HPP

#include <SDL2/SDL.h>
#include <vector>
#include "../dependencies/utils/hpp/Image.hpp"
#include "../dependencies/scene/hpp/scene.hpp"
#include "../dependencies/RTMotors/hpp/RTMotor.hpp" 


#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

class RayTracerApp {
public:
    RayTracerApp();
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
    
    DefaultRenderer m_renderer; 

    // UI state (sliders)
    int m_samples = 5;
    int m_depth = 5;
    bool m_renderRequested = false;
    
    // path to loaded JSON scene file
    std::string m_jsonFilePath = "../SceneFromJson/Scene01.json";
};

#endif
