#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

struct ConfigItem {
    int x;
    int y;
    std::vector<int> color; // expecting 3 elements: R, G, B
    std::string format;
};

int main() {
    std::map<std::string, ConfigItem> configMap;

    YAML::Node config = YAML::LoadFile("TelemetryConfig.yaml");

    for (const auto& entry : config) {
        std::string name = entry.first.as<std::string>();
        YAML::Node data = entry.second;

        ConfigItem item;
        item.x = data["x"].as<int>();
        item.y = data["y"].as<int>();

        // Read color as a YAML array
        YAML::Node colorNode = data["color"];
        for (const auto& colorVal : colorNode) {
            item.color.push_back(colorVal.as<int>());
        }

        item.format = data["format"].as<std::string>();

        configMap[name] = item;
    }

    // Print to verify
    for (const auto& [key, val] : configMap) {
        std::cout << key << ": x=" << val.x << ", y=" << val.y
                  << ", color=(" << val.color[0] << "," << val.color[1] << "," << val.color[2] << ")"
                  << ", format=" << val.format << "\n";
    }

    return 0;
}
