#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <cmath>

namespace harness_011 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing: need enough for points, camera matrix, etc.
    if (size < 256) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    try {
        // Step 1: Consume parameters for number of 3D-2D point correspondences
        int num_points = fdp.ConsumeIntegralInRange(4, 50); // At least 4 points for PnP
        
        // Step 2: Create synthetic 3D object points using random data
        std::vector<cv::Point3f> object_points;
        object_points.reserve(num_points);
        
        // Generate 3D points in reasonable range [-10, 10] meters
        for (int i = 0; i < num_points; ++i) {
            float x = fdp.ConsumeFloatingPointInRange<float>(-10.0f, 10.0f);
            float y = fdp.ConsumeFloatingPointInRange<float>(-10.0f, 10.0f);
            float z = fdp.ConsumeFloatingPointInRange<float>(1.0f, 20.0f); // Ensure points in front of camera
            object_points.push_back(cv::Point3f(x, y, z));
        }
        
        // Step 3: Create synthetic camera pose (ground truth for generating 2D points)
        cv::Vec3f true_rvec(fdp.ConsumeFloatingPoint<float>(),
                           fdp.ConsumeFloatingPoint<float>(),
                           fdp.ConsumeFloatingPoint<float>());
        cv::Vec3f true_tvec(fdp.ConsumeFloatingPointInRange<float>(-5.0f, 5.0f),
                           fdp.ConsumeFloatingPointInRange<float>(-5.0f, 5.0f),
                           fdp.ConsumeFloatingPointInRange<float>(5.0f, 20.0f)); // Camera looking at origin
        
        // Step 4: Create camera intrinsic matrix
        cv::Mat camera_matrix = cv::Mat::eye(3, 3, CV_64F);
        double fx = fdp.ConsumeFloatingPointInRange<double>(300.0, 1000.0);
        double fy = fdp.ConsumeFloatingPointInRange<double>(300.0, 1000.0);
        double cx = fdp.ConsumeFloatingPointInRange<double>(320.0, 640.0);
        double cy = fdp.ConsumeFloatingPointInRange<double>(240.0, 480.0);
        
        camera_matrix.at<double>(0, 0) = fx;
        camera_matrix.at<double>(1, 1) = fy;
        camera_matrix.at<double>(0, 2) = cx;
        camera_matrix.at<double>(1, 2) = cy;
        
        // Step 5: Create distortion coefficients (up to 5 parameters)
        cv::Mat dist_coeffs = cv::Mat::zeros(5, 1, CV_64F);
        for (int i = 0; i < 5; ++i) {
            dist_coeffs.at<double>(i) = fdp.ConsumeFloatingPointInRange<double>(-0.5, 0.5);
        }
        
        // Step 6: Generate 2D image points by projecting 3D points with known pose
        std::vector<cv::Point2f> image_points;
        std::vector<float> proj_errors;
        
        cv::projectPoints(object_points, true_rvec, true_tvec, camera_matrix, dist_coeffs, image_points, cv::noArray(), 0.0);
        
        // Add some noise to simulate real measurements (optional)
        for (auto& pt : image_points) {
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
                pt.x += fdp.ConsumeFloatingPointInRange<float>(-2.0f, 2.0f);
                pt.y += fdp.ConsumeFloatingPointInRange<float>(-2.0f, 2.0f);
            }
        }
        
        // Step 7: Estimate pose using solvePnP with different methods
        cv::Vec3d estimated_rvec;
        cv::Vec3d estimated_tvec;
        
        // Choose solvePnP method based on fuzzer input
        int method_choice = fdp.ConsumeIntegralInRange(0, 5);
        cv::SolvePnPMethod method;
        switch (method_choice) {
            case 0: method = cv::SOLVEPNP_ITERATIVE; break;
            case 1: method = cv::SOLVEPNP_EPNP; break;
            case 2: method = cv::SOLVEPNP_P3P; break;
            case 3: method = cv::SOLVEPNP_AP3P; break;
            case 4: method = cv::SOLVEPNP_DLS; break;
            default: method = cv::SOLVEPNP_UPNP; break;
        }
        
        bool use_extrinsic_guess = fdp.ConsumeBool();
        
        // Initial guess if requested
        cv::Vec3d init_rvec, init_tvec;
        if (use_extrinsic_guess && fdp.remaining_bytes() > 12) {
            init_rvec = cv::Vec3d(fdp.ConsumeFloatingPoint<double>(),
                                 fdp.ConsumeFloatingPoint<double>(),
                                 fdp.ConsumeFloatingPoint<double>());
            init_tvec = cv::Vec3d(fdp.ConsumeFloatingPointInRange<double>(-3.0, 3.0),
                                 fdp.ConsumeFloatingPointInRange<double>(-3.0, 3.0),
                                 fdp.ConsumeFloatingPointInRange<double>(3.0, 15.0));
        }
        
        bool success = false;
        if (use_extrinsic_guess) {
            success = cv::solvePnP(object_points, image_points, camera_matrix, dist_coeffs,
                                  estimated_rvec, estimated_tvec, true, method);
        } else {
            success = cv::solvePnP(object_points, image_points, camera_matrix, dist_coeffs,
                                  estimated_rvec, estimated_tvec, false, method);
        }
        
        // Step 8: If pose estimation succeeded, verify with projectPoints
        if (success) {
            std::vector<cv::Point2f> reprojected_points;
            cv::projectPoints(object_points, estimated_rvec, estimated_tvec, 
                            camera_matrix, dist_coeffs, reprojected_points);
            
            // Optional: Convert rotation vector to matrix using Rodrigues
            cv::Mat rotation_matrix;
            cv::Rodrigues(estimated_rvec, rotation_matrix);
            
            // Step 9: Test solvePnPRansac with RANSAC robust estimation
            if (fdp.remaining_bytes() > 20) {
                cv::Vec3d ransac_rvec, ransac_tvec;
                std::vector<int> inliers;
                
                int iterations = fdp.ConsumeIntegralInRange(50, 500);
                float reproj_error = fdp.ConsumeFloatingPointInRange<float>(1.0f, 10.0f);
                double confidence = fdp.ConsumeFloatingPointInRange<double>(0.9, 0.999);
                
                bool ransac_success = cv::solvePnPRansac(object_points, image_points, 
                                                       camera_matrix, dist_coeffs,
                                                       ransac_rvec, ransac_tvec,
                                                       false, iterations, reproj_error,
                                                       confidence, inliers, method);
                
                if (ransac_success && !inliers.empty()) {
                    // Test projectPoints with RANSAC result
                    std::vector<cv::Point2f> ransac_reprojected;
                    cv::projectPoints(object_points, ransac_rvec, ransac_tvec,
                                    camera_matrix, dist_coeffs, ransac_reprojected);
                }
            }
            
            // Step 10: Test solvePnPGeneric for multiple solutions
            if (fdp.remaining_bytes() > 10) {
                std::vector<cv::Mat> rvecs_generic, tvecs_generic;
                int num_solutions = cv::solvePnPGeneric(object_points, image_points,
                                                       camera_matrix, dist_coeffs,
                                                       rvecs_generic, tvecs_generic,
                                                       false, method);
                
                if (num_solutions > 0) {
                    // Test projectPoints for each solution
                    for (int i = 0; i < num_solutions && i < 2; ++i) { // Limit to first 2 solutions
                        std::vector<cv::Point2f> generic_reprojected;
                        cv::projectPoints(object_points, rvecs_generic[i], tvecs_generic[i],
                                        camera_matrix, dist_coeffs, generic_reprojected);
                    }
                }
            }
        }
        
        // Step 11: Optionally test calibrateCamera with multiple views
        if (fdp.remaining_bytes() > 100) {
            int num_views = fdp.ConsumeIntegralInRange(1, 3);
            std::vector<std::vector<cv::Point3f>> all_object_points;
            std::vector<std::vector<cv::Point2f>> all_image_points;
            
            // Create synthetic views with different poses
            for (int view = 0; view < num_views && fdp.remaining_bytes() > 50; ++view) {
                // Use same 3D points for all views
                all_object_points.push_back(object_points);
                
                // Generate different pose for each view
                cv::Vec3f view_rvec(fdp.ConsumeFloatingPoint<float>(),
                                   fdp.ConsumeFloatingPoint<float>(),
                                   fdp.ConsumeFloatingPoint<float>());
                cv::Vec3f view_tvec(fdp.ConsumeFloatingPointInRange<float>(-5.0f, 5.0f),
                                   fdp.ConsumeFloatingPointInRange<float>(-5.0f, 5.0f),
                                   fdp.ConsumeFloatingPointInRange<float>(5.0f, 20.0f));
                
                // Project points for this view
                std::vector<cv::Point2f> view_image_points;
                cv::projectPoints(object_points, view_rvec, view_tvec, 
                                camera_matrix, dist_coeffs, view_image_points);
                
                all_image_points.push_back(view_image_points);
            }
            
            if (num_views > 1) {
                cv::Mat calib_camera_matrix, calib_dist_coeffs;
                std::vector<cv::Mat> rvecs, tvecs;
                
                // Try camera calibration with different flags
                int flags = 0;
                if (fdp.ConsumeBool()) flags |= cv::CALIB_USE_INTRINSIC_GUESS;
                if (fdp.ConsumeBool()) flags |= cv::CALIB_FIX_PRINCIPAL_POINT;
                if (fdp.ConsumeBool()) flags |= cv::CALIB_FIX_ASPECT_RATIO;
                
                double rms = cv::calibrateCamera(all_object_points, all_image_points,
                                                cv::Size(640, 480), calib_camera_matrix,
                                                calib_dist_coeffs, rvecs, tvecs, flags);
                
                // Use calibrated parameters for pose estimation
                if (rms >= 0 && !calib_camera_matrix.empty() && !all_image_points.empty()) {
                    cv::Vec3d calib_rvec, calib_tvec;
                    bool calib_success = cv::solvePnP(all_object_points[0], all_image_points[0],
                                                     calib_camera_matrix, calib_dist_coeffs,
                                                     calib_rvec, calib_tvec, false, method);
                    
                    if (calib_success) {
                        std::vector<cv::Point2f> calib_reprojected;
                        cv::projectPoints(all_object_points[0], calib_rvec, calib_tvec,
                                        calib_camera_matrix, calib_dist_coeffs, calib_reprojected);
                    }
                }
            }
        }
        
        // Step 12: Test refine functions if pose estimation was successful
        if (success && fdp.remaining_bytes() > 10) {
            cv::Vec3d refined_rvec = estimated_rvec;
            cv::Vec3d refined_tvec = estimated_tvec;
            
            if (fdp.ConsumeBool()) {
                // Levenberg-Marquardt refinement
                cv::solvePnPRefineLM(object_points, image_points, camera_matrix, dist_coeffs,
                                    refined_rvec, refined_tvec);
            } else if (fdp.ConsumeBool()) {
                // Virtual Visual Servoing refinement
                cv::solvePnPRefineVVS(object_points, image_points, camera_matrix, dist_coeffs,
                                     refined_rvec, refined_tvec);
            }
            
            // Test projectPoints with refined pose
            std::vector<cv::Point2f> refined_reprojected;
            cv::projectPoints(object_points, refined_rvec, refined_tvec,
                            camera_matrix, dist_coeffs, refined_reprojected);
        }
        
    } catch (const cv::Exception& e) {
        // Ignore OpenCV exceptions - they are expected for invalid inputs
    } catch (...) {
        // Ignore any other exceptions
    }
    
    return 0;
}

} // namespace harness_011
