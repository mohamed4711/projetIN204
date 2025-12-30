cmake_minimum_required(VERSION 3.22)
project(RT)

# Inclure tous les répertoires d'en-têtes
include_directories(dependencies)
include_directories(dependencies/utils/hpp)
include_directories(dependencies/objects/hpp)
include_directories(dependencies/camera/hpp)
include_directories(interface)
include_directories(rendering)

# Trouver tous les fichiers .cpp dans le projet, en excluant les doublons
file(GLOB_RECURSE SOURCES
    "main.cpp"
    "dependencies/camera/cpp/*.cpp"
    "dependencies/lights/cpp/*.cpp"
    "dependencies/objects/cpp/*.cpp"
    "dependencies/scene/cpp/*.cpp"
    "dependencies/utils/cpp/*.cpp"
    "dependencies/RTMotors/cpp/*.cpp"  # <-- AJOUTER CETTE LIGNE
    "rendering/*.cpp"
)

# Exclure le Vector3.cpp dupliqué dans objects/cpp
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/dependencies/objects/cpp/Vector3.cpp")

# Créer l'exécutable avec tous les fichiers sources trouvés
add_executable(${PROJECT_NAME} ${SOURCES})

# Lier SDL2
find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2)