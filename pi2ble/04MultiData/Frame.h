#ifndef FRAME_H
#define FRAME_H

#include <opencv2/opencv.hpp>

class Frame {
private:
    std::string windowName = "Please set the window name";
    int windowWidth = 320;
    int windowHeight = 240;
    std::string count;
    std::string velocity = "0";
public:
    Frame(const std::string& winName, int width, int height);
    void addDataPoint(const std::string& data);
    void setVelocity(const std::string& velocity);
    void render();
    char waitKey(int ms);
private:
    void drawData();

};

#endif // FRAME_H
