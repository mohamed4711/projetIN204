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

## TODO List (Objectifs à réaliser)

1. Architecture & Modularité (Prioritaire)

[ ]z Séparer le Moteur et la Scène :

Déplacer la logique de boucle de rendu (for y, for x...) de Scene::Render vers une nouvelle classe Renderer.

La classe Scene ne doit contenir que les objets et la caméra.

[ ] Nettoyage CMake : Vérifier que Renderer.cpp est bien inclus dans la compilation et supprimer les fichiers doublons (ex: dependencies/objects/cpp/Vector3.cpp s'il existe encore).

2. Fonctionnalités du Moteur (Renderer)

[ ] Anti-Aliasing (MSAA) : Implémenter le multi-échantillonnage (lancer plusieurs rayons aléatoires par pixel et faire la moyenne).

[ ] Gamma Correction : Appliquer une racine carrée (sqrt) sur les couleurs finales pour corriger l'affichage.

[ ] Gestion de la profondeur : Limiter le nombre de rebonds (recursion depth) pour éviter les boucles infinies.

3. Matériaux Avancés

[ ] Lambertian : Finir l'implémentation de la diffusion mate (rebonds aléatoires).

[ ] Metal : Ajouter la réflexion spéculaire avec un paramètre de "fuzz" (flou métallique).

[ ] Dielectric (Verre) : Implémenter la réfraction (loi de Snell) et la réflexion de Fresnel (Schlick approximation) pour les objets transparents.

4. Caméra & Scène

[ ] Depth of Field (Flou) : Ajouter les paramètres aperture (ouverture) et focus_dist à la caméra pour simuler le flou de mise au point.

[ ] Génération Aléatoire : Créer une fonction pour générer la scène finale avec de nombreuses petites sphères aléatoires (comme dans One Weekend).

5. Bonus / Optimisations

[ ] Sauvegarde Image : Ajouter une fonction pour exporter le rendu en .ppm ou .png depuis l'interface SDL.

[ ] Multithreading : Utiliser OpenMP (#pragma omp parallel for) dans la boucle de rendu pour accélérer le calcul sur CPU.