#ifndef IMAGE_H
#define IMAGE_H



#include <string>
#include <vector>
#include <SDL2/SDL.h>

class Image
{
    public:
        // Constructor.
        Image();
        
        // Destructor.
        ~Image();
        
        // Function to initialize.
        void Initialize(const int xSize, const int ySize, SDL_Renderer *pRenderer);
    
        // Function to set pixels.
        void SetPixel(const int x, const int y, const double red, const double green, const double blue);
        void SavePPM(const std::string& filename);
        // Function to return the image for display.
        void Display();
        int GetXsize(); 
        int GetYsize();
        
    private:
        Uint32 ConvertColor(const double red, const double green, const double blue);
        void InitTexture();
        
    private:
        // Arrays to store image data.
        std::vector<std::vector<double>> m_rChannel;
        std::vector<std::vector<double>> m_gChannel;
        std::vector<std::vector<double>> m_bChannel;
        
        // And store the size of the image.
        int m_xSize, m_ySize;
        
        
        // SDL2 stuff.
        SDL_Renderer *m_pRenderer;
        SDL_Texture *m_pTexture;

};

#endif