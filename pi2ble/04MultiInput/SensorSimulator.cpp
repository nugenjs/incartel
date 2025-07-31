#include "SensorSimulator.h"

SensorSimulator::SensorSimulator(const SensorConfig& cfg) 
    : config(cfg), generator(std::random_device{}()), noiseDistribution(0.0, 0.5) {
    startTime = std::chrono::steady_clock::now();
}

SensorReading SensorSimulator::generateReading() {
    double currentTime = getCurrentTime();
    
    // Generate temperature with sine wave + noise
    double temperature = config.tempBase + 
                        config.tempAmplitude * std::sin(2 * M_PI * config.tempFrequency * currentTime) +
                        noiseDistribution(generator);
    
    // Generate wind speed with sine wave + noise (different phase)
    double windSpeed = config.windBase + 
                      config.windAmplitude * std::sin(2 * M_PI * config.windFrequency * currentTime + M_PI/3) +
                      noiseDistribution(generator);
    
    // Generate humidity with sine wave + noise (different phase)
    double humidity = config.humidityBase + 
                     config.humidityAmplitude * std::sin(2 * M_PI * config.humidityFrequency * currentTime + M_PI/2) +
                     noiseDistribution(generator);
    
    // Clamp values to realistic ranges
    temperature = std::max(-30.0, std::min(50.0, temperature));
    windSpeed = std::max(0.0, std::min(100.0, windSpeed));
    humidity = std::max(0.0, std::min(100.0, humidity));
    
    return SensorReading(temperature, windSpeed, humidity);
}

double SensorSimulator::getCurrentTime() const {
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
    return duration.count() / 1000.0; // Convert to seconds
}

void SensorSimulator::setTemperatureParams(double base, double amplitude, double frequency) {
    config.tempBase = base;
    config.tempAmplitude = amplitude;
    config.tempFrequency = frequency;
}

void SensorSimulator::setWindSpeedParams(double base, double amplitude, double frequency) {
    config.windBase = base;
    config.windAmplitude = amplitude;
    config.windFrequency = frequency;
}

void SensorSimulator::setHumidityParams(double base, double amplitude, double frequency) {
    config.humidityBase = base;
    config.humidityAmplitude = amplitude;
    config.humidityFrequency = frequency;
}
