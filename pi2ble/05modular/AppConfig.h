#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

// telemetry:
//   velocity:
//     x: 100
//     y: 200
//     color: [255, 0, 0]
//     format: '$0mph'
//     ingress:
//       key: velocity
//       baud: 9600
//       type: serial
//   rpm:
//     x: 100
//     y: 300
//     color: [0, 255, 0]
//     format: '$0rpm'
//     ingress:
//       key: rpm
//       baud: null
//       type: ble
// video:
//   width: 640
//   height: 480
//   framerate: 30

struct VideoConfig {
    int width;
    int height;
    int framerate;
};

struct TelemetryIngressConfig {
    std::string key;
    int baud;
    std::string type;
};

struct TelemetryConfig {
    int x;
    int y;
    std::vector<int> color; // expecting 3 elements: R, G, B
    std::string format;
    TelemetryIngressConfig ingress;
};

struct AppConfig {
    std::map<std::string, TelemetryConfig> telemetry;
    VideoConfig video;
};

// Function declarations
AppConfig loadAppConfig(const std::string& filename = "AppConfig.yaml");
void printAppConfig(const AppConfig& config);

#endif // APPCONFIG_H
