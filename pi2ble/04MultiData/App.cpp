#include "Config.h"
#include "Frame.h"
#include <chrono>

class App {
    private:
        bool running;
        VideoConfig videoConfig;
    public:
        App() : running(true) {
            videoConfig.frameRate = 6;
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

