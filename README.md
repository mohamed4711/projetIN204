# Ray Tracer (RT) - IN204

Un moteur de rendu par lancer de rayons (Ray Tracing) développé en C++ moderne, basé sur la série *Ray Tracing in One Weekend* et sur le modèle d'un ainé académique. Ce projet utilise SDL2 et ImGui pour l'affichage et l'interface en temps réel du rendu.

---

## Analyse Fonctionnelle

### Objectif Global
Ce projet a pour objectif de mettre en place un moteur de ray tracing capable de :
- Générer des images photoréalistes grâce à la technique du lancer de rayons
- Modéliser des scènes 3D complexes avec différents types d'objets géométriques
- Appliquer des matériaux réalistes (Lambertien, métallique, diélectrique)
- Gérer l'éclairage via des sources lumineuses (directionnelle, ponctuelle, spot)
- Optimiser le rendu via parallélisation multi-thread (OpenMP) et structure de hiérarchie BVH
- Visualiser en temps réel et interagir via une interface ImGui

### Fonctionnalités Principales

1. Rendu par Ray Tracing
   - Lancer de rayons depuis la caméra à travers chaque pixel
   - Calcul d'intersection ray-objet
   - Récursion pour les réflexions et réfractions

2. Objets Géométriques
   - Sphères, Plans, Cylindres, Cônes
   - Triangles et Mailles (Mesh)
   - Parallélépipèdes rectangles
   - Hiérarchie BVH pour l'accélération

3. Matériaux et Lumière
   - Matériaux Lambertiens (diffus)
   - Matériaux Métalliques avec rugosité
   - Matériaux Diélectriques (verre, transparent)
   - Sources lumineuses : directionnelle, ponctuelle, spotlights

4. Interactivité
   - Interface ImGui pour configurer la scène
   - Sauvegarde de rendus en format PPM
   - Contrôle des paramètres en temps réel

---

## Structure des Dossiers

### `/dependencies`
Contient tous les modules de base du moteur :

#### `camera/`
- Ray.hpp/cpp : Structure d'un rayon avec origine et direction
- Camera.hpp/cpp : Gestion de la caméra, génération des rayons
- Contient les calculs de transformations d'espace

#### `lights/`
- Light.hpp/cpp : Classe abstraite de base pour les lumières
- DirectionalLight.hpp/cpp : Lumière directionnelle (like sun)
- PointLight.hpp/cpp : Lumière ponctuelle (ampoule)
- SpotLight.hpp/cpp : Lumière avec cône de diffusion
- Light_list.hpp/cpp : Gestionnaire de liste de lumières

#### `materials/`
- Material.hpp/cpp : Classe abstraite pour les matériaux
- Lambertian.hpp/cpp : Matériau diffus (mur, papier)
- Metal.hpp/cpp : Matériau réfléchissant avec rugosité
- Dielectric.hpp/cpp : Matériau transparent (verre, eau)
- Implémentent le comportement de la lumière sur les surfaces

#### `objects/`
- _Generic.hpp : Classe `hittable` abstraite et `hit_record` (intersection)
- Sphere.hpp/cpp : Objet sphérique
- Plan.hpp/cpp : Plan infini
- Triangle.hpp/cpp : Triangle simple
- Cylinder.hpp/cpp : Cylindre
- Cone.hpp/cpp : Cône
- Parallepiped.hpp/cpp : Boîte rectangulaire
- Mesh.hpp/cpp : Maille composée de triangles
- _Hittable_object_list.hpp/cpp : Conteneur d'objets
- _AABB.hpp : Boîte englobante (Axis-Aligned Bounding Box)
- _bvh_node.hpp : Hiérarchie BVH pour accélération

#### `RTMotors/`
- Renderer.hpp/cpp : Classe abstraite pour les moteurs de rendu
- SimpleRenderer.hpp/cpp : Rendu simple (mono-thread)
- ParallelRenderer.hpp/cpp : Rendu parallélisé avec OpenMP
- Contient la logique principale du ray tracing

#### `scene/`
- scene.hpp/cpp : Gestion de la scène (objets + lumières)
- SceneLoader.hpp/cpp : Chargement depuis fichiers JSON

#### `utils/`
- Vector3.hpp : Classe 3D vectorielle (position, direction, couleur)
- Image.hpp/cpp : Gestion des images en mémoire
- ColorUtils.hpp : Utilitaires de manipulation de couleur
- Random.hpp : Générateur de nombres aléatoires

### `/rendering`
Interface avec le système :
- RayTracerApp.hpp/cpp : Application principale SDL2/ImGui
- Gère la fenêtre, les événements, la boucle de rendu
- Intégration de l'interface utilisateur

### `/external`
- imgui/ : Interface utilisateur Dear ImGui (submodule)
- Permet la configuration interactive

### `/SceneFromJson`
- **Scene01.json, Scene02.json** : Fichiers de description de scènes
- Format JSON pour la sérialisation

### `/build`
- Dossier généré par CMake contenant l'exécutable compilé

---

## Installation et Compilation

### Prérequis
- Compilateur C++ (g++ ou clang) supportant C++17
- CMake en version 3.22 ou supérieure
- Bibliothèque SDL2
- OpenMP pour la parallélisation

Sur **Ubuntu/Debian/WSL** :
```bash
sudo apt update
sudo apt install build-essential cmake libsdl2-dev libomp-dev
```

Sur **Fedora** :
```bash
sudo dnf install gcc-c++ cmake SDL2-devel libomp-devel
```

Sur **macOS** (avec Homebrew) :
```bash
brew install cmake sdl2 libomp
```

### Compilation
```bash
cd /home/naps/Documents/Ensta/POO/projetIN204
mkdir -p build
cd build
cmake ..
make [-j$(nproc)]
```

Le binaire `RT` sera généré dans le dossier `build/`.

---

## Lancement et Utilisation

### Exécution
```bash
cd build
./RT
```

### Interface Utilisateur

Une fenêtre SDL2 s'ouvrira avec :
1. **Fenêtre de rendu** : Affichage du ray tracing en temps réel
2. **Panneau ImGui** (côté droit) :
   - **Scene Controls** : Sélection et configuration de la scène
   - **Render Settings** : Paramètres de rendu
     - Nombre d'échantillons par pixel (samples)
     - Profondeur de récursion (max bounces)
   - **Camera Controls** : Position et orientation de la caméra
   - **Material Selection** : Choix des matériaux

### Interaction avec les Scènes
Les scènes JSON peuvent être chargées pour initialiser des configurations pré-définies :
```json
{
  "objects": [
    {"type": "sphere", "center": [0, 0, 0], "radius": 1.0, "material": "lambertian"}
  ],
  "lights": [
    {"type": "directional", "direction": [1, 1, 1], "intensity": 1.0}
  ]
}
```

---

## Architecture et Design Patterns

### Patterns Utilisés
- **Héritage polymorphe** : Classes abstraites `hittable`, `Material`, `Light`, `Renderer`
- **Composition** : `Scene` contient des listes d'objets et de lumières
- **Strategy** : Différents types de renderers (Simple, Parallel)
- **Factory** : Création d'objets via le loader JSON

### Hiérarchie des Classes

```
hittable (abstract)
├── Sphere
├── Plan
├── Triangle
├── Cylinder
├── Cone
├── Parallepiped
├── Mesh
├── hittable_list
└── bvh_node (acceleration structure)

Material (abstract)
├── Lambertian
├── Metal
└── Dielectric

Light (abstract)
├── DirectionalLight
├── PointLight
└── SpotLight

Renderer (abstract)
├── SimpleRenderer
└── ParallelRenderer
```

---

## Axes d'Amélioration

### 1. Fonctionnalités de Rendu
- Implémenter le texture mapping avec coordonnées UV
- Ajouter les normales maps pour détails de surface
- Support des maps d'environment (HDRI)
- Denoising post-rendu (filtre temporal, ML-based)
- Depth of field et motion blur
- Caustics et effets sub-surface scattering

### 2. Performance
- Implémenter le GPU ray tracing (CUDA, OptiX)
- Spatial partitioning plus avancé (KD-tree, Octree)
- Caching des rayons tracés
- Progressive rendering avec refinement itératif
- Optimisation du memory bandwidth

### 3. Format et Export
- Export en PNG et JPEG (actuellement PPM uniquement)
- Support des formats OpenEXR (HDR)
- Export en multi-pass (normal map, depth, albedo)
- Sauvegarde des paramètres de rendu

### 4. Scénographie et Assets
- Import de modèles OBJ, FBX, glTF
- Gestionnaire de ressources Assets
- Bibliothèque de matériaux pré-définis
- Éditeur visuel de scène intégré
- Support du skeletal animation

### 5. Interface Utilisateur
- Viewport 3D interactif pour éditer la scène
- Gizmos pour transformer les objets (translate, rotate, scale)
- Real-time preview au niveau de la caméra
- Histogramme et statistiques de luminance
- Undo/Redo système

### 6. Optimisations Mathématiques
- SIMD vectorization (SSE, AVX)
- Half-precision floats pour certains calculs
- Approximations pour fonctions trigonométriques lentes

### 7. Testing et Qualité
- Unit tests pour les intersections géométriques
- Tests de convergence du ray tracer
- Benchmarking de performance
- Validation contre des références (Arnold, V-Ray)

### 8. Documentation et Exemples
- Tutoriels pas à pas
- Galerie de scènes d'exemple
- Documentation des APIs internes
- Contribution guide pour développeurs

---

## Bibliographies

### Livres de Référence
1. **"Ray Tracing in One Weekend"** - Peter Shirley (Series)
   - Fondation du projet, algorithmes de base
   - Accessible et progressive

2. **"Physically Based Rendering"** - Matt Pharr, Wenzel Jakob, Greg Humphreys (3rd Edition, 2016)
   - Bible du ray tracing physiquement basé
   - Couvre BSDF, spectral rendering, advanced techniques
   - https://www.pbrt.org/

3. **"Ray Tracing Gems"** - Eric Haines & Tomas Akenine-Möller (Series)
   - Collections d'articles pratiques et optimisations
   - Techniques récentes et applications réelles

### Articles Académiques et Resources Web
4. **"An Introduction to Ray Tracing"** - Andrew S. Glassner
   - Classique académique complet
   - Géométrie et algorithmes fondamentaux

5. **Real-Time Rendering (4th Edition)** - Möller, Haines, Hoffman (2018)
   - Couvre ray tracing et rasterization
   - Modern graphics pipeline

6. **NVIDIA OptiX Documentation**
   - Reference pour GPU ray tracing
   - Patterns d'optimisation parallèle

### Ressources en Ligne
- **Path Tracing Series** - YouTube, travaux académiques
- **Daniel Github code** (https://github.com/DanielFrulane/Ray-Tracer-Application /) - 

### Implémentation Spécifique
7. **"BVH Acceleration Structures"** - Akenine-Möller et al.
   - Essentiellement dans PBRT et Ray Tracing Gems

8. **"Importance Sampling for Monte Carlo Path Tracing"**
   - Variance reduction techniques
   - Critical for efficient rendering

### Outils et Frameworks
- OpenMP Documentation - Parallélisation multicore
- SDL2 Documentation - Graphics display and events
- Dear ImGui - https://github.com/ocornut/imgui

### Ressources Locales
Les documents suivants sont disponibles dans le dossier `/ressources` du projet :

9. "Object-Oriented Ray-Tracing" (2023)
   - Approche orientée objet du ray tracing
   - Architecture logicielle et design patterns
   - Directement applicable au projet

10. Thèse - Lancelot Perrotte (2011)
    - Recherche académique en informatique graphique
    - Fondements théoriques du ray tracing
    - Optimisations et techniques avancées

---

## État Actuel et Changelog

### Fonctionnalités Implémentées
- Ray tracing basique avec intersections géométriques
- Plusieurs types d'objets géométriques (sphère, plan, triangle, etc.)
- Trois types de matériaux (Lambertian, Metal, Dielectric)
- Sources lumineuses (directionnelle, ponctuelle, spotlight)
- Accélération BVH pour performances
- Parallélisation multi-thread (OpenMP)
- Visualisation SDL2 + ImGui
- Sauvegarde en PPM
- Chargement de scènes JSON

### En Cours
- Refactoring architecture Renderer/Scene
- Optimisations de performance

### À Faire
- Texture Mapping / UV coordinates
- Export PNG/JPEG
- Éditeur visuel complet
- GPU ray tracing

---

## Contributeurs
Projet pédagogique ENSTA - IN204 (Programmation Orientée Objet)

## Licence

The MIT License (MIT)

Copyright (c) 2014-2026 Omar Cornut

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

