#include "camera/hpp/_Camera.hpp"


// Default Constructor
Camera::Camera() {
    c_Position  = Vector3(0.0, -10.0, 0.0);
    c_LookAt    = Vector3(0.0, 0.0, 0.0);
    c_Up        = Vector3(0.0, 0.0, 1.0);
    c_Length    = 1.0;
    c_Aspect    = 1.0;
    c_HorizSize = 1.0;

    // Initialize the derived vectors immediately
    UpdateCameraGeometry();
}

// =============================================================================
// Setters (Must update geometry after every change)
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
// Core Functionality
// =============================================================================

// Compute the derived vectors for the camera (Screen Center, U, V)
void Camera::UpdateCameraGeometry() {
    // 1. Compute the forward alignment vector (from Camera to LookAt)
    c_Aligmentvector = (c_LookAt - c_Position).normalize();

    // 2. Compute the U vector (Right) using Cross Product
    c_ProjectionScreenU = c_Aligmentvector.cross(c_Up).normalize();

    // 3. Compute the V vector (Screen Up) perpendicular to both Alignment and U
    c_ProjectionScreenV = c_ProjectionScreenU.cross(c_Aligmentvector).normalize();

    // 4. Compute the center of the virtual screen
    c_ScreenVectorC = c_Position + (c_Aligmentvector * c_Length);

    // 5. Scale U and V to match the physical screen size and aspect ratio
    // Note: We use the full size here, assuming screen coordinates are centered (e.g., -0.5 to 0.5)
    c_ProjectionScreenU = c_ProjectionScreenU * c_HorizSize;
    
    double screenHeight = c_HorizSize / c_Aspect;
    c_ProjectionScreenV = c_ProjectionScreenV * screenHeight;
}

// Generate a Ray from the camera through a specific screen position (u, v)
Ray Camera::GenerateRay(float screenX, float screenY) {
    // Calculate the 3D point on the virtual screen
    // screenX and screenY should be normalized coordinates relative to the center
    Vector3 screenPoint = c_ScreenVectorC + 
                          (c_ProjectionScreenU * screenX) + 
                          (c_ProjectionScreenV * screenY);

    // Calculate the direction vector from the camera origin to the screen point
    Vector3 direction = (screenPoint - c_Position).normalize();

    return Ray(c_Position, direction);
}