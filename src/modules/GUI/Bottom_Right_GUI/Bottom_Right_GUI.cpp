#include "Bottom_Right_GUI.h"
#include "modules/GUI/GUI.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Bottom_Right_GUI::Bottom_Right_GUI()
    : gui_manager(nullptr), gui(nullptr)
{
    gui = new ofxDatGui(ofxDatGuiAnchor::BOTTOM_RIGHT);
    gui->addHeader(":: Configuration Menu ::");
    gui->addFooter();
    gui->getFooter()->setLabelWhenExpanded("CLOSE CONFIGURATION MENU");
    gui->getFooter()->setLabelWhenCollapsed("EXPAND CONFIGURATION MENU");
    gui->getHeader()->setDraggable(true);
    gui->collapse();

    try
    {
        auto config = Configuration::getEntireConfig();
        setupCategories(config);
    }
    catch (const std::exception& e)
    {
        ofLogError("Bottom_Right_GUI") << "Exception: " << e.what();
        abort();
    }
}

void Bottom_Right_GUI::setup(GUI* gui_manager)
{
    this->gui_manager = gui_manager;
}

void Bottom_Right_GUI::setupCategories(const nlohmann::json& config)
{
    if (config.contains("Categories"))
    {
        for (const auto& category : config["Categories"].items())
        {
            std::string categoryName = category.key();
            ofxDatGuiFolder* folder = gui->addFolder(categoryName, ofColor::white);

            if (category.value().contains("contents"))
            {
                for (const auto& component : category.value()["contents"].items())
                {
                    if (component.value().contains("type") && component.value().contains("defaultValue"))
                    {
                        createComponent(folder, component.value()["type"][0], component.key(), component.value());
                    }
                }
            }
        }
    }
}

void Bottom_Right_GUI::createComponent(ofxDatGuiFolder* folder, const std::string& type, const std::string& label, const nlohmann::json& details)
{
    std::string fullKey = folder->getName() + "." + label;
    std::string savedValue = Configuration::get(fullKey);

    if (type == "color")
    {
        std::string defaultColorHex = savedValue.empty() ? details["defaultValue"] : savedValue;
        int hexColor = std::stoi(defaultColorHex, nullptr, 16);
        ofColor defaultColor = ofColor::fromHex(hexColor);

        auto colorPicker = folder->addColorPicker(label, defaultColor);
        colorPicker->onColorPickerEvent([fullKey](ofxDatGuiColorPickerEvent e)
        {
            std::stringstream ss;
            ss << std::hex << std::uppercase << std::setw(6) << std::setfill('0') << e.color.getHex();
            Configuration::set(fullKey, ss.str());
        });
    }
    else if (type == "slider")
    {
        float defaultValue = savedValue.empty() ? details["defaultValue"].get<float>() : std::stof(savedValue);
        auto slider = folder->addSlider(label, details["type"][1], details["type"][2], defaultValue);
        slider->onSliderEvent([fullKey](ofxDatGuiSliderEvent e)
        {
            Configuration::set(fullKey, std::to_string(e.value));
        });
    }
}
