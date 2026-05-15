#include <fuzzer/FuzzedDataProvider.h>
#include "turbojpeg.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

namespace harness_015 {
/*
 * Fuzzing harness for libjpeg-turbo 12-bit precision JPEG compression targeting
 * blocked branches in j12init_color_converter (160 blocked branches, 151 blocker hits)
 * 
 * This harness specifically focuses on unlocking the 160 blocked branches by:
 * 1. Testing all extended RGB pixel formats that map to JCS_EXT_* color spaces
 * 2. Comprehensive testing of 12-bit precision color conversion paths
 * 3. All JPEG colorspaces (YCbCr, RGB, GRAY, CMYK, YCCK)
 * 4. Various subsampling options and quality levels
 * 5. Lossless compression mode to hit C_LOSSLESS_SUPPORTED paths
 * 
 * Differentiation from harness_014:
 * - harness_014 tests basic pixel formats but doesn't focus on JCS_EXT_* mappings
 * - This harness explicitly targets extended color space conversion paths
 * - More comprehensive parameter combinations to trigger all blocked branches
 * - Focus on 12-bit precision with diverse color space combinations
 * 
 * Mapping from TJPF pixel formats to JCS_EXT_* color spaces:
 * - TJPF_RGBX -> JCS_EXT_RGBX
 * - TJPF_BGRX -> JCS_EXT_BGRX  
 * - TJPF_XBGR -> JCS_EXT_XBGR
 * - TJPF_XRGB -> JCS_EXT_XRGB
 * - TJPF_RGBA -> JCS_EXT_RGBA
 * - TJPF_BGRA -> JCS_EXT_BGRA
 * - TJPF_ABGR -> JCS_EXT_ABGR
 * - TJPF_ARGB -> JCS_EXT_ARGB
 * - TJPF_RGB  -> JCS_EXT_RGB
 * - TJPF_BGR  -> JCS_EXT_BGR
 * - TJPF_GRAY -> JCS_GRAYSCALE
 * - TJPF_CMYK -> JCS_CMYK or JCS_YCCK
 * 
 * These JCS_EXT_* color spaces are the source of 160 blocked branches in j12init_color_converter.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for comprehensive 12-bit testing
    // Required: dimensions (2 ints) + pixel format + colorspace + subsampling + 
    // quality + lossless flag + fastdct + optimize + progressive + arithmetic + bottomup
    // + extended parameters + image data
    if (size < 512) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize compression handle
    tjhandle compress_handle = tj3Init(TJINIT_COMPRESS);
    if (!compress_handle) {
        return 0;
    }
    
    // Consume image dimensions from fuzzer input with reasonable bounds
    // Testing various dimensions helps exercise different processing paths
    int width = fdp.ConsumeIntegralInRange<int>(8, 256);
    int height = fdp.ConsumeIntegralInRange<int>(8, 256);
    
    // Choose pixel format from ALL 12 supported formats (critical for JCS_EXT_* coverage)
    // The order matters for testing different color space mappings
    int pixel_format_idx = fdp.ConsumeIntegralInRange<int>(0, 11);
    enum TJPF pixel_formats[] = {
        TJPF_RGB,     // JCS_EXT_RGB
        TJPF_BGR,     // JCS_EXT_BGR
        TJPF_RGBX,    // JCS_EXT_RGBX
        TJPF_BGRX,    // JCS_EXT_BGRX
        TJPF_XBGR,    // JCS_EXT_XBGR
        TJPF_XRGB,    // JCS_EXT_XRGB
        TJPF_GRAY,    // JCS_GRAYSCALE
        TJPF_RGBA,    // JCS_EXT_RGBA
        TJPF_BGRA,    // JCS_EXT_BGRA
        TJPF_ABGR,    // JCS_EXT_ABGR
        TJPF_ARGB,    // JCS_EXT_ARGB
        TJPF_CMYK     // JCS_CMYK or JCS_YCCK
    };
    int pixelFormat = pixel_formats[pixel_format_idx % 12];
    int pixelSize = tjPixelSize[pixelFormat];
    
    // Choose JPEG colorspace (critical for color conversion logic)
    // Testing all 5 colorspaces to exercise different conversion paths
    int colorspace_idx = fdp.ConsumeIntegralInRange<int>(0, 5);
    enum TJCS colorspaces[] = {
        TJCS_YCbCr,     // Default JPEG colorspace - most common path
        TJCS_RGB,       // RGB colorspace (no color conversion)
        TJCS_GRAY,      // Grayscale - exercises monochrome path
        TJCS_CMYK,      // CMYK colorspace - 4-component path
        TJCS_YCCK,      // YCCK colorspace (CMYK converted to YCCK)
        TJCS_DEFAULT    // Let library choose based on pixel format
    };
    enum TJCS colorspace = colorspaces[colorspace_idx % 6];
    
    // Choose subsampling - different combinations affect chroma processing
    int subsamp_idx = fdp.ConsumeIntegralInRange<int>(0, 5);
    enum TJSAMP subsampling[] = {
        TJSAMP_444,    // No chroma subsampling
        TJSAMP_422,    // Horizontal 2:1 subsampling
        TJSAMP_420,    // Both horizontal and vertical 2:1 subsampling
        TJSAMP_GRAY,   // Grayscale (no chroma)
        TJSAMP_440,    // Vertical 2:1 subsampling
        TJSAMP_411     // Horizontal 4:1 subsampling
    };
    enum TJSAMP subsamp = subsampling[subsamp_idx % 6];
    
    // Choose quality for lossy compression (1-100)
    // Different quality levels use different quantization tables
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    
    // Choose whether to use lossless compression (critical for C_LOSSLESS_SUPPORTED paths)
    bool lossless = fdp.ConsumeBool();
    
    // Set precision to 12-bit (primary target of this harness)
    // This triggers j12init_color_converter instead of jinit_color_converter
    tj3Set(compress_handle, TJPARAM_PRECISION, 12);
    
    // Set compression parameters from fuzzer input
    // Each parameter affects different code paths
    tj3Set(compress_handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(compress_handle, TJPARAM_QUALITY, quality);
    tj3Set(compress_handle, TJPARAM_COLORSPACE, colorspace);
    tj3Set(compress_handle, TJPARAM_LOSSLESS, lossless ? 1 : 0);
    
    // Set additional parameters that affect color conversion and processing paths
    // These parameters create diverse execution paths through the codebase
    tj3Set(compress_handle, TJPARAM_FASTDCT, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_OPTIMIZE, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_PROGRESSIVE, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_ARITHMETIC, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_BOTTOMUP, fdp.ConsumeBool());
    
    // Calculate buffer sizes
    size_t src_image_size = (size_t)width * height * pixelSize;
    size_t jpeg_buf_size = tj3JPEGBufSize(width, height, subsamp);
    
    if (src_image_size == 0 || jpeg_buf_size == 0) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    // Allocate source buffer for 12-bit data (short type)
    // 12-bit precision uses short (16-bit) samples
    size_t src_sample_count = src_image_size;
    std::vector<short> src_buf_12(src_sample_count);
    if (src_buf_12.empty()) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    // Fill source buffer with fuzzer data
    // Use all remaining fuzzer input for maximum coverage potential
    size_t bytes_needed = src_sample_count * sizeof(short);
    size_t bytes_to_consume = std::min(bytes_needed, fdp.remaining_bytes());
    
    if (bytes_to_consume > 0) {
        auto byte_data = fdp.ConsumeBytes<uint8_t>(bytes_to_consume);
        if (byte_data.size() > 0) {
            size_t copy_size = std::min(byte_data.size(), src_buf_12.size() * sizeof(short));
            memcpy(src_buf_12.data(), byte_data.data(), copy_size);
        }
    }
    
    // Allocate JPEG buffer using tj3Alloc (proper library allocation)
    unsigned char* jpeg_buf = nullptr;
    size_t jpeg_size = jpeg_buf_size;
    jpeg_buf = static_cast<unsigned char*>(tj3Alloc(jpeg_buf_size));
    if (!jpeg_buf) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    // Primary API call: tj3Compress12 - directly triggers j12init_color_converter
    // This is where the 160 blocked branches are located
    int pitch = width * pixelSize;
    int result = tj3Compress12(compress_handle,
                              src_buf_12.data(),
                              width,
                              pitch,
                              height,
                              pixelFormat,
                              &jpeg_buf,
                              &jpeg_size);
    
    // Free JPEG buffer
    if (jpeg_buf) {
        tj3Free(jpeg_buf);
    }
    
    // Test 2: Additional test with different pitch values
    // Exercises different memory access patterns in color conversion
    if (fdp.remaining_bytes() > 128) {
        int padded_pitch = pitch + fdp.ConsumeIntegralInRange<int>(0, 32);
        size_t padded_size = (size_t)padded_pitch * height;
        std::vector<short> padded_buf(padded_size);
        
        if (!padded_buf.empty() && fdp.remaining_bytes() > 0) {
            size_t pad_bytes = std::min(padded_buf.size() * sizeof(short), fdp.remaining_bytes());
            auto pad_data = fdp.ConsumeBytes<uint8_t>(pad_bytes);
            if (pad_data.size() > 0) {
                memcpy(padded_buf.data(), pad_data.data(), 
                       std::min(pad_data.size(), padded_buf.size() * sizeof(short)));
            }
            
            // Try compression with padded pitch
            unsigned char* jpeg_buf2 = static_cast<unsigned char*>(tj3Alloc(jpeg_buf_size));
            if (jpeg_buf2) {
                size_t jpeg_size2 = jpeg_buf_size;
                tj3Compress12(compress_handle,
                             padded_buf.data(),
                             width,
                             padded_pitch,
                             height,
                             pixelFormat,
                             &jpeg_buf2,
                             &jpeg_size2);
                
                if (jpeg_buf2) {
                    tj3Free(jpeg_buf2);
                }
            }
        }
    }
    
    // Test 3: Test with invalid color space combinations for error path coverage
    // Exercises error handling in color conversion initialization
    if (fdp.remaining_bytes() > 32) {
        // Test with mismatched pixel format and colorspace (e.g., CMYK pixel format with RGB colorspace)
        if (fdp.ConsumeBool()) {
            unsigned char* jpeg_buf3 = static_cast<unsigned char*>(tj3Alloc(jpeg_buf_size));
            if (jpeg_buf3) {
                size_t jpeg_size3 = jpeg_buf_size;
                
                // Temporarily change colorspace to something potentially invalid
                // This tests error handling paths in color conversion
                int temp_colorspace = fdp.ConsumeIntegralInRange<int>(0, 5);
                enum TJCS test_colorspaces[] = {
                    TJCS_YCbCr, TJCS_RGB, TJCS_GRAY, TJCS_CMYK, TJCS_YCCK, TJCS_DEFAULT
                };
                tj3Set(compress_handle, TJPARAM_COLORSPACE, test_colorspaces[temp_colorspace % 6]);
                
                tj3Compress12(compress_handle,
                             src_buf_12.data(),
                             width,
                             pitch,
                             height,
                             pixelFormat,
                             &jpeg_buf3,
                             &jpeg_size3);
                
                // Restore original colorspace for subsequent tests
                tj3Set(compress_handle, TJPARAM_COLORSPACE, colorspace);
                
                tj3Free(jpeg_buf3);
            }
        }
    }
    
    // Test 4: Test error paths by providing invalid parameter combinations
    // Exercises error handling in color conversion initialization
    if (fdp.remaining_bytes() > 32) {
        // Test with zero dimensions (should fail gracefully)
        if (fdp.ConsumeBool()) {
            unsigned char* jpeg_buf4 = static_cast<unsigned char*>(tj3Alloc(1024));
            if (jpeg_buf4) {
                size_t jpeg_size4 = 1024;
                // Width or height of 0 should trigger error path
                int test_width = fdp.ConsumeBool() ? 0 : width;
                int test_height = fdp.ConsumeBool() ? 0 : height;
                
                tj3Compress12(compress_handle,
                             src_buf_12.data(),
                             test_width,
                             test_width * pixelSize,
                             test_height,
                             pixelFormat,
                             &jpeg_buf4,
                             &jpeg_size4);
                
                tj3Free(jpeg_buf4);
            }
        }
    }
    
    // Cleanup
    tj3Destroy(compress_handle);
    
    return 0;
}

} // namespace harness_015
