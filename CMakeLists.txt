
cmake_minimum_required(VERSION 3.22)
project(RT)

# Find dependencies
find_package(OpenMP REQUIRED)
find_package(SDL2 REQUIRED)

# Source files
file(GLOB_RECURSE SOURCES
    "main.cpp"
    "dependencies/**/*.cpp"
    "interface/**/*.cpp"
    "rendering/*.cpp"
)
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/dependencies/objects/cpp/Vector3.cpp")

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Modern include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/dependencies
    ${CMAKE_SOURCE_DIR}/dependencies/utils/hpp
    ${CMAKE_SOURCE_DIR}/dependencies/objects/hpp
    ${CMAKE_SOURCE_DIR}/dependencies/camera/hpp
    ${CMAKE_SOURCE_DIR}/interface
    ${CMAKE_SOURCE_DIR}/rendering
)

# Link libraries
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)