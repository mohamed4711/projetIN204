/*
    _Camera.cpp
    Camera class implementation for ray tracing
    Handles position, orientation and ray generation
*/

#include "camera/hpp/_Camera.hpp"


// Default constructor - sets up camera with basic values
Camera::Camera() {
    c_Position  = Vector3(0.0, -10.0, 0.0);  // camera placed back on Y axis
    c_LookAt    = Vector3(0.0, 0.0, 0.0);    // looking at origin
    c_Up        = Vector3(0.0, 0.0, 1.0);    // Z is up
    c_Length    = 1.0;
    c_Aspect    = 1.0;
    c_HorizSize = 1.0;

    UpdateCameraGeometry();
}

// =============================================================================
// Setters - each modification triggers screen geometry recalculation
// =============================================================================

void Camera::SetPosition(const Vector3& position) {
    c_Position = position;
    UpdateCameraGeometry();
}

void Camera::LookAt(const Vector3& lookAtPoint) {
    c_LookAt = lookAtPoint;
    UpdateCameraGeometry();
}

void Camera::SetUpVector(const Vector3& upVector) {
    c_Up = upVector;
    UpdateCameraGeometry();
}

void Camera::SetLength(double length) {
    c_Length = length;
    UpdateCameraGeometry();
}

void Camera::SetHorizSize(double horizSize) {
    c_HorizSize = horizSize;
    UpdateCameraGeometry();
}

void Camera::SetAspect(double aspect) {
    c_Aspect = aspect;
    UpdateCameraGeometry();
}

// =============================================================================
// Getters
// =============================================================================

Vector3 Camera::GetPosition() const     { return c_Position; }
Vector3 Camera::GetLookAt() const       { return c_LookAt; }
Vector3 Camera::GetUpVector() const     { return c_Up; }
double  Camera::GetLength() const       { return c_Length; }
double  Camera::GetHorizSize() const    { return c_HorizSize; }
double  Camera::GetAspect() const       { return c_Aspect; }
Vector3 Camera::GetU() const            { return c_ProjectionScreenU; }
Vector3 Camera::GetV() const            { return c_ProjectionScreenV; }
Vector3 Camera::GetScreenCenter() const { return c_ScreenVectorC; }

// =============================================================================
// Core functions
// =============================================================================

// Updates U, V vectors and virtual screen center
// Builds the camera's local coordinate system
void Camera::UpdateCameraGeometry() {
    // direction vector: from camera to target point
    c_Aligmentvector = (c_LookAt - c_Position).normalize();

    // U = horizontal vector (right) via cross product
    c_ProjectionScreenU = c_Aligmentvector.cross(c_Up).normalize();

    // V = screen vertical vector, perpendicular to U and direction
    c_ProjectionScreenV = c_ProjectionScreenU.cross(c_Aligmentvector).normalize();

    // virtual screen center = position + direction * focal distance
    c_ScreenVectorC = c_Position + (c_Aligmentvector * c_Length);

    // scale U and V according to screen size
    c_ProjectionScreenU = c_ProjectionScreenU * c_HorizSize;
    
    double screenHeight = c_HorizSize / c_Aspect;
    c_ProjectionScreenV = c_ProjectionScreenV * screenHeight;
}

// Generates a ray through pixel (screenX, screenY)
// screenX and screenY are normalized coordinates [-0.5, 0.5]
Ray Camera::GenerateRay(float screenX, float screenY) {
    // point on virtual screen corresponding to this pixel
    Vector3 screenPoint = c_ScreenVectorC + 
                          (c_ProjectionScreenU * screenX) + 
                          (c_ProjectionScreenV * screenY);

    // ray direction: from camera towards this point
    Vector3 direction = (screenPoint - c_Position).normalize();

    return Ray(c_Position, direction);
}