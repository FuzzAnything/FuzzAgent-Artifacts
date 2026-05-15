#include <cstddef>
#include <cstdint>
#include <unistd.h>
#include <vector>
#include <string>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/imgcodecs/legacy/constants_c.h>
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_000 {
// First comprehensive OpenCV fuzzing harness
// Combines image decoding, matrix operations, and basic image processing

// Temporary file utility for file-based APIs
static char* fuzzer_get_tmpfile(const uint8_t* data, size_t size) {
  char* filename_buffer = strdup("/tmp/opencv_fuzz_temp.XXXXXX");
  if (!filename_buffer) {
    perror("Failed to allocate file name buffer.");
    abort();
  }
  const int file_descriptor = mkstemp(filename_buffer);
  if (file_descriptor < 0) {
    perror("Failed to make temporary file.");
    abort();
  }
  FILE* file = fdopen(file_descriptor, "wb");
  if (!file) {
    perror("Failed to open file descriptor.");
    close(file_descriptor);
    abort();
  }
  const size_t bytes_written = fwrite(data, sizeof(uint8_t), size, file);
  if (bytes_written < size) {
    close(file_descriptor);
    fprintf(stderr, "Failed to write all bytes to file (%zu out of %zu)",
            bytes_written, size);
    abort();
  }
  fclose(file);
  return filename_buffer;
}

static void fuzzer_release_tmpfile(char* filename) {
  if (unlink(filename) != 0) {
    perror("WARNING: Failed to delete temporary file.");
  }
  free(filename);
}

// Test image decoding from raw data
bool test_imdecode(const std::vector<uint8_t>& image_data) {
  if (image_data.empty()) return false;
  try {
    cv::Mat decoded = cv::imdecode(image_data, CV_LOAD_IMAGE_UNCHANGED);
    return !decoded.empty();
  } catch (const cv::Exception& e) {
    // Silently catch OpenCV exceptions during fuzzing
    return false;
  }
}

// Test file-based image reading
bool test_imread(const uint8_t* data, size_t size) {
  if (size == 0) return false;
  
  char* temp_filename = fuzzer_get_tmpfile(data, size);
  if (!temp_filename) return false;
  
  try {
    cv::Mat image = cv::imread(temp_filename);
    fuzzer_release_tmpfile(temp_filename);
    return !image.empty();
  } catch (const cv::Exception& e) {
    fuzzer_release_tmpfile(temp_filename);
    return false;
  }
}

// Test basic matrix operations
void test_matrix_operations(const cv::Mat& mat) {
  if (mat.empty()) return;
  
  try {
    // Basic matrix operations
    cv::Mat transposed = mat.t();
    cv::Mat cloned = mat.clone();
    
    // Matrix statistics
    if (mat.rows > 0 && mat.cols > 0) {
      cv::Scalar sum_result = cv::sum(mat);
      cv::Scalar mean_result = cv::mean(mat);
      cv::Scalar stddev_result;
      cv::meanStdDev(mat, mean_result, stddev_result);
    }
    
    // Test split and merge for multi-channel images
    if (mat.channels() > 1) {
      std::vector<cv::Mat> channels;
      cv::split(mat, channels);
      if (!channels.empty()) {
        cv::Mat merged;
        cv::merge(channels, merged);
      }
    }
    
    // Test basic transformations
    if (mat.rows > 10 && mat.cols > 10) {
      cv::Mat resized;
      cv::resize(mat, resized, cv::Size(mat.cols/2, mat.rows/2));
      
      cv::Mat cropped = mat(cv::Rect(0, 0, 
                                    std::min(10, mat.cols), 
                                    std::min(10, mat.rows)));
      
      cv::Mat flipped;
      cv::flip(mat, flipped, 1); // Horizontal flip
    }
    
  } catch (const cv::Exception& e) {
    // Silently catch OpenCV exceptions during fuzzing
  }
}

// Test image encoding
void test_imencode(const cv::Mat& image) {
  if (image.empty()) return;
  
  try {
    std::vector<uint8_t> buffer;
    bool success = cv::imencode(".png", image, buffer);
    
    if (success && !buffer.empty()) {
      // Try JPEG encoding as well
      std::vector<uint8_t> jpeg_buffer;
      cv::imencode(".jpg", image, jpeg_buffer);
    }
  } catch (const cv::Exception& e) {
    // Silently catch OpenCV exceptions during fuzzing
  }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  if (size < 16) return 0; // Minimum size for meaningful testing
  
  FuzzedDataProvider fdp(data, size);
  
  // Consume configuration parameters from fuzzer input
  uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
  size_t max_image_size = fdp.ConsumeIntegralInRange<size_t>(100, 10000);
  
  // Consume image data from remaining input
  std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(
      std::min(max_image_size, fdp.remaining_bytes()));
  
  // Test different modes based on fuzzer input
  switch (test_mode) {
    case 0:
      // Test imdecode
      test_imdecode(image_data);
      break;
      
    case 1:
      // Test imread (file-based)
      test_imread(image_data.data(), image_data.size());
      break;
      
    case 2: {
      // Create a test matrix from input data and test operations
      if (image_data.size() >= 100) {
        // Create a simple test matrix (10x10 grayscale)
        cv::Mat test_mat(10, 10, CV_8UC1);
        size_t copy_size = std::min(image_data.size(), 
                                   static_cast<size_t>(test_mat.total()));
        memcpy(test_mat.data, image_data.data(), copy_size);
        test_matrix_operations(test_mat);
      }
      break;
    }
      
    case 3: {
      // Test both decoding and encoding
      if (!image_data.empty()) {
        try {
          cv::Mat decoded = cv::imdecode(image_data, CV_LOAD_IMAGE_UNCHANGED);
          if (!decoded.empty()) {
            test_matrix_operations(decoded);
            test_imencode(decoded);
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

} // namespace harness_000
