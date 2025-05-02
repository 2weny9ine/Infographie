#pragma once
#include "ofMain.h"
#include "GUI.h"

class Scene;
class IlluminationClassique {
public:
    enum class Mode { AUCUN, LAMBERT, GOURAUD, PHONG, BLINN_PHONG,TOON };
    
    explicit IlluminationClassique(Scene* scene);
    ~IlluminationClassique();
    
    void setup();
    void update(float dt);
    void draw();
    void renderMaterialPass();
    
    void setMode(Mode m){
        modeCourant = m; }
    Mode getMode() const{
        return modeCourant; }
    bool materialEffectEnabled = false;
    
    bool activeLightAmbient      = false;
    bool activeLightDirectional  = false;
    bool activeLightPoint        = false;
    bool activeLightSpot         = false;
    bool activeMouseLight        = false;
    ofLight     lightDirectional, lightPoint,lightSpot,lightMouse;
    
    float globalIntensity = 1.0f;
private:
    
    void appliqueCouleur(const ofFloatColor& col);
    void chargerShaders();
    void initialisationEclairage();
    void initialisationMateriaux();
    ofShader& shaderActuel();
    void appliquerUniformsMateriau(const ofMaterial& m);
    
    void applyMaterials();
    void afficherSymboleLumieres();

    void initialiserLumieres();
    void appliquerEtatLumieres();

    
    bool debugLights = true;

    
    Scene*      scene;
    Mode        modeCourant = Mode::AUCUN;
    
    ofShader    shaderLambert, shaderGouraud, shaderPhong, shaderBlinn,shaderToon;

    ofFloatColor globalAmbientColor;

   

    ofMaterial  matDiffuse, matPlastique, matMetal;
};
