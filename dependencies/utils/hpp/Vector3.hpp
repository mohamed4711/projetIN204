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

    Vector3 operator-() const { return Vector3(-x, -y, -z); }
    double operator[](int i) const { return i == 0 ? x : (i == 1 ? y : z); }
    double& operator[](int i) { return i == 0 ? x : (i == 1 ? y : z); }

    // --- Opérations Vectorielles de base ---
    
    // Calcule la longueur (magnitude) du vecteur
    double length() const;
    
    // Calcule la longueur au carré 
    double lengthSquared() const;

    // Rend le vecteur unitaire 
    Vector3& normalize();

    // Produit Scalaire (Dot Product)
   
    double dot(const Vector3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }
    // Produit Vectoriel (Cross Product)
    // Essentiel pour calculer les normales d'un plan ou d'un triangle
    Vector3 cross(const Vector3& v) const;

    // --- Surcharges d'opérateurs pour l'arithmétique ---
    
    Vector3 operator+(const Vector3& v) const;
    Vector3 operator-(const Vector3& v) const; // Implemented in Vector3.cpp
    Vector3 operator*(const Vector3& v) const; // Multiplication composante par composante (pour les couleurs)
    Vector3 operator*(double scalar) const;    // Multiplication par un scalaire
    Vector3 operator/(double scalar) const;    // Division par un scalaire

    Vector3& operator+=(const Vector3& v);
    Vector3& operator-=(const Vector3& v);
    Vector3& operator*=(double scalar);

    Vector3& operator=(const Vector3& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            z = other.z;
        }
        return *this;
    }

    // Accès par index (implemented inline above)
};

// Fonction pour normaliser un vecteur (fonction libre)

inline Vector3 unit_vector(const Vector3& v)
{
    return v / v.length();
}

// Produit scalaire en fonction libre
inline double dot(const Vector3& u, const Vector3& v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}
// Permet d'écrire "double * Vector3"
Vector3 operator*(double scalar, const Vector3& v);

// Permet d'afficher le vecteur avec std::cout << v
std::ostream& operator<<(std::ostream& os, const Vector3& v);

inline Vector3 reflect(const Vector3& v, const Vector3& n) {
    return v - 2 * dot(v, n) * n;
}

inline Vector3 refract(const Vector3& uv, const Vector3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    Vector3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    Vector3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.lengthSquared())) * n;
    return r_out_perp + r_out_parallel;
}

// Schlick's approximation for reflectance (Glass reflectivity varies by angle)
inline double reflectance(double cosine, double ref_idx) {
    auto r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}

#endif // VECTOR3_HPP