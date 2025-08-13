#ifndef DATA_VISUALIZER_H
#define DATA_VISUALIZER_H

#include "SensorData.h"
#include <opencv2/opencv.hpp>
#include <deque>

class DataVisualizer {
private:
    std::string windowName;
    int windowWidth;
    int windowHeight;
    std::deque<SensorReading> dataBuffer;
    SensorConfig config;
    int fifthFrameCount = 0;
    
    // Graph properties
    int graphHeight;
    int graphWidth;
    int graphMargin;
    int headerHeight;
    
    // Colors for different data types
    cv::Scalar tempColor;
    cv::Scalar windColor;
    cv::Scalar humidityColor;
    cv::Scalar bgColor;
    cv::Scalar textColor;
    cv::Scalar gridColor;
    
public:
    DataVisualizer(const std::string& winName = "Multi-Input Data Visualization", 
                   int width = 1200, int height = 800);
    
    void addDataPoint(const SensorReading& reading);
    void render();
    void setConfig(const SensorConfig& cfg) { config = cfg; }
    
    bool shouldClose();
    
private:
    void drawHeader(cv::Mat& image, const SensorReading& latestReading);
    void drawGraph(cv::Mat& image, const std::vector<double>& data, 
                   cv::Scalar color, double minVal, double maxVal, 
                   int yOffset, const std::string& label, const std::string& unit);
    void drawGrid(cv::Mat& image, int yOffset);
    void drawLegend(cv::Mat& image);
    std::vector<double> extractTemperatureData() const;
    std::vector<double> extractWindSpeedData() const;
    std::vector<double> extractHumidityData() const;
};

#endif // DATA_VISUALIZER_H
