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
    void mergeDefaults(const nlohmann::json& defaultConfig);
    std::vector<std::string> splitKey(const std::string& key) const;

    std::string configFilePath;
    nlohmann::json configData;

    const std::string defaultConfigFilePath = "defaultConfig.json";
};

#endif
