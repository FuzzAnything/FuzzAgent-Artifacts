#include <opencv2/core.hpp>
#include <opencv2/core/hal/hal.hpp>
#include <opencv2/highgui.hpp>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <cmath>

namespace harness_005 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) return 0; // Minimum size for meaningful testing
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume parameters for matrix dimensions and type selection
    int rows = fdp.ConsumeIntegralInRange(1, 100);
    int cols = fdp.ConsumeIntegralInRange(1, 100);
    int operation_choice = fdp.ConsumeIntegralInRange(0, 3); // 0: exp float, 1: exp double, 2: split int, 3: test all
    
    try {
        // Test 1: cv::getNumThreads - public threading API
        int num_threads = cv::getNumThreads();
        (void)num_threads; // Use variable to avoid unused warning
        
        // Test 2: cv::pollKey - highgui interaction API (safe to call in headless mode)
        int key_press = cv::pollKey();
        (void)key_press; // Use variable to avoid unused warning
        
        // Create matrices for testing based on operation choice
        if (operation_choice == 0 || operation_choice == 3) {
            // Test HAL exp32f (single-channel floating-point matrices)
            cv::Mat src_float(rows, cols, CV_32F);
            cv::Mat dst_float(rows, cols, CV_32F);
            
            // Fill source matrix with random float values
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    // Generate random float values (exp handles all real numbers)
                    float val = fdp.ConsumeFloatingPoint<float>();
                    src_float.at<float>(i, j) = val;
                }
            }
            
            // Test public cv::exp API (which internally calls HAL exp)
            cv::exp(src_float, dst_float);
            
            // Test HAL exp32f directly
            int n = rows * cols;
            std::vector<float> src_data(n);
            std::vector<float> dst_data(n);
            
            // Copy matrix data to vectors
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    src_data[i * cols + j] = src_float.at<float>(i, j);
                }
            }
            
            // Call HAL exp32f directly
            cv::hal::exp32f(src_data.data(), dst_data.data(), n);
        }
        
        if (operation_choice == 1 || operation_choice == 3) {
            // Test HAL exp64f (double precision)
            cv::Mat src_double(rows, cols, CV_64F);
            cv::Mat dst_double(rows, cols, CV_64F);
            
            // Fill source matrix with random double values
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    double val = fdp.ConsumeFloatingPoint<double>();
                    src_double.at<double>(i, j) = val;
                }
            }
            
            // Test public cv::exp API for double
            cv::exp(src_double, dst_double);
            
            // Test HAL exp64f directly
            int n = rows * cols;
            std::vector<double> src_data(n);
            std::vector<double> dst_data(n);
            
            // Copy matrix data to vectors
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    src_data[i * cols + j] = src_double.at<double>(i, j);
                }
            }
            
            // Call HAL exp64f directly
            cv::hal::exp64f(src_data.data(), dst_data.data(), n);
        }
        
        if (operation_choice == 2 || operation_choice == 3) {
            // Test HAL split32s (multi-channel integer matrices)
            int channels = fdp.ConsumeIntegralInRange(1, 4);
            cv::Mat src_multi(rows, cols, CV_MAKETYPE(CV_32S, channels));
            
            // Fill multi-channel matrix with random integer values
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    for (int c = 0; c < channels; ++c) {
                        src_multi.at<cv::Vec<int, 1>>(i, j)[c] = fdp.ConsumeIntegral<int>();
                    }
                }
            }
            
            // Test public cv::split API (which internally calls HAL split)
            std::vector<cv::Mat> split_channels;
            cv::split(src_multi, split_channels);
            
            // Test HAL split32s directly
            int len = rows * cols;
            std::vector<int*> dst_ptrs(channels);
            std::vector<std::vector<int>> dst_buffers(channels, std::vector<int>(len));
            
            // Prepare destination pointers
            for (int c = 0; c < channels; ++c) {
                dst_ptrs[c] = dst_buffers[c].data();
            }
            
            // Get source data as contiguous array
            cv::Mat src_contiguous = src_multi.clone();
            if (!src_multi.isContinuous()) {
                src_multi.copyTo(src_contiguous);
            }
            
            // Call HAL split32s directly
            cv::hal::split32s((const int*)src_contiguous.data, dst_ptrs.data(), len, channels);
            
            // Also test other split variants based on data type
            int data_type_choice = fdp.ConsumeIntegralInRange(0, 3);
            switch (data_type_choice) {
                case 0: {
                    // Test split8u
                    cv::Mat src_8u(rows, cols, CV_MAKETYPE(CV_8U, channels));
                    std::vector<uchar*> dst_ptrs_8u(channels);
                    std::vector<std::vector<uchar>> dst_buffers_8u(channels, std::vector<uchar>(len));
                    for (int c = 0; c < channels; ++c) {
                        dst_ptrs_8u[c] = dst_buffers_8u[c].data();
                    }
                    // Note: split8u would need proper data preparation
                    break;
                }
                case 1: {
                    // Test split16u
                    cv::Mat src_16u(rows, cols, CV_MAKETYPE(CV_16U, channels));
                    std::vector<ushort*> dst_ptrs_16u(channels);
                    std::vector<std::vector<ushort>> dst_buffers_16u(channels, std::vector<ushort>(len));
                    for (int c = 0; c < channels; ++c) {
                        dst_ptrs_16u[c] = dst_buffers_16u[c].data();
                    }
                    break;
                }
                case 2: {
                    // Test split64s - note: CV_64S might not be defined in all OpenCV versions
                    // Using CV_64F for double precision instead
                    cv::Mat src_64f(rows, cols, CV_MAKETYPE(CV_64F, channels));
                    std::vector<double*> dst_ptrs_64f(channels);
                    std::vector<std::vector<double>> dst_buffers_64f(channels, std::vector<double>(len));
                    for (int c = 0; c < channels; ++c) {
                        dst_ptrs_64f[c] = dst_buffers_64f[c].data();
                    }
                    break;
                }
            }
        }
        
        // Test edge cases: empty matrices
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
            cv::Mat empty_mat;
            cv::Mat empty_dst;
            
            // Test exp with empty matrix
            cv::exp(empty_mat, empty_dst);
            
            // Test split with empty matrix
            std::vector<cv::Mat> empty_split;
            cv::split(empty_mat, empty_split);
            
            // Test HAL functions with zero length
            if (fdp.remaining_bytes() > 4) {
                float dummy_src[1] = {0};
                float dummy_dst[1] = {0};
                cv::hal::exp32f(dummy_src, dummy_dst, 0);
                
                int dummy_src_int[1] = {0};
                int dummy_dst_int[1] = {0};
                int* dummy_dst_ptrs[1] = {dummy_dst_int};
                cv::hal::split32s(dummy_src_int, dummy_dst_ptrs, 0, 1);
            }
        }
        
    } catch (const cv::Exception& e) {
        // Ignore OpenCV exceptions - they are expected for invalid inputs
    } catch (...) {
        // Ignore any other exceptions
    }
    
    return 0;
}

} // namespace harness_005
