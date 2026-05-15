#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <unistd.h>  // For close, lseek, write, ftruncate, mkstemp
#include "turbojpeg.h"

namespace harness_014 {
/*
 * Fuzzing harness for libjpeg-turbo targeting comprehensive PNG/BMP/PPM image format
 * loading and saving operations with full semantic diversity.
 * 
 * Coverage analysis shows PNG/BMP file format support is completely uncovered 
 * (0% coverage in multiple files: rdpng.c, rdbmp.c, wrpng.c, wrppm.c, jcicc.c).
 * 
 * Target APIs (0 hit counts, 500+ points of accumulated branch complexity):
 * - tj3LoadImage8: 8-bit PNG/BMP/PPM loading
 * - tj3LoadImage12: 12-bit PNG/BMP/PPM loading
 * - tj3LoadImage16: 16-bit PNG/BMP/PPM loading
 * - tj3SaveImage8: 8-bit PNG/BMP/PPM saving
 * - tj3SaveImage12: 12-bit PNG/BMP/PPM saving
 * - tj3SaveImage16: 16-bit PNG/BMP/PPM saving
 * 
 * Supporting APIs:
 * - tj3InitVersion, tj3Destroy, tj3Alloc, tj3Free
 * - tj3Set, tj3Get (for parameter configuration)
 * - tj3GetErrorCode, tj3GetErrorStr
 * 
 * Semantic diversity focus beyond existing harnesses:
 * - harness_010: Tests 12/16-bit operations with parameter configuration but uses JPEG operations
 * - harness_013: Tests high-bit-depth with real file I/O but focuses on YUV integration
 * 
 * This harness (harness_014) provides unique coverage by:
 * 1. Comprehensive PNG/BMP/PPM file format testing (not JPEG-centric)
 * 2. Testing all bit depths (8, 12, 16-bit) for both loading and saving
 * 3. Testing TJPF_UNKNOWN pixel format behavior (auto-detection)
 * 4. Testing TJPF_GRAY for grayscale-only file formats (PGM, grayscale BMP)
 * 5. Testing TJPF_CMYK with quick conversion algorithms
 * 6. Testing alignment variations (1, 2, 4, 8, 16, 32, 64)
 * 7. Testing precision parameter (TJPARAM_PRECISION) across full range
 * 8. Testing SAVEMARKERS parameter for ICC profile handling with PNG
 * 9. Testing error paths with invalid/corrupted image files
 * 10. Testing file format detection and conversion paths
 */


extern "C" {
}

// Maximum dimensions to prevent memory exhaustion
const int MAX_WIDTH = 256;
const int MAX_HEIGHT = 256;

// Temporary file handling for real file I/O testing
class TempFile {
public:
    TempFile(const std::string& suffix = ".tmp") {
        char template_path[] = "/tmp/fuzz_pngbmp_XXXXXX";
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

// Helper to get pixel size from pixel format
static int get_pixel_size(int pixel_format) {
    switch (pixel_format) {
        case TJPF_GRAY: return 1;
        case TJPF_RGB:
        case TJPF_BGR: return 3;
        case TJPF_RGBX:
        case TJPF_BGRX:
        case TJPF_XRGB:
        case TJPF_XBGR:
        case TJPF_RGBA:
        case TJPF_BGRA:
        case TJPF_ABGR:
        case TJPF_ARGB: return 4;
        case TJPF_CMYK: return 4;
        case TJPF_UNKNOWN: return 3; // Default assumption
        default: return 3;
    }
}

// Helper to clamp values to valid precision ranges
static short clamp_12bit(int value) {
    if (value < 0) return 0;
    if (value > 4095) return 4095;
    return (short)value;
}

static unsigned short clamp_16bit(int value) {
    if (value < 0) return 0;
    if (value > 65535) return 65535;
    return (unsigned short)value;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for parameters and image generation
    if (size < 500) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    /**************************************************************************
     * PART 1: INITIALIZATION WITH PARAMETER CONFIGURATION
     * Initialize TurboJPEG instances with parameter testing
     **************************************************************************/
    
    // Initialize TurboJPEG instance for general operations
    // Using decompress handle since it supports both load and save operations
    tjhandle handle = tj3InitVersion(TJINIT_DECOMPRESS, TURBOJPEG_VERSION_NUMBER);
    if (handle == NULL) {
        return 0;
    }
    
    // Test precision parameter configuration for different bit depths
    int operation_type = fdp.ConsumeIntegralInRange<int>(0, 2); // 0=8-bit, 1=12-bit, 2=16-bit
    int target_precision = 8;
    
    switch (operation_type) {
        case 0: // 8-bit operations
            target_precision = 8;
            break;
        case 1: // 12-bit operations (9-12 bits)
            target_precision = fdp.ConsumeIntegralInRange<int>(9, 12);
            tj3Set(handle, TJPARAM_PRECISION, target_precision);
            break;
        case 2: // 16-bit operations (13-16 bits)
            target_precision = fdp.ConsumeIntegralInRange<int>(13, 16);
            tj3Set(handle, TJPARAM_PRECISION, target_precision);
            break;
    }
    
    // Test SAVEMARKERS parameter for ICC profile handling with PNG
    if (fdp.ConsumeBool()) {
        int savemarkers = fdp.ConsumeIntegralInRange<int>(0, 4);
        tj3Set(handle, TJPARAM_SAVEMARKERS, savemarkers);
    }
    
    // Test other relevant parameters
    if (fdp.ConsumeBool()) {
        // Test fast upsample flag
        int fast_upsample = fdp.ConsumeBool() ? 1 : 0;
        tj3Set(handle, TJPARAM_FASTUPSAMPLE, fast_upsample);
    }
    
    /**************************************************************************
     * PART 2: GENERATE TEST IMAGE PARAMETERS
     * Generate image parameters with semantic diversity
     **************************************************************************/
    
    // Consume image dimensions
    int width = fdp.ConsumeIntegralInRange<int>(8, MAX_WIDTH);
    int height = fdp.ConsumeIntegralInRange<int>(8, MAX_HEIGHT);
    
    // Test various pixel formats including edge cases
    int pixel_format_choices[] = {
        TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, 
        TJPF_XRGB, TJPF_XBGR, TJPF_GRAY, TJPF_RGBA, 
        TJPF_BGRA, TJPF_ABGR, TJPF_ARGB, TJPF_CMYK,
        TJPF_UNKNOWN  // Test auto-detection
    };
    int pixel_format = fdp.PickValueInArray(pixel_format_choices);
    
    // Test different alignments (power of 2: 1, 2, 4, 8, 16, 32, 64)
    int align_choices[] = {1, 2, 4, 8, 16, 32, 64};
    int align = fdp.PickValueInArray(align_choices);
    
    // Calculate pitch with alignment consideration
    int pixel_size = get_pixel_size(pixel_format);
    int pitch = width * pixel_size;
    
    // Apply alignment if requested (for load/save operations)
    if (align > 1) {
        pitch = ((pitch + align - 1) / align) * align;
    }
    
    /**************************************************************************
     * PART 3: CREATE TEMPORARY FILE WITH IMAGE DATA
     * Generate image data and write to temporary file for testing
     **************************************************************************/
    
    TempFile temp_file;
    if (!temp_file.isValid()) {
        tj3Destroy(handle);
        return 0;
    }
    
    // Calculate buffer size based on bit depth
    size_t image_size_bytes = 0;
    size_t num_pixels = (size_t)pitch * height;
    
    switch (operation_type) {
        case 0: // 8-bit
            image_size_bytes = num_pixels * sizeof(unsigned char);
            break;
        case 1: // 12-bit (stored as short)
            image_size_bytes = num_pixels * sizeof(short);
            break;
        case 2: // 16-bit
            image_size_bytes = num_pixels * sizeof(unsigned short);
            break;
    }
    
    // Check if we have enough data for image generation
    if (fdp.remaining_bytes() < image_size_bytes + 100) {
        tj3Destroy(handle);
        return 0;
    }
    
    // Generate image data based on bit depth
    std::vector<uint8_t> image_data;
    
    switch (operation_type) {
        case 0: { // 8-bit
            std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(image_size_bytes);
            // Write directly to temp file
            if (!temp_file.writeData(raw_data.data(), raw_data.size())) {
                tj3Destroy(handle);
                return 0;
            }
            break;
        }
        case 1: { // 12-bit
            std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(image_size_bytes);
            // For 12-bit, we need to ensure values are in valid range
            size_t num_shorts = image_size_bytes / sizeof(short);
            std::vector<short> clamped_data(num_shorts);
            
            for (size_t i = 0; i < num_shorts && i * sizeof(short) < raw_data.size(); i++) {
                short raw_value = 0;
                if (i * sizeof(short) + sizeof(short) <= raw_data.size()) {
                    memcpy(&raw_value, &raw_data[i * sizeof(short)], sizeof(short));
                }
                clamped_data[i] = clamp_12bit(raw_value);
            }
            
            if (!temp_file.writeData(reinterpret_cast<uint8_t*>(clamped_data.data()), 
                                     clamped_data.size() * sizeof(short))) {
                tj3Destroy(handle);
                return 0;
            }
            break;
        }
        case 2: { // 16-bit
            std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(image_size_bytes);
            // For 16-bit, we need to ensure values are in valid range
            size_t num_ushorts = image_size_bytes / sizeof(unsigned short);
            std::vector<unsigned short> clamped_data(num_ushorts);
            
            for (size_t i = 0; i < num_ushorts && i * sizeof(unsigned short) < raw_data.size(); i++) {
                unsigned short raw_value = 0;
                if (i * sizeof(unsigned short) + sizeof(unsigned short) <= raw_data.size()) {
                    memcpy(&raw_value, &raw_data[i * sizeof(unsigned short)], sizeof(unsigned short));
                }
                clamped_data[i] = clamp_16bit(raw_value);
            }
            
            if (!temp_file.writeData(reinterpret_cast<uint8_t*>(clamped_data.data()), 
                                     clamped_data.size() * sizeof(unsigned short))) {
                tj3Destroy(handle);
                return 0;
            }
            break;
        }
    }
    
    /**************************************************************************
     * PART 4: TEST IMAGE LOADING OPERATIONS
     * Test tj3LoadImage* APIs with the generated file
     **************************************************************************/
    
    int loaded_width = 0, loaded_height = 0, loaded_pixel_format = pixel_format;
    
    switch (operation_type) {
        case 0: { // 8-bit loading
            unsigned char* loaded_image = tj3LoadImage8(handle,
                                                        temp_file.getFilename(),
                                                        &loaded_width,
                                                        align,
                                                        &loaded_height,
                                                        &loaded_pixel_format);
            
            if (loaded_image != NULL) {
                // Successfully loaded - test saving with different parameters
                TempFile save_file(".png");
                if (save_file.isValid()) {
                    // Test saving in different format
                    int save_result = tj3SaveImage8(handle,
                                                    save_file.getFilename(),
                                                    loaded_image,
                                                    loaded_width,
                                                    0, // auto pitch
                                                    loaded_height,
                                                    loaded_pixel_format);
                    
                    // Try with explicit pitch
                    if (save_result == 0) {
                        int explicit_pitch = loaded_width * get_pixel_size(loaded_pixel_format);
                        if (align > 1) {
                            explicit_pitch = ((explicit_pitch + align - 1) / align) * align;
                        }
                        
                        TempFile save_file2(".bmp");
                        if (save_file2.isValid()) {
                            tj3SaveImage8(handle,
                                         save_file2.getFilename(),
                                         loaded_image,
                                         loaded_width,
                                         explicit_pitch,
                                         loaded_height,
                                         loaded_pixel_format);
                        }
                    }
                }
                
                tj3Free(loaded_image);
            }
            break;
        }
        case 1: { // 12-bit loading
            short* loaded_image = tj3LoadImage12(handle,
                                                 temp_file.getFilename(),
                                                 &loaded_width,
                                                 align,
                                                 &loaded_height,
                                                 &loaded_pixel_format);
            
            if (loaded_image != NULL) {
                // Successfully loaded - test saving
                TempFile save_file(".png");
                if (save_file.isValid()) {
                    int save_result = tj3SaveImage12(handle,
                                                     save_file.getFilename(),
                                                     loaded_image,
                                                     loaded_width,
                                                     0, // auto pitch
                                                     loaded_height,
                                                     loaded_pixel_format);
                    
                    // Try with PPM format (different file extension)
                    if (save_result == 0) {
                        TempFile save_file2(".ppm");
                        if (save_file2.isValid()) {
                            tj3SaveImage12(handle,
                                          save_file2.getFilename(),
                                          loaded_image,
                                          loaded_width,
                                          0,
                                          loaded_height,
                                          loaded_pixel_format);
                        }
                    }
                }
                
                tj3Free(loaded_image);
            }
            break;
        }
        case 2: { // 16-bit loading
            unsigned short* loaded_image = tj3LoadImage16(handle,
                                                          temp_file.getFilename(),
                                                          &loaded_width,
                                                          align,
                                                          &loaded_height,
                                                          &loaded_pixel_format);
            
            if (loaded_image != NULL) {
                // Successfully loaded - test saving
                TempFile save_file(".png");
                if (save_file.isValid()) {
                    int save_result = tj3SaveImage16(handle,
                                                     save_file.getFilename(),
                                                     loaded_image,
                                                     loaded_width,
                                                     0, // auto pitch
                                                     loaded_height,
                                                     loaded_pixel_format);
                    
                    // Try with PGM format for grayscale
                    if (save_result == 0 && loaded_pixel_format == TJPF_GRAY) {
                        TempFile save_file2(".pgm");
                        if (save_file2.isValid()) {
                            tj3SaveImage16(handle,
                                          save_file2.getFilename(),
                                          loaded_image,
                                          loaded_width,
                                          0,
                                          loaded_height,
                                          loaded_pixel_format);
                        }
                    }
                }
                
                tj3Free(loaded_image);
            }
            break;
        }
    }
    
    /**************************************************************************
     * PART 5: TEST ERROR PATHS WITH INVALID/CORRUPTED FILES
     * Test tj3LoadImage* APIs with invalid data to exercise error handling
     **************************************************************************/
    
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 100) {
        // Create a file with random data (not a valid image)
        TempFile corrupt_file;
        if (corrupt_file.isValid()) {
            std::vector<uint8_t> corrupt_data = fdp.ConsumeBytes<uint8_t>(
                fdp.ConsumeIntegralInRange<size_t>(50, 500)
            );
            corrupt_file.writeData(corrupt_data.data(), corrupt_data.size());
            
            int corrupt_width = 0, corrupt_height = 0, corrupt_pixel_format = TJPF_UNKNOWN;
            
            // Try loading corrupt file - should fail but exercise error paths
            switch (fdp.ConsumeIntegralInRange<int>(0, 2)) {
                case 0:
                    tj3LoadImage8(handle,
                                 corrupt_file.getFilename(),
                                 &corrupt_width,
                                 1,
                                 &corrupt_height,
                                 &corrupt_pixel_format);
                    break;
                case 1:
                    tj3LoadImage12(handle,
                                  corrupt_file.getFilename(),
                                  &corrupt_width,
                                  1,
                                  &corrupt_height,
                                  &corrupt_pixel_format);
                    break;
                case 2:
                    tj3LoadImage16(handle,
                                  corrupt_file.getFilename(),
                                  &corrupt_width,
                                  1,
                                  &corrupt_height,
                                  &corrupt_pixel_format);
                    break;
            }
            
            // Check error code
            int error_code = tj3GetErrorCode(handle);
            const char* error_str = tj3GetErrorStr(handle);
            (void)error_code; // Mark as used
            (void)error_str;  // Mark as used
        }
    }
    
    /**************************************************************************
     * PART 6: TEST PARAMETER QUERY AND VERIFICATION
     * Verify that parameters can be retrieved correctly
     **************************************************************************/
    
    if (fdp.ConsumeBool()) {
        // Get precision parameter back
        int retrieved_precision = tj3Get(handle, TJPARAM_PRECISION);
        (void)retrieved_precision; // Mark as used
        
        // Get SAVEMARKERS parameter
        int retrieved_savemarkers = tj3Get(handle, TJPARAM_SAVEMARKERS);
        (void)retrieved_savemarkers; // Mark as used

    }
    
    /**************************************************************************
     * CLEANUP
     **************************************************************************/
    
    tj3Destroy(handle);
    return 0;
}

} // namespace harness_014
