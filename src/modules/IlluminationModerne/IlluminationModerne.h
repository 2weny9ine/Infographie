#pragma once
#include "ofMain.h"
#include "GUI.h"

class Scene;
class IlluminationModerne
{
public:
    explicit IlluminationModerne(Scene* scene);
    ~IlluminationModerne();

    void setup();
    void update(float dt);
    void draw();
    void renderMaterialPass();

    bool materialEffectEnabled = false;
    bool activated = false;

    bool activeLightAmbient = false;
    bool activeLightDirectional = false;
    bool activeLightPoint = false;
    bool activeLightSpot = false;
    bool activeMouseLight = false;
    float metallicityAmount = 0;
    float roughnessAmount = 0;
    ofLight     lightDirectional, lightPoint, lightSpot, lightMouse;

    float globalIntensity = 1.0f;
private:

    void appliquerCouleur(const ofFloatColor& col);
    void chargerShaders();
    void chargerTextures();
    void initialisationEclairage();
    void initialisationMateriaux();
    void appliquerUniformsMateriau(const ofMaterial& m);

    void applyMaterials();
    void afficherSymboleLumieres();

    void initialiserLumieres();
    void appliquerEtatLumieres();


    bool debugLights = true;


    Scene* scene;

    ofShader    shaderPBR;

    ofFloatColor globalAmbientColor;

    ofImage image_ao, image_diffuse, image_metallic, image_roughness;

    ofMaterial  matDiffuse, matPlastique, matMetal;
};

