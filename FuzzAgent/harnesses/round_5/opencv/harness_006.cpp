#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <cstddef>
#include <cstdint>

namespace harness_006 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing: need enough data for at least partial image data
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    try {
        // Split input into two parts for two images
        size_t first_part_size = fdp.ConsumeIntegralInRange<size_t>(32, size / 2);
        std::vector<uint8_t> image1_data = fdp.ConsumeBytes<uint8_t>(first_part_size);
        std::vector<uint8_t> image2_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        // Skip if either image data is too small
        if (image1_data.size() < 32 || image2_data.size() < 32) return 0;
        
        // Decode first image
        cv::Mat img1 = cv::imdecode(cv::Mat(1, image1_data.size(), CV_8UC1, image1_data.data()), 
                                    cv::IMREAD_GRAYSCALE);
        
        // Decode second image
        cv::Mat img2 = cv::imdecode(cv::Mat(1, image2_data.size(), CV_8UC1, image2_data.data()), 
                                    cv::IMREAD_GRAYSCALE);
        
        // Skip if either image failed to decode
        if (img1.empty() || img2.empty()) return 0;
        
        // Step 2: Create ORB detector with parameters from fuzzer input
        int nfeatures = fdp.ConsumeIntegralInRange(100, 1000);
        float scaleFactor = fdp.ConsumeFloatingPointInRange<float>(1.1f, 2.0f);
        int nlevels = fdp.ConsumeIntegralInRange(1, 10);
        int edgeThreshold = fdp.ConsumeIntegralInRange(10, 50);
        int firstLevel = fdp.ConsumeIntegralInRange(0, 3);
        int WTA_K = fdp.ConsumeIntegralInRange(2, 4);
        int scoreTypeChoice = fdp.ConsumeIntegralInRange(0, 1);
        cv::ORB::ScoreType scoreType = (scoreTypeChoice == 0) ? cv::ORB::HARRIS_SCORE : cv::ORB::FAST_SCORE;
        int patchSize = fdp.ConsumeIntegralInRange(20, 40);
        int fastThreshold = fdp.ConsumeIntegralInRange(10, 30);
        
        cv::Ptr<cv::ORB> orb_detector = cv::ORB::create(nfeatures, scaleFactor, nlevels, 
                                                         edgeThreshold, firstLevel, WTA_K, 
                                                         scoreType, patchSize, fastThreshold);
        
        // Step 3: Detect and compute features/descriptors for both images
        std::vector<cv::KeyPoint> keypoints1, keypoints2;
        cv::Mat descriptors1, descriptors2;
        
        orb_detector->detectAndCompute(img1, cv::noArray(), keypoints1, descriptors1);
        orb_detector->detectAndCompute(img2, cv::noArray(), keypoints2, descriptors2);
        
        // Skip if not enough keypoints in either image
        if (keypoints1.size() < 8 || keypoints2.size() < 8 || 
            descriptors1.empty() || descriptors2.empty()) return 0;
        
        // Step 4: Create BFMatcher with parameters from fuzzer input
        int normTypeChoice = fdp.ConsumeIntegralInRange(0, 3);
        int normType;
        switch (normTypeChoice) {
            case 0: normType = cv::NORM_L1; break;
            case 1: normType = cv::NORM_L2; break;
            case 2: normType = cv::NORM_HAMMING; break;
            default: normType = cv::NORM_HAMMING2; break;
        }
        bool crossCheck = fdp.ConsumeBool();
        
        cv::Ptr<cv::BFMatcher> matcher = cv::BFMatcher::create(normType, crossCheck);
        
        // Step 5: Match descriptors
        std::vector<cv::DMatch> matches;
        matcher->match(descriptors1, descriptors2, matches);
        
        // Skip if not enough matches for fundamental matrix
        if (matches.size() < 8) return 0;
        
        // Prepare point arrays for fundamental matrix computation
        std::vector<cv::Point2f> points1, points2;
        points1.reserve(matches.size());
        points2.reserve(matches.size());
        
        for (const auto& match : matches) {
            points1.push_back(keypoints1[match.queryIdx].pt);
            points2.push_back(keypoints2[match.trainIdx].pt);
        }
        
        // Step 6: Compute fundamental matrix with parameters from fuzzer input
        int methodChoice = fdp.ConsumeIntegralInRange(0, 3);
        int method;
        switch (methodChoice) {
            case 0: method = cv::FM_7POINT; break;
            case 1: method = cv::FM_8POINT; break;
            case 2: method = cv::FM_RANSAC; break;
            default: method = cv::FM_LMEDS; break;
        }
        
        double ransacReprojThreshold = fdp.ConsumeFloatingPointInRange<double>(1.0, 10.0);
        double confidence = fdp.ConsumeFloatingPointInRange<double>(0.5, 0.999);
        
        cv::Mat mask;
        cv::Mat fundamental_matrix = cv::findFundamentalMat(points1, points2, method, 
                                                           ransacReprojThreshold, confidence, 
                                                           1000, mask);
        
        // Step 7: Cleanup happens automatically when objects go out of scope
        // All cv::Ptr objects and cv::Mat objects will be automatically released
        
    } catch (const cv::Exception& e) {
        // Ignore OpenCV exceptions - they are expected for invalid inputs
    } catch (...) {
        // Ignore any other exceptions
    }
    
    return 0;
}

} // namespace harness_006
