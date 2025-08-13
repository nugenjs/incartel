#include <chrono>
#include <iostream>
#include "FrameConfig.h"

struct VideoConfig {
    int width = 320;
    int height = 240;
    int frameRate = 12;
};

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



class MockDataCVFrameOut {
    private:
        bool running;
        VideoConfig videoConfig;
        ImageConfig imageConfig;
        TelemetryDrawConfig telemetryDrawConfig;
    public:
        MockDataCVFrameOut() : running(true) {
            videoConfig.frameRate = 6;
            videoConfig.width = imageConfig.width;
            videoConfig.height = imageConfig.height;
            telemetryDrawConfig.dataDrawProps = {
                {"velocity", DatumDrawProps{100, 200, {255, 0, 0}, "Velocity"}},
                {"rpm", DatumDrawProps{100, 200, {255, 0, 0}, "RPM"}}
            };

            auto& velocityProps = telemetryDrawConfig.dataDrawProps["velocity"];
            // Example: print the label of the velocity property
            std::cout << "Velocity label: " << velocityProps.displayVal << std::endl;
            // videoConfig.frameRate = 1;
            // videoConfig.width = 120;
            // videoConfig.height = 60;
        }
        void run() {
            Frame frame("Sample Frame", videoConfig.width, videoConfig.height);
            int count = 0;
            int frameCount = 0;
            int msBetweenFrames = videoConfig.frameRate > 0 ? 1000 / videoConfig.frameRate : 0;

            std::cout << "Frame rate: " << videoConfig.frameRate << " FPS" << std::endl;
            std::cout << "Milliseconds between frames: " << msBetweenFrames << " ms" << std::endl;

            auto lastFrame = std::chrono::steady_clock::now();


            while(running) {
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFrame);

                if (count == 0 || elapsed.count() >= msBetweenFrames) {
                    frame.addDataPoint("Data Point " + std::to_string(frameCount) + "_" + std::to_string(count));
                    frame.render();

                    std::cout << "frameCount: " + std::to_string(frameCount) << std::endl;
                    std::cout << "count: " + std::to_string(count) << std::endl;
    
                    char charRead = frame.waitKey(1);
                    if(charRead == 'q' || charRead == 27) {
                        running = false;
                    }
    
                    frameCount++;
                    lastFrame = now;
                }
                count++;
            }

            std::cout << "App finished" << std::endl;
            return;
        }
};

