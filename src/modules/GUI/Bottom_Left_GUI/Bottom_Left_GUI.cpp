#include "Bottom_Left_GUI.h"
#include "modules/GUI/GUI.h"
#include "objects/Object3D.h"
#include "modules/Properties/ModularProperties.h"
#include "enums/SurfaceType.h"
#include <iostream>
#include <sstream>

Bottom_Left_GUI::Bottom_Left_GUI()
    : gui_manager(nullptr), gui(nullptr),
    shape_dropdown(nullptr), create_shape_btn(nullptr)
{
    // Create the main GUI anchored at bottom-left.
    gui = new ofxDatGui(ofxDatGuiAnchor::BOTTOM_LEFT);
    gui->addHeader(":: Create Menu ::");
    gui->addFooter();
    gui->getFooter()->setLabelWhenExpanded("CLOSE CREATE MENU");
    gui->getFooter()->setLabelWhenCollapsed("EXPAND CREATE MENU");
    gui->getHeader()->setDraggable(false);

    shape_options = { "CUBE", "SPHERE", "CYLINDER", "CONE" };
    shape_dropdown = gui->addDropdown("Shape", shape_options);
    shape_dropdown->setStripeColor(ofColor::darkRed);
    for (int i = 0; i < shape_dropdown->size(); i++)
    {
        shape_dropdown->getChildAt(i)->setStripeColor(ofColor::red);
    }

    shape_color_picker = gui->addColorPicker("COLOR", ofColor::grey);
    shape_color_picker->setStripeColor(ofColor::darkRed);

    shape_scale_slider = gui->addSlider("SCALE", 1.0f, 100.0f);
    shape_scale_slider->setValue(50.0f);
    shape_scale_slider->setPrecision(0);
    shape_scale_slider->setStripeColor(ofColor::darkRed);

    shape_opacity_slider = gui->addSlider("OPACITY", 0.0f, 1.0f);
    shape_opacity_slider->setValue(1.0f);
    shape_opacity_slider->setStripeColor(ofColor::darkRed);

    create_shape_btn = gui->addButton("Add to workspace");
    create_shape_btn->setStripeVisible(false);
    create_shape_btn->setBackgroundColor(ofColor::darkRed);
    create_shape_btn->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    create_shape_btn->onButtonEvent(this, &Bottom_Left_GUI::createShape);

    shape_color_picker->onColorPickerEvent([this](ofxDatGuiColorPickerEvent e)
    {
        for (Object3D* object : gui_manager->getScene()->selectedObjects)
        {
            object->setColor(shape_color_picker->getColor());
        }
    });

    shape_scale_slider->onSliderEvent([this](ofxDatGuiSliderEvent e)
    {
        for (Object3D* object : gui_manager->getScene()->selectedObjects)
        {
            float val = shape_scale_slider->getValue();
            object->setScale(ofVec3f(val, val, val));
        }
    });

    shape_opacity_slider->onSliderEvent([this](ofxDatGuiSliderEvent e)
    {
        for (Object3D* object : gui_manager->getScene()->selectedObjects)
        {
            object->setOpacity(shape_opacity_slider->getValue());
        }
    });

    // Create a folder for dynamic property controls.
    attributes_folder = gui->addFolder("Attributes", ofColor::white);

    // Transform & duplication buttons.
    translateButton = attributes_folder->addButton("Translate [T]");
    rotateButton = attributes_folder->addButton("Rotate [R]");
    scaleButton = attributes_folder->addButton("Resize [P]");
    duplicateInstanceButton = attributes_folder->addButton("Duplicate Instance [CTRL + D]");

    translateButton->onButtonEvent([this](ofxDatGuiButtonEvent e)
    {
        if (gui_manager->getScene()->currentTransform != Scene::TransformMode::Translate)
            gui_manager->getScene()->currentTransform = Scene::TransformMode::Translate;
        else
            gui_manager->getScene()->currentTransform = Scene::TransformMode::None;
    });
    rotateButton->onButtonEvent([this](ofxDatGuiButtonEvent e)
    {
        if (gui_manager->getScene()->currentTransform != Scene::TransformMode::Rotate)
            gui_manager->getScene()->currentTransform = Scene::TransformMode::Rotate;
        else
            gui_manager->getScene()->currentTransform = Scene::TransformMode::None;
    });
    scaleButton->onButtonEvent([this](ofxDatGuiButtonEvent e)
    {
        if (gui_manager->getScene()->currentTransform != Scene::TransformMode::Resize)
            gui_manager->getScene()->currentTransform = Scene::TransformMode::Resize;
        else
            gui_manager->getScene()->currentTransform = Scene::TransformMode::None;
    });
    duplicateInstanceButton->onButtonEvent([this](ofxDatGuiButtonEvent e)
    {
        gui_manager->getScene()->duplicateSelectedInstances();
    });
}

void Bottom_Left_GUI::setup(GUI* gui_manager)
{
    this->gui_manager = gui_manager;
}

void Bottom_Left_GUI::createShape(ofxDatGuiButtonEvent e)
{
    std::string selectedShape = shape_dropdown->getSelected()->getName();
    Object3D* modelObj = nullptr;

    if (selectedShape == "CUBE")
        modelObj = new Cube();
    else if (selectedShape == "SPHERE")
        modelObj = new Sphere();
    else if (selectedShape == "CYLINDER")
        modelObj = new Cylinder();
    else if (selectedShape == "CONE")
        modelObj = new Cone();

    if (modelObj)
    {
        ofColor model_color = shape_color_picker->getColor();
        modelObj->setColor(model_color);
        float model_size = shape_scale_slider->getValue();
        modelObj->setScale(ofVec3f(model_size, model_size, model_size));
        float model_opacity = shape_opacity_slider->getValue();
        modelObj->setOpacity(model_opacity);
        gui_manager->getScene()->addObject(modelObj);
    }
}

void Bottom_Left_GUI::resetTransformations()
{
    // Clear dynamic controls by clearing the folder.
    if (attributes_folder)
        attributes_folder->children.clear();
}

ofxDatGui* Bottom_Left_GUI::getGui()
{
    return gui;
}

/**
 * updatePropertyControls() dynamically creates property controls based on the
 * properties of the selected object.
 * If exactly one object is selected, it calls getProperties() and creates appropriate
 * controls (sliders, toggles, text inputs, or color pickers) in the attributes folder.
 */
void Bottom_Left_GUI::updatePropertyControls()
{
    if (attributes_folder)
        attributes_folder->children.clear();

    auto& selectedObjects = gui_manager->getScene()->selectedObjects;
    if (selectedObjects.empty())
        return;

    std::vector<Property> commonProps;
    bool firstObject = true;

    for (auto* obj : selectedObjects)
    {
        auto props = obj->getProperties();
        if (firstObject)
        {
            commonProps = props;
            firstObject = false;
        }
        else
        {
            std::vector<Property> filtered;
            for (const auto& cp : commonProps)
            {
                auto it = std::find_if(props.begin(), props.end(), [&](const Property& p)
                {
                    return p.name == cp.name;
                });
                if (it != props.end())
                {
                    filtered.push_back(cp);
                }
            }
            commonProps = std::move(filtered);
        }
    }

    for (const auto& prop : commonProps)
    {
        const std::string& propName = prop.name;
        switch (prop.type)
        {
            case PropertyType::Float:
            case PropertyType::Int:
            case PropertyType::Percent:
            {
                float val = 0.0f;
                if (std::holds_alternative<float>(prop.value))
                    val = std::get<float>(prop.value);
                else if (std::holds_alternative<int>(prop.value))
                    val = static_cast<float>(std::get<int>(prop.value));

                auto slider = attributes_folder->addSlider(prop.name, prop.min, prop.max);
                slider->setValue(val);
                slider->setPrecision(prop.decimals);
                slider->onSliderEvent([this, propName, prop](ofxDatGuiSliderEvent e) mutable
                {
                    Property updatedProp = prop;
                    updatedProp.value = (prop.type == PropertyType::Int)
                        ? static_cast<int>(e.value)
                        : e.value;

                    for (auto* obj : gui_manager->getScene()->selectedObjects)
                    {
                        auto objProps = obj->getProperties();
                        if (std::any_of(objProps.begin(), objProps.end(),
                                        [&propName](const Property& p) { return p.name == propName; }))
                        {
                            obj->setProperty(updatedProp);
                        }
                    }
                    gui_manager->getScene()->update_Attributes();
                });
                break;
            }

            case PropertyType::Bool:
            {
                bool checked = std::get<bool>(prop.value);
                auto toggle = attributes_folder->addToggle(prop.name, checked);
                toggle->onToggleEvent([this, propName, prop](ofxDatGuiToggleEvent e) mutable
                {
                    Property updatedProp = prop;
                    updatedProp.value = e.checked;

                    for (auto* obj : gui_manager->getScene()->selectedObjects)
                    {
                        auto objProps = obj->getProperties();
                        if (std::any_of(objProps.begin(), objProps.end(),
                                        [&propName](const Property& p) { return p.name == propName; }))
                        {
                            obj->setProperty(updatedProp);
                        }
                    }
                });
                break;
            }

            case PropertyType::String:
            {
                /*std::string strVal = std::get<std::string>(prop.value);
                auto textInput = attributes_folder->addTextInput(prop.name, strVal);
                textInput->onTextInputEvent([this, propName, prop](ofxDatGuiTextInputEvent e) mutable
                {
                    Property updatedProp = prop;
                    updatedProp.value = e.text;

                    for (auto* obj : gui_manager->getScene()->selectedObjects)
                    {
                        auto objProps = obj->getProperties();
                        if (std::any_of(objProps.begin(), objProps.end(),
                                        [&propName](const Property& p) { return p.name == propName; }))
                        {
                            obj->setProperty(updatedProp);
                        }
                    }
                });*/
                if (prop.name == "Filter") {
                    std::string currentVal = std::get<std::string>(prop.value);
                    std::vector<std::string> filters = { "None", "Emboss", "Sharpen", "EdgeDetect" };

                    attributes_folder->addLabel("Texture Filter");

                    for (const auto& filterName : filters) {
                        bool isSelected = (filterName == currentVal);
                        auto* toggle = attributes_folder->addToggle(filterName, isSelected);

                        toggle->onToggleEvent([this, filterName, propName, prop](ofxDatGuiToggleEvent e) mutable {
                            if (!e.checked) return; 

                            Property updatedProp = prop;
                            updatedProp.value = filterName;

                            for (auto* obj : gui_manager->getScene()->selectedObjects) {
                                auto objProps = obj->getProperties();
                                if (std::any_of(objProps.begin(), objProps.end(),
                                    [&propName](const Property& p) { return p.name == propName; })) {
                                    obj->setProperty(updatedProp);
                                }
                            }

    
                            updatePropertyControls();
                            });
                    }
                }
                else if (prop.name == "TextureType") {
                    std::string currentVal = std::get<std::string>(prop.value);
                    std::vector<std::string> textures = { "wood", "tree", "brick", "checkerboard" };

                    attributes_folder->addLabel("Texture Type");

                    for (const auto& texName : textures) {
                        bool isSelected = (texName == currentVal);
                        auto* toggle = attributes_folder->addToggle(texName, isSelected);

                        toggle->onToggleEvent([this, texName, propName, prop](ofxDatGuiToggleEvent e) mutable {
                            if (!e.checked) return; 

                            Property updatedProp = prop;
                            updatedProp.value = texName;

                            for (auto* obj : gui_manager->getScene()->selectedObjects) {
                                auto objProps = obj->getProperties();
                                if (std::any_of(objProps.begin(), objProps.end(),
                                    [&propName](const Property& p) { return p.name == propName; })) {
                                    obj->setProperty(updatedProp);
                                }
                            }

                            updatePropertyControls();
                            });
                    }
                }
                else {
                    std::string strVal = std::get<std::string>(prop.value);
                    auto textInput = attributes_folder->addTextInput(prop.name, strVal);
                    textInput->onTextInputEvent([this, propName, prop](ofxDatGuiTextInputEvent e) mutable {
                        Property updatedProp = prop;
                        updatedProp.value = e.text;

                        for (auto* obj : gui_manager->getScene()->selectedObjects) {
                            auto objProps = obj->getProperties();
                            if (std::any_of(objProps.begin(), objProps.end(),
                                [&propName](const Property& p) { return p.name == propName; })) {
                                obj->setProperty(updatedProp);
                            }
                        }
                        });
                }
                break;
            }

            case PropertyType::Color:
            {
                ofColor initialColor = std::get<ofColor>(prop.value);
                auto colorPicker = attributes_folder->addColorPicker(prop.name, initialColor);
                colorPicker->onColorPickerEvent([this, propName, prop](ofxDatGuiColorPickerEvent e) mutable
                {
                    Property updatedProp = prop;
                    updatedProp.value = e.color;

                    for (auto* obj : gui_manager->getScene()->selectedObjects)
                    {
                        auto objProps = obj->getProperties();
                        if (std::any_of(objProps.begin(), objProps.end(),
                                        [&propName](const Property& p) { return p.name == propName; }))
                        {
                            obj->setProperty(updatedProp);
                        }
                    }
                });
                break;
            }

            case PropertyType::SurfaceType:
            {
                int current = std::get<int>(prop.value);
                const std::vector<std::pair<std::string, SurfaceType>> surfaceOptions = {
                    {"NONE", SurfaceType::NONE},
                    {"MIRROR", SurfaceType::MIRROR},
                    {"GLASS", SurfaceType::GLASS }
                };

                for (const auto& [label, enumVal] : surfaceOptions)
                {
                    bool selected = (current == static_cast<int>(enumVal));
                    auto toggle = attributes_folder->addToggle(label, selected);
                    toggle->onToggleEvent([this, propName, prop, enumVal](ofxDatGuiToggleEvent e) mutable
                    {
                        Property updatedProp = prop;
                        updatedProp.value = e.checked ? static_cast<int>(enumVal)
                            : static_cast<int>(SurfaceType::NONE);

                        for (auto* obj : gui_manager->getScene()->selectedObjects)
                        {
                            auto objProps = obj->getProperties();
                            if (std::any_of(objProps.begin(), objProps.end(),
                                            [&propName](const Property& p) { return p.name == propName; }))
                            {
                                obj->setProperty(updatedProp);
                            }
                        }
                        gui_manager->getScene()->update_Attributes();
                    });
                }
                break;
            }

            default:
                break;
        }
    }

    attributes_folder->expand();
}


bool Bottom_Left_GUI::isMouseOverGui(int x, int y) const
{
    if (!gui) return false;

    float guiX = gui->getPosition().x;
    float guiY = gui->getPosition().y;
    float guiWidth = gui->getWidth();
    float guiHeight = gui->getHeight();

    return (x >= guiX && x <= guiX + guiWidth &&
            y >= guiY && y <= guiY + guiHeight);
}
