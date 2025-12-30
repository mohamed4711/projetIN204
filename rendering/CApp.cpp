#include "CApp.hpp"
#include <iostream>

CApp::CApp() {
    isRunning = true;
    pWindow = NULL;
    pRenderer = NULL;
    // Initialize render flag
    m_isRendered = false; 
}  

bool CApp::OnInit() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;
    
    pWindow = SDL_CreateWindow("RayTracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 1280, SDL_WINDOW_SHOWN);
    
    if (pWindow != NULL) {
        pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
        m_image.Initialize(1280, 1280, pRenderer);
    } else {
        return false;
    }
    return true;
}

int CApp::OnExecute() {
    SDL_Event event;
    if (OnInit() == false) return -1;

    // --- RENDER ONCE HERE ---
    if (!m_isRendered) {
        std::cout << "Starting Render..." << std::endl;
        m_renderer.Render(m_scene, m_image); 
    
        m_image.SavePPM("final_render.ppm");
        m_isRendered = true;
        std::cout << "Render Done! Window will now display result." << std::endl;
    }
    // ------------------------
    
    while (isRunning) {
        while (SDL_PollEvent(&event) != 0) {
            OnEvent(&event);
        }
        OnLoop();
        OnRender(); // Only displays the already rendered image
    }
    OnExit();
    return 0;
}

// ... Keep OnEvent, OnLoop as they were ...

void CApp::OnEvent(SDL_Event *event) {
    if (event->type == SDL_QUIT) isRunning = false;
}
void CApp::OnLoop() {}

void CApp::OnRender()
{
    SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
    SDL_RenderClear(pRenderer);
    
    // On ne lance le calcul (très lourd) qu'une seule fois
    if (!m_isRendered) {
        m_renderer.Render(m_scene, m_image);
        m_isRendered = true;
    }
    
    m_image.Display();
    SDL_RenderPresent(pRenderer);
}
void CApp::OnExit() {
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    pWindow = NULL;
    SDL_Quit();
}