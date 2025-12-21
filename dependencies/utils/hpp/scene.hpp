#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include<SDL2/SDL.h>
#include "Image.hpp"

class Scene {
    public : 
    //default constuctor
    Scene() ;

    //to render the image 
    bool Render(Image &image );

    //privte functions 
    private:  

    //privte members 
    private: 
}
;

#endif 