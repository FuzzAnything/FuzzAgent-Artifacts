#include <cstddef>
#include <cstdint>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace harness_007 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum input size check - need enough for basic parameters
  if (size < 100) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Consume parameters for calibration setup
  int pattern_width = fdp.ConsumeIntegralInRange<int>(5, 15);  // chessboard width in corners
  int pattern_height = fdp.ConsumeIntegralInRange<int>(5, 15); // chessboard height in corners
  float square_size = fdp.ConsumeFloatingPointInRange<float>(10.0f, 50.0f); // mm
  
  // Image dimensions
  int img_width = fdp.ConsumeIntegralInRange<int>(640, 1920);
  int img_height = fdp.ConsumeIntegralInRange<int>(480, 1080);
  
  // Camera intrinsic parameters from fuzzer input
  double fx = fdp.ConsumeFloatingPointInRange<double>(300.0, 2000.0);
  double fy = fdp.ConsumeFloatingPointInRange<double>(300.0, 2000.0);
  double cx = fdp.ConsumeFloatingPointInRange<double>(img_width * 0.3, img_width * 0.7);
  double cy = fdp.ConsumeFloatingPointInRange<double>(img_height * 0.3, img_height * 0.7);
  
  // Distortion coefficients
  double k1 = fdp.ConsumeFloatingPointInRange<double>(-0.5, 0.5);
  double k2 = fdp.ConsumeFloatingPointInRange<double>(-0.5, 0.5);
  double p1 = fdp.ConsumeFloatingPointInRange<double>(-0.5, 0.5);
  double p2 = fdp.ConsumeFloatingPointInRange<double>(-0.5, 0.5);
  double k3 = fdp.ConsumeFloatingPointInRange<double>(-0.5, 0.5);
  
  // Rotation vector (axis-angle representation)
  double rvec_x = fdp.ConsumeFloatingPointInRange<double>(-CV_PI, CV_PI);
  double rvec_y = fdp.ConsumeFloatingPointInRange<double>(-CV_PI, CV_PI);
  double rvec_z = fdp.ConsumeFloatingPointInRange<double>(-CV_PI, CV_PI);
  
  // Translation vector
  double tvec_x = fdp.ConsumeFloatingPointInRange<double>(-100.0, 100.0);
  double tvec_y = fdp.ConsumeFloatingPointInRange<double>(-100.0, 100.0);
  double tvec_z = fdp.ConsumeFloatingPointInRange<double>(200.0, 1000.0);
  
  // Choose which operations to test
  uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
  
  try {
    // Create camera matrix
    cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << 
      fx, 0.0, cx,
      0.0, fy, cy,
      0.0, 0.0, 1.0);
    
    // Create distortion coefficients
    cv::Mat dist_coeffs = (cv::Mat_<double>(5, 1) << k1, k2, p1, p2, k3);
    
    // Create rotation and translation vectors
    cv::Mat rvec = (cv::Mat_<double>(3, 1) << rvec_x, rvec_y, rvec_z);
    cv::Mat tvec = (cv::Mat_<double>(3, 1) << tvec_x, tvec_y, tvec_z);
    
    // Generate synthetic 3D object points (chessboard corners in world coordinates)
    std::vector<cv::Point3f> object_points;
    for (int i = 0; i < pattern_height; ++i) {
      for (int j = 0; j < pattern_width; ++j) {
        object_points.push_back(cv::Point3f(j * square_size, i * square_size, 0.0f));
      }
    }
    
    // Project 3D points to 2D image points using the camera model
    std::vector<cv::Point2f> image_points;
    cv::projectPoints(object_points, rvec, tvec, camera_matrix, dist_coeffs, image_points);
    
    // Add some noise to image points to simulate detection errors
    for (auto& pt : image_points) {
      float noise_x = fdp.ConsumeFloatingPointInRange<float>(-2.0f, 2.0f);
      float noise_y = fdp.ConsumeFloatingPointInRange<float>(-2.0f, 2.0f);
      pt.x += noise_x;
      pt.y += noise_y;
    }
    
    // Test different calibration and 3D reconstruction operations
    switch (test_mode) {
      case 0: {
        // Test 1: solvePnP - estimate pose from 3D-2D correspondences
        cv::Mat estimated_rvec, estimated_tvec;
        bool success = cv::solvePnP(
          object_points, image_points, camera_matrix, dist_coeffs,
          estimated_rvec, estimated_tvec,
          false,  // useExtrinsicGuess
          cv::SOLVEPNP_ITERATIVE
        );
        
        if (success) {
          // Test refinement with Levenberg-Marquardt
          cv::Mat refined_rvec = estimated_rvec.clone();
          cv::Mat refined_tvec = estimated_tvec.clone();
          cv::solvePnPRefineLM(
            object_points, image_points, camera_matrix, dist_coeffs,
            refined_rvec, refined_tvec
          );
        }
        break;
      }
      
      case 1: {
        // Test 2: solvePnPRansac - robust pose estimation
        cv::Mat estimated_rvec, estimated_tvec;
        cv::Mat inliers;
        bool success = cv::solvePnPRansac(
          object_points, image_points, camera_matrix, dist_coeffs,
          estimated_rvec, estimated_tvec,
          false,  // useExtrinsicGuess
          100,    // iterationsCount
          8.0f,   // reprojectionError
          0.99,   // confidence
          inliers,
          cv::SOLVEPNP_EPNP
        );
        break;
      }
      
      case 2: {
        // Test 3: calibrateCamera - camera calibration from multiple views
        // For simplicity, we'll use a single view with the synthetic data
        std::vector<std::vector<cv::Point3f>> object_points_array;
        std::vector<std::vector<cv::Point2f>> image_points_array;
        
        object_points_array.push_back(object_points);
        image_points_array.push_back(image_points);
        
        cv::Mat estimated_camera_matrix = camera_matrix.clone();
        cv::Mat estimated_dist_coeffs = dist_coeffs.clone();
        std::vector<cv::Mat> rvecs, tvecs;
        
        double rms_error = cv::calibrateCamera(
          object_points_array, image_points_array, 
          cv::Size(img_width, img_height),
          estimated_camera_matrix, estimated_dist_coeffs,
          rvecs, tvecs,
          cv::CALIB_USE_INTRINSIC_GUESS | cv::CALIB_FIX_ASPECT_RATIO
        );
        
        // Test calibrateCameraRO with releasing object method
        if (object_points.size() > 2) {
          cv::Mat new_obj_points;
          int iFixedPoint = 1;  // Fix the second point
          
          double rms_error_ro = cv::calibrateCameraRO(
            object_points_array, image_points_array, 
            cv::Size(img_width, img_height), iFixedPoint,
            estimated_camera_matrix, estimated_dist_coeffs,
            rvecs, tvecs, new_obj_points
          );
        }
        break;
      }
      
      case 3: {
        // Test 4: findChessboardCorners - detect chessboard pattern
        // Create a synthetic chessboard image
        cv::Mat chessboard_img = cv::Mat::zeros(img_height, img_width, CV_8UC1);
        
        // Draw chessboard pattern
        int square_pixels = static_cast<int>(square_size);
        for (int i = 0; i < pattern_height; ++i) {
          for (int j = 0; j < pattern_width; ++j) {
            int start_x = j * square_pixels + 50;
            int start_y = i * square_pixels + 50;
            cv::Rect square_rect(start_x, start_y, square_pixels, square_pixels);
            
            // Alternate black and white squares
            uchar color = ((i + j) % 2 == 0) ? 255 : 0;
            cv::rectangle(chessboard_img, square_rect, cv::Scalar(color), cv::FILLED);
          }
        }
        
        // Add some noise to make it more realistic
        cv::Mat noise = cv::Mat(chessboard_img.size(), CV_8UC1);
        cv::randn(noise, 0, 30);
        chessboard_img += noise;
        
        // Try to find chessboard corners
        std::vector<cv::Point2f> detected_corners;
        cv::Size pattern_size(pattern_width, pattern_height);
        bool found = cv::findChessboardCorners(
          chessboard_img, pattern_size, detected_corners,
          cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK
        );
        
        if (found && !detected_corners.empty()) {
          // Refine corner locations
          cv::cornerSubPix(
            chessboard_img, detected_corners, cv::Size(5, 5), cv::Size(-1, -1),
            cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1)
          );
          
          // Test findChessboardCornersSB (statistically better version)
          std::vector<cv::Point2f> corners_sb;
          bool found_sb = cv::findChessboardCornersSB(
            chessboard_img, pattern_size, corners_sb,
            cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_EXHAUSTIVE | cv::CALIB_CB_ACCURACY
          );
        }
        
        // Test drawChessboardCorners
        cv::Mat drawn_img;
        cv::cvtColor(chessboard_img, drawn_img, cv::COLOR_GRAY2BGR);
        cv::drawChessboardCorners(drawn_img, pattern_size, detected_corners, found);
        break;
      }
    }
    
    // Additional tests with remaining bytes
    if (fdp.remaining_bytes() > 100) {
      // Test stereo calibration if we have enough data
      std::vector<std::vector<cv::Point3f>> object_points_stereo;
      std::vector<std::vector<cv::Point2f>> image_points1, image_points2;
      
      object_points_stereo.push_back(object_points);
      image_points1.push_back(image_points);
      
      // Create slightly different image points for second camera
      std::vector<cv::Point2f> image_points2_single;
      for (const auto& pt : image_points) {
        image_points2_single.push_back(cv::Point2f(pt.x + 20.0f, pt.y + fdp.ConsumeFloatingPointInRange<float>(-5.0f, 5.0f)));
      }
      image_points2.push_back(image_points2_single);
      
      cv::Mat camera_matrix1 = camera_matrix.clone();
      cv::Mat camera_matrix2 = camera_matrix.clone();
      cv::Mat dist_coeffs1 = dist_coeffs.clone();
      cv::Mat dist_coeffs2 = dist_coeffs.clone();
      cv::Mat R, T, E, F;
      
      double stereo_error = cv::stereoCalibrate(
        object_points_stereo, image_points1, image_points2,
        camera_matrix1, dist_coeffs1, camera_matrix2, dist_coeffs2,
        cv::Size(img_width, img_height), R, T, E, F,
        cv::CALIB_FIX_INTRINSIC,
        cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 30, 1e-6)
      );
    }
    
  } catch (const cv::Exception& e) {
    // Catch OpenCV exceptions and ignore - they're expected with random input
  } catch (...) {
    // Catch any other exceptions
  }
  
  return 0;
}

} // namespace harness_007
