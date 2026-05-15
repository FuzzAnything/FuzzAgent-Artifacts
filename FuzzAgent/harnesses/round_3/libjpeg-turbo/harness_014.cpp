#include <fuzzer/FuzzedDataProvider.h>
#include "turbojpeg.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

namespace harness_014 {
/*
 * Fuzzing harness for libjpeg-turbo 12-bit precision JPEG compression with diverse color space configurations
 * This harness targets blocked branches in j12init_color_converter (129 blocked branches, 78 blocker hits)
 * Specifically focuses on 12-bit precision color conversion logic with various color spaces and lossless compression
 * 
 * Differentiates from existing harnesses:
 * - harness_000.cpp: Modern tj3* TurboJPEG APIs (general)
 * - harness_001.cpp: Traditional tj* TurboJPEG APIs
 * - harness_002.cpp: Modern tj3* YUV APIs
 * - harness_003.cpp: TurboJPEG 12/16-bit precision APIs (basic coverage)
 * - harness_004.cpp: Traditional tj* YUV operations
 * - harness_005.cpp: High-bit-depth image I/O
 * - harness_006.cpp: Traditional YUV encoding
 * - harness_007.cpp: Traditional libjpeg I/O APIs (8-bit)
 * - harness_008.cpp: Traditional libjpeg 12-bit APIs (non-lossless)
 * - harness_009.cpp: Old turbojpeg YUV decompression APIs
 * - harness_010.cpp: Traditional file-based I/O APIs
 * - harness_011.cpp: JPEG error recovery and marker handling APIs
 * - harness_012.cpp: 12-bit lossless JPEG operations
 * - harness_013.cpp: Scanline manipulation operations
 * 
 * Strategy: Systematically test tj3Compress12 API with all supported color spaces (RGB, BGR, RGBX, BGRX, XBGR, XRGB, GRAY, RGBA, BGRA, ABGR, ARGB, CMYK)
 * to exercise the j12init_color_converter logic. Test both lossy and lossless compression modes, various JPEG colorspaces
 * (YCbCr, RGB, GRAY, CMYK, YCCK), and extended color space paths to maximize coverage of blocked branches.
 * 
 * Expected Coverage Impact:
 * - Target j12init_color_converter (129 blocked branches)
 * - Cover extended color space validation paths (JCS_EXT_RGB, JCS_EXT_BGR, etc.)
 * - Exercise lossless compression path with 12-bit precision
 * - Test CMYK and YCCK color spaces for 4-component paths
 * - Test grayscale conversion path (JCS_GRAYSCALE)
 * - Test YCbCr conversion path with various subsampling options
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for 12-bit image operations
    // We need: dimensions (2 ints) + pixel format + colorspace + subsampling + quality + lossless flag + image data
    if (size < 256) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize compression handle
    tjhandle compress_handle = tj3Init(TJINIT_COMPRESS);
    if (!compress_handle) {
        return 0;
    }
    
    // Consume image dimensions from fuzzer input with reasonable bounds
    int width = fdp.ConsumeIntegralInRange<int>(8, 128);
    int height = fdp.ConsumeIntegralInRange<int>(8, 128);
    
    // Choose pixel format from all 12 supported formats
    int pixel_format_idx = fdp.ConsumeIntegralInRange<int>(0, 11);
    enum TJPF pixel_formats[] = {
        TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX,
        TJPF_XBGR, TJPF_XRGB, TJPF_GRAY, TJPF_RGBA,
        TJPF_BGRA, TJPF_ABGR, TJPF_ARGB, TJPF_CMYK
    };
    int pixelFormat = pixel_formats[pixel_format_idx % 12];
    int pixelSize = tjPixelSize[pixelFormat];
    
    // Choose JPEG colorspace (important for color conversion logic)
    int colorspace_idx = fdp.ConsumeIntegralInRange<int>(0, 4);
    enum TJCS colorspaces[] = {
        TJCS_YCbCr,     // Default JPEG colorspace
        TJCS_RGB,       // RGB colorspace (no color conversion)
        TJCS_GRAY,      // Grayscale
        TJCS_CMYK,      // CMYK colorspace
        TJCS_YCCK       // YCCK colorspace (CMYK converted to YCCK)
    };
    enum TJCS colorspace = colorspaces[colorspace_idx % 5];
    
    // Choose subsampling (some combinations may not be valid for all color spaces)
    int subsamp_idx = fdp.ConsumeIntegralInRange<int>(0, 5);
    enum TJSAMP subsampling[] = {
        TJSAMP_444, TJSAMP_422, TJSAMP_420, 
        TJSAMP_GRAY, TJSAMP_440, TJSAMP_411
    };
    enum TJSAMP subsamp = subsampling[subsamp_idx % 6];
    
    // Choose quality for lossy compression (1-100)
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    
    // Choose whether to use lossless compression (critical for blocked branches)
    bool lossless = fdp.ConsumeBool();
    
    // Set precision to 12-bit (target of this harness)
    tj3Set(compress_handle, TJPARAM_PRECISION, 12);
    
    // Set compression parameters from fuzzer input
    tj3Set(compress_handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(compress_handle, TJPARAM_QUALITY, quality);
    tj3Set(compress_handle, TJPARAM_COLORSPACE, colorspace);
    tj3Set(compress_handle, TJPARAM_LOSSLESS, lossless ? 1 : 0);
    
    // Set additional parameters that affect color conversion paths
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
    size_t src_sample_count = src_image_size;
    std::vector<short> src_buf_12(src_sample_count);
    if (src_buf_12.empty()) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    // Fill source buffer with fuzzer data
    size_t bytes_needed = src_sample_count * sizeof(short);
    size_t bytes_to_consume = std::min(bytes_needed, fdp.remaining_bytes());
    
    if (bytes_to_consume > 0) {
        auto byte_data = fdp.ConsumeBytes<uint8_t>(bytes_to_consume);
        if (byte_data.size() > 0) {
            size_t copy_size = std::min(byte_data.size(), src_buf_12.size() * sizeof(short));
            memcpy(src_buf_12.data(), byte_data.data(), copy_size);
        }
    }
    
    // Allocate JPEG buffer
    unsigned char* jpeg_buf = nullptr;
    size_t jpeg_size = jpeg_buf_size;
    // Allocate JPEG buffer using tj3Alloc
    jpeg_buf = static_cast<unsigned char*>(tj3Alloc(jpeg_buf_size));
    if (!jpeg_buf) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    // Call tj3Compress12 - primary target API
    int pitch = width * pixelSize;  // Default pitch (no padding)
    int result = tj3Compress12(compress_handle,
                              src_buf_12.data(),
                              width,
                              pitch,
                              height,
                              pixelFormat,
                              &jpeg_buf,
                              &jpeg_size);
    
    // Free allocated buffers
    if (jpeg_buf) {
        tj3Free(jpeg_buf);
    }
    
    // Additional test: Test with different pitch values (row alignment)
    // This exercises different memory access patterns in color conversion
    if (fdp.remaining_bytes() > 64) {
        // Create buffer with padded rows
        int padded_pitch = pitch + fdp.ConsumeIntegralInRange<int>(0, 16);
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
    
    // Additional test: Test tj3Compress8 for comparison with 8-bit path
    // This helps exercise different code paths in color conversion initialization
    if (fdp.remaining_bytes() > 128) {
        // Create 8-bit source buffer
        std::vector<unsigned char> src_buf_8(src_image_size);
        if (!src_buf_8.empty()) {
            size_t bytes_to_use = std::min(src_buf_8.size(), fdp.remaining_bytes());
            auto byte_data = fdp.ConsumeBytes<uint8_t>(bytes_to_use);
            if (byte_data.size() > 0) {
                memcpy(src_buf_8.data(), byte_data.data(), 
                       std::min(byte_data.size(), src_buf_8.size()));
            }
            
            unsigned char* jpeg_buf3 = static_cast<unsigned char*>(tj3Alloc(jpeg_buf_size));
            if (jpeg_buf3) {
                size_t jpeg_size3 = jpeg_buf_size;
                
                // Temporarily set precision to 8-bit for this test
                tj3Set(compress_handle, TJPARAM_PRECISION, 8);
                tj3Compress8(compress_handle,
                            src_buf_8.data(),
                            width,
                            width * pixelSize,
                            height,
                            pixelFormat,
                            &jpeg_buf3,
                            &jpeg_size3);
                // Restore 12-bit precision
                tj3Set(compress_handle, TJPARAM_PRECISION, 12);
                
                if (jpeg_buf3) {
                    tj3Free(jpeg_buf3);
                }
            }
        }
    }
    
    // Cleanup
    tj3Destroy(compress_handle);
    
    return 0;
}

} // namespace harness_014
