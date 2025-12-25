#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <iostream>
#include <cmath>

class Vector3 {
public:
    // Coordonnées publiques pour un accès rapide (standard en graphisme)
    double x, y, z;

    // --- Constructeurs ---
    Vector3() ;
    Vector3(double x, double y, double z);

    // --- Opérations Vectorielles de base ---
    
    // Calcule la longueur (magnitude) du vecteur
    double length() const;
    
    // Calcule la longueur au carré 
    double lengthSquared() const;

    // Rend le vecteur unitaire 
    Vector3& normalize();

    // Produit Scalaire (Dot Product)
   
    double dot(const Vector3& v) const;

    // Produit Vectoriel (Cross Product)
    // Essentiel pour calculer les normales d'un plan ou d'un triangle
    Vector3 cross(const Vector3& v) const;

    // --- Surcharges d'opérateurs pour l'arithmétique ---
    
    Vector3 operator+(const Vector3& v) const;
    Vector3 operator-(const Vector3& v) const;
    Vector3 operator*(const Vector3& v) const; // Multiplication composante par composante (pour les couleurs)
    Vector3 operator*(double scalar) const;    // Multiplication par un scalaire
    Vector3 operator/(double scalar) const;    // Division par un scalaire

    Vector3& operator+=(const Vector3& v);
    Vector3& operator-=(const Vector3& v);
    Vector3& operator*=(double scalar);

    // Accès par index
    double operator[](int i) const;
    double& operator[](int i);
    Vector3& operator=(const Vector3& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
        }
        return *this;
    }
};

// --- Fonctions utilitaires externes ---

// Permet d'écrire "double * Vector3"
Vector3 operator*(double scalar, const Vector3& v);

// Permet d'afficher le vecteur avec std::cout << v
std::ostream& operator<<(std::ostream& os, const Vector3& v);


#endif // VECTOR3_HPP