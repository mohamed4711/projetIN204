cmake_minimum_required(VERSION 3.22)
project(RT)

# --- 1. Dépendances existantes ---
find_package(OpenMP REQUIRED)
find_package(SDL2 REQUIRED)

# --- 2. Ajout de Dear ImGui depuis le dossier local ---
set(imgui_SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/imgui")

# Créer la bibliothèque ImGui
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer2.cpp
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

# Lier SDL2 à ImGui
target_link_libraries(imgui PUBLIC SDL2::SDL2)

# --- 3. Tes fichiers sources ---
file(GLOB_RECURSE SOURCES_DEPS 
    "${CMAKE_SOURCE_DIR}/dependencies/camera/cpp/*.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/lights/cpp/*.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/materials/cpp/*.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/objects/cpp/*.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/RTMotors/cpp/*.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/scene/cpp/*.cpp"
    "${CMAKE_SOURCE_DIR}/dependencies/utils/cpp/*.cpp"
)

file(GLOB SOURCES_RENDERING "${CMAKE_SOURCE_DIR}/rendering/*.cpp")
file(GLOB SOURCES_INTERFACE "${CMAKE_SOURCE_DIR}/interface/*.cpp")

set(SOURCES 
    "${CMAKE_SOURCE_DIR}/main.cpp"
    ${SOURCES_DEPS}
    ${SOURCES_RENDERING}
    ${SOURCES_INTERFACE}
)

# On garde ton exclusion spécifique
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/dependencies/objects/cpp/Vector3.cpp")

# --- 4. Exécutable ---
add_executable(${PROJECT_NAME} ${SOURCES})

# --- 5. Includes ---
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/dependencies
    ${CMAKE_SOURCE_DIR}/dependencies/utils/hpp
    ${CMAKE_SOURCE_DIR}/dependencies/objects/hpp
    ${CMAKE_SOURCE_DIR}/dependencies/camera/hpp
    ${CMAKE_SOURCE_DIR}/interface
    ${CMAKE_SOURCE_DIR}/rendering
    # Pas besoin d'ajouter ImGui ici manuellement, c'est géré par target_link_libraries
)

# --- 6. Link ---
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()

# On lie SDL2 et ImGui
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 imgui)