#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "AppConfig.h"

const std::string APP_CONFIG_FILE = "AppConfig.yaml";

// Function to load AppConfig from YAML file
AppConfig loadAppConfig(const std::string& filename) {
    AppConfig config;
    
    try {
        YAML::Node yamlFile = YAML::LoadFile(filename);
        
        // Load video configuration
        if (yamlFile["video"]) {
            YAML::Node videoNode = yamlFile["video"];
            config.video.width = videoNode["width"].as<int>();
            config.video.height = videoNode["height"].as<int>();
            config.video.framerate = videoNode["framerate"].as<int>();
        }
        
        // Load telemetry configurations
        if (yamlFile["telemetry"]) {
            YAML::Node telemetryNode = yamlFile["telemetry"];
            
            for (const auto& entry : telemetryNode) {
                std::string telemetryName = entry.first.as<std::string>();
                YAML::Node telemetryData = entry.second;
                
                TelemetryConfig telemetryConfig;
                telemetryConfig.x = telemetryData["x"].as<int>();
                telemetryConfig.y = telemetryData["y"].as<int>();
                telemetryConfig.format = telemetryData["format"].as<std::string>();
                
                // Load color array
                YAML::Node colorNode = telemetryData["color"];
                for (const auto& colorVal : colorNode) {
                    telemetryConfig.color.push_back(colorVal.as<int>());
                }
                
                // Load ingress configuration
                if (telemetryData["ingress"]) {
                    YAML::Node ingressNode = telemetryData["ingress"];
                    telemetryConfig.ingress.key = ingressNode["key"].as<std::string>();
                    telemetryConfig.ingress.type = ingressNode["type"].as<std::string>();
                    
                    // Handle baud (might be null/~ in YAML)
                    if (ingressNode["baud"] && !ingressNode["baud"].IsNull()) {
                        telemetryConfig.ingress.baud = ingressNode["baud"].as<int>();
                    } else {
                        telemetryConfig.ingress.baud = 0; // or some default value
                    }
                }
                
                config.telemetry[telemetryName] = telemetryConfig;
            }
        }
        
    } catch (const YAML::Exception& e) {
        std::cerr << "Error loading YAML file: " << e.what() << std::endl;
        throw;
    }
    
    return config;
}

// Function to print AppConfig (useful for debugging)
void printAppConfig(const AppConfig& config) {
    std::cout << "=== AppConfig ===" << std::endl;
    
    // Print video config
    std::cout << "Video:" << std::endl;
    std::cout << "  width: " << config.video.width << std::endl;
    std::cout << "  height: " << config.video.height << std::endl;
    std::cout << "  framerate: " << config.video.framerate << std::endl;
    
    // Print telemetry configs
    std::cout << "Telemetry:" << std::endl;
    for (const auto& [name, telemetry] : config.telemetry) {
        std::cout << "  " << name << ":" << std::endl;
        std::cout << "    x: " << telemetry.x << std::endl;
        std::cout << "    y: " << telemetry.y << std::endl;
        std::cout << "    color: [";
        for (size_t i = 0; i < telemetry.color.size(); ++i) {
            std::cout << telemetry.color[i];
            if (i < telemetry.color.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
        std::cout << "    format: " << telemetry.format << std::endl;
        std::cout << "    ingress:" << std::endl;
        std::cout << "      key: " << telemetry.ingress.key << std::endl;
        std::cout << "      type: " << telemetry.ingress.type << std::endl;
        std::cout << "      baud: " << telemetry.ingress.baud << std::endl;
    }
}

int main() {
    try {
        // Load configuration from YAML
        AppConfig appConfig = loadAppConfig();
        
        // Print the loaded configuration
        printAppConfig(appConfig);
        
        // Example of using the configuration
        std::cout << "\n=== Usage Examples ===" << std::endl;
        std::cout << "Video resolution: " << appConfig.video.width << "x" << appConfig.video.height << std::endl;
        
        if (appConfig.telemetry.find("velocity") != appConfig.telemetry.end()) {
            const auto& velocity = appConfig.telemetry.at("velocity");
            std::cout << "Velocity display at: (" << velocity.x << ", " << velocity.y << ")" << std::endl;
            std::cout << "Velocity format: " << velocity.format << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
