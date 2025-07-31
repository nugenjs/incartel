#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <vector>
#include <string>
#include <chrono>

struct SensorReading {
    double temperature;    // Celsius
    double windSpeed;      // km/h
    double humidity;       // Percentage
    std::chrono::steady_clock::time_point timestamp;
    
    SensorReading() : temperature(0.0), windSpeed(0.0), humidity(0.0) {
        timestamp = std::chrono::steady_clock::now();
    }
    
    SensorReading(double temp, double wind, double hum) 
        : temperature(temp), windSpeed(wind), humidity(hum) {
        timestamp = std::chrono::steady_clock::now();
    }
};

struct SensorConfig {
    // Temperature configuration
    double tempBase = 20.0;        // Base temperature (°C)
    double tempAmplitude = 15.0;   // Temperature variation amplitude
    double tempFrequency = 0.1;    // Temperature change frequency
    
    // Wind speed configuration
    double windBase = 15.0;        // Base wind speed (km/h)
    double windAmplitude = 10.0;   // Wind speed variation amplitude
    double windFrequency = 0.15;   // Wind speed change frequency
    
    // Humidity configuration
    double humidityBase = 60.0;    // Base humidity (%)
    double humidityAmplitude = 25.0; // Humidity variation amplitude
    double humidityFrequency = 0.08; // Humidity change frequency
    
    // Data collection
    int maxDataPoints = 200;       // Maximum number of data points to store
    int updateIntervalMs = 50;     // Update interval in milliseconds
};

#endif // SENSOR_DATA_H
