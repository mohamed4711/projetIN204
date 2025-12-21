#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "dependencies/utils/hpp/Vector3.hpp"
#include "dependencies/utils/hpp/Ray.hpp"



class Camera{
  public :
    // =========================================================================
    // 1. PARAMÈTRES D'ENTRÉE (Configuration de la caméra)
    // =========================================================================

    // La position absolue de "l'oeil" de la caméra dans le monde 3D.
    // C'est de là que partent tous les rayons primaires.
    Vector3 c_Position; 

    // Le point précis que la caméra regarde. 
    // Sert à calculer la direction "Avant" (Forward).
    Vector3 c_LookAt; 

    // Le vecteur de référence pour le "Haut" (souvent l'axe Z ou Y du monde).
    // Il empêche la caméra de rouler sur elle-même (tête penchée).
    Vector3 c_Up; 

    // La largeur physique de la fenêtre virtuelle (en unités/mètres).
    // Détermine la couverture horizontale de la scène.
    double c_HorizSize;

    // Le ratio (Largeur / Hauteur). 
    // Sert à calculer la hauteur verticale (V) automatiquement pour empêcher 
    // la déformation de l'image sur ton écran.
    double c_Aspect; 

    // La distance focale (Distance entre l'oeil c_Position et l'écran virtuel).
    // Plus c'est grand = Zoom (Téléobjectif). Plus c'est petit = Grand angle.
    double c_Length;  


    // =========================================================================
    // 2. VECTEURS DU REPÈRE (Calculés automatiquement par UpdateGeometry)
    // Ces vecteurs définissent la "vitre" virtuelle pixel par pixel.
    // =========================================================================

    // (Axe Z local) Vecteur de direction normalisé (longueur 1).
    // Il pointe de c_Position vers c_LookAt. Définit la profondeur.
    Vector3 c_Aligmentvector; 

    // (Axe X local) Vecteur horizontal de l'écran.
    // Calculé par produit vectoriel. Sa longueur correspond souvent à la largeur de l'écran.
    // Sert à parcourir les pixels de gauche à droite.
    Vector3 c_ProjectionScreenU; 

    // (Axe Y local) Vecteur vertical de l'écran.
    // Calculé pour être perpendiculaire au regard. Sa longueur est ajustée par l'Aspect Ratio.
    // Sert à parcourir les pixels de haut en bas.
    Vector3 c_ProjectionScreenV; 

    // Le point central exact de l'écran virtuel dans l'espace 3D.
    // C'est le point d'ancrage : c_Position + (Direction * Length).
    Vector3 c_ScreenVectorC;

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


    //function to generate  a ray 
    Ray GenerateRay(float screenX, float ScrennY); 

    //function to update camera geometry 

    void UpdateCameraGeometry( ); 


  
}
;


#endif