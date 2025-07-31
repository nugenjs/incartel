#include "SensorSimulator.h"
#include "DataVisualizer.h"
#include <iostream>
#include <thread>
#include <chrono>

class MultiInputApp {
private:
    SensorSimulator simulator;
    DataVisualizer visualizer;
    SensorConfig config;
    bool running;
    
public:
    MultiInputApp() : running(true) {
        // Configure sensor simulation parameters
        config.maxDataPoints = 200;
        config.updateIntervalMs = 50;
        
        // Temperature: varies between 5°C and 35°C over ~20 seconds
        config.tempBase = 20.0;
        config.tempAmplitude = 15.0;
        config.tempFrequency = 0.05;
        
        // Wind speed: varies between 5 and 25 km/h over ~13 seconds
        config.windBase = 15.0;
        config.windAmplitude = 10.0;
        config.windFrequency = 0.075;
        
        // Humidity: varies between 35% and 85% over ~25 seconds
        config.humidityBase = 60.0;
        config.humidityAmplitude = 25.0;
        config.humidityFrequency = 0.04;
        
        simulator = SensorSimulator(config);
        visualizer.setConfig(config);
    }
    
    void run() {
        std::cout << "Starting Multi-Input Data Visualization..." << std::endl;
        std::cout << "Simulating Temperature, Wind Speed, and Humidity data" << std::endl;
        std::cout << "Data update interval: " << config.updateIntervalMs << "ms" << std::endl;
        std::cout << "Press 'q' or ESC in the window to quit" << std::endl;
        
        auto lastUpdate = std::chrono::steady_clock::now();
        
        while (running) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);
            
            if (elapsed.count() >= config.updateIntervalMs) {
                // Generate new sensor reading
                SensorReading reading = simulator.generateReading();
                
                // Add to visualizer
                visualizer.addDataPoint(reading);
                
                lastUpdate = now;
            }
            
            // Render visualization
            visualizer.render();
            
            // Check for exit condition
            if (visualizer.shouldClose()) {
                running = false;
            }
            
            // Small delay to prevent excessive CPU usage
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        
        std::cout << "Application shutting down..." << std::endl;
    }
    
    void printConfiguration() const {
        std::cout << "\n=== Sensor Configuration ===" << std::endl;
        std::cout << "Temperature: Base=" << config.tempBase << "°C, "
                  << "Amplitude=±" << config.tempAmplitude << "°C, "
                  << "Frequency=" << config.tempFrequency << " Hz" << std::endl;
        std::cout << "Wind Speed: Base=" << config.windBase << " km/h, "
                  << "Amplitude=±" << config.windAmplitude << " km/h, "
                  << "Frequency=" << config.windFrequency << " Hz" << std::endl;
        std::cout << "Humidity: Base=" << config.humidityBase << "%, "
                  << "Amplitude=±" << config.humidityAmplitude << "%, "
                  << "Frequency=" << config.humidityFrequency << " Hz" << std::endl;
        std::cout << "Max Data Points: " << config.maxDataPoints << std::endl;
        std::cout << "==============================\n" << std::endl;
    }
};

int main(int /* argc */, char* /* argv */[]) {
    try {
        MultiInputApp app;
        
        // Print configuration
        app.printConfiguration();
        
        // Run the application
        app.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return -1;
    }
    
    return 0;
}