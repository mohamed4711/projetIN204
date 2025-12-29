#include "../hpp/Light.hpp"
#include <math.h>

Light::Light() {
    L_Color = Vector3(255.0, 255.0, 255.0);
    L_intensity = 1.0;
    
   
    L_Position = Vector3(-5.0, -10.0, -55.0); 
}; 

bool Light::computeIllumination(hit_record &hitPoint,
                                hittable_list Objects,
                                const std::shared_ptr<hittable>& CurrentObject)
{
    
     Vector3 LightDirection = L_Position - hitPoint.p;

  
     LightDirection.normalize(); 

  
     double CosAngle = hitPoint.normal.dot(LightDirection); 

     
     // 
     double angle = acos(CosAngle);

     // PI/2
     if (angle > 1.5708) { 
        
        hitPoint.LocalColor = Vector3(0,0,0); 
        hitPoint.ColorIntensity = 0.0; 
        return false; 

     } else {
        // Illumination
        hitPoint.LocalColor = L_Color;
        
        
        hitPoint.ColorIntensity = L_intensity * (1.0 - angle / 1.5708);
        
        return true; 
     }
};

Light::~Light() {}