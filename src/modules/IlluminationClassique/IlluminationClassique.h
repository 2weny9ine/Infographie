#pragma once
#include "ofMain.h"
#include "GUI.h"

class Scene;
class IlluminationClassique {
public:
    enum class Mode { AUCUN, LAMBERT, GOURAUD, PHONG, BLINN_PHONG };
    
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
    
private:
    
    void chargerShaders();
    ofShader& shaderActuel();
    void appliquerUniformsMateriau(const ofMaterial& m);
    
    void applyMaterials();
    
    Scene*      scene;
    Mode        modeCourant = Mode::AUCUN;
    
    ofShader    shaderLambert, shaderGouraud, shaderPhong, shaderBlinn;
    ofLight     lightDirectional;
    ofMaterial  matDiffuse, matPlastique, matMetal;
    };
