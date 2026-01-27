/*
    RayTracerApp.cpp
    Application implementation with SDL2 window and ImGui interface
    Controls ray tracing parameters and scene loading
*/

#include "RayTracerApp.hpp"
#include <iostream>
#include <cstring>
#include <ctime>
#include <chrono>
#include <sstream>
#include "../dependencies/scene/hpp/Sceneloader.hpp"
#include "../dependencies/camera/hpp/Camera.hpp"

class LogCapture {
public:
    LogCapture(std::string& log_buffer) : m_log_buffer(log_buffer), m_old_cout(nullptr) {
        m_log_buffer.clear();
        m_old_cout = std::cout.rdbuf(m_string_stream.rdbuf());
    }
    
    ~LogCapture() {
        if (m_old_cout) {
            std::cout.rdbuf(m_old_cout);
            m_log_buffer = m_string_stream.str();
        }
    }
    
private:
    std::string& m_log_buffer;
    std::stringstream m_string_stream;
    std::streambuf* m_old_cout;
};

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

    LoadScene();
    CreateRenderer();
    
    return true;
}

void RayTracerApp::CreateRenderer() {
    if (m_motorType == 0) {
        m_renderer = std::make_shared<SimpleRenderer>();
        std::cout << "Switched to SimpleRenderer" << std::endl;
    } else {
        m_renderer = std::make_shared<ParallelRenderer>();
        std::cout << "Switched to ParallelRenderer" << std::endl;
    }
    m_lastMotorType = m_motorType;
}

void RayTracerApp::LoadScene() {
    LogCapture capture(m_renderLog);
    m_scene.Clear();
    
    if (m_sceneType == 0) {
        std::cout << "[Scene] Creating default scene" << std::endl;
        CreateDefaultScene();
    } else if (m_loaderType == 0) {
        std::cout << "[Loader] Using Default loader" << std::endl;
        SceneLoader::LoadJSON(m_jsonFilePath.c_str(), m_scene);
    } else {
        std::cout << "[Loader] Using BVH loader" << std::endl;
        SceneLoader::LoadJSONBVH(m_jsonFilePath.c_str(), m_scene);
    }
    
    m_lastLoaderType = m_loaderType;
}

void RayTracerApp::CreateDefaultScene() {
    std::cout << "Building default scene with 3 large spheres and many small spheres..." << std::endl;
    
    double aspect_ratio = 16.0 / 9.0;
    m_scene.SetupCamera(
        Point3(13, 2, 3),
        Point3(0, 0, 0),
        Vector3(0, 1, 0),
        20.0,
        aspect_ratio,
        0.1,
        10.0
    );
    
    auto ground = std::make_shared<Lambertian>(Vector3(0.5, 0.5, 0.5));
    m_scene.AddObject(std::make_shared<sphere>(Point3(0, -1000, 0), 1000, ground));
    
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
            
            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;
                
                if (choose_mat < 0.8) {
                    auto albedo = Vector3(random_double(), random_double(), random_double()) *
                                  Vector3(random_double(), random_double(), random_double());
                    sphere_material = std::make_shared<Lambertian>(albedo);
                } else if (choose_mat < 0.95) {
                    auto albedo = Vector3(0.5 + 0.5*random_double(),
                                         0.5 + 0.5*random_double(),
                                         0.5 + 0.5*random_double());
                    auto fuzz = 0.5 * random_double();
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                } else {
                    sphere_material = std::make_shared<Dielectric>(1.5);
                }
                
                m_scene.AddObject(std::make_shared<sphere>(center, 0.2, sphere_material));
            }
        }
    }
    
    auto material1 = std::make_shared<Dielectric>(1.5);
    m_scene.AddObject(std::make_shared<sphere>(Point3(0, 1, 0), 1.0, material1));
    
    auto material2 = std::make_shared<Lambertian>(Vector3(0.4, 0.2, 0.1));
    m_scene.AddObject(std::make_shared<sphere>(Point3(-4, 1, 0), 1.0, material2));
    
    auto material3 = std::make_shared<Metal>(Vector3(0.7, 0.6, 0.5), 0.0);
    m_scene.AddObject(std::make_shared<sphere>(Point3(4, 1, 0), 1.0, material3));
    
    auto light = std::make_shared<PointLight>(Point3(5, 5, 5), Vector3(1.0, 1.0, 1.0));
    m_scene.AddLight(light);
    
    std::cout << "Default scene created!" << std::endl;
}

void RayTracerApp::SaveImage(const std::string& format) {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm* timeinfo = std::localtime(&time);
    
    char timestamp[20];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", timeinfo);
    
    if (format == "png") {
        std::string filename = std::string("render_output_") + timestamp + ".png";
        m_image.SavePPM("render_output_temp.ppm");
        m_renderLog += "Saved as PPM (PNG not yet supported)\n";
        std::cout << "Saved as " << filename << " (PPM format)" << std::endl;
    } else {
        std::string filename = std::string("render_output_") + timestamp + ".ppm";
        m_image.SavePPM(filename);
        m_renderLog += std::string("Image saved as ") + filename + "!\n";
        std::cout << "Saved as " << filename << "!" << std::endl;
    }
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
    SDL_SetRenderDrawColor(pRenderer, 40, 40, 40, 255);
    SDL_RenderClear(pRenderer);
    
    m_image.Display();
    
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 screen_size = io.DisplaySize;
    
    float left_panel_width = 350.0f;
    float right_panel_width = screen_size.x - left_panel_width;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(left_panel_width, screen_size.y));
    ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    
    ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "RAY TRACER");
    ImGui::Separator();
    
    if (ImGui::Button("Reset", ImVec2((left_panel_width - 40) / 2 - 10, 35))) {
        m_samples = 5;
        m_depth = 5;
        m_motorType = 1;
        m_loaderType = 1;
        m_sceneType = 0;
        m_saveFormat = 1;
        m_lastRenderTime = 0.0;
        std::cout << "Settings reset to defaults" << std::endl;
    }
    ImGui::SameLine();
    if (ImGui::Button("Start", ImVec2((left_panel_width - 40) / 2 - 10, 35))) {
        if (m_motorType != m_lastMotorType) {
            CreateRenderer();
        }
        
        if (m_loaderType != m_lastLoaderType) {
            LoadScene();
        }
        
        m_renderer->SetSamplesPerPixel(m_samples);
        m_renderer->SetMaxDepth(m_depth);
        {
            LogCapture capture(m_renderLog);
            std::cout << "Starting render..." << std::endl;
            auto t1 = std::chrono::high_resolution_clock::now();
            m_renderer->Render(m_scene, m_image);
            auto t2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> ms_double = t2 - t1;
            m_lastRenderTime = ms_double.count();
            std::cout << "Render complete. Time: " << m_lastRenderTime << "ms" << std::endl;
        }
    }
    
    ImGui::Separator();
    
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Motors");
    ImGui::RadioButton("Default##motor", &m_motorType, 0);
    ImGui::RadioButton("OpenMP##motor", &m_motorType, 1);
    
    ImGui::Separator();
    
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Loaders");
    ImGui::RadioButton("Default##loader", &m_loaderType, 0);
    ImGui::RadioButton("BVH##loader", &m_loaderType, 1);
    
    ImGui::Separator();
    
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Scene");
    ImGui::RadioButton("Default##scene", &m_sceneType, 0);
    ImGui::RadioButton("Upload##scene", &m_sceneType, 1);
    
    if (m_sceneType == 1) {
        static char jsonPathBuffer[512];
        static bool bufferInitialized = false;
        if (!bufferInitialized) {
            strncpy(jsonPathBuffer, m_jsonFilePath.c_str(), sizeof(jsonPathBuffer) - 1);
            bufferInitialized = true;
        }
        
        ImGui::InputText("##jsonpath", jsonPathBuffer, sizeof(jsonPathBuffer));
        ImGui::SameLine();
        if (ImGui::Button("Load")) {
            m_jsonFilePath = std::string(jsonPathBuffer);
            std::cout << "Loading file: " << m_jsonFilePath << std::endl;
            SceneLoader::LoadJSONBVH(m_jsonFilePath.c_str(), m_scene);
            std::cout << "Scene loaded!" << std::endl;
        }
    }
    
    ImGui::Separator();
    
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Params");
    ImGui::SliderInt("Nb Sample", &m_samples, 1, 100);
    ImGui::SliderInt("Nb Bounce", &m_depth, 1, 25);
    
    ImGui::Separator();
    
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Results");
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.0f, 1.0f), "time t");
    ImGui::Text("%.1f ms", m_lastRenderTime);
    
    ImGui::Separator();
    
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Save options");
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Format:");
    ImGui::RadioButton("PNG##format", &m_saveFormat, 0);
    ImGui::RadioButton("PPM##format", &m_saveFormat, 1);
    
    if (ImGui::Button("Save", ImVec2(-1, 35))) {
        if (m_saveFormat == 0) {
            SaveImage("png");
        } else {
            SaveImage("ppm");
        }
    }
    
    ImGui::Separator();
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Logs");
    ImGui::TextWrapped("%s", m_renderLog.c_str());
    
    ImGui::End();

    // ============ RIGHT PANEL - Image Display ============
    ImGui::SetNextWindowPos(ImVec2(left_panel_width, 0));
    ImGui::SetNextWindowSize(ImVec2(right_panel_width, screen_size.y));
    ImGui::Begin("Render", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "Rendered Image");
    
    ImGui::End();

    ImGui::Render();
    
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
