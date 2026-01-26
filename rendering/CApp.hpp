#ifndef CAPP_HPP
#define CAPP_HPP

#include <SDL2/SDL.h>
#include <vector>
#include "../dependencies/utils/hpp/Image.hpp"
#include "../dependencies/scene/hpp/scene.hpp"
#include "../dependencies/RTMotors/hpp/RTMotor.hpp" 


#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

class CApp {
public:
    CApp();
    int OnExecute();
    bool OnInit();
    void OnEvent(SDL_Event* event);
    void OnLoop();
    void OnRender();
    void OnExit();

private:
    bool isRunning;
    SDL_Window* pWindow;
    SDL_Renderer* pRenderer;
    
    // Moteur et Scène
    Image m_image;
    Scene m_scene;
    
    DefaultRenderer m_renderer; 

    // État de l'interface (pour les sliders)
    int m_samples = 5;
    int m_depth = 5;
    bool m_renderRequested = false;
    
    // Chemin du fichier JSON chargé
    std::string m_jsonFilePath = "../SceneFromJson/Scene01.json";
};

#endif