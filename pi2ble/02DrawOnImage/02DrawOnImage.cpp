#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

int main() {
    std::cout << "Starting..." << std::endl;
    // Start timing the entire program
    auto program_start = std::chrono::high_resolution_clock::now();
    
    // Time OpenCV library initialization (first OpenCV call)
    std::cout << "About to make first OpenCV call..." << std::endl;
    auto opencv_init_start = std::chrono::high_resolution_clock::now();
    
    // Load the image
    auto load_start = std::chrono::high_resolution_clock::now();
    cv::Mat image = cv::imread("image02.png");
    auto load_end = std::chrono::high_resolution_clock::now();
    auto load_duration = std::chrono::duration_cast<std::chrono::milliseconds>(load_end - load_start);
    auto opencv_init_duration = std::chrono::duration_cast<std::chrono::milliseconds>(load_end - opencv_init_start);
    std::cout << "OpenCV initialization + image loading took: " << opencv_init_duration.count() << " milliseconds" << std::endl;
    std::cout << "Image loading alone took: " << load_duration.count() << " milliseconds" << std::endl;
    
    // Check if image is loaded successfully
    if (image.empty()) {
        std::cout << "Image 'image02.png' not found. Creating a sample image..." << std::endl;
        
        // Time image creation
        auto create_start = std::chrono::high_resolution_clock::now();
        
        // Create a sample image (800x600, light blue background)
        image = cv::Mat(600, 800, CV_8UC3, cv::Scalar(230, 216, 173)); // Light blue in BGR
        
        // Add some sample content to the image
        cv::putText(image, "Sample Image for OpenCV Demo", cv::Point(150, 300), 
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(50, 50, 50), 2);
        
        // Save the sample image
        cv::imwrite("image01.png", image);
        
        auto create_end = std::chrono::high_resolution_clock::now();
        auto create_duration = std::chrono::duration_cast<std::chrono::milliseconds>(create_end - create_start);
        std::cout << "Image creation and saving took: " << create_duration.count() << " milliseconds" << std::endl;
        std::cout << "Created and saved 'image01.png'" << std::endl;
    }
    
    std::cout << "Image loaded successfully!" << std::endl;
    std::cout << "Image size: " << image.cols << "x" << image.rows << std::endl;
    
    // Time the image cloning operation
    auto clone_start = std::chrono::high_resolution_clock::now();
    // Create a copy to work on (preserve original)
    cv::Mat drawingImage = image.clone();
    auto clone_end = std::chrono::high_resolution_clock::now();
    auto clone_duration = std::chrono::duration_cast<std::chrono::milliseconds>(clone_end - clone_start);
    std::cout << "Image cloning took: " << clone_duration.count() << " milliseconds" << std::endl;
    
    // Start timing the OpenCV drawing operations
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Draw some lines
    // Diagonal line from top-left to bottom-right (red)
    cv::line(drawingImage, cv::Point(0, 0), cv::Point(drawingImage.cols, drawingImage.rows), 
             cv::Scalar(0, 0, 255), 3);
    
    // Diagonal line from top-right to bottom-left (green)
    cv::line(drawingImage, cv::Point(drawingImage.cols, 0), cv::Point(0, drawingImage.rows), 
             cv::Scalar(0, 255, 0), 3);
    
    // Horizontal line in the middle (blue)
    cv::line(drawingImage, cv::Point(0, drawingImage.rows/2), 
             cv::Point(drawingImage.cols, drawingImage.rows/2), cv::Scalar(255, 0, 0), 2);
    
    // Vertical line in the middle (yellow)
    cv::line(drawingImage, cv::Point(drawingImage.cols/2, 0), 
             cv::Point(drawingImage.cols/2, drawingImage.rows), cv::Scalar(0, 255, 255), 2);
    
    // Draw some additional shapes for visual interest
    // Circle in the center (magenta)
    cv::circle(drawingImage, cv::Point(drawingImage.cols/2, drawingImage.rows/2), 
               50, cv::Scalar(255, 0, 255), 3);
    
    // Rectangle in top-left corner (cyan)
    cv::rectangle(drawingImage, cv::Point(20, 20), cv::Point(150, 80), 
                  cv::Scalar(255, 255, 0), 2);
    
    // Add text
    std::string text1 = "OpenCV Drawing Demo";
    std::string text2 = "Lines and Shapes";
    std::string text3 = "Press any key to exit";
    
    // Main title (white text with black outline for visibility)
    cv::putText(drawingImage, text1, cv::Point(30, 50), 
                cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0, 0, 0), 4);  // Black outline
    cv::putText(drawingImage, text1, cv::Point(30, 50), 
                cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(255, 255, 255), 2);  // White text
    
    // Subtitle
    cv::putText(drawingImage, text2, cv::Point(30, 100), 
                cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0), 2);
    
    // Instructions at the bottom
    cv::putText(drawingImage, text3, cv::Point(30, drawingImage.rows - 30), 
                cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 255), 2);
    
    // End timing and calculate duration
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    std::cout << "OpenCV drawing operations completed in: " << duration.count() << " milliseconds" << std::endl;
    
    // Check timing before GUI operations
    auto pre_gui_time = std::chrono::high_resolution_clock::now();
    auto pre_gui_duration = std::chrono::duration_cast<std::chrono::milliseconds>(pre_gui_time - program_start);
    std::cout << "Time elapsed before GUI operations: " << pre_gui_duration.count() << " milliseconds" << std::endl;
    
    // Display the original and modified images
    std::cout << "About to create GUI windows..." << std::endl;
    auto display_start = std::chrono::high_resolution_clock::now();
    cv::imshow("Original Image", image);
    cv::imshow("Image with Drawings", drawingImage);
    auto display_end = std::chrono::high_resolution_clock::now();
    auto display_duration = std::chrono::duration_cast<std::chrono::milliseconds>(display_end - display_start);
    std::cout << "Image display setup took: " << display_duration.count() << " milliseconds" << std::endl;
    
    std::cout << "Images displayed. Press any key in the image window to continue..." << std::endl;
    
    // Time the waitKey operation (this is where GUI actually appears)
    auto waitkey_start = std::chrono::high_resolution_clock::now();
    // Wait for a key press
    cv::waitKey(0);
    auto waitkey_end = std::chrono::high_resolution_clock::now();
    auto waitkey_duration = std::chrono::duration_cast<std::chrono::milliseconds>(waitkey_end - waitkey_start);
    std::cout << "waitKey (user interaction) took: " << waitkey_duration.count() << " milliseconds" << std::endl;
    
    // Optional: Save the modified image
    auto save_start = std::chrono::high_resolution_clock::now();
    std::string outputFilename = "image01_with_drawings.png";
    cv::imwrite(outputFilename, drawingImage);
    auto save_end = std::chrono::high_resolution_clock::now();
    auto save_duration = std::chrono::duration_cast<std::chrono::milliseconds>(save_end - save_start);
    std::cout << "Image saving took: " << save_duration.count() << " milliseconds" << std::endl;
    std::cout << "Modified image saved as: " << outputFilename << std::endl;
    
    // Clean up
    cv::destroyAllWindows();
    
    // Calculate total program time
    auto program_end = std::chrono::high_resolution_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(program_end - program_start);
    std::cout << "\n=== TIMING SUMMARY ===" << std::endl;
    std::cout << "Total program execution time: " << total_duration.count() << " milliseconds" << std::endl;
    
    std::cout << "Program completed successfully!" << std::endl;
    return 0;
}