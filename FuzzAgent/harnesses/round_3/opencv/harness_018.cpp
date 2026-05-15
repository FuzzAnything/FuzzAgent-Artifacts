#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

namespace harness_018 {

#ifdef HAVE_OPENCV_IMGCODECS
#endif

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for window name and basic parameters
    const size_t MIN_SIZE = sizeof(int) + 16;  // flags + minimum window name length
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume window name from fuzzer input
    std::string window_name = fdp.ConsumeRandomLengthString(256);
    if (window_name.empty()) {
        window_name = "FuzzerWindow";  // Default name if no string consumed
    }
    
    // Consume window flags
    // OpenCV window flags: WINDOW_NORMAL=0x00000000, WINDOW_AUTOSIZE=0x00000001, 
    // WINDOW_OPENGL=0x00001000, WINDOW_FULLSCREEN=0x00000001, WINDOW_FREERATIO=0x00000100,
    // WINDOW_KEEPRATIO=0x00000000, WINDOW_GUI_EXPANDED=0x00000000, WINDOW_GUI_NORMAL=0x00000010
    int window_flags = fdp.ConsumeIntegral<int>();
    
    // Consume waitKey delay (0 means wait forever, positive means milliseconds)
    int delay = fdp.ConsumeIntegral<int>();
    
    // Consume whether to test imshow functionality
    bool test_imshow = fdp.ConsumeBool();
    
    try {
        // Step 1: Create a window using namedWindow
        cv::namedWindow(window_name, window_flags);
        
        // Step 2: Optionally create and display an image using imshow if available
        if (test_imshow && fdp.remaining_bytes() >= 100) {
            // Create a simple synthetic image from fuzzer data
            // Use small dimensions for fuzzing performance
            int img_width = fdp.ConsumeIntegralInRange<int>(16, 64);
            int img_height = fdp.ConsumeIntegralInRange<int>(16, 64);
            
            // Check if we have enough data for the image
            size_t min_img_data = img_width * img_height * 3;  // 3 channels for CV_8UC3
            if (fdp.remaining_bytes() >= min_img_data) {
                // Create image matrix
                cv::Mat img(img_height, img_width, CV_8UC3);
                
                // Fill image with pattern based on fuzzer data
                for (int y = 0; y < img_height; y++) {
                    for (int x = 0; x < img_width; x++) {
                        // Create pixel from fuzzer data
                        uint8_t b = fdp.ConsumeIntegral<uint8_t>();
                        uint8_t g = fdp.ConsumeIntegral<uint8_t>();
                        uint8_t r = fdp.ConsumeIntegral<uint8_t>();
                        
                        cv::Vec3b pixel(b, g, r);
                        img.at<cv::Vec3b>(y, x) = pixel;
                    }
                }
                
                // Display the image
                cv::imshow(window_name, img);
            } else {
                // Create a default small colored rectangle if not enough data
                cv::Mat img = cv::Mat::zeros(32, 32, CV_8UC3);
                cv::rectangle(img, cv::Rect(5, 5, 20, 20), cv::Scalar(255, 0, 0), -1);  // Blue rectangle
                cv::imshow(window_name, img);
            }
        }
        
        // Step 3: Call waitKey to wait for user input (with fuzzer-controlled delay)
        // Note: In fuzzing context, we use a very short delay or 0
        // Using absolute value to ensure non-negative delay
        int abs_delay = delay < 0 ? -delay : delay;
        // Limit delay to reasonable range for fuzzing performance
        if (abs_delay > 1000) {
            abs_delay = abs_delay % 1000;
        }
        
        int key_pressed = cv::waitKey(abs_delay);
        // key_pressed result is ignored in fuzzing context
        
        // Step 4: Clean up - destroy all windows
        cv::destroyAllWindows();
        
    } catch (const cv::Exception& e) {
        // Catch OpenCV exceptions to prevent crashes from invalid parameters
        // Silently ignore - the fuzzer will explore different parameter combinations
    } catch (...) {
        // Catch any other exceptions
    }
    
    return 0;
}

} // namespace harness_018
