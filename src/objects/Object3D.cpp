#include "Object3D.h"

Object3D::Object3D()
    : position(0), rotation(0), scale(1), color(ofColor::grey), opacity(1.0f), selected(false), surfaceType(SurfaceType::NONE)
{
    initializeDrawingTools();
}

Object3D::Object3D(const Object3D& instance)
    : position(instance.position), rotation(instance.rotation), scale(instance.scale),
    color(instance.color), opacity(instance.opacity), selected(instance.selected),
    strokeColor(instance.strokeColor), fillColor(instance.fillColor), lineWidth(instance.lineWidth)
{
    initializeDrawingTools();
}

Object3D::~Object3D() {}

void Object3D::setup() {}
void Object3D::update(float) {}
void Object3D::draw() {}
void Object3D::drawBoundingBox() {}

Object3D* Object3D::copy() const { return new Object3D(*this); }
ofRectangle Object3D::getScreenBoundingBox(ofCamera*) { return ofRectangle(); }

void Object3D::initializeDrawingTools()
{
    strokeColor = ofColor(255);
    fillColor = ofColor(127, 127, 127, 127);
    lineWidth = 2.0f;
}

void Object3D::getWorldBounds(glm::vec3& min, glm::vec3& max) const
{
    min = position - scale * 0.5f;
    max = position + scale * 0.5f;
}

// Getters & Setters
ofVec3f Object3D::getPosition() const { return position; }
ofVec3f Object3D::getRotation() const { return rotation; }
ofVec3f Object3D::getScale() const { return scale; }
ofColor Object3D::getColor() const { return color; }
float Object3D::getOpacity() const { return opacity; }
bool Object3D::getSelected() const { return selected; }

void Object3D::setPosition(const ofVec3f& pos) { position = pos; }
void Object3D::setRotation(const ofVec3f& rot) { rotation = rot; }
void Object3D::setScale(const ofVec3f& scl) { scale = scl; }
void Object3D::setColor(const ofColor& col) { color = col; }
void Object3D::setOpacity(float op) { opacity = op; }
void Object3D::setSelected(bool sel) { selected = sel; }

void Object3D::transformPosition(const ofVec3f& delta) { position += delta; }
void Object3D::transformRotation(const ofVec3f& delta) { rotation += delta; }
void Object3D::transformScale(const ofVec3f& delta) { scale += delta; }

std::vector<Property> Object3D::getProperties() const
{
    return {
        {"position.x", PropertyType::Float, position.x, -1000, 1000, 0.1f, 2},
        {"position.y", PropertyType::Float, position.y, -1000, 1000, 0.1f, 2},
        {"position.z", PropertyType::Float, position.z, -1000, 1000, 0.1f, 2},
        {"rotation.x", PropertyType::Float, rotation.x, -360, 360, 1.0f, 1},
        {"rotation.y", PropertyType::Float, rotation.y, -360, 360, 1.0f, 1},
        {"rotation.z", PropertyType::Float, rotation.z, -360, 360, 1.0f, 1},
        {"scale.x", PropertyType::Float, scale.x, 0.1f, 1000, 0.1f, 2},
        {"scale.y", PropertyType::Float, scale.y, 0.1f, 1000, 0.1f, 2},
        {"scale.z", PropertyType::Float, scale.z, 0.1f, 1000, 0.1f, 2},
        {"color", PropertyType::Color, color},
        {"opacity", PropertyType::Float, opacity, 0.0f, 1.0f, 0.01f, 2},
    };
}

void Object3D::setProperty(const Property& prop)
{
    if (prop.name == "position.x") position.x = std::get<float>(prop.value);
    else if (prop.name == "position.y") position.y = std::get<float>(prop.value);
    else if (prop.name == "position.z") position.z = std::get<float>(prop.value);
    else if (prop.name == "rotation.x") rotation.x = std::get<float>(prop.value);
    else if (prop.name == "rotation.y") rotation.y = std::get<float>(prop.value);
    else if (prop.name == "rotation.z") rotation.z = std::get<float>(prop.value);
    else if (prop.name == "scale.x") scale.x = std::get<float>(prop.value);
    else if (prop.name == "scale.y") scale.y = std::get<float>(prop.value);
    else if (prop.name == "scale.z") scale.z = std::get<float>(prop.value);
    else if (prop.name == "color") color = std::get<ofColor>(prop.value);
    else if (prop.name == "opacity") opacity = std::get<float>(prop.value);
   
    else if (prop.name == "surface type")
    {
        if (auto valuePtr = std::get_if<int>(&prop.value))
            surfaceType = static_cast<SurfaceType>(*valuePtr);
    };
}
