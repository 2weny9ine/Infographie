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
    image_diffuse.load("texture/worn-shiny-metal-albedo.png");
    image_metallic.load("texture/worn-shiny-metal-Metallic.png");
    image_roughness.load("texture/worn-shiny-metal-Roughness.png");
    image_ao.load("texture/worn-shiny-metal-ao.png");

    image_diffuse.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    image_metallic.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    image_roughness.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
    image_ao.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);

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

void IlluminationModerne::appliquerUniformsMateriau(const ofMaterial& m) {
    ofFloatColor d = m.getDiffuseColor();
    ofFloatColor s = m.getSpecularColor();
    float metallicity = m.getMetallic();
    float roughness = m.getRoughness();
    ofColor cambiant = ofColor(63, 63, 63);
    ofColor cdiffuse = ofColor(255, 255, 255);
    ofColor cspecular = ofColor(255, 255, 255);

    if (activeLightAmbient)
        shaderPBR.setUniform3f("material_color_ambient", cdiffuse.r/255.0f * 0.2, cdiffuse.g / 255.0f * 0.2, cdiffuse.b / 255.0f * 0.2);
    else
        shaderPBR.setUniform3f("material_color_ambient", 0,0,0);

    shaderPBR.setUniform3f("material_color_diffuse", cdiffuse.r / 255.0f, cdiffuse.g / 255.0f, cdiffuse.b / 255.0f);
    shaderPBR.setUniform3f("material_color_specular", cdiffuse.r/255.0f, cdiffuse.g / 255.0f, cdiffuse.b / 255.0f);
    shaderPBR.setUniform1f("material_brightness", 1.0f);

    shaderPBR.setUniform1f("material_metallic", metallicity);
    shaderPBR.setUniform1f("material_roughness", roughness);
    shaderPBR.setUniform1f("material_occlusion", 1.0f);
    shaderPBR.setUniform3f("material_fresnel_ior", 0.04f, 0.04f, 0.04f);

    shaderPBR.setUniform1f("tone_mapping_toggle", true);
    shaderPBR.setUniform1f("tone_mapping_exposure", 1.0f);
    shaderPBR.setUniform1f("tone_mapping_gamma", 2.2f);

    shaderPBR.setUniformTexture("texture_diffuse", image_diffuse.getTexture(), 1);
    shaderPBR.setUniformTexture("texture_metallic", image_metallic.getTexture(), 2);
    shaderPBR.setUniformTexture("texture_roughness", image_roughness.getTexture(), 3);
    shaderPBR.setUniformTexture("texture_occlusion", image_ao.getTexture(), 4);

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

    std::vector<glm::vec3> activeLightPositions;
    std::vector<glm::vec3> activeLightColors;
    std::vector<float> activeLightIntensities;
    activeLightIntensities.push_back(100.0f);
    activeLightIntensities.push_back(100.0f);
    activeLightIntensities.push_back(100.0f);
    activeLightIntensities.push_back(100.0f);
    activeLightIntensities.push_back(100.0f);
    shaderPBR.setUniform1fv("light_intensities", &activeLightIntensities[0], 5);

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


    int count = (int)activeLightPositions.size();
    /*shaderPBR.setUniform1i("num_active_lights", count);
    if (count > 0) {
        shaderPBR.setUniform3fv("light_positions",
            &activeLightPositions[0].x,
            count);
        shaderPBR.setUniform3fv("light_colors",
            &activeLightColors[0].x,
            count);
    }*/




    //applyMaterials();
    glm::vec3 lightPos = activeMouseLight
        ? lightMouse.getGlobalPosition()
        : lightDirectional.getGlobalPosition();

    shaderPBR.setUniform3f("light_position", lightPos);
    shaderPBR.setUniform3f("light_color", ofColor(255).r / 255.0f, ofColor(255).g / 255.0f, ofColor(255).b / 255.0f);
    shaderPBR.setUniform1f("light_intensity", 1.0f);



    for (auto* object : scene->objects)
    {
        object->draw();
    }

    shaderPBR.end();
    afficherSymboleLumieres();
    ofDisableLighting();
    ofDisableDepthTest();
}



void IlluminationModerne::applyMaterials()
{
    int materialIndex = scene->gui->top_left->getCurrentMaterialIndex();
    shaderPBR.begin();
    ofPushMatrix();

    for (auto* obj : scene->objects) {
        if (!obj->getSelected()) continue;
        if (materialIndex == 3)
        {
            ofMaterial matToApply;
            matToApply.setDiffuseColor(obj->getColor());
            matToApply.setMetallic(metallicityAmount);
            matToApply.setRoughness(roughnessAmount);
            appliquerUniformsMateriau(matToApply);
        }

        obj->draw();
    }
    ofPopMatrix();
    shaderPBR.end();
}

void IlluminationModerne::renderMaterialPass()
{
    if (scene->selectedObjects.empty()) return;

    ofEnableDepthTest();
    //lightDirectional.enable();

    glDepthFunc(GL_EQUAL);

    applyMaterials();


    glDepthFunc(GL_LESS);
    //lightDirectional.disable();
    ofDisableDepthTest();
}



void IlluminationModerne::update(float dt)
{
    if (!scene || scene->selectedObjects.empty()) return;

    Object3D* selected = scene->selectedObjects[0];

    glm::vec3 scale = selected->getScale();
    if (scale.x < 1.0f) return;
    
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
