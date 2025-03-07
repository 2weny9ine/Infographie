#include "Configuration.h"
#include "Application.h"
#include <fstream>
#include <sstream>
#include "ofLog.h"
#include <algorithm>
#include <iomanip>

Configuration::Configuration() : configFilePath("data/infographie.config")
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
            ofLogError("Configuration") << "Default configuration file not found.";
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
    }
    catch (const std::exception& e)
    {
        ofLogError("Configuration") << "Exception: " << e.what();
        abort();
    }
}

void Configuration::saveConfig() const
{
    std::ofstream file(configFilePath);
    if (file.is_open())
    {
        for (const auto& pair : configOverrides.items())
        {
            file << pair.key() << "=" << pair.value() << "\n";
        }
        file.close();
    }
    else
    {
        ofLogError("Configuration") << "Unable to save configuration file.";
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
    if (configOverrides.find(key) != configOverrides.end())
    {
        return configOverrides.at(key);
    }

    // If not in user config, look for it in default config
    auto keys = splitKey(key);
    const nlohmann::json* currentLevel = &defaultConfig["Categories"];

    // Traverse through the keys to find the corresponding value in defaultConfig
    for (const auto& k : keys)
    {
        if (currentLevel->find(k) != currentLevel->end())
        {
            currentLevel = &((*currentLevel)[k]);
        }
        else
        {
            // Key not found in default config, return default value
            return getDefaultValueForKey(key);
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

// Function to get the default value from the JSON config
std::string Configuration::getDefaultValueForKey(const std::string& key) const
{
    // Split the key to find the proper location in defaultConfig
    auto keys = splitKey(key);
    const nlohmann::json* currentLevel = &defaultConfig["Categories"];

    for (const auto& k : keys)
    {
        if (currentLevel->find(k) != currentLevel->end())
        {
            currentLevel = &((*currentLevel)[k]);
        }
        else
        {
            // If we can't find the key in the default config, return an empty string or fallback value
            return "";
        }
    }

    // Return the default value if found
    if (currentLevel->is_string())
    {
        return currentLevel->get<std::string>();
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
