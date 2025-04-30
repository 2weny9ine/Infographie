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

void IlluminationClassique::initialisationEclairage(){
    lightMouse.setPointLight();
    lightMouse.setDiffuseColor(ofColor::white);
    lightMouse.setSpecularColor(ofColor(191));
    lightMouse.setAttenuation(1.0f, 0.002f, 0.0001f);
    
}

void IlluminationClassique::initialisationMateriaux(){
    matDiffuse.setAmbientColor  (ofFloatColor(0.05f));
    matDiffuse.setSpecularColor (ofFloatColor(0.0f));
    matDiffuse.setShininess     (  8.0f );
    
    matPlastique.setAmbientColor  (ofFloatColor(0.05f));
    matPlastique.setSpecularColor (ofFloatColor(150/255.0f));
    matPlastique.setShininess     ( 32.0f );
    
    matMetal.setAmbientColor      (ofFloatColor(0.05f));
    matMetal.setSpecularColor     (ofFloatColor(1.0f));
    matMetal.setShininess         ( 64.0f );
}


//critère: 7.3 Types de lumière
void IlluminationClassique::initialiserLumieres()
{
    // Lumière directionnelle
    lightDirectional.setDirectional();
    lightDirectional.setDiffuseColor(ofColor(255));
    lightDirectional.setSpecularColor(ofColor(255));
    lightDirectional.setOrientation({0, -90, 0});

    
    // Lumière ponctuelle
    lightPoint.setPointLight();
    lightPoint.setDiffuseColor(ofColor(255, 200, 50));
    lightPoint.setSpecularColor(ofColor(255));

    
    // Projecteur (Spotlight)
    lightSpot.setSpotlight();
    lightSpot.setDiffuseColor(ofColor(200, 200, 255));
    lightSpot.setSpecularColor(ofColor(255));
    lightSpot.setSpotlightCutOff(45);
    lightSpot.setSpotConcentration(128);

    
    
    globalAmbientColor = ofFloatColor(0.3f);
}


void IlluminationClassique::appliquerEtatLumieres()
{
    if (activeLightAmbient)
        ofSetGlobalAmbientColor(globalAmbientColor);
    else
        ofSetGlobalAmbientColor(ofFloatColor(0.0f));
    
    if (activeLightDirectional)
        lightDirectional.enable();
    else
        lightDirectional.disable();
    
    if (activeLightPoint)
        lightPoint.enable();
    else
        lightPoint.disable();
    
    if (activeLightSpot)
        lightSpot.enable();
    else
        lightSpot.disable();
    
    if (activeMouseLight)
        lightMouse.enable();
    else
        lightMouse.disable();

}



void IlluminationClassique::setup() {
    chargerShaders();
    initialisationEclairage();
    initialisationMateriaux();
    initialiserLumieres();
    
    
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

void IlluminationClassique::appliqueCouleur(const ofFloatColor& col){
    auto& sh = shaderActuel();
    
    if (activeLightAmbient)
        sh.setUniform3f("color_ambient", col.r * 0.2f, col.g * 0.2f, col.b * 0.2f);
    else
        sh.setUniform3f("color_ambient", 0.0f, 0.0f, 0.0f);
    
    sh.setUniform3f("color_diffuse", col.r, col.g, col.b);
    sh.setUniform3f("color_specular", 0.3f, 0.3f, 0.3f);
    sh.setUniform1f("brightness", 8.0f);
}

void IlluminationClassique::appliquerUniformsMateriau(const ofMaterial& m) {
    auto& sh = shaderActuel();
    ofFloatColor d = m.getDiffuseColor();
    ofFloatColor s = m.getSpecularColor();
    
    if (activeLightAmbient)
        sh.setUniform3f("color_ambient", d.r * 0.2f, d.g * 0.2f, d.b * 0.2f);
    else
        sh.setUniform3f("color_ambient", d.r * 0.1f, d.g * 0.1f, d.b * 0.1f);
    
    
    sh.setUniform3f("color_diffuse", d.r, d.g, d.b);
    sh.setUniform3f("color_specular", s.r, s.g, s.b);
    sh.setUniform1f("brightness", m.getShininess());
}

void IlluminationClassique::afficherSymboleLumieres() {
    if (!debugLights || scene->selectedObjects.empty()) return;
    
    ofPushStyle();
    
    int sourceLightColor = 255;
    ofSetColor(sourceLightColor);
    
    if (activeLightPoint) {
        lightPoint.draw();
    }
    
    if (activeLightSpot) {
        lightSpot.draw();
    }
    
    if (activeLightDirectional) {
        lightDirectional.draw();
    }
    
    if (activeMouseLight) {
        lightMouse.draw();
    }
    
    ofPopStyle();
}


void IlluminationClassique::draw()
{
    if (modeCourant == Mode::AUCUN) return;

    ofEnableDepthTest();
    ofEnableLighting();
    appliquerEtatLumieres();

    ofShader& shader = shaderActuel();
    shader.begin();

    // Déterminer la position de la lumière active
    glm::vec3 lightPosition(0.0f);
    if (activeLightSpot) {
        lightPosition += lightSpot.getGlobalPosition();
    } if (activeLightPoint) {
        lightPosition += lightPoint.getGlobalPosition();
    } if (activeLightDirectional) {
        lightPosition += lightDirectional.getGlobalPosition();
    } if (activeMouseLight) {
        lightPosition += lightMouse.getGlobalPosition();
    }

    shader.setUniform3f("light_position", lightPosition);

    for (auto* object : scene->objects)
    {
        int matIndex = scene->gui->top_left->getCurrentMaterialIndex();
        const ofMaterial* baseMaterial = &matDiffuse;

        if (matIndex == 1) baseMaterial = &matPlastique;
        else if (matIndex == 2) baseMaterial = &matMetal;

        ofMaterial finalMaterial = *baseMaterial;

        if (materialEffectEnabled) {
            finalMaterial.setDiffuseColor(object->getColor());
        } else {
            finalMaterial.setDiffuseColor(object->getColor());
            finalMaterial.setSpecularColor(ofFloatColor(0.3f));
            finalMaterial.setShininess(8.0f);
        }

        appliquerUniformsMateriau(finalMaterial);
        object->draw();
    }

    shader.end();

    afficherSymboleLumieres();

    ofDisableLighting();
    ofDisableDepthTest();
}



void IlluminationClassique::applyMaterials()
{
    int materialIndex = scene->gui->top_left->getCurrentMaterialIndex();
    const ofMaterial* selectedMaterial = &matDiffuse;

    if (materialIndex == 1) {
        selectedMaterial = &matPlastique;
    } else if (materialIndex == 2) {
        selectedMaterial = &matMetal;
    }

    ofShader& shader = shaderActuel();
    glm::vec3 lightPos = useMouseLight
                         ? lightMouse.getGlobalPosition()
                         : lightDirectional.getGlobalPosition();

    
    shader.begin();
    shader.setUniform3f("light_position", lightPos);

    for (auto* obj : scene->objects) {
        if (!obj->getSelected()) continue;

        ofMaterial matToApply = *selectedMaterial;
        matToApply.setDiffuseColor(obj->getColor());

        appliquerUniformsMateriau(matToApply);
        obj->draw();
    }

    shader.end();
}

void IlluminationClassique::renderMaterialPass()
{
    if (scene->selectedObjects.empty()) return;
    
    ofEnableDepthTest();
    lightDirectional.enable();
    
    
    glDepthFunc(GL_EQUAL);
    
    applyMaterials();
    
    glDepthFunc(GL_LESS);
    lightDirectional.disable();
    ofDisableDepthTest();
}



void IlluminationClassique::update(float dt)
{
    if (!scene || scene->selectedObjects.empty()) return;

    Object3D* selected = scene->selectedObjects[0];

    glm::vec3 minBound, maxBound;
    selected->getWorldBounds(minBound, maxBound);

    glm::vec3 size = (maxBound - minBound) * 0.5f;
    glm::vec3 center = selected->getPosition();

    if (activeLightDirectional) {
        glm::vec3 offset(0, size.y * 2.5f, size.z * 2.5f);
        lightDirectional.setPosition(center + offset);
        lightDirectional.lookAt(center);
    }

    if (activeLightPoint) {
        glm::vec3 offset(size.x * 1.5f, 0, 0);
        lightPoint.setPosition(center + offset);
    }

    if (activeLightSpot) {
        glm::vec3 offset(0, size.y * 1.5f, -size.z * 1.5f);
        lightSpot.setPosition(center + offset);
        lightSpot.lookAt(center);
    }

    if (activeMouseLight && scene->camera) {
        glm::vec3 mouseScreen(ofGetMouseX(), ofGetMouseY(), 0);

        glm::vec3 near = scene->camera->screenToWorld(mouseScreen + glm::vec3(0, 0, scene->camera->getNearClip()));
        glm::vec3 far  = scene->camera->screenToWorld(mouseScreen + glm::vec3(0, 0, scene->camera->getFarClip()));
        glm::vec3 dir  = glm::normalize(far - near);

        if (std::abs(dir.y) > 1e-6f) {
            float t = -near.y / dir.y;
            if (t > 0.0f) {
                lightMouse.setGlobalPosition(near + dir * t);
            }
        }
    }
}



