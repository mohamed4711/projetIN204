/*
    RayTracerApp.cpp
    Application implementation with SDL2 window and ImGui interface
    Controls ray tracing parameters and scene loading
*/

#include "RayTracerApp.hpp"
#include <iostream>
#include <cstring>
#include "../dependencies/scene/hpp/Sceneloader.hpp"

RayTracerApp::RayTracerApp() {
    isRunning = true;
    pWindow = NULL;
    pRenderer = NULL;
}

bool RayTracerApp::OnInit() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;
    
    // create resizable window
    pWindow = SDL_CreateWindow("Projet IN204 - RayTracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    if (pWindow != NULL) {
        pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        m_image.Initialize(1280, 720, pRenderer);
    } else {
        return false;
    }
    
    // ImGui setup (see: https://github.com/ocornut/imgui/wiki/Getting-Started)
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    
    // setup platform/renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(pWindow, pRenderer);
    ImGui_ImplSDLRenderer2_Init(pRenderer);

    // load initial scene
    SceneLoader::LoadJSON(m_jsonFilePath.c_str(), m_scene);
    
    return true;
}

void RayTracerApp::OnEvent(SDL_Event* event) {
    // pass event to ImGui first
    ImGui_ImplSDL2_ProcessEvent(event);
    
    // check if ImGui wants mouse/keyboard
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse || io.WantCaptureKeyboard) {
        
    }

    if (event->type == SDL_QUIT) isRunning = false;
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_CLOSE && event->window.windowID == SDL_GetWindowID(pWindow)) isRunning = false;
}

void RayTracerApp::OnLoop() {
    // update logic (empty for now)
}

void RayTracerApp::OnRender() {
    // start ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // draw control panel
    {
        ImGui::Begin("Controls");

        ImGui::Text("Performance: %.1f FPS", ImGui::GetIO().Framerate);
        ImGui::Separator();

        // JSON file path input
        static char jsonPathBuffer[512];
        static bool bufferInitialized = false;
        if (!bufferInitialized) {
            strncpy(jsonPathBuffer, m_jsonFilePath.c_str(), sizeof(jsonPathBuffer) - 1);
            bufferInitialized = true;
        }
        
        ImGui::Text("JSON File:");
        ImGui::InputText("##jsonpath", jsonPathBuffer, sizeof(jsonPathBuffer));
        ImGui::SameLine();
        if (ImGui::Button("Load")) {
            m_jsonFilePath = std::string(jsonPathBuffer);
            std::cout << "Loading file: " << m_jsonFilePath << std::endl;
            SceneLoader::LoadJSON(m_jsonFilePath.c_str(), m_scene);
            std::cout << "Scene loaded!" << std::endl;
        }
        ImGui::Separator();
        
        ImGui::Text("Render Settings");
        ImGui::SliderInt("Samples (Quality)", &m_samples, 1, 100);
        ImGui::SliderInt("Bounces (Depth)", &m_depth, 1, 50);

        if (ImGui::Button("START RENDER")) {
            // configure renderer
            m_renderer.SetSamplesPerPixel(m_samples);
            m_renderer.SetMaxDepth(m_depth);
            
            std::cout << "Starting render..." << std::endl;
            m_renderer.Render(m_scene, m_image);
            std::cout << "Render complete." << std::endl;
        }

        if (ImGui::Button("Save Image")) {
            m_image.SavePPM("render_output.ppm");
        }

        ImGui::End();
    }

    // final SDL render
    ImGui::Render();
    
    // dark grey background
    SDL_SetRenderDrawColor(pRenderer, 50, 50, 50, 255);
    SDL_RenderClear(pRenderer);

    // display ray traced image
    m_image.Display(); 

    // draw ImGui on top
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), pRenderer);

    SDL_RenderPresent(pRenderer);
}

void RayTracerApp::OnExit() {
    // cleanup ImGui
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    pWindow = NULL;
    SDL_Quit();
}

// main application loop
int RayTracerApp::OnExecute() {
    if (OnInit() == false) return -1;
    SDL_Event event;
    while (isRunning) {
        while (SDL_PollEvent(&event) != 0) {
            OnEvent(&event);
        }
        OnLoop();
        OnRender();
    }
    OnExit();
    return 0;
}
