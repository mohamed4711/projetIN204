# Ray Tracer (RT) - IN204

Un moteur de rendu par lancer de rayons (Ray Tracing) développé en C++ moderne, basé sur la série *Ray Tracing in One Weekend*. Ce projet utilise la SDL2 pour l'affichage en temps réel du rendu.

## 📋 Journal de Bord & État actuel

### Fait :
* **Environnement** : Configuration de la SDL2 sur WSL/Linux.
* **Maths** : Implémentation complète de la classe `Vector3` et des opérations vectorielles.
* **Objets** : Gestion des sphères et des listes d'objets (`hittable_list`).
* **Architecture** : 
    * Séparation de la classe `Ray` (hpp/cpp).
    * **Refactoring en cours** : Séparation du moteur de calcul (`Renderer`) et de la scène (`Scene`) pour une meilleure modularité.

---

##  Installation et Compilation

### Prérequis
Assurez-vous d'avoir installé les outils de compilation et la librairie SDL2.

Sur Ubuntu/WSL :
```bash
sudo apt update
sudo apt install build-essential cmake libsdl2-dev
```

### Compilation
Le projet utilise CMake. Placez-vous à la racine du projet :
```bash
mkdir build
cd build
cmake ..
make
```

### Exécution
```bash
./RT
```

## To-Do List

Gérer les textures (Texture Mapping / UV)
Rédiger l'analyse fonctionnelle et architecturale

si on'a le temps (Implémenter la sauvegarde d'image en PNG et JPG)

