#include "IlluminationModerne.h"
#include "Scene.h"
#include "GUI.h"

IlluminationModerne::IlluminationModerne(Scene* sc)
    : scene(sc)
{
}

void IlluminationModerne::chargerShaders() {
    shaderPBR.load("shaders/pbr_vs.glsl", "shaders/pbr_fs.glsl");
}
void IlluminationModerne::chargerTextures()
{
    ofDisableArbTex();
    diffuse.load("texture/metal_plate_diffuse_1k.jpg");
    metallic.load("texture/metal_plate_metallic_1k.jpg");
    roughness.load("texture/metal_plate_roughness_1k.jpg");
    ao.load("texture/metal_plate_ao_1k.jpg");
    diffuse.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    metallic.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    roughness.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    ao.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);

}
void IlluminationModerne::initialisationEclairage() {
    lightMouse.setPointLight();
    lightMouse.setDiffuseColor(ofColor::white);
    lightMouse.setSpecularColor(ofColor(255));
    lightMouse.setAttenuation(1.0f, 0.002f, 0.0001f);
}

void IlluminationModerne::initialisationMateriaux() {
    matDiffuse.setAmbientColor(ofFloatColor(0.05f));
    matDiffuse.setSpecularColor(ofFloatColor(0.0f));
    matDiffuse.setShininess(8.0f);

    matPlastique.setAmbientColor(ofFloatColor(0.05f));
    matPlastique.setSpecularColor(ofFloatColor(150 / 255.0f));
    matPlastique.setShininess(32.0f);

    matMetal.setAmbientColor(ofFloatColor(0.05f));
    matMetal.setSpecularColor(ofFloatColor(1.0f));
    matMetal.setShininess(64.0f);
}


//critère: 7.3 Types de lumière
void IlluminationModerne::initialiserLumieres()
{
    // Lumière directionnelle
    lightDirectional.setDirectional();
    lightDirectional.setDiffuseColor(ofColor(255));
    lightDirectional.setSpecularColor(ofColor(255));
    lightDirectional.setOrientation({ 0, -90, 0 });


    // Lumière ponctuelle
    lightPoint.setPointLight();
    lightPoint.setDiffuseColor(ofColor(255));
    lightPoint.setSpecularColor(ofColor(255));


    // Projecteur (Spotlight)
    lightSpot.setSpotlight();
    lightSpot.setDiffuseColor(ofColor(255));
    lightSpot.setSpecularColor(ofColor(255));
    lightSpot.setSpotlightCutOff(45);
    lightSpot.setSpotConcentration(128);



    globalAmbientColor = ofFloatColor(0.3f);
}


void IlluminationModerne::appliquerEtatLumieres()
{
    if (activeLightAmbient)
        ofSetGlobalAmbientColor(globalAmbientColor);
    else
        ofSetGlobalAmbientColor(ofFloatColor(0.0f));

    if (activeLightDirectional)
        lightDirectional.enable();
    else
        lightDirectional.enable();

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



void IlluminationModerne::setup() {
    chargerShaders();
    chargerTextures();
    initialisationEclairage();
    initialisationMateriaux();
    initialiserLumieres();
}


void IlluminationModerne::appliqueCouleur(const ofFloatColor& col) {

    if (activeLightAmbient)
        shaderPBR.setUniform3f("material_color_ambient", col.r * 0.2f, col.g * 0.2f, col.b * 0.2f);
    else
        shaderPBR.setUniform3f("material_color_ambient", 0.0f, 0.0f, 0.0f);

    shaderPBR.setUniform3f("material_color_diffuse", col.r, col.g, col.b);
    shaderPBR.setUniform3f("material_color_specular", 0.3f, 0.3f, 0.3f);
    shaderPBR.setUniform1f("material_brightness", 1.0f);

    shaderPBR.setUniform1f("material_metallic", 0.50f);
    shaderPBR.setUniform1f("material_roughness", 0.50f);
    shaderPBR.setUniform1f("material_occlusion", 1.0f);
    shaderPBR.setUniform3f("material_fresnel_ior", 0.3f, 0.3f, 0.3f);

    shaderPBR.setUniform1f("tone_mapping_toggle", false);
    shaderPBR.setUniform1f("tone_mapping_exposure", 1.0f);
    shaderPBR.setUniform1f("tone_mapping_gamma", 2.5f);

    shaderPBR.setUniformTexture("texture_diffuse", diffuse.getTexture(), 1);
    shaderPBR.setUniformTexture("texture_metallic", metallic.getTexture(), 2);
    shaderPBR.setUniformTexture("texture_roughness", roughness.getTexture(), 3);
    shaderPBR.setUniformTexture("texture_occlusion", ao.getTexture(), 4);

    shaderPBR.setUniform3f("light_position", lightDirectional.getGlobalPosition());
    shaderPBR.setUniform3f("light_color", ofColor(255).r/255.0f, ofColor(255).g / 255.0f, ofColor(255).b / 255.0f);
    shaderPBR.setUniform1f("light_intensity", 1.0f);

}

void IlluminationModerne::appliquerUniformsMateriau(const ofMaterial& m) {
    ofFloatColor d = m.getDiffuseColor();
    ofFloatColor s = m.getSpecularColor();
    ofColor cambiant = ofColor(63, 63, 63);
    ofColor cdiffuse = ofColor(255, 255, 255);
    ofColor cspecular = ofColor(255, 255, 255);

    if (activeLightAmbient)
        shaderPBR.setUniform3f("material_color_ambient", cambiant.r/255.0f, cambiant.g / 255.0f, cambiant.b / 255.0f);
    else
        shaderPBR.setUniform3f("material_color_ambient", cambiant.r / 255.0f, cambiant.g / 255.0f, cambiant.b / 255.0f);

    shaderPBR.setUniform3f("material_color_diffuse", cdiffuse.r / 255.0f, cdiffuse.g / 255.0f, cdiffuse.b / 255.0f);
    shaderPBR.setUniform3f("material_color_specular", cspecular.r/255.0f, cspecular.g / 255.0f, cspecular.b / 255.0f);
    shaderPBR.setUniform1f("material_brightness", 1.0f);

    shaderPBR.setUniform1f("material_metallic", 0.5f);
    shaderPBR.setUniform1f("material_roughness", 0.5f);
    shaderPBR.setUniform1f("material_occlusion", 1.0f);
    shaderPBR.setUniform3f("material_fresnel_ior", 0.04f, 0.04f, 0.04f);

    shaderPBR.setUniform1f("tone_mapping_toggle", true);
    shaderPBR.setUniform1f("tone_mapping_exposure", 1.0f);
    shaderPBR.setUniform1f("tone_mapping_gamma", 2.2f);

    shaderPBR.setUniformTexture("texture_diffuse", diffuse.getTexture(), 1);
    shaderPBR.setUniformTexture("texture_metallic", metallic.getTexture(), 2);
    shaderPBR.setUniformTexture("texture_roughness", roughness.getTexture(), 3);
    shaderPBR.setUniformTexture("texture_occlusion", ao.getTexture(), 4);

    shaderPBR.setUniform3f("light_position", ofVec3f(25,25,25));
    shaderPBR.setUniform3f("light_color", ofColor(255).r / 255.0f, ofColor(255).g / 255.0f, ofColor(255).b / 255.0f);
    shaderPBR.setUniform1f("light_intensity", 0.4f);

}

void IlluminationModerne::afficherSymboleLumieres() {
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


void IlluminationModerne::draw()
{
    if (!activated) return;

    update(ofGetLastFrameTime());

    ofEnableDepthTest();
    ofEnableLighting();
    appliquerEtatLumieres();

    shaderPBR.begin();

    applyMaterials();



    std::vector<glm::vec3> activeLightPositions;
    std::vector<glm::vec3> activeLightColors;

    if (activeLightPoint) {
        activeLightPositions.push_back(lightPoint.getGlobalPosition());
        auto c = lightPoint.getDiffuseColor();
        activeLightColors.push_back(glm::vec3(c.r, c.g, c.b));
    }
    if (activeLightSpot) {
        activeLightPositions.push_back(lightSpot.getGlobalPosition());
        auto c = lightSpot.getDiffuseColor();
        activeLightColors.push_back(glm::vec3(c.r, c.g, c.b));
    }
    if (activeLightDirectional) {
        activeLightPositions.push_back(lightDirectional.getGlobalPosition());
        auto c = lightDirectional.getDiffuseColor();
        activeLightColors.push_back(glm::vec3(c.r, c.g, c.b));
    }
    if (activeMouseLight) {
        activeLightPositions.push_back(lightMouse.getGlobalPosition());
        auto c = lightMouse.getDiffuseColor();
        activeLightColors.push_back(glm::vec3(c.r, c.g, c.b));

    }

    int count = std::min((int)activeLightPositions.size(), 4);
    //shader.setUniform1i("num_active_lights", count);
    /*if (count > 0) {
        shader.setUniform3fv("light_positions",
            &activeLightPositions[0].x,
            count);
        shader.setUniform3fv("light_colors",
            &activeLightColors[0].x,
            count);
    }*/


    for (auto* object : scene->objects)
    {
        /*int matIndex = scene->gui->top_left->getCurrentMaterialIndex();
        const ofMaterial* baseMaterial = &matDiffuse;
        if (matIndex == 1) baseMaterial = &matPlastique;
        else if (matIndex == 2) baseMaterial = &matMetal;

        ofMaterial finalMaterial = *baseMaterial;
        if (materialEffectEnabled) {
            finalMaterial.setDiffuseColor(object->getColor());
        }
        else {
            finalMaterial.setDiffuseColor(object->getColor());
            finalMaterial.setSpecularColor(ofFloatColor(0.3f));
            finalMaterial.setShininess(8.0f);
        }*/

        appliquerUniformsMateriau(matDiffuse);
        object->draw();
    }
    //appliquerUniformsMateriau(matDiffuse);


    shaderPBR.end();
    afficherSymboleLumieres();

    ofDisableLighting();
    ofDisableDepthTest();
}



void IlluminationModerne::applyMaterials()
{
    int materialIndex = scene->gui->top_left->getCurrentMaterialIndex();
    const ofMaterial* selectedMaterial = &matDiffuse;

    if (materialIndex == 1) {
        selectedMaterial = &matPlastique;
    }
    else if (materialIndex == 2) {
        selectedMaterial = &matMetal;
    }

    glm::vec3 lightPos = activeMouseLight
        ? lightMouse.getGlobalPosition()
        : lightDirectional.getGlobalPosition();


    shaderPBR.begin();
    ofPushMatrix();
    //shaderPBR.setUniform3f("light_position", lightPos);

    for (auto* obj : scene->objects) {
        if (!obj->getSelected()) continue;

        ofMaterial matToApply = *selectedMaterial;
        matToApply.setDiffuseColor(obj->getColor());

        appliquerUniformsMateriau(matToApply);
        obj->draw();
    }
    ofPopMatrix();
    shaderPBR.end();
}

void IlluminationModerne::renderMaterialPass()
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



void IlluminationModerne::update(float dt)
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

        glm::vec3 n = scene->camera->screenToWorld(mouseScreen + glm::vec3(0, 0, scene->camera->getNearClip()));
        glm::vec3 f = scene->camera->screenToWorld(mouseScreen + glm::vec3(0, 0, scene->camera->getFarClip()));
        glm::vec3 dir = glm::normalize(f - n);

        if (std::abs(dir.y) > 1e-6f) {
            float t = -n.y / dir.y;
            if (t > 0.0f) {
                lightMouse.setGlobalPosition(n + dir * t);
            }
        }
    }
}
