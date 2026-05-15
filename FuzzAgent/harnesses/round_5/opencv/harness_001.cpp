#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <cstddef>
#include <cstdint>

namespace harness_001 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) return 0; // Minimum size for meaningful testing
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume parameters for matrix dimensions and type
    int rows = fdp.ConsumeIntegralInRange(1, 100);
    int cols = fdp.ConsumeIntegralInRange(1, 100);
    int mat_type_choice = fdp.ConsumeIntegralInRange(0, 1); // 0 for float, 1 for double
    int mat_type = (mat_type_choice == 0) ? CV_32F : CV_64F;
    
    // Create random matrices for testing
    cv::Mat x(rows, cols, mat_type);
    cv::Mat y(rows, cols, mat_type);
    
    // Fill matrices with random data from fuzzer input
    // We'll fill with random values using loops
    if (mat_type == CV_32F) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                x.at<float>(i, j) = fdp.ConsumeFloatingPoint<float>();
                y.at<float>(i, j) = fdp.ConsumeFloatingPoint<float>();
            }
        }
    } else { // CV_64F
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                x.at<double>(i, j) = fdp.ConsumeFloatingPoint<double>();
                y.at<double>(i, j) = fdp.ConsumeFloatingPoint<double>();
            }
        }
    }
    
    // Output matrices
    cv::Mat magnitude, angle;
    bool angle_in_degrees = fdp.ConsumeBool();
    
    try {
        // Test cartToPolar - convert cartesian to polar coordinates
        cv::cartToPolar(x, y, magnitude, angle, angle_in_degrees);
        
        // Test magnitude function
        cv::Mat magnitude2;
        cv::magnitude(x, y, magnitude2);
        
        // Test phase function
        cv::Mat phase_result;
        cv::phase(x, y, phase_result, angle_in_degrees);
        
        // Test polarToCart - convert polar back to cartesian
        cv::Mat x_recon, y_recon;
        cv::polarToCart(magnitude, angle, x_recon, y_recon, angle_in_degrees);
        
        // Test split and merge operations on multi-channel matrices
        // Create a multi-channel matrix for split testing
        int channels = fdp.ConsumeIntegralInRange(1, 4);
        cv::Mat multi_channel(rows, cols, CV_MAKETYPE(mat_type, channels));
        
        // Fill multi-channel matrix with random data
        if (mat_type == CV_32F) {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    for (int c = 0; c < channels; ++c) {
                        multi_channel.at<cv::Vec<float, 1>>(i, j)[c] = fdp.ConsumeFloatingPoint<float>();
                    }
                }
            }
        } else { // CV_64F
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    for (int c = 0; c < channels; ++c) {
                        multi_channel.at<cv::Vec<double, 1>>(i, j)[c] = fdp.ConsumeFloatingPoint<double>();
                    }
                }
            }
        }
        
        // Test split operation
        std::vector<cv::Mat> split_channels;
        cv::split(multi_channel, split_channels);
        
        // Test merge operation
        if (!split_channels.empty()) {
            cv::Mat merged;
            cv::merge(split_channels, merged);
        }
        
    } catch (const cv::Exception& e) {
        // Ignore OpenCV exceptions - they are expected for invalid inputs
    } catch (...) {
        // Ignore any other exceptions
    }
    
    return 0;
}
} // namespace harness_001
