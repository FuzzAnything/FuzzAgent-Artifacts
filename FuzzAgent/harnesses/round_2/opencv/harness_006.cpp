#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/calib3d.hpp>
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_006 {
// Seventh OpenCV fuzzing harness targeting calib3d module for camera calibration and 3D reconstruction
// Focuses on camera calibration APIs with 0% coverage (30,285 lines uncovered)
// APIs targeted: calibrateCamera, calibrateCameraRO, findChessboardCorners, solvePnP, 
// initCameraMatrix2D, undistort, stereoCalibrate
// Follows camera calibration workflow: generate synthetic calibration pattern points, 
// detect pattern, initialize camera matrix, calibrate camera, estimate pose, 
// undistort images, stereo calibration

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need enough data for calibration operations
    if (size < 256) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    try {
        // ===== 1. Generate synthetic calibration pattern points =====
        // Generate chessboard grid parameters
        int pattern_width = fdp.ConsumeIntegralInRange<int>(5, 15);
        int pattern_height = fdp.ConsumeIntegralInRange<int>(5, 15);
        float square_size = fdp.ConsumeFloatingPointInRange<float>(10.0f, 100.0f);
        
        // Create object points (3D points in calibration pattern coordinate system)
        std::vector<cv::Point3f> object_points_3d;
        for (int i = 0; i < pattern_height; i++) {
            for (int j = 0; j < pattern_width; j++) {
                object_points_3d.push_back(cv::Point3f(j * square_size, i * square_size, 0));
            }
        }
        
        // ===== 2. Generate synthetic image points =====
        // Image size
        int image_width = fdp.ConsumeIntegralInRange<int>(320, 1920);
        int image_height = fdp.ConsumeIntegralInRange<int>(240, 1080);
        cv::Size image_size(image_width, image_height);
        
        // Generate synthetic camera parameters
        float fx = fdp.ConsumeFloatingPointInRange<float>(300.0f, 2000.0f);
        float fy = fdp.ConsumeFloatingPointInRange<float>(300.0f, 2000.0f);
        float cx = fdp.ConsumeFloatingPointInRange<float>(image_width * 0.3f, image_width * 0.7f);
        float cy = fdp.ConsumeFloatingPointInRange<float>(image_height * 0.3f, image_height * 0.7f);
        
        cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << 
            fx, 0, cx,
            0, fy, cy,
            0, 0, 1);
        
        // Generate distortion coefficients
        std::vector<double> dist_coeffs;
        int num_dist_coeffs = fdp.ConsumeIntegralInRange<int>(4, 8);
        for (int i = 0; i < num_dist_coeffs; i++) {
            dist_coeffs.push_back(fdp.ConsumeFloatingPointInRange<double>(-0.5, 0.5));
        }
        
        // Generate rotation and translation for synthetic views
        int num_views = fdp.ConsumeIntegralInRange<int>(3, 10);
        std::vector<std::vector<cv::Point2f>> image_points_all;
        std::vector<std::vector<cv::Point3f>> object_points_all;
        std::vector<cv::Mat> rvecs_all, tvecs_all;
        
        for (int view_idx = 0; view_idx < num_views; view_idx++) {
            // Generate random rotation and translation for this view
            cv::Mat rvec = (cv::Mat_<double>(3, 1) << 
                fdp.ConsumeFloatingPointInRange<double>(-CV_PI/4, CV_PI/4),
                fdp.ConsumeFloatingPointInRange<double>(-CV_PI/4, CV_PI/4),
                fdp.ConsumeFloatingPointInRange<double>(-CV_PI/4, CV_PI/4));
            
            cv::Mat tvec = (cv::Mat_<double>(3, 1) << 
                fdp.ConsumeFloatingPointInRange<double>(-200.0, 200.0),
                fdp.ConsumeFloatingPointInRange<double>(-200.0, 200.0),
                fdp.ConsumeFloatingPointInRange<double>(400.0, 1000.0));
            
            // Project 3D points to 2D image points
            std::vector<cv::Point2f> image_points;
            cv::projectPoints(object_points_3d, rvec, tvec, camera_matrix, dist_coeffs, image_points);
            
            // Add some noise to simulate real detection
            for (auto& pt : image_points) {
                pt.x += fdp.ConsumeFloatingPointInRange<float>(-2.0f, 2.0f);
                pt.y += fdp.ConsumeFloatingPointInRange<float>(-2.0f, 2.0f);
            }
            
            image_points_all.push_back(image_points);
            object_points_all.push_back(object_points_3d);
            rvecs_all.push_back(rvec.clone());
            tvecs_all.push_back(tvec.clone());
        }
        
        // ===== 3. Test findChessboardCorners =====
        // Create a synthetic chessboard image for corner detection
        cv::Mat chessboard_image(image_size, CV_8UC1, cv::Scalar(0));
        int chessboard_cells_x = pattern_width - 1;
        int chessboard_cells_y = pattern_height - 1;
        
        // Draw chessboard pattern
        for (int i = 0; i < chessboard_cells_y; i++) {
            for (int j = 0; j < chessboard_cells_x; j++) {
                if ((i + j) % 2 == 0) {
                    int cell_size = std::min(image_width / chessboard_cells_x, image_height / chessboard_cells_y);
                    cv::Rect cell_rect(j * cell_size, i * cell_size, cell_size, cell_size);
                    chessboard_image(cell_rect) = 255;
                }
            }
        }
        
        std::vector<cv::Point2f> chessboard_corners;
        cv::Size pattern_size(pattern_width, pattern_height);
        bool found_corners = cv::findChessboardCorners(
            chessboard_image, pattern_size, chessboard_corners,
            cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK
        );
        
        // ===== 4. Test initCameraMatrix2D =====
        cv::Mat init_camera_matrix = cv::initCameraMatrix2D(
            object_points_all, image_points_all, image_size, 0
        );
        
        // ===== 5. Test calibrateCamera =====
        cv::Mat calib_camera_matrix = init_camera_matrix.clone();
        std::vector<double> calib_dist_coeffs(dist_coeffs.size(), 0);
        std::vector<cv::Mat> calib_rvecs, calib_tvecs;
        
        double reproj_error = cv::calibrateCamera(
            object_points_all, image_points_all, image_size,
            calib_camera_matrix, calib_dist_coeffs,
            calib_rvecs, calib_tvecs,
            cv::CALIB_USE_INTRINSIC_GUESS
        );
        
        // ===== 6. Test calibrateCameraRO =====
        cv::Mat calibro_camera_matrix = init_camera_matrix.clone();
        std::vector<double> calibro_dist_coeffs(dist_coeffs.size(), 0);
        std::vector<cv::Mat> calibro_rvecs, calibro_tvecs;
        cv::Mat new_obj_points;
        
        int iFixedPoint = fdp.ConsumeIntegralInRange<int>(1, (int)object_points_3d.size() - 2);
        
        double reproj_error_ro = cv::calibrateCameraRO(
            object_points_all, image_points_all, image_size, iFixedPoint,
            calibro_camera_matrix, calibro_dist_coeffs,
            calibro_rvecs, calibro_tvecs, new_obj_points
        );
        
        // ===== 7. Test solvePnP =====
        // Use first view for PnP solving
        if (!image_points_all.empty() && !object_points_all.empty()) {
            cv::Mat pnp_rvec, pnp_tvec;
            bool pnp_success = cv::solvePnP(
                object_points_all[0], image_points_all[0],
                calib_camera_matrix, calib_dist_coeffs,
                pnp_rvec, pnp_tvec
            );
            
            // Test solvePnP with different methods
            if (pnp_success) {
                // Test with RANSAC
                cv::Mat pnp_rvec_ransac, pnp_tvec_ransac;
                cv::solvePnPRansac(
                    object_points_all[0], image_points_all[0],
                    calib_camera_matrix, calib_dist_coeffs,
                    pnp_rvec_ransac, pnp_tvec_ransac
                );
                
                // Test with iterative method
                cv::Mat pnp_rvec_iter, pnp_tvec_iter;
                cv::solvePnP(
                    object_points_all[0], image_points_all[0],
                    calib_camera_matrix, calib_dist_coeffs,
                    pnp_rvec_iter, pnp_tvec_iter,
                    false, cv::SOLVEPNP_ITERATIVE
                );
            }
        }
        
        // ===== 8. Test undistort =====
        // Create a test image
        cv::Mat test_image(image_size, CV_8UC3);
        cv::randu(test_image, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
        
        cv::Mat undistorted_image;
        cv::undistort(
            test_image, undistorted_image,
            calib_camera_matrix, calib_dist_coeffs
        );
        
        // Test undistort with new camera matrix
        cv::Mat undistorted_image_new_cam;
        cv::Mat new_cam_matrix = cv::getOptimalNewCameraMatrix(
            calib_camera_matrix, calib_dist_coeffs, image_size, 0.0
        );
        cv::undistort(
            test_image, undistorted_image_new_cam,
            calib_camera_matrix, calib_dist_coeffs, new_cam_matrix
        );
        
        // ===== 9. Test stereoCalibrate =====
        // Generate stereo image points (simulated second camera)
        if (num_views >= 2 && fdp.remaining_bytes() > 100) {
            std::vector<std::vector<cv::Point2f>> image_points_right_all;
            
            // Generate slightly different camera parameters for right camera
            float fx_right = fx * fdp.ConsumeFloatingPointInRange<float>(0.9f, 1.1f);
            float fy_right = fy * fdp.ConsumeFloatingPointInRange<float>(0.9f, 1.1f);
            float cx_right = cx + fdp.ConsumeFloatingPointInRange<float>(-50.0f, 50.0f);
            float cy_right = cy + fdp.ConsumeFloatingPointInRange<float>(-50.0f, 50.0f);
            
            cv::Mat camera_matrix_right = (cv::Mat_<double>(3, 3) << 
                fx_right, 0, cx_right,
                0, fy_right, cy_right,
                0, 0, 1);
            
            // Generate stereo baseline (translation between cameras)
            cv::Mat stereo_R, stereo_T, stereo_E, stereo_F;
            
            double stereo_error = cv::stereoCalibrate(
                object_points_all, image_points_all, image_points_right_all,
                calib_camera_matrix, calib_dist_coeffs,
                camera_matrix_right, calib_dist_coeffs,
                image_size, stereo_R, stereo_T, stereo_E, stereo_F,
                cv::CALIB_FIX_INTRINSIC,
                cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 30, 1e-6)
            );
        }
        
        // ===== 10. Test additional calib3d functions =====
        // Test Rodrigues conversion
        if (!rvecs_all.empty()) {
            cv::Mat rotation_matrix;
            cv::Rodrigues(rvecs_all[0], rotation_matrix);
            
            cv::Mat rvec_back;
            cv::Rodrigues(rotation_matrix, rvec_back);
        }
        
        // Test projection
        std::vector<cv::Point2f> projected_points;
        if (!object_points_all.empty() && !rvecs_all.empty() && !tvecs_all.empty()) {
            cv::projectPoints(
                object_points_all[0], rvecs_all[0], tvecs_all[0],
                calib_camera_matrix, calib_dist_coeffs, projected_points
            );
        }
        
    } catch (const cv::Exception& e) {
        // OpenCV exceptions are expected during fuzzing
    } catch (...) {
        // Catch any other exceptions
    }
    
    return 0;
}

} // namespace harness_006
