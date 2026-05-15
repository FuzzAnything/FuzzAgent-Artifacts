#include <fuzzer/FuzzedDataProvider.h>
#include "turbojpeg.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string>

namespace harness_023 {
/*
 * Fuzzing harness for libjpeg-turbo CMYK color space compression with 12-bit and 16-bit precision
 * targeting blocked color conversion paths in j12init_color_converter (168 blocked branches).
 * 
 * This harness specifically focuses on unlocking 168 blocked branches in j12init_color_converter
 * by targeting tj3Compress12 and tj3Compress16 with TJCS_CMYK color space. The blocked branches
 * correspond to different color space conversions, particularly CMYK to YCCK conversion for
 * 16-bit precision paths.
 * 
 * Target APIs:
 * - tj3Compress12 (Primary Target: 12-bit CMYK compression)
 * - tj3Compress16 (Primary Target: 16-bit CMYK compression) 
 * - tj3Set (Required Helper: Configure CMYK color space)
 * - tj3Alloc (Required Helper: Memory allocation for JPEG buffer)
 * - tj3Free (Required Helper: Memory cleanup)
 * - tj3Destroy (Required Helper: Cleanup)
 * 
 * Invocation sequence:
 * 1. Allocate compress handle with tj3Alloc
 * 2. Initialize compression with tj3Init(TJINIT_COMPRESS)
 * 3. Set color space to TJCS_CMYK using TJPARAM_COLORSPACE
 * 4. Set precision to 12 or 16 bits based on test path
 * 5. Create appropriate CMYK image data (4 components per pixel)
 * 6. Call tj3Compress12 for 12-bit path or tj3Compress16 for 16-bit path
 * 7. Clean up with tj3Free and tj3Destroy
 * 
 * Differentiation from existing harnesses:
 * - harness_020 tests 12-bit CMYK/YCCK compression with file operations
 * - This harness specifically tests both 12-bit and 16-bit precision CMYK compression
 * - Focuses on pure CMYK color space (TJCS_CMYK) not YCCK
 * - Uses direct buffer compression without file I/O operations
 * - Provides semantic diversity by exercising 16-bit precision path which is critical
 *   for unlocking blocked branches in j12init_color_converter
 * 
 * Implementation strategy:
 * 1. Generate CMYK source data from fuzzer input (4 components per pixel)
 * 2. Test both 12-bit and 16-bit precision compression paths
 * 3. Use appropriate data types: short for 12-bit, unsigned short for 16-bit
 * 4. Set precision parameter accordingly (12 or 16)
 * 5. Include proper error handling and resource cleanup
 * 6. Test various compression parameters (quality, subsampling, etc.)
 * 7. Ensure proper memory management with tj3Alloc/tj3Free
 */
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    FuzzedDataProvider fdp(data, size);
    
    // Allocate compression handle using tj3Alloc
    tjhandle compress_handle = tj3Init(TJINIT_COMPRESS);
    if (!compress_handle) {
        return 0;
    }
    
    // Consume image dimensions from fuzzer input
    // Use reasonable bounds for efficient fuzzing
    int width = fdp.ConsumeIntegralInRange<int>(8, 128);
    int height = fdp.ConsumeIntegralInRange<int>(8, 128);
    
    // Fixed pixel format: TJPF_CMYK (4 components per pixel)
    // This is the key requirement for CMYK color space testing
    int pixelFormat = TJPF_CMYK;
    int pixelSize = tjPixelSize[pixelFormat];  // Should be 4 for CMYK
    
    // Choose precision: 12-bit or 16-bit
    // This determines which compression API we test
    bool test_12bit = fdp.ConsumeBool();
    int precision = test_12bit ? 12 : 16;
    
    // Set precision parameter - critical for 12-bit vs 16-bit paths
    tj3Set(compress_handle, TJPARAM_PRECISION, precision);
    
    // Set color space to CMYK - this directly affects j12init_color_converter
    tj3Set(compress_handle, TJPARAM_COLORSPACE, TJCS_CMYK);
    
    // Choose subsampling - valid options for CMYK
    // CMYK can use 444 (no subsampling) for direct encoding
    int subsamp_idx = fdp.ConsumeIntegralInRange<int>(0, 2);
    enum TJSAMP subsampling[] = {
        TJSAMP_444,    // No chroma subsampling - valid for CMYK
        TJSAMP_422,    // Horizontal 2:1 subsampling
        TJSAMP_420,    // 2:2 subsampling
    };
    enum TJSAMP subsamp = subsampling[subsamp_idx % 3];
    
    // Set subsampling parameter
    tj3Set(compress_handle, TJPARAM_SUBSAMP, subsamp);
    
    // Choose quality - affects quantization tables
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    tj3Set(compress_handle, TJPARAM_QUALITY, quality);
    
    // Set other compression parameters that may affect color conversion
    tj3Set(compress_handle, TJPARAM_FASTDCT, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_OPTIMIZE, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_PROGRESSIVE, fdp.ConsumeBool());
    
    // Generate CMYK source image data
    // CMYK uses 4 components per pixel
    size_t image_samples = (size_t)width * height * pixelSize;  // pixelSize should be 4 for CMYK
    
    if (image_samples == 0) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    // Calculate worst-case JPEG buffer size
    size_t jpeg_buf_size = tj3JPEGBufSize(width, height, subsamp);
    if (jpeg_buf_size == 0) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    // Add padding for safety
    jpeg_buf_size += 1024;
    
    // Allocate JPEG buffer using tj3Alloc
    unsigned char* jpeg_buf = (unsigned char*)tj3Alloc(jpeg_buf_size);
    if (!jpeg_buf) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    size_t jpeg_size = jpeg_buf_size;
    
    if (test_12bit) {
        // Test 12-bit CMYK compression path (tj3Compress12)
        
        // 12-bit data stored in short (16-bit container, 12-bit valid range)
        size_t image_bytes = image_samples * sizeof(short);
        
        if (fdp.remaining_bytes() < image_bytes) {
            tj3Free(jpeg_buf);
            tj3Destroy(compress_handle);
            return 0;
        }
        
        // Create source buffer for 12-bit CMYK data
        std::vector<short> src_buf(image_samples);
        
        auto src_data = fdp.ConsumeBytes<uint8_t>(image_bytes);
        if (src_data.size() != image_bytes) {
            tj3Free(jpeg_buf);
            tj3Destroy(compress_handle);
            return 0;
        }
        
        // Copy fuzzer data into source buffer
        memcpy(src_buf.data(), src_data.data(), image_bytes);
        
        // Ensure 12-bit range for valid data (0-4095)
        for (size_t i = 0; i < src_buf.size(); i++) {
            src_buf[i] = src_buf[i] & 0x0FFF;  // Keep only 12 bits
        }
        
        // Call tj3Compress12 - this triggers j12init_color_converter with CMYK color space
        int result = tj3Compress12(compress_handle, src_buf.data(), width, 
                                   0, height, pixelFormat,
                                   &jpeg_buf, &jpeg_size);
        
        // Result is ignored for fuzzing purposes
        (void)result;
        
    } else {
        // Test 16-bit CMYK compression path (tj3Compress16)
        
        // 16-bit data stored in unsigned short (full 16-bit range)
        size_t image_bytes = image_samples * sizeof(unsigned short);
        
        if (fdp.remaining_bytes() < image_bytes) {
            tj3Free(jpeg_buf);
            tj3Destroy(compress_handle);
            return 0;
        }
        
        // Create source buffer for 16-bit CMYK data
        std::vector<unsigned short> src_buf(image_samples);
        
        auto src_data = fdp.ConsumeBytes<uint8_t>(image_bytes);
        if (src_data.size() != image_bytes) {
            tj3Free(jpeg_buf);
            tj3Destroy(compress_handle);
            return 0;
        }
        
        // Copy fuzzer data into source buffer
        memcpy(src_buf.data(), src_data.data(), image_bytes);
        
        // Ensure valid 16-bit range (0-65535)
        // No masking needed for 16-bit data
        
        // Call tj3Compress16 - this triggers j12init_color_converter with CMYK color space
        // Critical: 16-bit path is where the 168 blocked branches are located
        int result = tj3Compress16(compress_handle, src_buf.data(), width, 
                                   0, height, pixelFormat,
                                   &jpeg_buf, &jpeg_size);
        
        // Result is ignored for fuzzing purposes
        (void)result;
    }
    
    // Clean up allocated resources
    tj3Free(jpeg_buf);
    tj3Destroy(compress_handle);
    
    return 0;
}

} // namespace harness_023
