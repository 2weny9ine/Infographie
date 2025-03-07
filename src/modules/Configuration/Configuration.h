#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <vector>
#include "json.hpp"
#include "ofLog.h"

class Configuration {
public:
    static std::string get(const std::string& key);
    static void set(const std::string& key, const std::string& value);
    static nlohmann::json getEntireConfig();
    static void save();

private:
    Configuration();
    ~Configuration();
    Configuration(const Configuration&) = delete;
    Configuration& operator=(const Configuration&) = delete;

    static Configuration& getInstance();
    void loadConfig();
    void saveConfig() const;
    std::string getConfig(const std::string& key) const;
    void setConfig(const std::string& key, const std::string& value);
    std::vector<std::string> splitKey(const std::string& key) const;

    std::string getDefaultValueForKey(const std::string& key) const;

    std::string configFilePath;
    nlohmann::json configOverrides;
    nlohmann::json defaultConfig;

    const std::string defaultConfigFilePath = "data/defaultConfig.json";
};

#endif
