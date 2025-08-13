#include "DataVisualizer.h"
#include <algorithm>
#include <iomanip>
#include <sstream>

DataVisualizer::DataVisualizer(const std::string& winName, int width, int height)
    : windowName(winName), windowWidth(width), windowHeight(height) {
    
    // Initialize graph properties
    graphMargin = 60;
    headerHeight = 100;
    graphHeight = (windowHeight - headerHeight - 4 * graphMargin) / 3;
    graphWidth = windowWidth - 2 * graphMargin;
    
    // Initialize colors
    tempColor = cv::Scalar(0, 0, 255);      // Red for temperature
    windColor = cv::Scalar(255, 0, 0);      // Blue for wind speed
    humidityColor = cv::Scalar(0, 255, 0);  // Green for humidity
    bgColor = cv::Scalar(40, 40, 40);       // Dark gray background
    textColor = cv::Scalar(255, 255, 255);  // White text
    gridColor = cv::Scalar(80, 80, 80);     // Light gray grid
    
    // Create window
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
}

void DataVisualizer::addDataPoint(const SensorReading& reading) {
    dataBuffer.push_back(reading);
    
    // Limit buffer size
    while (dataBuffer.size() > static_cast<size_t>(config.maxDataPoints)) {
        dataBuffer.pop_front();
    }
}

void DataVisualizer::render() {
    cv::Mat image(windowHeight, windowWidth, CV_8UC3, bgColor);
    
    if (dataBuffer.empty()) {
        cv::putText(image, "Waiting for data...", cv::Point(50, 50), 
                   cv::FONT_HERSHEY_SIMPLEX, 1.0, textColor, 2);
        cv::imshow(windowName, image);
        return;
    }
    
    const SensorReading& latestReading = dataBuffer.back();
    
    // Draw header with current values
    drawHeader(image, latestReading);
    
    // Extract data for each sensor type
    std::vector<double> tempData = extractTemperatureData();
    std::vector<double> windData = extractWindSpeedData();
    std::vector<double> humidityData = extractHumidityData();
    
    // Draw graphs
    int yOffset = headerHeight;
    drawGraph(image, tempData, tempColor, -30.0, 50.0, yOffset, "Temperature", "°C");
    
    yOffset += graphHeight + graphMargin;
    drawGraph(image, windData, windColor, 0.0, 100.0, yOffset, "Wind Speed", "km/h");
    
    yOffset += graphHeight + graphMargin;
    drawGraph(image, humidityData, humidityColor, 0.0, 100.0, yOffset, "Humidity", "%");
    
    // Draw legend
    if (++fifthFrameCount % 5 == 0) {
        drawLegend(image);
    }
    // drawLegend(image);
    
    cv::imshow(windowName, image);
}

void DataVisualizer::drawHeader(cv::Mat& image, const SensorReading& latestReading) {
    // Background for header
    cv::rectangle(image, cv::Point(0, 0), cv::Point(windowWidth, headerHeight), 
                 cv::Scalar(60, 60, 60), -1);
    
    // Title
    cv::putText(image, "Real-time Sensor Data Monitor", cv::Point(20, 30), 
               cv::FONT_HERSHEY_SIMPLEX, 1.0, textColor, 2);
    
    // Current values
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1);
    
    // Temperature
    ss.str("");
    ss << "Temp: " << latestReading.temperature << "°C";
    cv::putText(image, ss.str(), cv::Point(20, 65), 
               cv::FONT_HERSHEY_SIMPLEX, 0.6, tempColor, 2);
    
    // Wind Speed
    ss.str("");
    ss << "Wind: " << latestReading.windSpeed << " km/h";
    cv::putText(image, ss.str(), cv::Point(200, 65), 
               cv::FONT_HERSHEY_SIMPLEX, 0.6, windColor, 2);
    
    // Humidity
    ss.str("");
    ss << "Humidity: " << latestReading.humidity << "%";
    cv::putText(image, ss.str(), cv::Point(400, 65), 
               cv::FONT_HERSHEY_SIMPLEX, 0.6, humidityColor, 2);
    
    // Data points count
    ss.str("");
    ss << "Data Points: " << dataBuffer.size();
    cv::putText(image, ss.str(), cv::Point(windowWidth - 200, 65), 
               cv::FONT_HERSHEY_SIMPLEX, 0.6, textColor, 1);
}

void DataVisualizer::drawGraph(cv::Mat& image, const std::vector<double>& data, 
                              cv::Scalar color, double minVal, double maxVal, 
                              int yOffset, const std::string& label, const std::string& unit) {
    if (data.empty()) return;
    
    // Draw background
    cv::rectangle(image, cv::Point(graphMargin, yOffset), 
                 cv::Point(graphMargin + graphWidth, yOffset + graphHeight), 
                 cv::Scalar(20, 20, 20), -1);
    
    // Draw grid
    drawGrid(image, yOffset);
    
    // Draw border
    cv::rectangle(image, cv::Point(graphMargin, yOffset), 
                 cv::Point(graphMargin + graphWidth, yOffset + graphHeight), 
                 gridColor, 1);
    
    // Draw label
    cv::putText(image, label + " (" + unit + ")", cv::Point(graphMargin, yOffset - 10), 
               cv::FONT_HERSHEY_SIMPLEX, 0.6, color, 2);
    
    // Draw min/max labels
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << maxVal;
    cv::putText(image, ss.str(), cv::Point(5, yOffset + 15), 
               cv::FONT_HERSHEY_SIMPLEX, 0.4, textColor, 1);
    
    ss.str("");
    ss << std::fixed << std::setprecision(1) << minVal;
    cv::putText(image, ss.str(), cv::Point(5, yOffset + graphHeight - 5), 
               cv::FONT_HERSHEY_SIMPLEX, 0.4, textColor, 1);
    
    // Draw data points
    if (data.size() > 1) {
        for (size_t i = 1; i < data.size(); ++i) {
            // Calculate positions
            double x1 = graphMargin + (static_cast<double>(i - 1) / (data.size() - 1)) * graphWidth;
            double x2 = graphMargin + (static_cast<double>(i) / (data.size() - 1)) * graphWidth;
            
            double y1 = yOffset + graphHeight - ((data[i - 1] - minVal) / (maxVal - minVal)) * graphHeight;
            double y2 = yOffset + graphHeight - ((data[i] - minVal) / (maxVal - minVal)) * graphHeight;
            
            // Clamp Y coordinates
            y1 = std::max(static_cast<double>(yOffset), std::min(static_cast<double>(yOffset + graphHeight), y1));
            y2 = std::max(static_cast<double>(yOffset), std::min(static_cast<double>(yOffset + graphHeight), y2));
            
            cv::line(image, cv::Point(static_cast<int>(x1), static_cast<int>(y1)), 
                    cv::Point(static_cast<int>(x2), static_cast<int>(y2)), color, 2);
        }
    }
    
    // Draw current value indicator
    if (!data.empty()) {
        double currentVal = data.back();
        double y = yOffset + graphHeight - ((currentVal - minVal) / (maxVal - minVal)) * graphHeight;
        y = std::max(static_cast<double>(yOffset), std::min(static_cast<double>(yOffset + graphHeight), y));
        
        cv::circle(image, cv::Point(graphMargin + graphWidth - 5, static_cast<int>(y)), 4, color, -1);
        
        // Current value text
        std::stringstream currentSs;
        currentSs << std::fixed << std::setprecision(1) << currentVal;
        cv::putText(image, currentSs.str(), cv::Point(graphMargin + graphWidth + 10, static_cast<int>(y) + 5), 
                   cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
    }
}

void DataVisualizer::drawGrid(cv::Mat& image, int yOffset) {
    // Horizontal grid lines
    for (int i = 1; i < 4; ++i) {
        int y = yOffset + (i * graphHeight) / 4;
        cv::line(image, cv::Point(graphMargin, y), 
                cv::Point(graphMargin + graphWidth, y), gridColor, 1);
    }
    
    // Vertical grid lines
    for (int i = 1; i < 10; ++i) {
        int x = graphMargin + (i * graphWidth) / 10;
        cv::line(image, cv::Point(x, yOffset), 
                cv::Point(x, yOffset + graphHeight), gridColor, 1);
    }
}

void DataVisualizer::drawLegend(cv::Mat& image) {
    int legendX = windowWidth - 150;
    int legendY = windowHeight - 80;
    
    cv::putText(image, "Press 'q' to quit", cv::Point(legendX, legendY), 
               cv::FONT_HERSHEY_SIMPLEX, 0.5, textColor, 1);
}

bool DataVisualizer::shouldClose() {
    char key = cv::waitKey(1) & 0xFF;
    return (key == 'q' || key == 27); // 'q' or ESC
}

std::vector<double> DataVisualizer::extractTemperatureData() const {
    std::vector<double> data;
    for (const auto& reading : dataBuffer) {
        data.push_back(reading.temperature);
    }
    return data;
}

std::vector<double> DataVisualizer::extractWindSpeedData() const {
    std::vector<double> data;
    for (const auto& reading : dataBuffer) {
        data.push_back(reading.windSpeed);
    }
    return data;
}

std::vector<double> DataVisualizer::extractHumidityData() const {
    std::vector<double> data;
    for (const auto& reading : dataBuffer) {
        data.push_back(reading.humidity);
    }
    return data;
}
