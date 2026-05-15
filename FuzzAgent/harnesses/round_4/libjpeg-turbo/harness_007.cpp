#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <unistd.h>  // For close, lseek, write, ftruncate, mkstemp
#include "turbojpeg.h"

namespace harness_007 {
/*
 * Fuzzing harness for libjpeg-turbo targeting multi-precision image file I/O operations.
 * This harness specifically targets PNG/PNM file loading and saving at 8, 12, and 16-bit precision
 * as identified by coverage analysis. All target APIs are currently 0% covered.
 * 
 * Target APIs (0% covered):
 * - tj3LoadImage12: 12-bit image loading (PNG/PNM)
 * - tj3LoadImage16: 16-bit image loading (PNG/PNM)
 * - tj3LoadImage8: 8-bit image loading (PNG/PNM) 
 * - tj3SaveImage12: 12-bit image saving (PNG/PNM)
 * - tj3SaveImage16: 16-bit image saving (PNG/PNM)
 * 
 * Required helper APIs:
 * - tj3Alloc: Memory allocation for buffers
 * - tj3Init: Instance initialization
 * - tj3Set: Parameter configuration (TJPARAM_PRECISION)
 * - tj3Get: Parameter retrieval
 * - tj3Free: Memory deallocation
 * - tj3Destroy: Instance cleanup
 * 
 * Lifecycle: Allocate -> Configure -> Load -> Process -> Save -> Cleanup
 * 
 * Semantic diversity: This harness focuses specifically on file-based I/O operations for
 * high-precision image formats (12-bit and 16-bit PNG/PNM), complementing existing harnesses
 * that focus on memory-based operations, YUV operations, or 8-bit file I/O.
 */


extern "C" {
}

// Maximum dimensions to prevent memory exhaustion
const int MAX_WIDTH = 512;
const int MAX_HEIGHT = 512;

// Temporary file handling for PNG/PNM test files
class TempFile {
public:
    TempFile(const std::string& suffix = ".tmp") {
        char template_path[] = "/tmp/fuzz_tmp_XXXXXX";
        fd = mkstemp(template_path);
        if (fd != -1) {
            filename = template_path;
        }
    }
    
    ~TempFile() {
        if (fd != -1) {
            close(fd);
            std::remove(filename.c_str());
        }
    }
    
    bool writeData(const uint8_t* data, size_t size) {
        if (fd == -1) return false;
        
        // Reset file pointer to beginning
        lseek(fd, 0, SEEK_SET);
        ftruncate(fd, 0);
        
        ssize_t written = write(fd, data, size);
        return written == (ssize_t)size;
    }
    
    const char* getFilename() const {
        return filename.c_str();
    }
    
    bool isValid() const {
        return fd != -1;
    }

private:
    int fd = -1;
    std::string filename;
};

// Helper to limit dimensions safely
static void safe_dimensions(int* width, int* height) {
    if (*width <= 0) *width = 64;
    if (*height <= 0) *height = 64;
    
    if (*width > MAX_WIDTH) *width = MAX_WIDTH;
    if (*height > MAX_HEIGHT) *height = MAX_HEIGHT;
    
    // Ensure at least 8x8 for format compliance
    if (*width < 8) *width = 8;
    if (*height < 8) *height = 8;
}

// Create a simple PNM (PPM) file header and data for testing
static bool create_test_pnm(const std::vector<uint8_t>& image_data, 
                           int width, int height, int maxval,
                           TempFile& file) {
    std::string header;
    if (maxval <= 255) {
        // PPM format (P6 for binary, P3 for ASCII - we use P6)
        header = "P6\n" + std::to_string(width) + " " + 
                 std::to_string(height) + "\n" + 
                 std::to_string(maxval) + "\n";
    } else {
        // For 12/16-bit, we'd need a different approach
        // For simplicity, we'll create binary data that might be parsed
        header = "P6\n" + std::to_string(width) + " " + 
                 std::to_string(height) + "\n" + 
                 std::to_string(maxval) + "\n";
    }
    
    std::vector<uint8_t> file_data;
    file_data.insert(file_data.end(), header.begin(), header.end());
    file_data.insert(file_data.end(), image_data.begin(), image_data.end());
    
    return file.writeData(file_data.data(), file_data.size());
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for parameters and image data
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    /**************************************************************************
     * PART 1: CREATE TEST IMAGE FILES FROM FUZZER INPUT
     * Create PNG/PNM test files with different precisions
     **************************************************************************/
    
    // Consume parameters for test image generation
    int test_width = fdp.ConsumeIntegralInRange<int>(8, 128);
    int test_height = fdp.ConsumeIntegralInRange<int>(8, 128);
    int maxval_8bit = 255;
    int maxval_12bit = 4095;  // 12-bit max value
    int maxval_16bit = 65535; // 16-bit max value
    
    safe_dimensions(&test_width, &test_height);
    
    // Calculate image data size for different precisions
    int pixel_format = TJPF_RGB; // Use RGB for testing
    int pixel_size = 3; // RGB
    
    size_t image_data_size_8bit = (size_t)test_width * test_height * pixel_size;
    size_t image_data_size_12bit = image_data_size_8bit * sizeof(short);
    size_t image_data_size_16bit = image_data_size_8bit * sizeof(unsigned short);
    
    // Check if we have enough data
    if (fdp.remaining_bytes() < image_data_size_8bit + 100) {
        return 0;
    }
    
    // Create 8-bit test image data
    std::vector<uint8_t> image_data_8bit = fdp.ConsumeBytes<uint8_t>(image_data_size_8bit);
    
    // Create 12-bit test image data (shorts)
    std::vector<short> image_data_12bit;
    if (fdp.remaining_bytes() >= image_data_size_12bit) {
        // Use ConsumeBytes<uint8_t> and convert to short
        std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(image_data_size_12bit);
        image_data_12bit.resize(test_width * test_height * pixel_size);
        size_t num_samples = image_data_12bit.size();
        
        for (size_t i = 0; i < num_samples && i * sizeof(short) < raw_data.size(); i++) {
            short raw_value = 0;
            if (i * sizeof(short) + sizeof(short) <= raw_data.size()) {
                memcpy(&raw_value, &raw_data[i * sizeof(short)], sizeof(short));
            }
            // Clamp to valid 12-bit range (0-4095)
            if (raw_value < 0) raw_value = 0;
            if (raw_value > 4095) raw_value = 4095;
            image_data_12bit[i] = raw_value;
        }
    } else {
        // If not enough data, fill with zeros
        image_data_12bit.resize(test_width * test_height * pixel_size, 0);
    }
    
    // Create 16-bit test image data (unsigned shorts)
    std::vector<unsigned short> image_data_16bit;
    if (fdp.remaining_bytes() >= image_data_size_16bit) {
        // Use ConsumeBytes<uint8_t> and convert to unsigned short
        std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(image_data_size_16bit);
        image_data_16bit.resize(test_width * test_height * pixel_size);
        size_t num_samples = image_data_16bit.size();
        
        for (size_t i = 0; i < num_samples && i * sizeof(unsigned short) < raw_data.size(); i++) {
            unsigned short raw_value = 0;
            if (i * sizeof(unsigned short) + sizeof(unsigned short) <= raw_data.size()) {
                memcpy(&raw_value, &raw_data[i * sizeof(unsigned short)], sizeof(unsigned short));
            }
            // Clamp to valid 16-bit range (0-65535)
            if (raw_value > 65535) raw_value = 65535;
            image_data_16bit[i] = raw_value;
        }
    } else {
        // If not enough data, fill with zeros
        image_data_16bit.resize(test_width * test_height * pixel_size, 0);
    }
    
    // Create temporary files for testing
    TempFile pnm_file_8bit(".ppm");
    TempFile pnm_file_12bit(".ppm");
    TempFile pnm_file_16bit(".ppm");
    
    if (!pnm_file_8bit.isValid() || !pnm_file_12bit.isValid() || !pnm_file_16bit.isValid()) {
        return 0;
    }
    
    // Write test data to files
    if (!create_test_pnm(image_data_8bit, test_width, test_height, maxval_8bit, pnm_file_8bit)) {
        return 0;
    }
    
    // For 12-bit and 16-bit, we need to convert data to byte format
    std::vector<uint8_t> image_data_12bit_bytes;
    image_data_12bit_bytes.reserve(image_data_12bit.size() * sizeof(short));
    for (short val : image_data_12bit) {
        uint8_t bytes[2];
        bytes[0] = (val >> 8) & 0xFF;
        bytes[1] = val & 0xFF;
        image_data_12bit_bytes.push_back(bytes[0]);
        image_data_12bit_bytes.push_back(bytes[1]);
    }
    
    if (!create_test_pnm(image_data_12bit_bytes, test_width, test_height, maxval_12bit, pnm_file_12bit)) {
        return 0;
    }
    
    std::vector<uint8_t> image_data_16bit_bytes;
    image_data_16bit_bytes.reserve(image_data_16bit.size() * sizeof(unsigned short));
    for (unsigned short val : image_data_16bit) {
        uint8_t bytes[2];
        bytes[0] = (val >> 8) & 0xFF;
        bytes[1] = val & 0xFF;
        image_data_16bit_bytes.push_back(bytes[0]);
        image_data_16bit_bytes.push_back(bytes[1]);
    }
    
    if (!create_test_pnm(image_data_16bit_bytes, test_width, test_height, maxval_16bit, pnm_file_16bit)) {
        return 0;
    }
    
    /**************************************************************************
     * PART 2: INITIALIZE TURBOJPEG INSTANCES WITH PRECISION CONFIGURATION
     * Create handles and configure precision parameters
     **************************************************************************/
    
    // Initialize TurboJPEG instance for compression (needed for loading)
    tjhandle compress_handle = tj3Init(TJINIT_COMPRESS);
    if (compress_handle == NULL) {
        return 0;
    }
    
    // Initialize TurboJPEG instance for decompression (needed for saving)
    tjhandle decompress_handle = tj3Init(TJINIT_DECOMPRESS);
    if (decompress_handle == NULL) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    // Configure precision parameters
    int precision_8bit = 8;
    int precision_12bit = 12;
    int precision_16bit = 16;
    
    // Set precision for different operations
    tj3Set(compress_handle, TJPARAM_PRECISION, precision_12bit);
    tj3Set(decompress_handle, TJPARAM_PRECISION, precision_12bit);
    
    /**************************************************************************
     * PART 3: TEST MULTI-PRECISION IMAGE LOADING OPERATIONS
     * Test tj3LoadImage8, tj3LoadImage12, tj3LoadImage16
     **************************************************************************/
    
    // Test tj3LoadImage8 (8-bit loading)
    {
        int width = 0, height = 0, pixelFormat = TJPF_UNKNOWN;
        int align = 4;
        
        unsigned char* loaded_image_8bit = tj3LoadImage8(compress_handle,
                                                         pnm_file_8bit.getFilename(),
                                                         &width, align, &height,
                                                         &pixelFormat);
        
        if (loaded_image_8bit != NULL) {
            safe_dimensions(&width, &height);
            
            // Test tj3SaveImage8 with loaded image
            TempFile output_file_8bit(".ppm");
            if (output_file_8bit.isValid() && pixelFormat != TJPF_UNKNOWN) {
                int bytes_per_pixel = (pixelFormat == TJPF_GRAY) ? 1 : 
                                     ((pixelFormat == TJPF_RGB || pixelFormat == TJPF_BGR) ? 3 : 4);
                int pitch = width * bytes_per_pixel;
                
                tj3SaveImage8(decompress_handle,
                             output_file_8bit.getFilename(),
                             loaded_image_8bit,
                             width, pitch, height,
                             pixelFormat);
            }
            
            tj3Free(loaded_image_8bit);
        }
    }
    
    // Test tj3LoadImage12 (12-bit loading)
    // First set precision to 12-bit
    tj3Set(compress_handle, TJPARAM_PRECISION, precision_12bit);
    {
        int width = 0, height = 0, pixelFormat = TJPF_UNKNOWN;
        int align = 4;
        
        short* loaded_image_12bit = tj3LoadImage12(compress_handle,
                                                   pnm_file_12bit.getFilename(),
                                                   &width, align, &height,
                                                   &pixelFormat);
        
        if (loaded_image_12bit != NULL) {
            safe_dimensions(&width, &height);
            
            // Test tj3SaveImage12 with loaded image
            TempFile output_file_12bit(".ppm");
            if (output_file_12bit.isValid() && pixelFormat != TJPF_UNKNOWN) {
                int bytes_per_pixel = (pixelFormat == TJPF_GRAY) ? 1 : 
                                     ((pixelFormat == TJPF_RGB || pixelFormat == TJPF_BGR) ? 3 : 4);
                int pitch = width * bytes_per_pixel;
                
                tj3SaveImage12(decompress_handle,
                              output_file_12bit.getFilename(),
                              loaded_image_12bit,
                              width, pitch, height,
                              pixelFormat);
            }
            
            tj3Free(loaded_image_12bit);
        }
    }
    
    // Test tj3LoadImage16 (16-bit loading)
    // First set precision to 16-bit
    tj3Set(compress_handle, TJPARAM_PRECISION, precision_16bit);
    {
        int width = 0, height = 0, pixelFormat = TJPF_UNKNOWN;
        int align = 4;
        
        unsigned short* loaded_image_16bit = tj3LoadImage16(compress_handle,
                                                            pnm_file_16bit.getFilename(),
                                                            &width, align, &height,
                                                            &pixelFormat);
        
        if (loaded_image_16bit != NULL) {
            safe_dimensions(&width, &height);
            
            // Test tj3SaveImage16 with loaded image
            TempFile output_file_16bit(".ppm");
            if (output_file_16bit.isValid() && pixelFormat != TJPF_UNKNOWN) {
                int bytes_per_pixel = (pixelFormat == TJPF_GRAY) ? 1 : 
                                     ((pixelFormat == TJPF_RGB || pixelFormat == TJPF_BGR) ? 3 : 4);
                int pitch = width * bytes_per_pixel;
                
                tj3SaveImage16(decompress_handle,
                              output_file_16bit.getFilename(),
                              loaded_image_16bit,
                              width, pitch, height,
                              pixelFormat);
            }
            
            tj3Free(loaded_image_16bit);
        }
    }
    
    /**************************************************************************
     * PART 4: TEST ADDITIONAL PRECISION COMBINATIONS AND EDGE CASES
     * Test cross-precision loading/saving and parameter retrieval
     **************************************************************************/
    
    // Test precision parameter retrieval
    {
        int retrieved_precision = tj3Get(compress_handle, TJPARAM_PRECISION);
        
        // Test loading with different precisions than file data
        if (retrieved_precision == precision_16bit) {
            // Try loading 8-bit file with 16-bit precision (should upconvert)
            int width = 0, height = 0, pixelFormat = TJPF_UNKNOWN;
            int align = 4;
            
            unsigned short* cross_load = tj3LoadImage16(compress_handle,
                                                        pnm_file_8bit.getFilename(),
                                                        &width, align, &height,
                                                        &pixelFormat);
            if (cross_load != NULL) {
                tj3Free(cross_load);
            }
        }
    }
    // Test creating and saving synthetic multi-precision images
    {
        // Create synthetic 12-bit image buffer
        int synth_width = 64;
        int synth_height = 64;
        int synth_pixel_format = TJPF_RGB;
        int synth_pitch = synth_width * 3; // RGB
        
        std::vector<short> synth_12bit_buffer(synth_width * synth_height * 3);
        for (size_t i = 0; i < synth_12bit_buffer.size(); i++) {
            synth_12bit_buffer[i] = (short)(i % 4096); // 12-bit range
        }
        
        TempFile synth_file_12bit(".ppm");
        if (synth_file_12bit.isValid()) {
            tj3Set(decompress_handle, TJPARAM_PRECISION, precision_12bit);
            tj3SaveImage12(decompress_handle,
                          synth_file_12bit.getFilename(),
                          synth_12bit_buffer.data(),
                          synth_width, synth_pitch, synth_height,
                          synth_pixel_format);
        }
        
        // Create synthetic 16-bit image buffer
        std::vector<unsigned short> synth_16bit_buffer(synth_width * synth_height * 3);
        for (size_t i = 0; i < synth_16bit_buffer.size(); i++) {
            synth_16bit_buffer[i] = (unsigned short)(i % 65536); // 16-bit range
        }
        
        TempFile synth_file_16bit(".ppm");
        if (synth_file_16bit.isValid()) {
            tj3Set(decompress_handle, TJPARAM_PRECISION, precision_16bit);
            tj3SaveImage16(decompress_handle,
                          synth_file_16bit.getFilename(),
                          synth_16bit_buffer.data(),
                          synth_width, synth_pitch, synth_height,
                          synth_pixel_format);
        }
    }
    
    /**************************************************************************
     * PART 5: CLEANUP
     * Free resources and destroy handles
     **************************************************************************/
    
    tj3Destroy(compress_handle);
    tj3Destroy(decompress_handle);
    
    return 0;
}

} // namespace harness_007
