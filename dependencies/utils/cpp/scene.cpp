#include"scene.hpp"

Scene::Scene() {
    // Default constructor
}

bool Scene::Render(Image & image ){
int m_Xsize = image.GetXsize();
int m_Ysize=image.GetYsize(); 
     //create some colour variations 
     for( int x =0 ; x<m_Xsize; x ++){
        for (int y =0 ; y< m_Ysize ; y++){
            double red =(static_cast<double >(x)/static_cast<double>(m_Xsize))*255.0 ;
            double green=(static_cast<double >(y)/static_cast<double>(m_Ysize))*255.0; 
            image.SetPixel(x,y,red,green,0.0);
        }
     }
     return true;  // Add return statement
}