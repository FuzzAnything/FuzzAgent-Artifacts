#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/opencv.hpp>
#include <opencv2/photo.hpp>

namespace harness_016 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for operation selection, image dimensions, and basic parameters
    const size_t MIN_SIZE = sizeof(uint8_t) + sizeof(int) * 6 + sizeof(float) * 4 + sizeof(double) + 64;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation type to test different photo module functions
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Consume image dimensions (keep small for fuzzing performance)
    int img_width = fdp.ConsumeIntegralInRange<int>(8, 64);
    int img_height = fdp.ConsumeIntegralInRange<int>(8, 64);
    int channels = fdp.ConsumeIntegralInRange<int>(1, 4);
    
    // Consume denoising parameters
    float denoise_h = fdp.ConsumeFloatingPointInRange<float>(1.0f, 50.0f);
    float denoise_h_color = fdp.ConsumeFloatingPointInRange<float>(1.0f, 50.0f);
    int template_window = fdp.ConsumeIntegralInRange<int>(3, 21);
    if (template_window % 2 == 0) template_window++; // Ensure odd
    int search_window = fdp.ConsumeIntegralInRange<int>(3, 21);
    if (search_window % 2 == 0) search_window++; // Ensure odd
    
    // Consume inpainting parameters
    double inpaint_radius = fdp.ConsumeFloatingPointInRange<double>(1.0, 20.0);
    uint8_t inpaint_flags = fdp.ConsumeIntegral<uint8_t>() % 2; // 0 = INPAINT_NS, 1 = INPAINT_TELEA
    
    // Use remaining bytes for image data
    std::vector<uint8_t> image_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    try {
        // Create input image from fuzzer data
        cv::Mat input_img;
        if (!image_data.empty() && image_data.size() >= (size_t)(img_width * img_height * channels)) {
            // Use a portion of the data for the image
            size_t img_size = img_width * img_height * channels;
            if (img_size > image_data.size()) {
                img_size = image_data.size();
            }
            
            // Create image with appropriate type based on channels
            if (channels == 1) {
                input_img = cv::Mat(img_height, img_width, CV_8UC1, image_data.data());
            } else if (channels == 2) {
                input_img = cv::Mat(img_height, img_width, CV_8UC2, image_data.data());
            } else if (channels == 3) {
                input_img = cv::Mat(img_height, img_width, CV_8UC3, image_data.data());
            } else { // channels == 4
                input_img = cv::Mat(img_height, img_width, CV_8UC4, image_data.data());
            }
        } else {
            // Create a small default image if not enough data
            input_img = cv::Mat::zeros(img_height, img_width, channels == 1 ? CV_8UC1 : 
                                      (channels == 2 ? CV_8UC2 : 
                                      (channels == 3 ? CV_8UC3 : CV_8UC4)));
        }
        
        // Test different photo module operations based on operation type
        switch (operation) {
            case 0: {
                // Test fastNlMeansDenoising on grayscale image
                cv::Mat gray_img;
                if (input_img.channels() > 1) {
                    cv::cvtColor(input_img, gray_img, cv::COLOR_BGR2GRAY);
                } else {
                    gray_img = input_img.clone();
                }
                
                cv::Mat denoised_gray;
                try {
                    cv::fastNlMeansDenoising(gray_img, denoised_gray, denoise_h, 
                                           template_window, search_window);
                } catch (...) {
                    // Denoising may fail with random data - that's expected
                }
                break;
            }
            
            case 1: {
                // Test fastNlMeansDenoisingColored on color image
                cv::Mat color_img;
                if (input_img.channels() == 1) {
                    cv::cvtColor(input_img, color_img, cv::COLOR_GRAY2BGR);
                } else if (input_img.channels() == 3) {
                    color_img = input_img.clone();
                } else if (input_img.channels() == 4) {
                    cv::cvtColor(input_img, color_img, cv::COLOR_BGRA2BGR);
                } else {
                    // For 2-channel images, convert to 3-channel
                    std::vector<cv::Mat> channels_vec;
                    cv::split(input_img, channels_vec);
                    if (channels_vec.size() >= 2) {
                        cv::merge(std::vector<cv::Mat>{channels_vec[0], channels_vec[1], channels_vec[0]}, color_img);
                    } else {
                        color_img = cv::Mat::zeros(img_height, img_width, CV_8UC3);
                    }
                }
                
                cv::Mat denoised_color;
                try {
                    cv::fastNlMeansDenoisingColored(color_img, denoised_color, denoise_h, 
                                                   denoise_h_color, template_window, search_window);
                } catch (...) {
                    // Denoising may fail with random data - that's expected
                }
                break;
            }
            
            case 2: {
                // Test fastNlMeansDenoising with vector h parameter
                cv::Mat gray_img;
                if (input_img.channels() > 1) {
                    cv::cvtColor(input_img, gray_img, cv::COLOR_BGR2GRAY);
                } else {
                    gray_img = input_img.clone();
                }
                
                cv::Mat denoised_vec_h;
                std::vector<float> h_params;
                h_params.push_back(denoise_h);
                if (gray_img.channels() > 1) {
                    // Add additional h parameters for multi-channel images
                    for (int i = 1; i < gray_img.channels(); i++) {
                        h_params.push_back(denoise_h + i * 2.0f);
                    }
                }
                
                try {
                    cv::fastNlMeansDenoising(gray_img, denoised_vec_h, h_params, 
                                           template_window, search_window, cv::NORM_L2);
                } catch (...) {
                    // Denoising may fail with random data - that's expected
                }
                break;
            }
            
            case 3: {
                // Test inpaint with INPAINT_NS algorithm
                cv::Mat src_img;
                if (input_img.channels() == 1) {
                    cv::cvtColor(input_img, src_img, cv::COLOR_GRAY2BGR);
                } else if (input_img.channels() == 3) {
                    src_img = input_img.clone();
                } else if (input_img.channels() == 4) {
                    cv::cvtColor(input_img, src_img, cv::COLOR_BGRA2BGR);
                } else {
                    src_img = cv::Mat::zeros(img_height, img_width, CV_8UC3);
                }
                
                // Create a simple mask for inpainting (damage area)
                cv::Mat mask = cv::Mat::zeros(img_height, img_width, CV_8UC1);
                if (img_width > 10 && img_height > 10) {
                    // Create a rectangular damaged area in the center
                    int rect_width = std::min(10, img_width / 4);
                    int rect_height = std::min(10, img_height / 4);
                    int rect_x = (img_width - rect_width) / 2;
                    int rect_y = (img_height - rect_height) / 2;
                    cv::rectangle(mask, cv::Rect(rect_x, rect_y, rect_width, rect_height), 
                                 cv::Scalar(255), cv::FILLED);
                }
                
                cv::Mat inpainted;
                try {
                    cv::inpaint(src_img, mask, inpainted, inpaint_radius, cv::INPAINT_NS);
                } catch (...) {
                    // Inpainting may fail - that's expected
                }
                break;
            }
            
            case 4: {
                // Test inpaint with INPAINT_TELEA algorithm
                cv::Mat src_img;
                if (input_img.channels() == 1) {
                    cv::cvtColor(input_img, src_img, cv::COLOR_GRAY2BGR);
                } else if (input_img.channels() == 3) {
                    src_img = input_img.clone();
                } else if (input_img.channels() == 4) {
                    cv::cvtColor(input_img, src_img, cv::COLOR_BGRA2BGR);
                } else {
                    src_img = cv::Mat::zeros(img_height, img_width, CV_8UC3);
                }
                
                // Create a different mask pattern (circular damage area)
                cv::Mat mask = cv::Mat::zeros(img_height, img_width, CV_8UC1);
                if (img_width > 10 && img_height > 10) {
                    // Create a circular damaged area
                    int center_x = img_width / 2;
                    int center_y = img_height / 2;
                    int radius = std::min(8, std::min(img_width, img_height) / 4);
                    cv::circle(mask, cv::Point(center_x, center_y), radius, 
                              cv::Scalar(255), cv::FILLED);
                }
                
                cv::Mat inpainted;
                try {
                    cv::inpaint(src_img, mask, inpainted, inpaint_radius, cv::INPAINT_TELEA);
                } catch (...) {
                    // Inpainting may fail - that's expected
                }
                break;
            }
            
            case 5: {
                // Test both denoising and inpainting in sequence
                cv::Mat color_img;
                if (input_img.channels() == 1) {
                    cv::cvtColor(input_img, color_img, cv::COLOR_GRAY2BGR);
                } else if (input_img.channels() == 3) {
                    color_img = input_img.clone();
                } else {
                    color_img = cv::Mat::zeros(img_height, img_width, CV_8UC3);
                }
                
                // First denoise
                cv::Mat denoised;
                try {
                    cv::fastNlMeansDenoisingColored(color_img, denoised, denoise_h, 
                                                   denoise_h_color, template_window, search_window);
                } catch (...) {
                    denoised = color_img.clone();
                }
                
                // Then inpaint
                cv::Mat mask = cv::Mat::zeros(img_height, img_width, CV_8UC1);
                if (img_width > 15 && img_height > 15) {
                    // Create multiple small damage areas
                    for (int i = 0; i < 3; i++) {
                        int x = fdp.ConsumeIntegralInRange<int>(5, img_width - 10);
                        int y = fdp.ConsumeIntegralInRange<int>(5, img_height - 10);
                        int w = fdp.ConsumeIntegralInRange<int>(3, 8);
                        int h = fdp.ConsumeIntegralInRange<int>(3, 8);
                        cv::rectangle(mask, cv::Rect(x, y, w, h), cv::Scalar(255), cv::FILLED);
                    }
                }
                
                cv::Mat inpainted;
                try {
                    cv::inpaint(denoised, mask, inpainted, inpaint_radius, 
                               inpaint_flags == 0 ? cv::INPAINT_NS : cv::INPAINT_TELEA);
                } catch (...) {
                    // Inpainting may fail - that's expected
                }
                break;
            }
            
            default:
                break;
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}

} // namespace harness_016
