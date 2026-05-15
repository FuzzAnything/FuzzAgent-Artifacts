#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

namespace harness_006 {
// Maximum input size to prevent processing large files (e.g., JPEG 2000 images)
constexpr size_t MAX_INPUT_SIZE = 4096;

// Maximum number of chessboard corners to prevent excessive computation
constexpr int MAX_CHESSBOARD_CORNERS = 80;

// Maximum image dimensions to prevent large image processing
constexpr int MAX_IMAGE_WIDTH = 640;
constexpr int MAX_IMAGE_HEIGHT = 480;

// Maximum number of views for calibration
constexpr int MAX_CALIBRATION_VIEWS = 3;

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Size limits: reject too small or too large inputs
    if (size < 128 || size > MAX_INPUT_SIZE) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    try {
        // Step 1: Early validation - check if we have enough data for basic operations
        if (fdp.remaining_bytes() < 50) return 0;
        
        // Step 2: Define chessboard pattern size with reasonable limits
        int pattern_cols = fdp.ConsumeIntegralInRange<int>(4, 8);  // Conservative limits
        int pattern_rows = fdp.ConsumeIntegralInRange<int>(4, 6);
        cv::Size pattern_size(pattern_cols, pattern_rows);
        
        // Early return if pattern is too large (unlikely to form valid chessboard)
        int num_corners = pattern_cols * pattern_rows;
        if (num_corners > MAX_CHESSBOARD_CORNERS) {
            return 0;  // Pattern too large for efficient fuzzing
        }
        
        // Step 3: Check remaining bytes before allocating large structures
        size_t bytes_needed = num_corners * sizeof(float) * 3;  // For 3D points
        if (fdp.remaining_bytes() < bytes_needed + 50) {
            return 0;  // Not enough data for meaningful chessboard generation
        }
        
        // Step 4: Create synthetic 3D object points
        std::vector<cv::Point3f> object_points;
        object_points.reserve(num_corners);
        
        for (int i = 0; i < pattern_rows; ++i) {
            for (int j = 0; j < pattern_cols; ++j) {
                // Use limited precision to avoid excessive computation
                float x = static_cast<float>(j);
                float y = static_cast<float>(i);
                float z = 0.0f;  // Keep points in plane for simplicity
                
                // Add small noise only if we have enough data
                if (fdp.remaining_bytes() > 20) {
                    x += fdp.ConsumeFloatingPointInRange<float>(-0.05f, 0.05f);
                    y += fdp.ConsumeFloatingPointInRange<float>(-0.05f, 0.05f);
                    z += fdp.ConsumeFloatingPointInRange<float>(-0.01f, 0.01f);
                }
                
                object_points.push_back(cv::Point3f(x, y, z));
            }
        }
        
        // Step 5: Create synthetic camera parameters with limited precision
        if (fdp.remaining_bytes() < 30) return 0;
        
        float fx = fdp.ConsumeFloatingPointInRange<float>(400.0f, 800.0f);
        float fy = fdp.ConsumeFloatingPointInRange<float>(400.0f, 800.0f);
        float cx = fdp.ConsumeFloatingPointInRange<float>(200.0f, 400.0f);
        float cy = fdp.ConsumeFloatingPointInRange<float>(150.0f, 300.0f);
        
        cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << fx, 0, cx,
                                                          0, fy, cy,
                                                          0, 0, 1);
        
        // Distortion coefficients - limit number to avoid excessive computation
        std::vector<double> dist_coeffs;
        int num_dist_coeffs = fdp.ConsumeIntegralInRange<int>(0, 4);  // Reduced from 8
        for (int i = 0; i < num_dist_coeffs && fdp.remaining_bytes() > 10; ++i) {
            dist_coeffs.push_back(fdp.ConsumeFloatingPointInRange<double>(-0.2, 0.2));
        }
        
        // Step 6: Create synthetic image points with simple projection
        std::vector<cv::Point2f> image_points;
        
        // Simple translation (no rotation for efficiency)
        cv::Mat rvec = (cv::Mat_<double>(3, 1) << 0.0, 0.0, 0.0);
        cv::Mat tvec = (cv::Mat_<double>(3, 1) << 0.0, 0.0, 5.0);
        
        cv::projectPoints(object_points, rvec, tvec, camera_matrix, dist_coeffs, image_points);
        
        // Add minimal noise
        for (auto& pt : image_points) {
            if (fdp.remaining_bytes() > 5) {
                pt.x += fdp.ConsumeFloatingPointInRange<float>(-1.0f, 1.0f);
                pt.y += fdp.ConsumeFloatingPointInRange<float>(-1.0f, 1.0f);
            }
        }
        
        // Step 7: Create small synthetic image
        int image_width = fdp.ConsumeIntegralInRange<int>(320, MAX_IMAGE_WIDTH);
        int image_height = fdp.ConsumeIntegralInRange<int>(240, MAX_IMAGE_HEIGHT);
        cv::Size image_size(image_width, image_height);
        
        // Early return if image would be too large
        if (image_width * image_height > 100000) {  // ~100KB image max
            return 0;
        }
        
        // Create minimal test image
        cv::Mat test_image = cv::Mat::zeros(image_height, image_width, CV_8UC1);
        
        // Draw only if points are within image bounds
        int points_drawn = 0;
        for (const auto& pt : image_points) {
            int x = static_cast<int>(pt.x);
            int y = static_cast<int>(pt.y);
            if (x >= 0 && x < image_width && y >= 0 && y < image_height) {
                cv::circle(test_image, cv::Point(x, y), 2, cv::Scalar(255), -1);
                points_drawn++;
            }
        }
        
        // Early return if too few points in image (unlikely valid chessboard)
        if (points_drawn < num_corners / 2) {
            return 0;
        }
        
        // Step 8: Target API 1: findChessboardCorners (with limited flags)
        std::vector<cv::Point2f> detected_corners;
        int find_flags = 0;
        
        // Consume limited number of flags
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) find_flags |= cv::CALIB_CB_ADAPTIVE_THRESH;
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) find_flags |= cv::CALIB_CB_NORMALIZE_IMAGE;
        
        bool pattern_found = cv::findChessboardCorners(test_image, pattern_size, detected_corners, find_flags);
        
        // Step 9: Prepare data for camera calibration (limited number of views)
        std::vector<std::vector<cv::Point3f>> object_points_array;
        std::vector<std::vector<cv::Point2f>> image_points_array;
        
        int num_views = fdp.ConsumeIntegralInRange<int>(2, MAX_CALIBRATION_VIEWS);
        
        for (int view = 0; view < num_views; ++view) {
            // Early exit if running out of data
            if (fdp.remaining_bytes() < 30) break;
            
            // Create simple alternative view
            cv::Mat view_rvec = (cv::Mat_<double>(3, 1) << 
                fdp.ConsumeFloatingPointInRange<double>(-0.2, 0.2),
                fdp.ConsumeFloatingPointInRange<double>(-0.2, 0.2),
                0.0);  // Keep Z rotation minimal
            
            cv::Mat view_tvec = (cv::Mat_<double>(3, 1) << 
                fdp.ConsumeFloatingPointInRange<double>(-0.5, 0.5),
                fdp.ConsumeFloatingPointInRange<double>(-0.5, 0.5),
                fdp.ConsumeFloatingPointInRange<double>(4.0, 6.0));
            
            std::vector<cv::Point2f> view_image_points;
            cv::projectPoints(object_points, view_rvec, view_tvec, camera_matrix, dist_coeffs, view_image_points);
            
            object_points_array.push_back(object_points);
            image_points_array.push_back(view_image_points);
        }
        
        // Step 10: Only calibrate if we have reasonable data
        if (object_points_array.size() >= 2) {
            cv::Mat calibrated_camera_matrix = cv::Mat::eye(3, 3, CV_64F);
            std::vector<double> calibrated_dist_coeffs;
            std::vector<cv::Mat> rvecs, tvecs;
            
            // Use minimal calibration flags to limit computation
            int calib_flags = cv::CALIB_FIX_ASPECT_RATIO;  // Fixed to reduce parameters
            
            // Limited iteration count for calibrateCamera
            cv::TermCriteria calib_criteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 30, 1e-4);
            
            // Timeout protection: calibrateCamera with limited data
            double reprojection_error = cv::calibrateCamera(object_points_array, image_points_array, 
                                                          image_size, calibrated_camera_matrix,
                                                          calibrated_dist_coeffs, rvecs, tvecs,
                                                          calib_flags, calib_criteria);
            
            // Quick test of undistort with small image
            if (fdp.remaining_bytes() > 20 && image_width * image_height < 50000) {
                cv::Mat small_image = cv::Mat::zeros(std::min(100, image_height), 
                                                    std::min(100, image_width), CV_8UC3);
                
                // Fill with simple pattern
                for (int y = 0; y < small_image.rows; ++y) {
                    for (int x = 0; x < small_image.cols; ++x) {
                        uchar val = static_cast<uchar>((x ^ y) & 0xFF);
                        small_image.at<cv::Vec3b>(y, x) = cv::Vec3b(val, val, val);
                    }
                }
                
                cv::Mat undistorted_image;
                cv::undistort(small_image, undistorted_image, 
                             calibrated_camera_matrix, calibrated_dist_coeffs);
            }
        }
        
    } catch (const cv::Exception& e) {
        // OpenCV exceptions are expected during fuzzing
    } catch (...) {
        // Catch any other exceptions
    }
    
    return 0;
}

} // namespace harness_006
