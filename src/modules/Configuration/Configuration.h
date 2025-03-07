#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <unordered_map>
#include <json.hpp>
#include "ofColor.h"

class Configuration {
public:
    static Configuration& getInstance();

    void loadConfig();
    void saveConfig() const;
    static void save();

    static std::string get(const std::string& key);
    static float getFloat(const std::string& key);
    static int getInt(const std::string& key);
    static ofColor getColor(const std::string& key);

    static void set(const std::string& key, const std::string& value);

    static nlohmann::json getEntireConfig();

    ~Configuration();

private:
    Configuration();

    std::string configFilePath;
    std::string defaultConfigFilePath;

    nlohmann::json defaultConfig;
    std::unordered_map<std::string, std::string> configOverrides;

    std::string getConfig(const std::string& key) const;
    std::string getDefaultValueForKey(const std::string& key) const;
    void setConfig(const std::string& key, const std::string& value);
    std::vector<std::string> splitKey(const std::string& key) const;
};

#endif // CONFIGURATION_H
