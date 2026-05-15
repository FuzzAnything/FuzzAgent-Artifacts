#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>

namespace harness_012 {
// OpenCV features2d module SIFT feature detection fuzzing harness
// Targets: features2d module with SIFT feature detection, description, and matching (0% coverage)
// harness_012.cpp - Targets SIFT feature detection operations in features2d module
// Different from previous harnesses (image processing, mathematical functions, dnn, tracking, segmentation, channel operations)
// by focusing exclusively on features2d module APIs with SIFT for maximum coverage gain


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic SIFT operations
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    try {
        using namespace cv;
        
        // Determine test scenario from fuzzer input
        uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 5;
        
        // Common parameters for SIFT
        int nfeatures = fdp.ConsumeIntegralInRange<int>(0, 1000);
        int nOctaveLayers = fdp.ConsumeIntegralInRange<int>(1, 10);
        double contrastThreshold = fdp.ConsumeFloatingPointInRange<double>(0.001, 0.1);
        double edgeThreshold = fdp.ConsumeFloatingPointInRange<double>(1.0, 50.0);
        double sigma = fdp.ConsumeFloatingPointInRange<double>(0.5, 3.0);
        int descriptorType = fdp.ConsumeBool() ? CV_32F : CV_8U;
        bool enable_precise_upscale = fdp.ConsumeBool();
        
        // Create SIFT detector with different parameter combinations
        Ptr<SIFT> sift;
        
        switch (scenario % 5) {
            case 0: {
                // Scenario 0: Default SIFT creation and basic detection
                sift = SIFT::create(nfeatures, nOctaveLayers, contrastThreshold, 
                                   edgeThreshold, sigma, descriptorType, enable_precise_upscale);
                
                // Test detector properties
                int actual_nfeatures = sift->getNFeatures();
                int actual_nOctaveLayers = sift->getNOctaveLayers();
                double actual_contrastThreshold = sift->getContrastThreshold();
                double actual_edgeThreshold = sift->getEdgeThreshold();
                double actual_sigma = sift->getSigma();
                
                (void)actual_nfeatures;
                (void)actual_nOctaveLayers;
                (void)actual_contrastThreshold;
                (void)actual_edgeThreshold;
                (void)actual_sigma;
                
                // Create synthetic test image
                int rows = fdp.ConsumeIntegralInRange<int>(10, 100);
                int cols = fdp.ConsumeIntegralInRange<int>(10, 100);
                int channels = fdp.ConsumeIntegralInRange<int>(1, 3);
                
                size_t image_size = rows * cols * channels;
                if (fdp.remaining_bytes() < image_size) {
                    return 0;
                }
                
                std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_size);
                Mat test_image;
                
                if (channels == 1) {
                    test_image = Mat(rows, cols, CV_8UC1, image_data.data());
                } else if (channels == 3) {
                    test_image = Mat(rows, cols, CV_8UC3, image_data.data());
                }
                
                if (!test_image.empty()) {
                    // Convert to grayscale for SIFT if needed
                    Mat gray_image;
                    if (test_image.channels() == 3) {
                        cvtColor(test_image, gray_image, COLOR_BGR2GRAY);
                    } else {
                        gray_image = test_image;
                    }
                    
                    // Detect keypoints
                    std::vector<KeyPoint> keypoints;
                    sift->detect(gray_image, keypoints);
                    
                    // Test keypoint filtering
                    if (!keypoints.empty()) {
                        // Test KeyPointsFilter functions
                        std::vector<KeyPoint> filtered_kps = keypoints;
                        KeyPointsFilter::runByImageBorder(filtered_kps, gray_image.size(), 10);
                        
                        // Test removing duplicate keypoints
                        filtered_kps = keypoints;
                        KeyPointsFilter::removeDuplicated(filtered_kps);
                        
                        // Test retaining best keypoints
                        filtered_kps = keypoints;
                        if (filtered_kps.size() > 10) {
                            KeyPointsFilter::retainBest(filtered_kps, 10);
                        }
                    }
                }
                break;
            }
            
            case 1: {
                // Scenario 1: SIFT detectAndCompute with descriptors
                sift = SIFT::create(nfeatures, nOctaveLayers, contrastThreshold,
                                   edgeThreshold, sigma, descriptorType, enable_precise_upscale);
                
                // Create synthetic test image
                int rows = fdp.ConsumeIntegralInRange<int>(20, 200);
                int cols = fdp.ConsumeIntegralInRange<int>(20, 200);
                
                size_t image_size = rows * cols;
                if (fdp.remaining_bytes() < image_size) {
                    return 0;
                }
                
                std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_size);
                Mat gray_image(rows, cols, CV_8UC1, image_data.data());
                
                if (!gray_image.empty()) {
                    std::vector<KeyPoint> keypoints;
                    Mat descriptors;
                    
                    // Test detectAndCompute with no mask
                    sift->detectAndCompute(gray_image, Mat(), keypoints, descriptors, false);
                    
                    // Test compute only with random keypoints
                    if (!keypoints.empty() && fdp.remaining_bytes() >= 16) {
                        // Create some synthetic keypoints
                        std::vector<KeyPoint> synthetic_kps;
                        int num_kps = fdp.ConsumeIntegralInRange<int>(1, 20);
                        
                        for (int i = 0; i < num_kps && fdp.remaining_bytes() >= 16; i++) {
                            float x = fdp.ConsumeFloatingPointInRange<float>(0, cols-1);
                            float y = fdp.ConsumeFloatingPointInRange<float>(0, rows-1);
                            float size = fdp.ConsumeFloatingPointInRange<float>(1.0, 50.0);
                            float angle = fdp.ConsumeFloatingPointInRange<float>(-180.0, 180.0);
                            float response = fdp.ConsumeFloatingPointInRange<float>(0, 100.0);
                            int octave = fdp.ConsumeIntegralInRange<int>(0, 5);
                            int class_id = fdp.ConsumeIntegralInRange<int>(-1, 10);
                            
                            synthetic_kps.push_back(KeyPoint(x, y, size, angle, response, octave, class_id));
                        }
                        
                        if (!synthetic_kps.empty()) {
                            Mat computed_descriptors;
                            sift->compute(gray_image, synthetic_kps, computed_descriptors);
                        }
                    }
                }
                break;
            }
            
            case 2: {
                // Scenario 2: Multiple SIFT instances with different parameters
                // Create first SIFT instance
                Ptr<SIFT> sift1 = SIFT::create(
                    fdp.ConsumeIntegralInRange<int>(0, 500),
                    fdp.ConsumeIntegralInRange<int>(1, 5),
                    fdp.ConsumeFloatingPointInRange<double>(0.01, 0.05),
                    fdp.ConsumeFloatingPointInRange<double>(5.0, 20.0),
                    fdp.ConsumeFloatingPointInRange<double>(1.0, 2.0),
                    CV_32F,
                    fdp.ConsumeBool()
                );
                
                // Create second SIFT instance with different parameters
                Ptr<SIFT> sift2 = SIFT::create(
                    fdp.ConsumeIntegralInRange<int>(500, 1000),
                    fdp.ConsumeIntegralInRange<int>(3, 8),
                    fdp.ConsumeFloatingPointInRange<double>(0.03, 0.09),
                    fdp.ConsumeFloatingPointInRange<double>(10.0, 30.0),
                    fdp.ConsumeFloatingPointInRange<double>(1.2, 2.4),
                    CV_8U,
                    fdp.ConsumeBool()
                );
                
                // Test with a simple gradient image
                int rows = fdp.ConsumeIntegralInRange<int>(30, 150);
                int cols = fdp.ConsumeIntegralInRange<int>(30, 150);
                
                size_t image_size = rows * cols;
                if (fdp.remaining_bytes() < image_size) {
                    return 0;
                }
                
                std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_size);
                Mat test_image(rows, cols, CV_8UC1, image_data.data());
                
                if (!test_image.empty()) {
                    // Test both detectors on the same image
                    std::vector<KeyPoint> keypoints1, keypoints2;
                    Mat descriptors1, descriptors2;
                    
                    sift1->detectAndCompute(test_image, Mat(), keypoints1, descriptors1);
                    sift2->detectAndCompute(test_image, Mat(), keypoints2, descriptors2);
                    
                    // Compare results
                    size_t kp1_count = keypoints1.size();
                    size_t kp2_count = keypoints2.size();
                    (void)kp1_count;
                    (void)kp2_count;
                    
                    // Test descriptor properties
                    if (!descriptors1.empty()) {
                        int desc_size1 = descriptors1.rows * descriptors1.cols;
                        int desc_type1 = descriptors1.type();
                        (void)desc_size1;
                        (void)desc_type1;
                    }
                    
                    if (!descriptors2.empty()) {
                        int desc_size2 = descriptors2.rows * descriptors2.cols;
                        int desc_type2 = descriptors2.type();
                        (void)desc_size2;
                        (void)desc_type2;
                    }
                }
                break;
            }
            
            case 3: {
                // Scenario 3: SIFT with mask operations
                sift = SIFT::create(nfeatures, nOctaveLayers, contrastThreshold,
                                   edgeThreshold, sigma, descriptorType, enable_precise_upscale);
                
                // Create test image and mask
                int rows = fdp.ConsumeIntegralInRange<int>(40, 120);
                int cols = fdp.ConsumeIntegralInRange<int>(40, 120);
                
                size_t image_size = rows * cols;
                size_t mask_size = rows * cols;
                
                if (fdp.remaining_bytes() < image_size + mask_size) {
                    return 0;
                }
                
                std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_size);
                std::vector<uint8_t> mask_data = fdp.ConsumeBytes<uint8_t>(mask_size);
                
                Mat test_image(rows, cols, CV_8UC1, image_data.data());
                Mat mask(rows, cols, CV_8UC1, mask_data.data());
                
                // Threshold mask to create binary mask
                threshold(mask, mask, 128, 255, THRESH_BINARY);
                
                if (!test_image.empty()) {
                    std::vector<KeyPoint> keypoints_with_mask;
                    Mat descriptors_with_mask;
                    
                    // Test detectAndCompute with mask
                    sift->detectAndCompute(test_image, mask, keypoints_with_mask, descriptors_with_mask, false);
                    
                    // Also test without mask for comparison
                    std::vector<KeyPoint> keypoints_no_mask;
                    Mat descriptors_no_mask;
                    sift->detectAndCompute(test_image, Mat(), keypoints_no_mask, descriptors_no_mask, false);
                    
                    // Keypoint count comparison
                    size_t with_mask_count = keypoints_with_mask.size();
                    size_t no_mask_count = keypoints_no_mask.size();
                    (void)with_mask_count;
                    (void)no_mask_count;
                }
                break;
            }
            
            case 4: {
                // Scenario 4: SIFT parameter modification and re-detection
                sift = SIFT::create();
                
                // Test default parameter getters
                int default_nfeatures = sift->getNFeatures();
                int default_nOctaveLayers = sift->getNOctaveLayers();
                double default_contrastThreshold = sift->getContrastThreshold();
                double default_edgeThreshold = sift->getEdgeThreshold();
                double default_sigma = sift->getSigma();
                
                (void)default_nfeatures;
                (void)default_nOctaveLayers;
                (void)default_contrastThreshold;
                (void)default_edgeThreshold;
                (void)default_sigma;
                
                // Modify parameters
                sift->setNFeatures(nfeatures);
                sift->setNOctaveLayers(nOctaveLayers);
                sift->setContrastThreshold(contrastThreshold);
                sift->setEdgeThreshold(edgeThreshold);
                sift->setSigma(sigma);
                
                // Verify modified parameters
                int modified_nfeatures = sift->getNFeatures();
                int modified_nOctaveLayers = sift->getNOctaveLayers();
                double modified_contrastThreshold = sift->getContrastThreshold();
                double modified_edgeThreshold = sift->getEdgeThreshold();
                double modified_sigma = sift->getSigma();
                
                (void)modified_nfeatures;
                (void)modified_nOctaveLayers;
                (void)modified_contrastThreshold;
                (void)modified_edgeThreshold;
                (void)modified_sigma;
                
                // Test with image after parameter modification
                int rows = fdp.ConsumeIntegralInRange<int>(50, 100);
                int cols = fdp.ConsumeIntegralInRange<int>(50, 100);
                
                size_t image_size = rows * cols;
                if (fdp.remaining_bytes() < image_size) {
                    return 0;
                }
                
                std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_size);
                Mat test_image(rows, cols, CV_8UC1, image_data.data());
                
                if (!test_image.empty()) {
                    std::vector<KeyPoint> keypoints;
                    Mat descriptors;
                    
                    // Detect with modified parameters
                    sift->detectAndCompute(test_image, Mat(), keypoints, descriptors, false);
                    
                    // Test descriptor normalization
                    if (!descriptors.empty() && descriptors.type() == CV_32F) {
                        // Simple descriptor analysis
                        double min_val, max_val;
                        minMaxLoc(descriptors, &min_val, &max_val);
                        
                        // Test descriptor statistics
                        Scalar mean_val = mean(descriptors);
                        Scalar stddev_val;
                        meanStdDev(descriptors, mean_val, stddev_val);
                        
                        (void)min_val;
                        (void)max_val;
                    }
                }
                break;
            }
        }
        
    } catch (const cv::Exception& e) {
        // OpenCV exceptions are expected with random input
        (void)e;
    } catch (...) {
        // Catch any other exceptions
    }
    
    return 0;
}

} // namespace harness_012
