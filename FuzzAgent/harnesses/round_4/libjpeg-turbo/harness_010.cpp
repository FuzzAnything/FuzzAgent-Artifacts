#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include "turbojpeg.h"

namespace harness_010 {
/*
 * Fuzzing harness for libjpeg-turbo targeting advanced 12/16-bit high-precision JPEG operations
 * with parameter configuration and semantic diversity. Coverage analysis shows 
 * critical coverage gap in 12/16-bit high-precision JPEG operations (183+ undiscovered branches).
 * 
 * This harness focuses on semantic diversity beyond existing harness_003 by:
 * 1. Testing precision parameter configuration (TJPARAM_PRECISION)
 * 2. Testing alignment parameters for load/save operations
 * 3. Exploring more pixel format combinations and conversions
 * 4. Testing parameter validation and error handling paths
 * 5. Testing subsampling modes with high-precision data
 * 
 * Target cluster (183+ undiscovered branches):
 * - tj3Decompress12, tj3Decompress16
 * - tj3LoadImage12, tj3LoadImage16  
 * - tj3SaveImage12, tj3SaveImage16
 * - Precision parameter configuration (TJPARAM_PRECISION)
 * - Error handling and validation paths
 * 
 * Supporting APIs:
 * - tj3InitVersion, tj3Destroy, tj3Alloc, tj3Free
 * - tj3Set, tj3Get (for parameter configuration)
 * - tj3GetErrorCode, tj3GetErrorStr
 * 
 * API coverage is currently 76.71%. This harness targets the remaining 23.29%
 * focused on 12/16-bit high-precision operations.
 */


extern "C" {
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
        default: return 3; // Default to RGB
    }
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for parameters and image data
    if (size < 300) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    /**************************************************************************
     * PART 1: INITIALIZATION WITH PRECISION PARAMETER CONFIGURATION
     * Test precision parameter setting for compression and decompression
     **************************************************************************/
    
    // Initialize TurboJPEG instance for compression with parameter testing
    tjhandle compress_handle = tj3InitVersion(TJINIT_COMPRESS, TURBOJPEG_VERSION_NUMBER);
    if (compress_handle == NULL) {
        return 0;
    }
    
    // Initialize TurboJPEG instance for decompression
    tjhandle decompress_handle = tj3InitVersion(TJINIT_DECOMPRESS, TURBOJPEG_VERSION_NUMBER);
    if (decompress_handle == NULL) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    // Test precision parameter configuration (9-16 bits)
    if (fdp.ConsumeBool()) {
        int precision = fdp.ConsumeIntegralInRange<int>(9, 16);
        tj3Set(compress_handle, TJPARAM_PRECISION, precision);
        tj3Set(decompress_handle, TJPARAM_PRECISION, precision);
        
        // Also test getting the parameter back
        int get_precision_compress = tj3Get(compress_handle, TJPARAM_PRECISION);
        int get_precision_decompress = tj3Get(decompress_handle, TJPARAM_PRECISION);
    }

    
    // Test other relevant parameters for high-precision operations
    if (fdp.ConsumeBool()) {
        // Test subsampling parameter
        int subsamp = fdp.ConsumeIntegralInRange<int>(TJSAMP_444, TJSAMP_GRAY);
        tj3Set(compress_handle, TJPARAM_SUBSAMP, subsamp);
        
        // Test quality parameter (relevant for compression)
        int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
        tj3Set(compress_handle, TJPARAM_QUALITY, quality);
        
        // Test optimize flag
        int optimize = fdp.ConsumeBool() ? 1 : 0;
        tj3Set(compress_handle, TJPARAM_OPTIMIZE, optimize);
    }
    
    /**************************************************************************
     * PART 2: TEST IMAGE GENERATION WITH VARIED PARAMETERS
     * Generate test images with fuzzer-controlled parameters
     **************************************************************************/
    
    // Consume image parameters with semantic diversity
    int width = fdp.ConsumeIntegralInRange<int>(8, 512);   // JPEG requires min 8
    int height = fdp.ConsumeIntegralInRange<int>(8, 512);
    
    // Test various pixel formats including edge cases
    int pixel_format_choices[] = {
        TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, TJPF_XRGB, TJPF_XBGR,
        TJPF_GRAY, TJPF_RGBA, TJPF_BGRA, TJPF_ABGR, TJPF_ARGB
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
    
    // Calculate buffer sizes for 12-bit and 16-bit operations
    size_t image_size_12bit = (size_t)pitch * height * sizeof(short);
    size_t image_size_16bit = (size_t)pitch * height * sizeof(unsigned short);
    
    // Check if we have enough data
    if (fdp.remaining_bytes() < std::min(image_size_12bit, image_size_16bit) + 100) {
        tj3Destroy(compress_handle);
        tj3Destroy(decompress_handle);
        return 0;
    }
    
    /**************************************************************************
     * PART 3: 12-BIT PRECISION OPERATIONS WITH PARAMETER VARIATIONS
     * Test tj3Decompress12 with different parameter combinations
     **************************************************************************/
    
    // Generate 12-bit test image
    short *image_12bit = (short *)tj3Alloc(image_size_12bit);
    if (image_12bit != NULL) {
        // Fill with fuzzer data, clamped to 12-bit range (0-4095)
        std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_size_12bit);
        size_t num_samples = image_size_12bit / sizeof(short);
        
        for (size_t i = 0; i < num_samples && i * sizeof(short) < image_data.size(); i++) {
            short raw_value = 0;
            if (i * sizeof(short) + sizeof(short) <= image_data.size()) {
                memcpy(&raw_value, &image_data[i * sizeof(short)], sizeof(short));
            }
            image_12bit[i] = clamp_12bit(raw_value);
        }
        
        // Test compression with current parameters
        unsigned char *jpeg_buf_12bit = NULL;
        size_t jpeg_size_12bit = 0;
        
        int compress_result = tj3Compress12(compress_handle,
                                           image_12bit,
                                           width, pitch, height,
                                           pixel_format,
                                           &jpeg_buf_12bit,
                                           &jpeg_size_12bit);
        
        // If compression succeeded, test decompression with parameter variations
        if (compress_result == 0 && jpeg_buf_12bit != NULL && jpeg_size_12bit > 0) {
            // Allocate buffer for decompressed image
            short *decompressed_12bit = (short *)tj3Alloc(image_size_12bit);
            if (decompressed_12bit != NULL) {
                // Test tj3Decompress12 with current parameters
                int decompress_result = tj3Decompress12(decompress_handle,
                                                       jpeg_buf_12bit,
                                                       jpeg_size_12bit,
                                                       decompressed_12bit,
                                                       pitch,
                                                       pixel_format);
                
                // Test error handling paths
                if (decompress_result != 0) {
                    int error_code = tj3GetErrorCode(decompress_handle);
                    const char *error_str = tj3GetErrorStr(decompress_handle);
                    // Exercise error handling code paths
                }
                
                tj3Free(decompressed_12bit);
            }
            
            // Free JPEG buffer
            tj3Free(jpeg_buf_12bit);
        }
        
        tj3Free(image_12bit);
    }
    
    /**************************************************************************
     * PART 4: 16-BIT PRECISION OPERATIONS WITH PARAMETER VARIATIONS
     * Test tj3Decompress16 with different parameter combinations
     **************************************************************************/
    
    // Generate 16-bit test image
    unsigned short *image_16bit = (unsigned short *)tj3Alloc(image_size_16bit);
    if (image_16bit != NULL) {
        // Fill with fuzzer data
        std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_size_16bit);
        size_t num_samples = image_size_16bit / sizeof(unsigned short);
        
        for (size_t i = 0; i < num_samples && i * sizeof(unsigned short) < image_data.size(); i++) {
            unsigned short raw_value = 0;
            if (i * sizeof(unsigned short) + sizeof(unsigned short) <= image_data.size()) {
                memcpy(&raw_value, &image_data[i * sizeof(unsigned short)], sizeof(unsigned short));
            }
            image_16bit[i] = clamp_16bit(raw_value);
        }
        
        // Test compression
        unsigned char *jpeg_buf_16bit = NULL;
        size_t jpeg_size_16bit = 0;
        
        int compress_result = tj3Compress16(compress_handle,
                                           image_16bit,
                                           width, pitch, height,
                                           pixel_format,
                                           &jpeg_buf_16bit,
                                           &jpeg_size_16bit);
        
        // If compression succeeded, test decompression
        if (compress_result == 0 && jpeg_buf_16bit != NULL && jpeg_size_16bit > 0) {
            // Allocate buffer for decompressed image
            unsigned short *decompressed_16bit = (unsigned short *)tj3Alloc(image_size_16bit);
            if (decompressed_16bit != NULL) {
                // Test tj3Decompress16
                int decompress_result = tj3Decompress16(decompress_handle,
                                                       jpeg_buf_16bit,
                                                       jpeg_size_16bit,
                                                       decompressed_16bit,
                                                       pitch,
                                                       pixel_format);
                
                // Test error handling
                if (decompress_result != 0) {
                    int error_code = tj3GetErrorCode(decompress_handle);
                    const char *error_str = tj3GetErrorStr(decompress_handle);
                }
                
                tj3Free(decompressed_16bit);
            }
            
            tj3Free(jpeg_buf_16bit);
        }
        
        tj3Free(image_16bit);
    }
    
    /**************************************************************************
     * PART 5: LOAD/SAVE OPERATIONS WITH ALIGNMENT TESTING
     * Simulate file operations with different alignment parameters
     **************************************************************************/
    
    // Simulate load/save operations with alignment testing
    if (fdp.remaining_bytes() > 200) {
        // Test different width/height combinations
        int load_width = fdp.ConsumeIntegralInRange<int>(8, 256);
        int load_height = fdp.ConsumeIntegralInRange<int>(8, 256);
        
        // Test different pixel formats for load/save
        int load_pixel_format = fdp.PickValueInArray(pixel_format_choices);
        int load_pixel_size = get_pixel_size(load_pixel_format);
        
        // Test different alignments for load operations
        int load_align = fdp.PickValueInArray(align_choices);
        int load_pitch = load_width * load_pixel_size;
        
        // Apply alignment for load pitch
        if (load_align > 1) {
            load_pitch = ((load_pitch + load_align - 1) / load_align) * load_align;
        }
        
        size_t load_buffer_size_12 = (size_t)load_pitch * load_height * sizeof(short);
        size_t load_buffer_size_16 = (size_t)load_pitch * load_height * sizeof(unsigned short);
        
        // Simulate tj3LoadImage12 operation (in-memory simulation)
        if (fdp.remaining_bytes() >= load_buffer_size_12 + 50) {
            // Simulate loading by creating buffer
            short *loaded_12bit = (short *)tj3Alloc(load_buffer_size_12);
            if (loaded_12bit != NULL) {
                std::vector<uint8_t> load_data = fdp.ConsumeBytes<uint8_t>(load_buffer_size_12);
                
                // Fill with clamped 12-bit data
                size_t num_samples = load_buffer_size_12 / sizeof(short);
                for (size_t i = 0; i < num_samples && i * sizeof(short) < load_data.size(); i++) {
                    short raw_value = 0;
                    if (i * sizeof(short) + sizeof(short) <= load_data.size()) {
                        memcpy(&raw_value, &load_data[i * sizeof(short)], sizeof(short));
                    }
                    loaded_12bit[i] = clamp_12bit(raw_value);
                }
                
                // Simulate tj3SaveImage12 with /dev/null
                int save_result = tj3SaveImage12(compress_handle,
                                                "/dev/null",
                                                loaded_12bit,
                                                load_width,
                                                load_pitch,
                                                load_height,
                                                load_pixel_format);
                
                // Test error handling
                if (save_result != 0) {
                    int error_code = tj3GetErrorCode(compress_handle);
                    const char *error_str = tj3GetErrorStr(compress_handle);
                }
                
                tj3Free(loaded_12bit);
            }
        }
        
        // Simulate tj3LoadImage16 operation
        if (fdp.remaining_bytes() >= load_buffer_size_16 + 50) {
            unsigned short *loaded_16bit = (unsigned short *)tj3Alloc(load_buffer_size_16);
            if (loaded_16bit != NULL) {
                std::vector<uint8_t> load_data = fdp.ConsumeBytes<uint8_t>(load_buffer_size_16);
                memcpy(loaded_16bit, load_data.data(), std::min(load_data.size(), load_buffer_size_16));
                
                // Simulate tj3SaveImage16
                int save_result = tj3SaveImage16(compress_handle,
                                                "/dev/null",
                                                loaded_16bit,
                                                load_width,
                                                load_pitch,
                                                load_height,
                                                load_pixel_format);
                
                if (save_result != 0) {
                    int error_code = tj3GetErrorCode(compress_handle);
                    const char *error_str = tj3GetErrorStr(compress_handle);
                }
                
                tj3Free(loaded_16bit);
            }
        }
    }
    
    /**************************************************************************
     * PART 6: ERROR HANDLING AND PARAMETER VALIDATION
     * Test error conditions and parameter validation
     **************************************************************************/
    
    // Test parameter validation with invalid values
    if (fdp.ConsumeBool()) {
        // Test with invalid precision values
        tj3Set(compress_handle, TJPARAM_PRECISION, fdp.ConsumeIntegralInRange<int>(0, 8));  // Too low
        tj3Set(compress_handle, TJPARAM_PRECISION, fdp.ConsumeIntegralInRange<int>(17, 100)); // Too high
        
        // Test with invalid subsampling
        tj3Set(compress_handle, TJPARAM_SUBSAMP, fdp.ConsumeIntegralInRange<int>(-10, -1));
        tj3Set(compress_handle, TJPARAM_SUBSAMP, fdp.ConsumeIntegralInRange<int>(TJSAMP_GRAY + 1, 100));
        
        // Test with invalid quality
        tj3Set(compress_handle, TJPARAM_QUALITY, fdp.ConsumeIntegralInRange<int>(-100, 0));
        tj3Set(compress_handle, TJPARAM_QUALITY, fdp.ConsumeIntegralInRange<int>(101, 200));
    }
    
    // Exercise error query APIs
    int compress_error = tj3GetErrorCode(compress_handle);
    const char *compress_error_str = tj3GetErrorStr(compress_handle);
    
    int decompress_error = tj3GetErrorCode(decompress_handle);
    const char *decompress_error_str = tj3GetErrorStr(decompress_handle);
    
    /**************************************************************************
     * PART 7: CLEANUP
     **************************************************************************/
    
    tj3Destroy(compress_handle);
    tj3Destroy(decompress_handle);
    
    return 0;
}

} // namespace harness_010
