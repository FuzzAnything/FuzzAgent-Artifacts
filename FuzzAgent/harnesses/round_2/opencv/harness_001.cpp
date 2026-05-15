#include <cstddef>
#include <cstdint>
#include <unistd.h>
#include <vector>
#include <string>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/imgcodecs/legacy/constants_c.h>
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_001 {
// Second OpenCV fuzzing harness focusing on image encoding, 
// FileStorage operations, and advanced matrix functions

// Test FileStorage read operations
void test_filestorage_read(const std::vector<uint8_t>& data) {
  if (data.empty()) return;
  
  try {
    // Create a temporary file with the data
    char filename[] = "/tmp/filestorage_fuzz_XXXXXX";
    int fd = mkstemp(filename);
    if (fd < 0) return;
    
    FILE* file = fdopen(fd, "wb");
    if (!file) {
      close(fd);
      return;
    }
    
    fwrite(data.data(), 1, data.size(), file);
    fclose(file);
    
    // Try to read with FileStorage
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if (fs.isOpened()) {
      cv::Mat mat;
      fs["data"] >> mat; // Try to read some data
      fs.release();
    }
    
    unlink(filename);
  } catch (const cv::Exception& e) {
    // Silently catch OpenCV exceptions during fuzzing
  }
}

// Test advanced matrix operations
void test_advanced_matrix_ops(const cv::Mat& mat) {
  if (mat.empty()) return;
  
  try {
    // Test matrix inversion for square matrices
    if (mat.rows == mat.cols && mat.rows > 0) {
      cv::Mat inverted;
      if (mat.rows <= 10) { // Limit size for performance
        cv::invert(mat, inverted);
      }
    }
    
    // Test matrix multiplication for compatible sizes
    if (mat.cols > 0 && mat.rows > 0) {
      cv::Mat result;
      if (mat.cols == mat.rows && mat.cols <= 10) {
        result = mat * mat.t(); // Multiply by transpose
      }
    }
    
    // Test matrix norms
    double norm_l2 = cv::norm(mat, cv::NORM_L2);
    double norm_l1 = cv::norm(mat, cv::NORM_L1);
    double norm_inf = cv::norm(mat, cv::NORM_INF);
    
    // Test matrix min/max
    double min_val, max_val;
    cv::minMaxLoc(mat, &min_val, &max_val);
    
    // Test matrix thresholding
    if (mat.type() == CV_8UC1) {
      cv::Mat thresholded;
      cv::threshold(mat, thresholded, 128, 255, cv::THRESH_BINARY);
    }
    
  } catch (const cv::Exception& e) {
    // Silently catch OpenCV exceptions during fuzzing
  }
}

// Test image encoding with different formats
void test_multiple_encodings(const cv::Mat& image) {
  if (image.empty()) return;
  
  try {
    std::vector<uint8_t> buffer;
    std::vector<int> params;
    
    // Try PNG encoding
    if (cv::imencode(".png", image, buffer, params)) {
      // Decode it back
      cv::Mat decoded = cv::imdecode(buffer, CV_LOAD_IMAGE_UNCHANGED);
    }
    
    // Try JPEG encoding if image is appropriate
    if (image.channels() == 3 || image.channels() == 1) {
      buffer.clear();
      params = std::vector<int>{cv::IMWRITE_JPEG_QUALITY, 90};
      if (cv::imencode(".jpg", image, buffer, params)) {
        cv::Mat decoded = cv::imdecode(buffer, CV_LOAD_IMAGE_UNCHANGED);
      }
    }
    
    // Try WebP encoding if supported
    buffer.clear();
    if (cv::imencode(".webp", image, buffer)) {
      cv::Mat decoded = cv::imdecode(buffer, CV_LOAD_IMAGE_UNCHANGED);
    }
    
  } catch (const cv::Exception& e) {
    // Silently catch OpenCV exceptions during fuzzing
  }
}

// Test image color space conversions
void test_color_conversions(const cv::Mat& image) {
  if (image.empty()) return;
  
  try {
    cv::Mat converted;
    
    // Test common color space conversions
    if (image.channels() == 3) {
      // BGR to grayscale
      cv::cvtColor(image, converted, cv::COLOR_BGR2GRAY);
      
      // BGR to HSV
      cv::cvtColor(image, converted, cv::COLOR_BGR2HSV);
      
      // BGR to LAB
      cv::cvtColor(image, converted, cv::COLOR_BGR2Lab);
    } else if (image.channels() == 1) {
      // Grayscale to BGR
      cv::cvtColor(image, converted, cv::COLOR_GRAY2BGR);
    }
    
  } catch (const cv::Exception& e) {
    // Silently catch OpenCV exceptions during fuzzing
  }
}

// Test image filtering operations
void test_image_filtering(const cv::Mat& image) {
  if (image.empty()) return;
  
  try {
    cv::Mat filtered;
    
    // Gaussian blur
    if (image.rows > 3 && image.cols > 3) {
      cv::GaussianBlur(image, filtered, cv::Size(3, 3), 0.5);
    }
    
    // Median blur (only for odd kernel sizes)
    if (image.rows > 3 && image.cols > 3) {
      cv::medianBlur(image, filtered, 3);
    }
    
    // Sobel edge detection
    if (image.channels() == 1 && image.rows > 3 && image.cols > 3) {
      cv::Mat grad_x, grad_y;
      cv::Sobel(image, grad_x, CV_16S, 1, 0, 3);
      cv::Sobel(image, grad_y, CV_16S, 0, 1, 3);
    }
    
  } catch (const cv::Exception& e) {
    // Silently catch OpenCV exceptions during fuzzing
  }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  if (size < 16) return 0; // Minimum size for meaningful testing
  
  FuzzedDataProvider fdp(data, size);
  
  // Consume test mode from fuzzer input
  uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 5;
  size_t max_image_size = fdp.ConsumeIntegralInRange<size_t>(100, 5000);
  
  // Consume image/data from remaining input
  std::vector<uint8_t> input_data = fdp.ConsumeBytes<uint8_t>(
      std::min(max_image_size, fdp.remaining_bytes()));
  
  switch (test_mode) {
    case 0:
      // Test FileStorage operations
      test_filestorage_read(input_data);
      break;
      
    case 1: {
      // Test image decoding and advanced operations
      if (!input_data.empty()) {
        try {
          cv::Mat decoded = cv::imdecode(input_data, CV_LOAD_IMAGE_UNCHANGED);
          if (!decoded.empty()) {
            test_advanced_matrix_ops(decoded);
          }
        } catch (const cv::Exception& e) {
          // Silently catch OpenCV exceptions during fuzzing
        }
      }
      break;
    }
      
    case 2: {
      // Test image encoding with multiple formats
      if (!input_data.empty()) {
        try {
          cv::Mat decoded = cv::imdecode(input_data, CV_LOAD_IMAGE_UNCHANGED);
          if (!decoded.empty()) {
            test_multiple_encodings(decoded);
          }
        } catch (const cv::Exception& e) {
          // Silently catch OpenCV exceptions during fuzzing
        }
      }
      break;
    }
      
    case 3: {
      // Test color space conversions
      if (!input_data.empty()) {
        try {
          cv::Mat decoded = cv::imdecode(input_data, CV_LOAD_IMAGE_UNCHANGED);
          if (!decoded.empty()) {
            test_color_conversions(decoded);
          }
        } catch (const cv::Exception& e) {
          // Silently catch OpenCV exceptions during fuzzing
        }
      }
      break;
    }
      
    case 4: {
      // Test image filtering operations
      if (!input_data.empty()) {
        try {
          cv::Mat decoded = cv::imdecode(input_data, CV_LOAD_IMAGE_UNCHANGED);
          if (!decoded.empty()) {
            test_image_filtering(decoded);
          }
        } catch (const cv::Exception& e) {
          // Silently catch OpenCV exceptions during fuzzing
        }
      }
      break;
    }
  }
  
  return 0;
}

} // namespace harness_001
