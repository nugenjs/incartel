#include "Frame.h"



// 720×480 
// 480×360
// 320×240

Frame::Frame(const std::string& winName, int width, int height)
    : windowName(winName), windowWidth(width), windowHeight(height)  {
}

void Frame::addDataPoint(const std::string& data) {
    count = data;
    // Implementation for adding a data point
}

void Frame::render() {
    // Implementation for rendering the frame
    // cv::Mat image = cv::Mat(windowHeight, windowWidth, CV_8UC3, cv::Scalar(230, 216, 173)); // Light blue in BGR
    cv::Mat image = cv::Mat(windowHeight, windowWidth, CV_8UC3, cv::Scalar(255, 255, 255)); // Light blue in BGR

    // cv::putText(image, "Sample Frame for OpenCV Demo", cv::Point(150, 300), 
    //             cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(50, 50, 50), 2);
    // cv::putText(image, "hi: " + count, cv::Point(150, 300), 
    //             cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(50, 50, 50), 2);
    cv::putText(image, "hi: " + count, cv::Point(windowWidth / 5, windowHeight / 2), 
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(50, 50, 50), 2);
    // Draw Velocity
    cv::putText(image, "velo: " + velocity, cv::Point(windowWidth / 2 - 10, windowHeight - 20),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(50,50,50), 1);
    cv::imshow(windowName, image);
    // cv::waitKey(0); // Wait for a key press to close the window
    // cv::destroyWindow(windowName);

}

void Frame::setVelocity(const std::string& velocity) {
    this->velocity = velocity;
}

void Frame::drawData() {
    // Implementation for drawing the data
}

char Frame::waitKey(int ms) {
    return cv::waitKey(ms) & 0xFF;
}
