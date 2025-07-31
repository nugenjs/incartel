#ifndef SENSOR_SIMULATOR_H
#define SENSOR_SIMULATOR_H

#include "SensorData.h"
#include <cmath>
#include <random>

class SensorSimulator {
private:
    SensorConfig config;
    std::chrono::steady_clock::time_point startTime;
    std::mt19937 generator;
    std::normal_distribution<double> noiseDistribution;
    
public:
    SensorSimulator(const SensorConfig& cfg = SensorConfig());
    
    SensorReading generateReading();
    double getCurrentTime() const;
    
    // Setters for real-time configuration
    void setTemperatureParams(double base, double amplitude, double frequency);
    void setWindSpeedParams(double base, double amplitude, double frequency);
    void setHumidityParams(double base, double amplitude, double frequency);
    
    const SensorConfig& getConfig() const { return config; }
};

#endif // SENSOR_SIMULATOR_H
