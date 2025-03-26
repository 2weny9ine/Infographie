#pragma once
#include <string>
#include <variant>
#include <vector>

/**
 * Enumeration of property data types.
 */
enum class PropertyType {
    Bool,
    Float,
    Int,
    String,
    Color,
    Percent
};

using PropertyValue = std::variant<bool, float, int, std::string>;

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
