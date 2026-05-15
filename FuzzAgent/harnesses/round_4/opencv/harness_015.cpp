#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

namespace harness_015 {
// OpenCV highgui comprehensive window management fuzzing harness
// Targets: destroyAllWindows, namedWindow, imshow, waitKey, destroyWindow, resizeWindow, moveWindow (0% coverage)
// harness_015.cpp - Focus on surface coverage exploration for OpenCV's GUI module
// Differentiates from existing harnesses by targeting complete window lifecycle management:
// 1. Window creation with various flags and names
// 2. Window manipulation (resize, move)
// 3. Image display with imshow using fuzzer-decoded images
// 4. Window interaction (waitKey)
// 5. Window destruction (individual and all windows)


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic window operations
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    try {
        using namespace cv;
        
        // Part 1: Create test image from fuzzer input for imshow operations
        // First, try to decode an image from part of the input
        size_t max_size = std::min(size/4, size_t(65536));
        if (max_size < 32) {
            return 0;  // Insufficient data for meaningful image testing
        }
        size_t image_data_size = fdp.ConsumeIntegralInRange<size_t>(32, max_size);
        std::vector<uint8_t> image_buffer = fdp.ConsumeBytes<uint8_t>(image_data_size);
        
        Mat display_image;
        bool has_valid_image = false;
        
        if (!image_buffer.empty()) {
            // Try to decode image
            Mat decoded_image = imdecode(image_buffer, IMREAD_UNCHANGED);
            
            if (!decoded_image.empty()) {
                display_image = decoded_image;
                has_valid_image = true;
            } else {
                // Create a synthetic image if decoding fails
                int width = fdp.ConsumeIntegralInRange<int>(10, 400);
                int height = fdp.ConsumeIntegralInRange<int>(10, 400);
                int channels = fdp.ConsumeIntegralInRange<int>(1, 4);
                
                int mat_type;
                switch (channels % 4) {
                    case 0: mat_type = CV_8UC1; break;
                    case 1: mat_type = CV_8UC2; break;
                    case 2: mat_type = CV_8UC3; break;
                    case 3: mat_type = CV_8UC4; break;
                }
                
                display_image = Mat(height, width, mat_type);
                
                // Fill with some pattern
                if (channels == 1) {
                    display_image.setTo(Scalar(fdp.ConsumeIntegral<uint8_t>()));
                } else if (channels == 3) {
                    display_image.setTo(Scalar(
                        fdp.ConsumeIntegral<uint8_t>(),
                        fdp.ConsumeIntegral<uint8_t>(),
                        fdp.ConsumeIntegral<uint8_t>()
                    ));
                } else if (channels == 4) {
                    display_image.setTo(Scalar(
                        fdp.ConsumeIntegral<uint8_t>(),
                        fdp.ConsumeIntegral<uint8_t>(),
                        fdp.ConsumeIntegral<uint8_t>(),
                        fdp.ConsumeIntegral<uint8_t>()
                    ));
                }
                
                has_valid_image = true;
            }
        }
        
        // Part 2: Determine window creation parameters
        int num_windows = fdp.ConsumeIntegralInRange<int>(1, 5);
        std::vector<std::string> window_names;
        std::vector<int> window_flags;
        
        // Generate window names from fuzzer input
        for (int i = 0; i < num_windows && fdp.remaining_bytes() > 0; i++) {
            // Create window name
            size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 32);
            std::string window_name = fdp.ConsumeBytesAsString(name_len);
            
            if (window_name.empty()) {
                window_name = "Window_" + std::to_string(i);
            }
            
            // Determine window flags from fuzzer input
            int flags = 0;
            int flag_choice = fdp.ConsumeIntegralInRange<int>(0, 7);
            
            switch (flag_choice % 8) {
                case 0:
                    flags = WINDOW_AUTOSIZE | WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED;
                    break;
                case 1:
                    flags = WINDOW_NORMAL | WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED;
                    break;
                case 2:
                    flags = WINDOW_AUTOSIZE | WINDOW_FREERATIO | WINDOW_GUI_EXPANDED;
                    break;
                case 3:
                    flags = WINDOW_NORMAL | WINDOW_FREERATIO | WINDOW_GUI_EXPANDED;
                    break;
                case 4:
                    flags = WINDOW_AUTOSIZE | WINDOW_KEEPRATIO | WINDOW_GUI_NORMAL;
                    break;
                case 5:
                    flags = WINDOW_NORMAL | WINDOW_KEEPRATIO | WINDOW_GUI_NORMAL;
                    break;
                case 6:
                    flags = WINDOW_OPENGL | WINDOW_AUTOSIZE | WINDOW_KEEPRATIO;
                    break;
                case 7:
                    flags = WINDOW_OPENGL | WINDOW_NORMAL | WINDOW_FREERATIO;
                    break;
            }
            
            window_names.push_back(window_name);
            window_flags.push_back(flags);
        }
        
        // Part 3: Create windows
        for (size_t i = 0; i < window_names.size(); i++) {
            try {
                namedWindow(window_names[i], window_flags[i]);
            } catch (const cv::Exception& e) {
                // Ignore HighGUI errors - common in headless fuzzing environments
                // The function call itself exercises the code paths
            }
        }
        
        // Part 4: Test imshow if we have a valid image
        if (has_valid_image && !display_image.empty() && !window_names.empty()) {
            for (size_t i = 0; i < window_names.size(); i++) {
                try {
                    imshow(window_names[i], display_image);
                } catch (const cv::Exception& e) {
                    // Ignore HighGUI errors
                }
            }
        }
        
        // Part 5: Test window manipulation operations
        if (!window_names.empty() && fdp.remaining_bytes() > 0) {
            int operation = fdp.ConsumeIntegralInRange<int>(0, 3);
            size_t window_idx = fdp.ConsumeIntegralInRange<size_t>(0, window_names.size() - 1);
            
            switch (operation % 4) {
                case 0:
                    // Test resizeWindow
                    if (fdp.remaining_bytes() >= 8) {
                        int width = fdp.ConsumeIntegralInRange<int>(100, 800);
                        int height = fdp.ConsumeIntegralInRange<int>(100, 600);
                        
                        try {
                            resizeWindow(window_names[window_idx], width, height);
                        } catch (const cv::Exception& e) {
                            // Ignore resize errors (window might be autosize)
                        }
                    }
                    break;
                    
                case 1:
                    // Test moveWindow
                    if (fdp.remaining_bytes() >= 8) {
                        int x = fdp.ConsumeIntegralInRange<int>(0, 1920);
                        int y = fdp.ConsumeIntegralInRange<int>(0, 1080);
                        
                        try {
                            moveWindow(window_names[window_idx], x, y);
                        } catch (const cv::Exception& e) {
                            // Ignore move errors
                        }
                    }
                    break;
                    
                case 2:
                    // Test waitKey with variable delay
                    if (fdp.remaining_bytes() >= 4) {
                        int delay = fdp.ConsumeIntegralInRange<int>(0, 1000);
                        
                        try {
                            waitKey(delay);
                        } catch (const cv::Exception& e) {
                            // Ignore waitKey errors
                        }
                    }
                    break;
                    
                case 3:
                    // Test destroyWindow on specific window
                    try {
                        destroyWindow(window_names[window_idx]);
                        // Remove from our list since it's destroyed
                        window_names.erase(window_names.begin() + window_idx);
                        window_flags.erase(window_flags.begin() + window_idx);
                    } catch (const cv::Exception& e) {
                        // Ignore destroy errors
                    }
                    break;
            }
        }
        
        // Part 6: Test additional window operations with remaining windows
        if (!window_names.empty() && fdp.remaining_bytes() > 0) {
            int additional_op = fdp.ConsumeIntegralInRange<int>(0, 2);
            
            switch (additional_op % 3) {
                case 0:
                    // Test multiple resize operations
                    for (size_t i = 0; i < window_names.size() && fdp.remaining_bytes() >= 8; i++) {
                        int width = fdp.ConsumeIntegralInRange<int>(50, 400);
                        int height = fdp.ConsumeIntegralInRange<int>(50, 300);
                        
                        try {
                            resizeWindow(window_names[i], width, height);
                        } catch (const cv::Exception& e) {
                            // Ignore errors
                        }
                    }
                    break;
                    
                case 1:
                    // Test multiple move operations
                    for (size_t i = 0; i < window_names.size() && fdp.remaining_bytes() >= 8; i++) {
                        int x = fdp.ConsumeIntegralInRange<int>(0, 500);
                        int y = fdp.ConsumeIntegralInRange<int>(0, 500);
                        
                        try {
                            moveWindow(window_names[i], x, y);
                        } catch (const cv::Exception& e) {
                            // Ignore errors
                        }
                    }
                    break;
                    
                case 2:
                    // Test imshow with different images
                    if (fdp.remaining_bytes() > 0 && has_valid_image) {
                        // Create a modified version of the display image
                        Mat modified_image;
                        if (display_image.channels() == 3) {
                            cvtColor(display_image, modified_image, cv::COLOR_BGR2GRAY);
                        } else if (display_image.channels() == 1) {
                            cvtColor(display_image, modified_image, cv::COLOR_GRAY2BGR);
                        } else {
                            modified_image = display_image.clone();
                        }
                        
                        for (size_t i = 0; i < window_names.size(); i++) {
                            try {
                                imshow(window_names[i], modified_image);
                            } catch (const cv::Exception& e) {
                                // Ignore errors
                            }
                        }
                    }
                    break;
            }
        }
        
        // Part 7: Test waitKey with different delays
        if (fdp.remaining_bytes() >= 8) {
            int num_wait_calls = fdp.ConsumeIntegralInRange<int>(1, 5);
            
            for (int i = 0; i < num_wait_calls && fdp.remaining_bytes() >= 4; i++) {
                int delay = fdp.ConsumeIntegralInRange<int>(0, 100);
                
                try {
                    waitKey(delay);
                } catch (const cv::Exception& e) {
                    // Ignore errors
                }
            }
        }
        
        // Part 8: Cleanup - destroy remaining windows
        if (!window_names.empty()) {
            // Test destroyWindow on each remaining window
            for (size_t i = 0; i < window_names.size(); i++) {
                try {
                    destroyWindow(window_names[i]);
                } catch (const cv::Exception& e) {
                    // Ignore errors
                }
            }
            
            // Clear our lists
            window_names.clear();
            window_flags.clear();
        }
        
        // Part 9: Final test of destroyAllWindows (should be safe even if no windows exist)
        try {
            destroyAllWindows();
        } catch (const cv::Exception& e) {
            // Ignore errors
        }
        
        // Part 10: Test edge cases - create and immediately destroy windows
        if (fdp.remaining_bytes() > 0) {
            int edge_case_ops = fdp.ConsumeIntegralInRange<int>(0, 3);
            
            switch (edge_case_ops % 4) {
                case 0:
                    // Create window with empty name
                    try {
                        namedWindow("", WINDOW_AUTOSIZE);
                        imshow("", display_image);
                        waitKey(1);
                        destroyWindow("");
                    } catch (const cv::Exception& e) {
                        // Ignore errors
                    }
                    break;
                    
                case 1:
                    // Test with very long window name
                    if (fdp.remaining_bytes() > 0) {
                        std::string long_name = fdp.ConsumeRemainingBytesAsString();
                        if (long_name.length() > 255) {
                            long_name = long_name.substr(0, 255);
                        }
                        
                        try {
                            namedWindow(long_name, WINDOW_NORMAL);
                            if (has_valid_image) {
                                imshow(long_name, display_image);
                            }
                            waitKey(1);
                            destroyWindow(long_name);
                        } catch (const cv::Exception& e) {
                            // Ignore errors
                        }
                    }
                    break;
                    
                case 2:
                    // Test destroyAllWindows multiple times
                    try {
                        destroyAllWindows();
                        destroyAllWindows();  // Should be safe to call multiple times
                        destroyAllWindows();
                    } catch (const cv::Exception& e) {
                        // Ignore errors
                    }
                    break;
                    
                case 3:
                    // Test waitKey with negative delay (should wait indefinitely)
                    try {
                        // Use a very small timeout instead of negative for safety
                        waitKey(0);
                    } catch (const cv::Exception& e) {
                        // Ignore errors
                    }
                    break;
            }
        }
        
    } catch (const cv::Exception& e) {
        // Catch OpenCV exceptions and continue
    } catch (...) {
        // Catch any other exceptions
    }
    
    return 0;
}

} // namespace harness_015
