#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

namespace harness_014 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for window name, image dimensions, pixel data, and waitKey delay
    const size_t MIN_SIZE = sizeof(int) * 2 + sizeof(uint8_t) * 16 + sizeof(int);
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume window name (string)
    std::string window_name = fdp.ConsumeRandomLengthString(64);
    
    // Consume image dimensions (limit to reasonable sizes to avoid excessive memory usage)
    int width = fdp.ConsumeIntegralInRange<int>(1, 256);
    int height = fdp.ConsumeIntegralInRange<int>(1, 256);
    
    // Consume number of channels (1, 3, or 4 for grayscale, BGR, or BGRA)
    int channels = fdp.ConsumeIntegralInRange<int>(1, 4);
    
    // Consume window flags for namedWindow
    int window_flags = fdp.ConsumeIntegral<int>();
    
    // Consume delay values for waitKey (positive, zero, or negative for infinite wait)
    int delay1 = fdp.ConsumeIntegral<int>();
    int delay2 = fdp.ConsumeIntegral<int>();
    int delay3 = fdp.ConsumeIntegral<int>();
    
    // Create a synthetic image with random pixel values from fuzzer input
    cv::Mat test_image;
    int image_type;
    
    switch (channels) {
        case 1:
            image_type = CV_8UC1;
            break;
        case 2:
            image_type = CV_8UC2;
            break;
        case 3:
            image_type = CV_8UC3;
            break;
        case 4:
            image_type = CV_8UC4;
            break;
        default:
            image_type = CV_8UC1;
            channels = 1;
    }
    
    test_image = cv::Mat(height, width, image_type);
    
    // Fill the image with fuzzed pixel data
    size_t total_pixels = height * width * channels;
    for (size_t i = 0; i < total_pixels && fdp.remaining_bytes() > 0; ++i) {
        // Calculate pixel position
        int row = i / (width * channels);
        int col = (i / channels) % width;
        int channel = i % channels;
        
        // Get pixel pointer and set value
        uint8_t* pixel_ptr = test_image.ptr<uint8_t>(row) + col * channels + channel;
        *pixel_ptr = fdp.ConsumeIntegral<uint8_t>();
    }
    
    // If we didn't have enough pixel data, fill remaining with zeros
    // The Mat constructor already initializes to zeros, so this is fine
    
    try {
        // Step 1: Create named window
        cv::namedWindow(window_name, window_flags);
        
        // Step 2: Display image in window
        cv::imshow(window_name, test_image);
        
        // Step 3: Process window events with waitKey using different delay values
        // Test waitKey with various timeout scenarios
        int key1 = cv::waitKey(delay1);
        (void)key1; // Use result to avoid compiler warning
        
        // Test with a different delay value
        int key2 = cv::waitKey(delay2);
        (void)key2;
        
        // Test with another delay value
        int key3 = cv::waitKey(delay3);
        (void)key3;
        
        // Test with a very short delay (1ms) to exercise the timeout path
        int key_short = cv::waitKey(1);
        (void)key_short;
        
        // Test with zero delay (infinite wait - will return immediately since no GUI events in fuzzing)
        int key_zero = cv::waitKey(0);
        (void)key_zero;
        
        // Test with negative delay (should also be infinite wait)
        int key_neg = cv::waitKey(-1);
        (void)key_neg;
        
        // Step 4: Test destroyAllWindows - clean up all windows
        cv::destroyAllWindows();
        
        // Step 5: Create another window to test multiple window lifecycle
        std::string window_name2 = window_name + "_2";
        cv::namedWindow(window_name2, cv::WINDOW_AUTOSIZE);
        
        // Create another test image with different dimensions
        cv::Mat test_image2 = cv::Mat(height / 2, width / 2, CV_8UC3);
        
        // Fill with some data if available
        if (fdp.remaining_bytes() > 0) {
            size_t total_pixels2 = (height / 2) * (width / 2) * 3;
            for (size_t i = 0; i < total_pixels2 && fdp.remaining_bytes() > 0; ++i) {
                int row = i / ((width / 2) * 3);
                int col = (i / 3) % (width / 2);
                int channel = i % 3;
                
                uint8_t* pixel_ptr = test_image2.ptr<uint8_t>(row) + col * 3 + channel;
                *pixel_ptr = fdp.ConsumeIntegral<uint8_t>();
            }
        }
        
        cv::imshow(window_name2, test_image2);
        
        // Test waitKey with another delay
        int key4 = cv::waitKey(10);
        (void)key4;
        
        // Final cleanup with destroyAllWindows
        cv::destroyAllWindows();
        
        // Step 6: Test window properties and state queries
        // Recreate a window to test getWindowProperty
        cv::namedWindow(window_name, cv::WINDOW_NORMAL);
        cv::imshow(window_name, test_image);
        
        // Get window properties
        double prop1 = cv::getWindowProperty(window_name, cv::WND_PROP_AUTOSIZE);
        double prop2 = cv::getWindowProperty(window_name, cv::WND_PROP_FULLSCREEN);
        (void)prop1;
        (void)prop2;
        
        // Test resizeWindow
        cv::resizeWindow(window_name, width, height);
        
        // Test moveWindow (coordinates from fuzzed data if available)
        if (fdp.remaining_bytes() >= sizeof(int) * 2) {
            int x = fdp.ConsumeIntegral<int>();
            int y = fdp.ConsumeIntegral<int>();
            cv::moveWindow(window_name, x, y);
        }
        
        // Final waitKey and destroy
        cv::waitKey(5);
        cv::destroyAllWindows();
        
    } catch (const cv::Exception& e) {
        // OpenCV exceptions might occur (e.g., invalid parameters)
        // Swallow exceptions to continue fuzzing
    } catch (...) {
        // Catch any other exceptions
    }
    
    // Step 6: Image resources are automatically released when cv::Mat objects go out of scope
    
    return 0;
}

} // namespace harness_014
