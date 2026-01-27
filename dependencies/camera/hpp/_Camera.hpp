#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "utils/hpp/Vector3.hpp"
#include "Ray.hpp"



class Camera{
  public :
    // =========================================================================
    // 1. INPUT PARAMETERS (Camera configuration)
    // =========================================================================

    // Camera position in world space (the "eye")
    // All primary rays originate from here
    Vector3 c_Position; 

    // The point the camera is looking at
    // Used to compute the forward direction
    Vector3 c_LookAt; 

    // Reference "up" vector (usually world Z or Y axis)
    // Prevents camera from rolling
    Vector3 c_Up; 

    // Physical width of the virtual screen (in world units)
    // Controls horizontal coverage of the scene
    double c_HorizSize;

    // Aspect ratio (width / height)
    // Used to compute vertical size and prevent image distortion
    double c_Aspect; 

    // Focal length (distance from eye to virtual screen)
    // Larger = zoom in (telephoto), smaller = wide angle
    double c_Length;  


    // =========================================================================
    // 2. LOCAL COORDINATE VECTORS (Computed by UpdateGeometry)
    // These vectors define the virtual screen pixel by pixel
    // =========================================================================

    // (Local Z axis) Normalized direction vector
    // Points from c_Position to c_LookAt, defines depth
    Vector3 c_Aligmentvector; 

    // (Local X axis) Horizontal screen vector
    // Computed via cross product, length matches screen width
    // Used to traverse pixels left to right
    Vector3 c_ProjectionScreenU; 

    // (Local Y axis) Vertical screen vector
    // Perpendicular to view direction, scaled by aspect ratio
    // Used to traverse pixels top to bottom
    Vector3 c_ProjectionScreenV; 

    // Center point of the virtual screen in 3D space
    // Anchor point: c_Position + (Direction * Length)
    Vector3 c_ScreenVectorC;

  // default constructor 
  public:
    Camera();

    // Set the position of the camera in world coordinates
    void SetPosition(const Vector3& position);

    // Define the point the camera is looking at
    void LookAt(const Vector3& lookAtPoint);

    // Define the "up" direction of the camera
    void SetUpVector(const Vector3& upVector);

    // Distance from the camera to the image plane (controls field of view)
    void SetLength(double length);

    // Physical width of the image plane
    void SetHorizSize(double horizSize);

    // Aspect ratio = width / height
    void SetAspect(double aspect);


    public : 
    //functions to get parameters 
    Vector3 GetPosition( ) const;
    Vector3 GetLookAt() const; 
    Vector3 GetUpVector() const;
    double GetLength() const; 
    double GetHorizSize() const ; 
    double GetAspect() const ; 
    Vector3 GetU() const ; 
    Vector3 GetV() const ; 
    Vector3 GetScreenCenter()const ;


    // generates a ray for given screen coordinates
    Ray GenerateRay(float screenX, float ScrennY); 

    // recomputes U, V vectors and screen center
    void UpdateCameraGeometry( ); 


  
}
;


#endif