#include "Configuration.h"
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
        // Load default configuration
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
        for (const auto& [key, value] : configOverrides.items())
        {
            file << key << "=" << value.get<std::string>() << "\n";
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
    if (configOverrides.contains(key))
    {
        return configOverrides[key].get<std::string>();
    }

    auto keys = splitKey(key);
    const nlohmann::json* currentLevel = &defaultConfig["Categories"];

    for (const auto& k : keys)
    {
        if (currentLevel->contains(k))
        {
            currentLevel = &((*currentLevel)[k]);
        }
        else
        {
            return "";
        }
    }

    if (currentLevel->is_string())
    {
        return currentLevel->get<std::string>();
    }
    return currentLevel->dump();
}

void Configuration::setConfig(const std::string& key, const std::string& value)
{
    configOverrides[key] = value;
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
