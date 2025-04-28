// IlluminationClassique.cpp
#include "IlluminationClassique.h"
#include "Scene.h"
#include "GUI.h"

IlluminationClassique::IlluminationClassique(Scene* sc)
: scene(sc)
{}

void IlluminationClassique::chargerShaders() {
    shaderLambert.load( "shaders/lambert_vs.glsl",     "shaders/lambert_fs.glsl");
    shaderGouraud.load( "shaders/gouraud_vs.glsl",     "shaders/gouraud_fs.glsl");
    shaderPhong  .load( "shaders/phong_vs.glsl",       "shaders/phong_fs.glsl");
    shaderBlinn  .load( "shaders/blinn_phong_vs.glsl", "shaders/blinn_phong_fs.glsl");
}

void IlluminationClassique::setup() {

    chargerShaders();
    

    matDiffuse.setAmbientColor  (ofFloatColor(0.05f));
    matDiffuse.setDiffuseColor  (ofFloatColor(200/255.0f, 30/255.0f, 30/255.0f));
    matDiffuse.setSpecularColor (ofFloatColor(0.0f));
    matDiffuse.setShininess     (  8.0f );
    
    matPlastique.setAmbientColor  (ofFloatColor(0.05f));
    matPlastique.setDiffuseColor  (ofFloatColor(30/255.0f, 200/255.0f, 30/255.0f));
    matPlastique.setSpecularColor (ofFloatColor(150/255.0f));
    matPlastique.setShininess     ( 32.0f );
    
    matMetal.setAmbientColor      (ofFloatColor(0.05f));
    matMetal.setDiffuseColor      (ofFloatColor(30/255.0f, 30/255.0f, 200/255.0f));
    matMetal.setSpecularColor     (ofFloatColor(1.0f));
    matMetal.setShininess         ( 64.0f );
    
    
    lightDirectional.setDirectional();
    lightDirectional.setDiffuseColor (ofColor::white);
    lightDirectional.setSpecularColor(ofColor::white);
    lightDirectional.setOrientation  ({0, -90, 0});
}

ofShader& IlluminationClassique::shaderActuel() {
    switch (modeCourant) {
        case Mode::LAMBERT:
            return shaderLambert;
        case Mode::GOURAUD:
            return shaderGouraud;
        case Mode::PHONG:
            return shaderPhong;
        case Mode::BLINN_PHONG:
            return shaderBlinn;
        default:
            return shaderLambert;
    }
}

void IlluminationClassique::appliquerUniformsMateriau(const ofMaterial& m) {
    auto& sh = shaderActuel();
    ofFloatColor d = m.getDiffuseColor();
    ofFloatColor s = m.getSpecularColor();
    
    sh.setUniform3f("color_ambient",  d.r * 0.2f, d.g * 0.2f, d.b * 0.2f);
    sh.setUniform3f("color_diffuse",  d.r,       d.g,       d.b);
    sh.setUniform3f("color_specular", s.r,       s.g,       s.b);
    sh.setUniform1f("shininess",      m.getShininess());
    
    glm::vec3 Lpos = lightDirectional.getGlobalPosition();
    sh.setUniform3f("light_position", Lpos.x, Lpos.y, Lpos.z);
}

void IlluminationClassique::draw() {
    if (modeCourant == Mode::AUCUN) return;
    
    ofEnableDepthTest();
    lightDirectional.enable();
    
    auto& sh = shaderActuel();
    sh.begin();
    
    
    int idx = scene->gui->top_left->getCurrentMaterialIndex();
    switch (idx) {
        case 1:
            appliquerUniformsMateriau(matPlastique);
            break;
        case 2:
            appliquerUniformsMateriau(matMetal);
            break;
        default: appliquerUniformsMateriau(matDiffuse);
            break;
    }
    

    for (auto* obj : scene->objects) {
        obj->draw();
    }
    
    sh.end();
    lightDirectional.disable();
    ofDisableDepthTest();
}

void IlluminationClassique::applyMaterials() {
    int idx = scene->gui->top_left->getCurrentMaterialIndex();
    for (auto* obj : scene->objects) {
        if (!obj->getSelected()) continue;
        ofPushMatrix();
        ofTranslate(obj->getPosition());
        switch (idx) {
            case 0:
                matDiffuse.begin();
                break;
            case 1:
                matPlastique.begin();
                break;
            case 2:
                matMetal.begin();
                break;
        }
        obj->draw();
        switch (idx) {
            case 0:
                matDiffuse.end();
                break;
            case 1:
                matPlastique.end();
                break;
            case 2:
                matMetal.end();
                break;
        }
        ofPopMatrix();
    }
}


void IlluminationClassique::renderMaterialPass() {
    ofEnableDepthTest();
    lightDirectional.enable();
    glDepthFunc(GL_LEQUAL);
    
    applyMaterials();
    
    glDepthFunc(GL_LESS);
    lightDirectional.disable();
}

void IlluminationClassique::update(float) {
    
}
