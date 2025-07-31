#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <thread>

class VideoStreamer {
private:
    cv::VideoCapture cap;
    cv::Mat frame;
    std::string windowName;
    int totalFrames;
    int currentFrame;
    double fps;
    
public:
    VideoStreamer(const std::string& videoPath, const std::string& winName = "Video Stream") 
        : windowName(winName), currentFrame(0) {
        
        // Try to open video file first, if that fails, try camera
        if (!videoPath.empty() && videoPath != "0") {
            cap.open(videoPath);
            if (!cap.isOpened()) {
                std::cout << "Could not open video file: " << videoPath << std::endl;
                std::cout << "Trying to open default camera..." << std::endl;
                cap.open(0);
            }
        } else {
            cap.open(0);  // Open default camera
            // downsize video to 320p
            cap.set(cv::CAP_PROP_FRAME_WIDTH, 480);
            cap.set(cv::CAP_PROP_FRAME_HEIGHT, 320);
        }
        
        if (!cap.isOpened()) {
            throw std::runtime_error("Could not open video source!");
        }
        
        // Get video properties
        totalFrames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
        fps = cap.get(cv::CAP_PROP_FPS);
        
        std::cout << "Video opened successfully!" << std::endl;
        std::cout << "Total frames: " << totalFrames << std::endl;
        std::cout << "FPS: " << fps << std::endl;
        
        // Create window
        cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
    }
    
    ~VideoStreamer() {
        cap.release();
        cv::destroyAllWindows();
    }
    
    void drawProgressBar(cv::Mat& image, double progress) {
        int barWidth = 400;
        int barHeight = 30;
        int barX = (image.cols - barWidth) / 2;
        int barY = image.rows - 60;
        
        // Ensure progress is between 0 and 1
        progress = std::max(0.0, std::min(1.0, progress));
        
        // Draw background rectangle (gray)
        cv::rectangle(image, 
                     cv::Point(barX, barY), 
                     cv::Point(barX + barWidth, barY + barHeight),
                     cv::Scalar(100, 100, 100), 
                     -1);
        
        // Draw progress rectangle (green)
        int progressWidth = static_cast<int>(barWidth * progress);
        if (progressWidth > 0) {
            cv::rectangle(image, 
                         cv::Point(barX, barY), 
                         cv::Point(barX + progressWidth, barY + barHeight),
                         cv::Scalar(0, 255, 0), 
                         -1);
        }
        
        // Draw border
        cv::rectangle(image, 
                     cv::Point(barX, barY), 
                     cv::Point(barX + barWidth, barY + barHeight),
                     cv::Scalar(255, 255, 255), 
                     2);
        
        // Add percentage text
        std::string progressText = std::to_string(static_cast<int>(progress * 100)) + "%";
        int baseline = 0;
        cv::Size textSize = cv::getTextSize(progressText, cv::FONT_HERSHEY_SIMPLEX, 0.8, 2, &baseline);
        cv::Point textOrg(barX + (barWidth - textSize.width) / 2, barY + (barHeight + textSize.height) / 2);
        
        cv::putText(image, progressText, textOrg, cv::FONT_HERSHEY_SIMPLEX, 0.8, 
                   cv::Scalar(255, 255, 255), 2);
        
        // Add frame info
        std::string frameInfo = "Frame: " + std::to_string(currentFrame) + "/" + std::to_string(totalFrames);
        cv::putText(image, frameInfo, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, 
                   cv::Scalar(255, 255, 255), 2);
    }
    
    void stream() {
        auto startTime = std::chrono::high_resolution_clock::now();
        
        while (true) {
            cap >> frame;
            
            if (frame.empty()) {
                if (totalFrames > 0) {
                    // Video ended, restart from beginning
                    cap.set(cv::CAP_PROP_POS_FRAMES, 0);
                    currentFrame = 0;
                    continue;
                } else {
                    // Camera disconnected
                    std::cout << "No frame captured from camera" << std::endl;
                    break;
                }
            }
            
            currentFrame++;
            
            // Calculate progress
            double progress = 0.0;
            if (totalFrames > 0) {
                // For video files
                progress = static_cast<double>(currentFrame) / totalFrames;
            } else {
                // For camera, create a cyclic progress bar
                auto currentTime = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
                progress = (elapsed % 5000) / 5000.0;  // 5-second cycle
            }
            
            // Draw progress bar on frame
            drawProgressBar(frame, progress);
            
            // Display frame
            cv::imshow(windowName, frame);
            
            // Check for exit condition
            char key = cv::waitKey(1) & 0xFF;
            if (key == 'q' || key == 27) {  // 'q' or ESC key
                break;
            }
            
            // Control frame rate for camera
            if (totalFrames == 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(33));  // ~30 FPS
            }
        }
    }
};

int main(int argc, char* argv[]) {
    try {
        std::string videoPath;
        
        if (argc > 1) {
            videoPath = argv[1];
            std::cout << "Using video file: " << videoPath << std::endl;
        } else {
            std::cout << "No video file specified. Using default camera." << std::endl;
            std::cout << "Usage: " << argv[0] << " [video_file_path]" << std::endl;
            videoPath = "0";  // Use camera
        }
        
        VideoStreamer streamer(videoPath);
        
        std::cout << "Starting video stream..." << std::endl;
        std::cout << "Press 'q' or ESC to quit" << std::endl;
        
        streamer.stream();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}