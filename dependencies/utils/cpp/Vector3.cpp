#include "../hpp/Vector3.hpp"
#include <cmath>

// --- Constructeurs ---
Vector3::Vector3() : x(0), y(0), z(0) {}

Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z) {}



// --- Méthodes ---
double Vector3::length() const {
    return std::sqrt(x*x + y*y + z*z);
}

double Vector3::lengthSquared() const {
    return x*x + y*y + z*z;
}

Vector3& Vector3::normalize() {
    double l = length();
    if (l > 0) {
        double inv = 1.0 / l;
        x *= inv; y *= inv; z *= inv;
    }
    return *this;
}


Vector3 Vector3::cross(const Vector3& v) const {
    return Vector3(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    );
}



// --- Opérateurs ---
Vector3 Vector3::operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
Vector3 Vector3::operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
Vector3 Vector3::operator*(const Vector3& v) const { return Vector3(x * v.x, y * v.y, z * v.z); }
Vector3 Vector3::operator*(double s) const { return Vector3(x * s, y * s, z * s); }
Vector3 Vector3::operator/(double s) const { return Vector3(x / s, y / s, z / s); }

Vector3& Vector3::operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
Vector3& Vector3::operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
Vector3& Vector3::operator*=(double s) { x *= s; y *= s; z *= s; return *this; }




// --- Fonctions externes ---
Vector3 operator*(double s, const Vector3& v) {
    return Vector3(v.x * s, v.y * s, v.z * s);
}
std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}




