#include <string>
#ifndef FRAMECONFIG_H
#define FRAMECONFIG_H

struct DatumDrawProps {
    int x = 0;
    int y = 0;
    int rgb[3] = {0, 0, 0};
    std::string displayVal = "";
};

#include <map>

struct TelemetryDrawConfig {
    std::map<std::string, DatumDrawProps> dataDrawProps;
    TelemetryDrawConfig();
};

struct ImageConfig {
    int width = 320;
    int height = 240;
};

#endif // FRAMECONFIG_H
