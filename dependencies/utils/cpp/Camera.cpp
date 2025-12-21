 #include "dependencies/utils/hpp/Camera.hpp"
 

    Camera::Camera(){
        c_Position=Vector3(0.0,-10.0,0.0); 
        c_LookAt=Vector3(0.0,0.0,0.0);
        c_Up=Vector3( 0.0,0.0,1.0);
        c_Length=1.0; 
        c_Aspect=1.0;
        c_HorizSize=1.0;
       

      
    };

    // Set the position of the camera in world coordinates
    void Camera::SetPosition(const Vector3& position){
        c_Position = position;
    };

    // Define the point the camera is looking at
    void Camera::LookAt(const Vector3& lookAtPoint){
        c_LookAt = lookAtPoint;
    };

    // Define the "up" direction of the camera
    void Camera::SetUpVector(const Vector3& upVector){
        c_Up = upVector;
    };

    // Distance from the camera to the image plane (controls field of view)
    void Camera::SetLength(double length){
        c_Length = length;
    };

    // Physical width of the image plane
    void Camera::SetHorizSize(double horizSize){
        c_HorizSize = horizSize;
    };

    // Aspect ratio = width / height
    void Camera::SetAspect(double aspect){
        c_Aspect = aspect;
    };


    
    //functions to get parameters 
    Vector3 Camera::GetPosition( ) const{
        return c_Position;
    };
    Vector3 Camera::GetLookAt() const{
        return c_LookAt;
    }; 
    Vector3 Camera::GetUpVector() const{
        return c_Up;
    };
    double Camera::GetLength() const{
        return c_Length;
    }; 
    double Camera::GetHorizSize() const{
        return c_HorizSize;
    }; 
    double Camera::GetAspect() const{
        return c_Aspect;
    }; 
    Vector3 Camera::GetU() const{
        return c_PrejectionScreenU;
    }; 
    Vector3 Camera::GetV() const{
        return c_ProjectionScreenV;
    }; 
    Vector3 Camera::GetScreenCenter() const{
        return c_ScreenVectorC;
    };


    //function to generate  a ray 
    Ray Camera::GenerateRay(float screenX, float screenY){
        // Calculate the point on the screen
        Vector3 screenPoint = c_ScreenVectorC + (c_PrejectionScreenU * screenX) + (c_ProjectionScreenV * screenY);
        
        // Direction from camera to screen point
        Vector3 direction = (screenPoint - c_Position).normalize();
        
        // Return the ray
        return Ray(c_Position, direction);
    };

    //function to update camera geometry 
    void Camera::UpdateCameraGeometry( ){
        // Calculate alignment vector (forward)
        c_Aligmentvector = (c_LookAt - c_Position).normalize();

        // Calculate U (right vector)
        c_PrejectionScreenU = c_Aligmentvector.cross(c_Up).normalize();

        // Calculate V (up vector for screen)
        c_ProjectionScreenV = c_PrejectionScreenU.cross(c_Aligmentvector).normalize();

        // Calculate screen center
        c_ScreenVectorC = c_Position + (c_Aligmentvector * c_Length);

        // Adjust U and V lengths based on horizSize and aspect
        double screenHeight = c_HorizSize / c_Aspect;
        c_PrejectionScreenU = c_PrejectionScreenU * (c_HorizSize / 2.0);
        c_ProjectionScreenV = c_ProjectionScreenV * (screenHeight / 2.0);
    }; 
  

    //function to update camera geometry 

 