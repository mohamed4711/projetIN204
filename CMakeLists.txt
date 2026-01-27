cmake_minimum_required(VERSION 3.22)
project(RT)

# existing dependencies
find_package(OpenMP REQUIRED)
find_package(SDL2 REQUIRED)

# adding Dear ImGui from the local folder
set(imgui_SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/imgui")

# Create the ImGui library
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

# Link SDL2 to ImGui
target_link_libraries(imgui PUBLIC SDL2::SDL2)

# source files 
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

list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/dependencies/objects/cpp/Vector3.cpp")

# executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/dependencies
    ${CMAKE_SOURCE_DIR}/dependencies/utils/hpp
    ${CMAKE_SOURCE_DIR}/dependencies/objects/hpp
    ${CMAKE_SOURCE_DIR}/dependencies/camera/hpp
    ${CMAKE_SOURCE_DIR}/interface
    ${CMAKE_SOURCE_DIR}/rendering
)

# --- 6. Link ---
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()

# Link SDL2 and ImGui
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 imgui)