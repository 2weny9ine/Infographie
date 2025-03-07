#include "Configuration.h"
#include <fstream>
#include "ofLog.h"
#include "json.hpp"
#include <algorithm>

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
    std::ifstream file(configFilePath);
    if (file.is_open())
    {
        file >> configData;
        file.close();
    }
    else
    {
        ofLogWarning("Configuration") << "Configuration file not found. Using default configuration.";
        std::ifstream defaultFile(defaultConfigFilePath);
        if (defaultFile.is_open())
        {
            defaultFile >> configData;
            defaultFile.close();
        }
        else
        {
            ofLogError("Configuration") << "Default configuration file not found.";
        }
    }

    // Ensure all default keys are present
    std::ifstream defaultFile(defaultConfigFilePath);
    nlohmann::json defaultConfig;
    if (defaultFile.is_open())
    {
        defaultFile >> defaultConfig;
        defaultFile.close();
    }

    mergeDefaults(defaultConfig);
    saveConfig();
}

void Configuration::saveConfig() const
{
    std::ofstream file(configFilePath);
    if (file.is_open())
    {
        file << configData.dump(4);
        file.close();
    }
    else
    {
        ofLogError("Configuration") << "Unable to save configuration file.";
    }
}

std::string Configuration::get(const std::string& key)
{
    return getInstance().getConfig(key);
}

void Configuration::set(const std::string& key, const std::string& value)
{
    getInstance().setConfig(key, value);
}

std::string Configuration::getConfig(const std::string& key) const
{
    std::string normalizedKey = key;
    std::replace(normalizedKey.begin(), normalizedKey.end(), ' ', '.');

    auto keys = splitKey(normalizedKey);
    const nlohmann::json* currentLevel = &configData;

    for (const auto& k : keys)
    {
        if (currentLevel->contains(k))
        {
            currentLevel = &((*currentLevel)[k]);
        }
        else
        {
            ofLogError("Configuration") << "Configuration key '" << key << "' not found.";
            return "";
        }
    }

    return currentLevel->get<std::string>();
}

void Configuration::setConfig(const std::string& key, const std::string& value)
{
    std::string normalizedKey = key;
    std::replace(normalizedKey.begin(), normalizedKey.end(), ' ', '.');

    auto keys = splitKey(normalizedKey);
    nlohmann::json* currentLevel = &configData;

    for (size_t i = 0; i < keys.size(); ++i)
    {
        if (i == keys.size() - 1)
        {
            (*currentLevel)[keys[i]] = value;
        }
        else
        {
            currentLevel = &((*currentLevel)[keys[i]]);
        }
    }
}

void Configuration::mergeDefaults(const nlohmann::json& defaultConfig)
{
    for (auto it = defaultConfig.begin(); it != defaultConfig.end(); ++it)
    {
        if (!configData.contains(it.key()))
        {
            configData[it.key()] = it.value();
        }
        else if (it.value().is_object() && configData[it.key()].is_object())
        {
            mergeDefaults(it.value());
        }
    }
}

std::vector<std::string> Configuration::splitKey(const std::string& key) const
{
    std::vector<std::string> keys;
    size_t start = 0;
    size_t end = key.find('.');
    while (end != std::string::npos)
    {
        keys.push_back(key.substr(start, end - start));
        start = end + 1;
        end = key.find('.', start);
    }
    keys.push_back(key.substr(start));
    return keys;
}
