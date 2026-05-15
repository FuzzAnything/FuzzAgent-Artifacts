#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>

namespace harness_000 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation type to decide which API to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Get remaining bytes as image data
    std::vector<uint8_t> image_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (image_data.empty()) return 0;
    
    try {
        switch (operation) {
            case 0: {
                // Test imread with temporary file (similar to oss-fuzz imread_fuzzer.cc)
                char filename[] = "/tmp/fuzz_image_XXXXXX";
                int fd = mkstemp(filename);
                if (fd >= 0) {
                    FILE* file = fdopen(fd, "wb");
                    if (file) {
                        fwrite(image_data.data(), 1, image_data.size(), file);
                        fclose(file);
                        
                        // Test imread - use default flags
                        cv::Mat img = cv::imread(filename, cv::IMREAD_COLOR);
                        
                        // Clean up
                        unlink(filename);
                    } else {
                        close(fd);
                        unlink(filename);
                    }
                }
                break;
            }
            
            case 1: {
                // Test imdecode (similar to oss-fuzz imdecode_fuzzer.cc)
                cv::Mat data_matrix = cv::Mat(1, image_data.size(), CV_8UC1, image_data.data());
                cv::Mat decoded = cv::imdecode(data_matrix, cv::IMREAD_UNCHANGED);
                break;
            }
            
            case 2: {
                // Test imencode (similar to oss-fuzz imencode_fuzzer.cc)
                cv::Mat data_matrix = cv::Mat(1, image_data.size(), CV_8UC1, image_data.data());
                try {
                    std::vector<uchar> buffer;
                    cv::imencode(".tiff", data_matrix, buffer);
                } catch (const cv::Exception& e) {
                    // Do nothing
                }
                break;
            }
        }
    } catch (const cv::Exception& e) {
        // OpenCV exceptions are expected during fuzzing
    } catch (...) {
        // Catch any other exceptions
    }
    
    return 0;
}

} // namespace harness_000
