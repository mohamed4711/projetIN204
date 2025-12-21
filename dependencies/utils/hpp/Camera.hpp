#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "dependencies/utils/hpp/Vector3.hpp"



class Camera{

  //default constuctor 
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
    //functions de get parameters 
    Vector3 GetPosition( ) const;
    Vector3 GetLookAt() const; 
    Vector3 GetUpVector() const;
    double GetLength() const; 
    double GetHorizSize() const ; 
    double GetAspect() const ; 
    Vector3 GetU() const ; 
    Vector3 GetV() const ; 
}
;


#endif