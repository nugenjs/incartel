#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

const std::string TELEMETRY_CONFIG_FILE = "TelemetryConfig.yaml";

struct ConfigFieldIngress {
    std::string key;
};

struct ConfigField {
    int x;
    int y;
    std::vector<int> color; // expecting 3 elements: R, G, B
    std::string format;
    ConfigFieldIngress ingress;
};

int main() {
    std::map<std::string, ConfigField> configMap;

    YAML::Node config = YAML::LoadFile(TELEMETRY_CONFIG_FILE);

    for (const auto& entry : config) {
        std::string name = entry.first.as<std::string>();
        YAML::Node data = entry.second;

        ConfigField item;
        item.x = data["x"].as<int>();
        item.y = data["y"].as<int>();

        // Read color as a YAML array
        YAML::Node colorNode = data["color"];
        for (const auto& colorVal : colorNode) {
            item.color.push_back(colorVal.as<int>());
        }

        item.format = data["format"].as<std::string>();
        
        // Read ingress configuration
        YAML::Node ingressNode = data["ingress"];
        item.ingress.key = ingressNode["key"].as<std::string>();
        // Future ingress fields can be read here:
        // if (ingressNode["type"]) item.ingress.type = ingressNode["type"].as<std::string>();
        // if (ingressNode["port"]) item.ingress.port = ingressNode["port"].as<int>();

        configMap[name] = item;
    }

    // Print to verify
    for (const auto& [key, val] : configMap) {
        std::cout << key << ": x=" << val.x << ", y=" << val.y
                  << ", color=(" << val.color[0] << "," << val.color[1] << "," << val.color[2] << ")"
                  << ", format=" << val.format 
                  << ", ingress.key=" << val.ingress.key << "\n";
    }

    return 0;
}
