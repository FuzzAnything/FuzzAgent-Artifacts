#include <cstddef>
#include <cstdint>
#include <vector>
#include <cmath>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

namespace harness_020 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for basic image dimensions and parameters
    const size_t MIN_SIZE = sizeof(int) * 4 + sizeof(double) * 3 + sizeof(bool) * 2 + 100; // Basic params + some image data
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Consume image dimensions and parameters
    int width = fdp.ConsumeIntegralInRange<int>(10, 640);
    int height = fdp.ConsumeIntegralInRange<int>(10, 480);
    int channels = fdp.ConsumeIntegralInRange<int>(1, 4);
    int image_type = CV_8UC1;
    if (channels == 1) image_type = CV_8UC1;
    else if (channels == 3) image_type = CV_8UC3;
    else if (channels == 4) image_type = CV_8UC4;
    
    // Step 2: Create input images using cv::Mat from random data
    // Create two images for optical flow and tracking (prev and next frames)
    cv::Mat prev_image(height, width, image_type);
    cv::Mat next_image(height, width, image_type);
    
    // Fill images with random pixel data
    size_t image_bytes = prev_image.total() * prev_image.elemSize();
    if (fdp.remaining_bytes() < image_bytes * 2) {
        // Not enough data for both images, reduce size
        width = std::min(width, 100);
        height = std::min(height, 100);
        prev_image = cv::Mat(height, width, image_type);
        next_image = cv::Mat(height, width, image_type);
        image_bytes = prev_image.total() * prev_image.elemSize();
    }
    
    if (fdp.remaining_bytes() >= image_bytes * 2) {
        // Fill previous image
        std::vector<uint8_t> prev_data = fdp.ConsumeBytes<uint8_t>(image_bytes);
        if (prev_data.size() == image_bytes) {
            std::memcpy(prev_image.data, prev_data.data(), image_bytes);
        }
        
        // Fill next image
        std::vector<uint8_t> next_data = fdp.ConsumeBytes<uint8_t>(image_bytes);
        if (next_data.size() == image_bytes) {
            std::memcpy(next_image.data, next_data.data(), image_bytes);
        }
    }
    
    // Step 3: Initialize tracking window using cv::Rect
    int rect_x = fdp.ConsumeIntegralInRange<int>(0, std::max(0, width - 20));
    int rect_y = fdp.ConsumeIntegralInRange<int>(0, std::max(0, height - 20));
    int rect_width = fdp.ConsumeIntegralInRange<int>(10, std::min(100, width - rect_x));
    int rect_height = fdp.ConsumeIntegralInRange<int>(10, std::min(100, height - rect_y));
    cv::Rect tracking_window(rect_x, rect_y, rect_width, rect_height);
    
    // Step 4: Termination criteria for iterative algorithms
    int max_iter = fdp.ConsumeIntegralInRange<int>(1, 100);
    double epsilon = fdp.ConsumeFloatingPointInRange<double>(0.001, 1.0);
    cv::TermCriteria term_criteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, max_iter, epsilon);
    
    // Step 5: Build optical flow pyramid
    cv::Mat prev_pyramid, next_pyramid;
    cv::Size winSize(21, 21);
    int maxLevel = fdp.ConsumeIntegralInRange<int>(1, 3);
    bool withDerivatives = fdp.ConsumeBool();
    int pyrBorder = cv::BORDER_REFLECT_101;
    int derivBorder = cv::BORDER_CONSTANT;
    bool tryReuseInputImage = fdp.ConsumeBool();
    
    try {
        cv::buildOpticalFlowPyramid(prev_image, prev_pyramid, winSize, maxLevel, 
                                    withDerivatives, pyrBorder, derivBorder, tryReuseInputImage);
        cv::buildOpticalFlowPyramid(next_image, next_pyramid, winSize, maxLevel, 
                                    withDerivatives, pyrBorder, derivBorder, tryReuseInputImage);
    } catch (...) {
        // Continue with other tests if pyramid building fails
    }
    
    // Step 6: CamShift object tracking
    cv::Rect camshift_window = tracking_window;
    cv::RotatedRect rotated_rect;
    try {
        rotated_rect = cv::CamShift(prev_image, camshift_window, term_criteria);
    } catch (...) {
        // Handle exceptions silently
    }
    
    // Step 7: MeanShift object tracking
    cv::Rect meanshift_window = tracking_window;
    try {
        int iterations = cv::meanShift(prev_image, meanshift_window, term_criteria);
        (void)iterations; // Use variable to avoid warning
    } catch (...) {
        // Handle exceptions silently
    }
    
    // Step 8: Sparse optical flow using Lucas-Kanade method
    // Create some feature points to track
    std::vector<cv::Point2f> prev_points, next_points;
    std::vector<uchar> status;
    std::vector<float> err;
    
    int num_points = fdp.ConsumeIntegralInRange<int>(1, 50);
    for (int i = 0; i < num_points && fdp.remaining_bytes() >= sizeof(float) * 2; i++) {
        float x = fdp.ConsumeFloatingPointInRange<float>(0.0f, static_cast<float>(width - 1));
        float y = fdp.ConsumeFloatingPointInRange<float>(0.0f, static_cast<float>(height - 1));
        prev_points.push_back(cv::Point2f(x, y));
    }
    
    if (!prev_points.empty() && !prev_pyramid.empty() && !next_pyramid.empty()) {
        try {
            cv::calcOpticalFlowPyrLK(prev_pyramid, next_pyramid, prev_points, next_points, 
                                     status, err, winSize, maxLevel, term_criteria, 
                                     cv::OPTFLOW_LK_GET_MIN_EIGENVALS, 0.001);
        } catch (...) {
            // Handle exceptions silently
        }
    }
    
    // Step 9: Dense optical flow using Farneback algorithm
    cv::Mat flow;
    double pyr_scale = fdp.ConsumeFloatingPointInRange<double>(0.5, 0.95);
    int levels = fdp.ConsumeIntegralInRange<int>(1, 3);
    int winsize = fdp.ConsumeIntegralInRange<int>(5, 21);
    int iterations_farneback = fdp.ConsumeIntegralInRange<int>(1, 10);
    int poly_n = fdp.ConsumeIntegralInRange<int>(5, 10);
    double poly_sigma = fdp.ConsumeFloatingPointInRange<double>(1.1, 2.0);
    int flags = 0;
    if (fdp.ConsumeBool()) flags |= cv::OPTFLOW_FARNEBACK_GAUSSIAN;
    if (fdp.ConsumeBool()) flags |= cv::OPTFLOW_USE_INITIAL_FLOW;
    
    try {
        cv::calcOpticalFlowFarneback(prev_image, next_image, flow, pyr_scale, levels, winsize,
                                     iterations_farneback, poly_n, poly_sigma, flags);
    } catch (...) {
        // Handle exceptions silently
    }
    
    // Step 10: Enhanced Correlation Coefficient for image alignment
    double ecc_value = 0.0;
    try {
        ecc_value = cv::computeECC(prev_image, next_image);
    } catch (...) {
        // Handle exceptions silently
    }
    
    // Step 11: Find geometric transform using ECC
    cv::Mat warp_matrix;
    int motion_type = fdp.ConsumeIntegralInRange<int>(0, 3); // 0: TRANSLATION, 1: EUCLIDEAN, 2: AFFINE, 3: HOMOGRAPHY
    
    // Initialize warp matrix based on motion type
    if (motion_type == cv::MOTION_TRANSLATION || motion_type == cv::MOTION_EUCLIDEAN || motion_type == cv::MOTION_AFFINE) {
        warp_matrix = cv::Mat::eye(2, 3, CV_32F);
    } else if (motion_type == cv::MOTION_HOMOGRAPHY) {
        warp_matrix = cv::Mat::eye(3, 3, CV_32F);
    }
    
    // Add some noise to the initial warp matrix
    if (fdp.remaining_bytes() >= sizeof(float) * 6) {
        float* warp_data = reinterpret_cast<float*>(warp_matrix.data);
        for (int i = 0; i < warp_matrix.rows * warp_matrix.cols && fdp.remaining_bytes() >= sizeof(float); i++) {
            warp_data[i] += fdp.ConsumeFloatingPointInRange<float>(-0.1f, 0.1f);
        }
    }
    
    try {
        double final_ecc = cv::findTransformECC(prev_image, next_image, warp_matrix, motion_type, term_criteria);
        (void)final_ecc; // Use variable to avoid warning
    } catch (...) {
        // Handle exceptions silently
    }
    
    // Step 12: Additional tests with RotatedRect operations
    if (rotated_rect.size.width > 0 && rotated_rect.size.height > 0) {
        try {
            cv::Rect bounding_rect = rotated_rect.boundingRect();
            cv::Point2f vertices[4];
            rotated_rect.points(vertices);
            
            // Test if the center is inside the image
            bool center_inside = bounding_rect.contains(rotated_rect.center);
            (void)center_inside;
        } catch (...) {
            // Handle exceptions silently
        }
    }
    
    // Step 13: Test with different image types for robustness
    if (fdp.remaining_bytes() > 100) {
        // Try with float images for ECC (which supports float)
        cv::Mat prev_float, next_float;
        prev_image.convertTo(prev_float, CV_32F);
        next_image.convertTo(next_float, CV_32F);
        
        try {
            double ecc_float = cv::computeECC(prev_float, next_float);
            (void)ecc_float;
        } catch (...) {
            // Handle exceptions silently
        }
    }
    
    // Step 14: Clean up resources (automatic via Mat destructors)
    // All cv::Mat objects will be automatically destroyed when they go out of scope
    
    return 0;
}

} // namespace harness_020
