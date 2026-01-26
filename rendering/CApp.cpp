#include "CApp.hpp"
#include <iostream>
#include <cstring>
#include "../dependencies/scene/hpp/Sceneloader.hpp"

CApp::CApp() {
    isRunning = true;
    pWindow = NULL;
    pRenderer = NULL;
}

bool CApp::OnInit() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;
    
    // Ajoute SDL_WINDOW_RESIZABLE pour être confort
    pWindow = SDL_CreateWindow("Projet IN204 - RayTracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    if (pWindow != NULL) {
        pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        m_image.Initialize(1280, 720, pRenderer);
    } else {
        return false;
    }
    // la doc ici :)
    //https://github.com/ocornut/imgui/wiki/Getting-Started/f8c6239561786dffd4a0b8916a1343a8873dd1b3#setting-up-dear-imgui--backends
    // --- INITIALISATION DEAR IMGUI ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark(); // Style foncé pro
    
    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(pWindow, pRenderer);
    ImGui_ImplSDLRenderer2_Init(pRenderer);
    // ---------------------------------

    // Charger la scène initiale
    SceneLoader::LoadJSON(m_jsonFilePath.c_str(), m_scene);
    
    return true;
}

void CApp::OnEvent(SDL_Event* event) {
    // 1. Passer l'événement à ImGui
    ImGui_ImplSDL2_ProcessEvent(event);
    
    // 2. Si ImGui utilise la souris/clavier, on ne fait rien d'autre
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse || io.WantCaptureKeyboard) {
        
    }

    if (event->type == SDL_QUIT) isRunning = false;
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_CLOSE && event->window.windowID == SDL_GetWindowID(pWindow)) isRunning = false;
}

void CApp::OnLoop() {
    // Logique de mise à jour (vide pour l'instant)
}

void CApp::OnRender() {
    // --- 1. DÉBUT FRAME IMGUI ---
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // --- 2. DESSIN DE L'INTERFACE ---
    {
        ImGui::Begin("Contrôles"); // Fenêtre flottante

        ImGui::Text("Performances : %.1f FPS", ImGui::GetIO().Framerate);
        ImGui::Separator();

        // Champ pour entrer le chemin du fichier JSON
        static char jsonPathBuffer[512];
        static bool bufferInitialized = false;
        if (!bufferInitialized) {
            strncpy(jsonPathBuffer, m_jsonFilePath.c_str(), sizeof(jsonPathBuffer) - 1);
            bufferInitialized = true;
        }
        
        ImGui::Text("Fichier JSON:");
        ImGui::InputText("##jsonpath", jsonPathBuffer, sizeof(jsonPathBuffer));
        ImGui::SameLine();
        if (ImGui::Button("Charger")) {
            m_jsonFilePath = std::string(jsonPathBuffer);
            std::cout << "Chargement du fichier: " << m_jsonFilePath << std::endl;
            SceneLoader::LoadJSON(m_jsonFilePath.c_str(), m_scene);
            std::cout << "Scène chargée avec succès!" << std::endl;
        }
        ImGui::Separator();
        
        ImGui::Text("Paramètres de Rendu");
        ImGui::SliderInt("Samples (Qualité)", &m_samples, 1, 100);
        ImGui::SliderInt("Rebonds (Depth)", &m_depth, 1, 50);

        if (ImGui::Button("LANCER RENDU ")) {
            // Configuration du moteur
            m_renderer.SetSamplesPerPixel(m_samples);
            m_renderer.SetMaxDepth(m_depth);
            
            std::cout << "Début du rendu..." << std::endl;
            // Appel à ton moteur existant
            m_renderer.Render(m_scene, m_image);
            std::cout << "Fin du rendu." << std::endl;
        }

        // Champ pour le nom du fichier de sauvegarde
        static char savePathBuffer[512] = "render_output.ppm";
        ImGui::Text("Fichier de sauvegarde:");
        ImGui::InputText("##savepath", savePathBuffer, sizeof(savePathBuffer));
        if (ImGui::Button("Sauvegarder Image")) {
            m_image.SavePPM(savePathBuffer);
            std::cout << "Image sauvegardée: " << savePathBuffer << std::endl;
        }

        ImGui::End();
    }

    // --- 3. RENDU FINAL SDL ---
    ImGui::Render();
    
    // Couleur de fond (Gris foncé)
    SDL_SetRenderDrawColor(pRenderer, 50, 50, 50, 255);
    SDL_RenderClear(pRenderer);

    // Afficher l'image du RayTracer (si elle existe)
    // Note: Ta classe Image gère sa propre texture SDL, donc on l'appelle avant ImGui
    m_image.Display(); 

    // Dessiner l'interface ImGui par-dessus tout le reste
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), pRenderer);

    SDL_RenderPresent(pRenderer);
}

void CApp::OnExit() {
    // Nettoyage ImGui
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    pWindow = NULL;
    SDL_Quit();
}

int CApp::OnExecute() {
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