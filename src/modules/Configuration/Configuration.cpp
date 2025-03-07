#include "Configuration.h"
#include "Application.h"
#include <fstream>
#include <sstream>
#include "ofLog.h"
#include <algorithm>
#include <iomanip>

Configuration::Configuration() : configFilePath("data/infographie.config"), defaultConfigFilePath("data/default_infographie.config")
{
    loadConfig();
}

Configuration::~Configuration()
{
    saveConfig();
}

Configuration& Configuration::getInstance()
{
    static Configuration instance;
    return instance;
}

void Configuration::loadConfig()
{
    try
    {
        // Load default config file
        std::ifstream defaultFile(defaultConfigFilePath);
        if (defaultFile.is_open())
        {
            defaultFile >> defaultConfig;
            defaultFile.close();
        }
        else
        {
            ofLogError("Configuration") << "Default configuration file not found at: " << defaultConfigFilePath;
            throw std::runtime_error("Default configuration file not found.");
        }

        // Load user overrides
        std::ifstream file(configFilePath);
        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                size_t delimiterPos = line.find('=');
                if (delimiterPos != std::string::npos)
                {
                    std::string key = line.substr(0, delimiterPos);
                    std::string value = line.substr(delimiterPos + 1);
                    configOverrides[key] = value;
                }
            }
            file.close();
        }
        else
        {
            ofLogWarning("Configuration") << "User configuration file not found at: " << configFilePath;
        }
    }
    catch (const std::exception& e)
    {
        ofLogError("Configuration") << "Exception: " << e.what();
    }
}

void Configuration::saveConfig() const
{
    std::ofstream file(configFilePath);
    if (file.is_open())
    {
        for (const auto& pair : configOverrides)
        {
            file << pair.first << "=" << pair.second << "\n";
        }
        file.close();
    }
    else
    {
        ofLogError("Configuration") << "Unable to save configuration file at: " << configFilePath;
    }
}

void Configuration::save()
{
    getInstance().saveConfig();
}

std::string Configuration::get(const std::string& key)
{
    return getInstance().getConfig(key);
}

float Configuration::getFloat(const std::string& key)
{
    std::string valueStr = getInstance().getConfig(key);
    try
    {
        return std::stof(valueStr);
    }
    catch (const std::exception& e)
    {
        ofLogError("Configuration") << "Invalid float value for key: " << key << " - " << e.what();
        throw; // Rethrow the exception after logging
    }
}

int Configuration::getInt(const std::string& key)
{
    std::string valueStr = getInstance().getConfig(key);
    try
    {
        return std::stoi(valueStr);
    }
    catch (const std::exception& e)
    {
        ofLogError("Configuration") << "Invalid int value for key: " << key << " - " << e.what();
        throw; // Rethrow the exception after logging
    }
}

ofColor Configuration::getColor(const std::string& key)
{
    std::string valueStr = getInstance().getConfig(key);
    try
    {
        // Remove potential '#' prefix
        if (!valueStr.empty() && valueStr[0] == '#')
        {
            valueStr = valueStr.substr(1);
        }

        // Convert hex string to ofColor
        ofColor color = ofColor::fromHex(std::stoul(valueStr, nullptr, 16));
        return color;
    }
    catch (const std::exception& e)
    {
        ofLogError("Configuration") << "Invalid color value for key: " << key << " - " << e.what();
        throw; // Rethrow the exception after logging
    }
}

void Configuration::set(const std::string& key, const std::string& value)
{
    getInstance().setConfig(key, value);
    save();
}

nlohmann::json Configuration::getEntireConfig()
{
    return getInstance().defaultConfig;
}

std::string Configuration::getConfig(const std::string& key) const
{
    // First check for user overrides
    auto overrideIt = configOverrides.find(key);
    if (overrideIt != configOverrides.end())
    {
        return overrideIt->second;
    }

    // Look for the key in the default configuration.
    auto keys = splitKey(key);
    const nlohmann::json* currentLevel = &defaultConfig["Categories"];

    for (const auto& k : keys)
    {
        if (currentLevel->contains(k))
        {
            currentLevel = &((*currentLevel)[k]);
        }
        else if (currentLevel->contains("contents") && (*currentLevel)["contents"].contains(k))
        {
            currentLevel = &((*currentLevel)["contents"])[k];
        }
        else
        {
            // Key not found in default config, return an empty string
            return "";
        }
    }

    // Check if the final level contains the "defaultValue" key
    if (currentLevel->contains("defaultValue"))
    {
        if (currentLevel->at("defaultValue").is_string())
        {
            return currentLevel->at("defaultValue").get<std::string>();
        }
        else
        {
            // If defaultValue is not a string, dump it as a string
            return currentLevel->at("defaultValue").dump();
        }
    }

    // Return the value as a string if it's a string type
    if (currentLevel->is_string())
    {
        return currentLevel->get<std::string>();
    }

    // Otherwise, return the entire value serialized to a string
    return currentLevel->dump();
}

std::string Configuration::getDefaultValueForKey(const std::string& key) const
{
    // Split the key to find the proper location in defaultConfig
    auto keys = splitKey(key);
    const nlohmann::json* currentLevel = &defaultConfig["Categories"];

    for (const auto& k : keys)
    {
        if (currentLevel->contains(k))
        {
            currentLevel = &((*currentLevel)[k]);
        }
        else if (currentLevel->contains("contents") && (*currentLevel)["contents"].contains(k))
        {
            currentLevel = &((*currentLevel)["contents"])[k];
        }
        else
        {
            // If we can't find the key in the default config, return an empty string or fallback value
            return "";
        }
    }

    // Return the default value if found
    if (currentLevel->contains("defaultValue"))
    {
        if (currentLevel->at("defaultValue").is_string())
        {
            return currentLevel->at("defaultValue").get<std::string>();
        }
        else
        {
            return currentLevel->at("defaultValue").dump();
        }
    }

    // Fallback to serialized value as default
    return currentLevel->dump();
}

void Configuration::setConfig(const std::string& key, const std::string& value)
{
    configOverrides[key] = value;

    if (key.find("Grid.") == 0)
    {
        Application* app = &Application::getInstance();
        Grid* grid = app->getScene().grid;
        grid->update();
    }
}

std::vector<std::string> Configuration::splitKey(const std::string& key) const
{
    std::vector<std::string> keys;
    std::stringstream ss(key);
    std::string token;
    while (std::getline(ss, token, '.'))
    {
        keys.push_back(token);
    }
    return keys;
}
