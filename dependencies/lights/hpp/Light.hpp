#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "./utils/hpp/Vector3.hpp"
#include "./utils/hpp/Point3.hpp"
#include"./objects/hpp/_Generic.hpp"
#include"./objects/hpp/_Hittable_object_list.hpp"



class Light{
    public: 
    //constructor /destuctor
    Light();
    virtual~Light();

    //function to compute the illumination
    virtual bool computeIllumination ( hit_record &hitPoint,
                                     hittable_list Objects,
                                     const std::shared_ptr<hittable>&CurrentObject); 
    public : 
      //parameters 
      Vector3 L_Color ; 
      Vector3 L_Position; 
      double  L_intensity; 
      
    
};
#endif