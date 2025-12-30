#ifndef CAPP_H
#define CAPP_H

#include <SDL2/SDL.h>
#include "../dependencies/scene/hpp/scene.hpp"
#include "../dependencies/utils/hpp/Image.hpp" 
#include "RTMotors/cpp/RTMotorOpenMP.cpp" // Inclusion du moteur

class CApp
{
    public:
        CApp();
        int OnExecute();
        
        // ... (OnInit, OnEvent, etc. inchangés) ...
        bool OnInit();
        void OnEvent(SDL_Event *event);
        void OnLoop();
        void OnRender();
        void OnExit();
        
    private:
        Image m_image;
        Scene m_scene;      // La donnée
        RendererOpenMP m_renderer;// Le moteur
        
        bool isRunning;
        SDL_Window *pWindow;
        SDL_Renderer *pRenderer;
        bool m_isRendered = false; // Pour ne calculer qu'une seule fois
};
#endif