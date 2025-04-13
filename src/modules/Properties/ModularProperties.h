#pragma once
#include <string>
#include <variant>
#include <vector>
#include "ofColor.h"  // Include this for ofColor support

enum class PropertyType {
    Bool,
    Float,
    Int,
    String,
    Color,
    Percent,
    SurfaceType
};

using PropertyValue = std::variant<bool, float, int, std::string, ofColor>;

struct Property {
    std::string   name;
    PropertyType  type;
    PropertyValue value;

    float         min = 0.0f;
    float         max = 1.0f;
    float         step = 0.1f;
    int           decimals = 2;
};

class IPropertyProvider {
public:
    virtual ~IPropertyProvider() = default;

    virtual std::vector<Property> getProperties() const = 0;
    virtual void setProperty(const Property& prop) = 0;
};
