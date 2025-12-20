#ifndef CAPP_H
#define CAPP_H

#include <SDL2/SDL.h>
#include "../dependencies/utils/hpp/Image.hpp" 
class CApp
{
    public:

        CApp();
        
        int OnExecute();
        bool OnInit();
        void OnEvent(SDL_Event *event);
        void OnLoop();
        void OnRender();
        void OnExit();
        
    private:
        // An instance of Image to store the image.
        Image m_image;
    
        // SDL2 stuff.
        bool isRunning;
        SDL_Window *pWindow;
        SDL_Renderer *pRenderer;

};

#endif