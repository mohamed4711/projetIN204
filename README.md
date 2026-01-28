# Ray Tracer (RT) - IN204

Un moteur de rendu par lancer de rayons (ray tracing) développé en C++17, inspiré de la série *Ray Tracing in One Weekend* et adapté pour le projet IN204. Le projet s’appuie sur SDL2 et ImGui pour l’affichage et l’interface en temps réel.

---

## Analyse fonctionnelle

### Objectif global
Mettre en place un moteur de ray tracing capable de :
- Générer des images photoréalistes par lancer de rayons
- Modéliser des scènes 3D avec différents types d’objets
- Appliquer des matériaux diffus, métalliques et diélectriques
- Gérer des sources lumineuses (directionnelle, ponctuelle, spot)
- Accélérer les intersections via BVH et parallélisation OpenMP
- Permettre une interaction simple via une interface ImGui

### Fonctionnalités principales

1) Rendu par ray tracing
   - Lancer de rayons depuis la caméra à travers chaque pixel
   - Calcul d’intersection rayon‑objet
   - Réflexion et réfraction selon le matériau

2) Objets géométriques
   - Sphères, plans, cylindres, cônes
   - Triangles et maillages (mesh)
   - Parallélépipèdes rectangles
   - Hiérarchie BVH pour l’accélération

3) Matériaux et lumière
   - Diffus (Lambertien)
   - Métallique (avec rugosité)
   - Diélectrique (verre)
   - Lumières directionnelles, ponctuelles et spots

4) Interactivité
   - Paramètres de rendu configurables via ImGui
   - Sauvegarde d’images en PPM
   - Ajustement en temps réel de la caméra et des matériaux

**Flowchart du système :**
![Ray Tracing Flow Chart](flow_chart.drawio.png)

---

## Principes de ray tracing

### Rappel rapide
Le lancer de rayons simule la propagation de la lumière. Pour chaque pixel, un rayon est envoyé depuis la caméra vers la scène :

1. **Intersection** : on récupère l’objet le plus proche touché par le rayon
2. **Éclairage** : on estime la lumière reçue au point d’impact
3. **Récursion** : réflexion/réfraction si le matériau le nécessite
4. **Accumulation** : on cumule les contributions jusqu’à une profondeur maximale

**Schéma simplifié :**
```
Pixel (x, y)
  -> Rayon primaire (caméra -> pixel)
      -> Intersection avec objet
      -> Calcul ombrage direct (lumières)
      -> Si réfléchissant : rayon réfléchi
      -> Si transparent : rayon transmis
```

![Ray Tracing Principle](image.png)

### Accélération avec BVH (Bounding Volume Hierarchy)
Sans structure d’accélération, chaque rayon teste tous les objets (complexité O(n × m)).  
La BVH regroupe les objets dans un arbre binaire de boîtes englobantes (AABB) :

1. Construction récursive en triant les objets par axe
2. Division en sous‑arbres gauche/droit
3. Test rapide sur la boîte englobante avant de descendre dans l’arbre

**Structure typique :**
```
           AABB (root)
          /          \
     AABB (L)      AABB (R)
     /     \       /     \
   O1      O2    O3      O4
```

**Implémentation dans le projet :**
- `dependencies/objects/_AABB.hpp` : AABB et test d’intersection
- `dependencies/objects/_bvh_node.hpp` : construction et traversée BVH

---

## Structure des dossiers

### `dependencies/`
Modules de base du moteur :

#### `camera/`
- Ray.hpp/cpp : structure d’un rayon
- Camera.hpp/cpp : génération des rayons

#### `lights/`
- Light.hpp/cpp : classe abstraite
- DirectionalLight.hpp/cpp : lumière directionnelle
- PointLight.hpp/cpp : lumière ponctuelle
- SpotLight.hpp/cpp : spot
- Light_list.hpp/cpp : liste de lumières

#### `materials/`
- Material.hpp/cpp : classe abstraite
- Lambertian.hpp/cpp : matériau diffus
- Metal.hpp/cpp : matériau métallique
- Dielectric.hpp/cpp : matériau transparent

#### `objects/`
- _Generic.hpp : `hittable` + `hit_record`
- Sphere.hpp/cpp, Plan.hpp/cpp, Triangle.hpp/cpp
- Cylinder.hpp/cpp, Cone.hpp/cpp, Parallepiped.hpp/cpp
- Mesh.hpp/cpp : mesh triangulé
- _Hittable_object_list.hpp/cpp : conteneur d’objets
- _AABB.hpp : boîtes englobantes
- _bvh_node.hpp : hiérarchie BVH

#### `RTMotors/`
- Renderer.hpp/cpp : classe abstraite
- SimpleRenderer.hpp/cpp : rendu mono‑thread
- ParallelRenderer.hpp/cpp : rendu OpenMP

#### `scene/`
- scene.hpp/cpp : gestion des objets et lumières
- SceneLoader.hpp/cpp : chargement JSON

#### `utils/`
- Vector3.hpp : vecteur 3D
- Image.hpp/cpp : images en mémoire
- ColorUtils.hpp : utilitaires de couleur
- Random.hpp : générateur aléatoire

### `rendering/`
Interface SDL2/ImGui :
- RayTracerApp.hpp/cpp : application principale

### `external/`
- imgui/ : interface utilisateur Dear ImGui (submodule)

### `SceneFromJson/`
- Scènes de démonstration au format JSON

### `build/`
- Dossier généré par CMake

---

## Installation et compilation

### Prérequis
- Compilateur C++ (g++/clang/msvc) compatible C++17
- CMake ≥ 3.22
- SDL2
- OpenMP

Sur **Ubuntu/Debian/WSL** :
```bash
sudo apt update
sudo apt install build-essential cmake libsdl2-dev libomp-dev
```

Sur **Fedora** :
```bash
sudo dnf install gcc-c++ cmake SDL2-devel libomp-devel
```

Sur **macOS** (Homebrew) :
```bash
brew install cmake sdl2 libomp
```

### Compilation
```bash
cd <chemin_du_projet>
mkdir -p build
cd build
cmake ..
cmake --build . -j
```

Le binaire `RT` est généré dans `build/`.

---

## Lancement et utilisation

### Exécution
```bash
cd build
./RT
```

### Interface utilisateur
La fenêtre SDL2 affiche :
1. **Fenêtre de rendu** : rendu en temps réel
2. **Panneau ImGui** :
   - Sélection de scène
   - Paramètres de rendu (samples, bounces)
   - Contrôles de caméra
   - Paramètres matériaux

### Format JSON (exemple minimal)
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

## Architecture et design patterns

- **Héritage polymorphe** : `hittable`, `Material`, `Light`, `Renderer`
- **Composition** : `Scene` regroupe objets et lumières
- **Strategy** : choix du renderer (simple/parallèle)
- **Factory** : création d’objets via le loader JSON

**Hiérarchie simplifiée :**
```
hittable
  -> Sphere / Plan / Triangle / Cylinder / Cone / Parallepiped / Mesh
  -> hittable_list
  -> bvh_node

Material
  -> Lambertian / Metal / Dielectric

Light
  -> DirectionalLight / PointLight / SpotLight

Renderer
  -> SimpleRenderer / ParallelRenderer
```

---

## Axes d’amélioration

### 1. Rendu
- Texture mapping et UV
- Normal maps
- HDRI / environment maps
- Depth of field et motion blur
- Caustics simples

### 2. Performance
- Accélération GPU (CUDA/OptiX)
- Structures alternatives (KD‑tree, Octree)
- Progressive rendering

### 3. Formats et export
- PNG/JPEG (actuellement PPM)
- OpenEXR (HDR)
- Export multi‑pass (normale, depth, albedo)

### 4. Scénographie et assets
- Import OBJ/FBX/glTF
- Gestionnaire d’assets
- Bibliothèque de matériaux

### 5. Interface utilisateur
- Viewport 3D pour édition de scène
- Gizmos de transformation
- Undo/Redo

### 6. Tests et qualité
- Tests unitaires d’intersections
- Benchmarks simples de performance

---

## Bibliographie (sélection)

1. *Ray Tracing in One Weekend* — Peter Shirley  
2. *Physically Based Rendering* — Pharr, Jakob, Humphreys  
3. *Ray Tracing Gems* — Haines, Akenine‑Möller  

---

## Licence
Ce projet est distribué sous licence MIT.  
La licence d’ImGui est disponible dans `external/imgui`.
