#include <fuzzer/FuzzedDataProvider.h>
#include "turbojpeg.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

namespace harness_021 {
/*
 * Comprehensive 12-bit JPEG compression fuzzing harness targeting diverse color spaces
 * to unlock 168 blocked branches in j12init_color_converter.
 * 
 * This harness specifically targets the j12init_color_converter function which has
 * 168/204 branches blocked (82.4% blocked). The function is critical for 12-bit
 * JPEG compression with various color space conversions.
 * 
 * Target APIs:
 * - tj3Compress12 (Primary Target: 12-bit compression with diverse color spaces)
 * - tj3InitVersion (Required Helper: Initialize turbojpeg instance with version)
 * - tj3Set (Required Helper: Configure 12-bit parameters and color spaces)
 * - tj3Alloc (Required Helper: Memory allocation for JPEG buffer)
 * - tj3Destroy (Required Helper: Cleanup)
 * 
 * Invocation sequence:
 * 1. Initialize compress object with tj3InitVersion(TJINIT_COMPRESS, TURBOJPEG_VERSION_NUMBER)
 * 2. Set precision to 12-bit using TJPARAM_PRECISION
 * 3. Configure color space (TJCS_RGB, TJCS_YCbCr, TJCS_GRAY, TJCS_CMYK, TJCS_YCCK)
 * 4. Configure chroma subsampling (TJSAMP_444, TJSAMP_422, TJSAMP_420, TJSAMP_GRAY)
 * 5. Configure lossy/lossless compression (TJPARAM_LOSSLESS)
 * 6. Configure progressive mode (TJPARAM_PROGRESSIVE)
 * 7. Generate 12-bit source data from fuzzer input
 * 8. Call tj3Compress12 to trigger color conversion paths
 * 9. Clean up with tj3Destroy
 * 
 * Differentiation from existing harnesses:
 * - harness_020 focuses on CMYK/YCCK color spaces only
 * - This harness comprehensively tests ALL 5 color spaces
 * - Tests more diverse pixel format combinations
 * - Includes lossy/lossless and progressive mode testing
 * - Provides broader coverage of j12init_color_converter paths
 * 
 * Implementation strategy:
 * 1. Use FuzzedDataProvider to consume all configuration parameters from input
 * 2. Test all 5 JPEG colorspaces to exercise different conversion paths
 * 3. Use appropriate pixel formats for each colorspace
 * 4. Test various subsampling options for each colorspace
 * 5. Include both lossy and lossless compression
 * 6. Test with and without progressive mode
 * 7. Generate 12-bit source data dynamically from fuzzer input
 * 8. Include proper error handling and resource cleanup
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for comprehensive 12-bit testing
    // Required: dimensions (2 ints) + colorspace + subsampling + lossless + progressive + image data
    if (size < 1024) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize compression handle with version check
    tjhandle compress_handle = tj3InitVersion(TJINIT_COMPRESS, TURBOJPEG_VERSION_NUMBER);
    if (!compress_handle) {
        return 0;
    }
    
    // Consume image dimensions from fuzzer input
    // Use reasonable bounds for efficient fuzzing while testing various aspect ratios
    int width = fdp.ConsumeIntegralInRange<int>(16, 256);
    int height = fdp.ConsumeIntegralInRange<int>(16, 256);
    
    // Set precision to 12-bit explicitly for 12-bit JPEG compression
    // This is critical for targeting j12init_color_converter
    tj3Set(compress_handle, TJPARAM_PRECISION, 12);
    
    // Choose JPEG colorspace - test all 5 colorspaces to exercise different conversion paths
    // This directly affects which branches are taken in j12init_color_converter
    int colorspace_idx = fdp.ConsumeIntegralInRange<int>(0, 4);
    enum TJCS colorspace;
    switch (colorspace_idx) {
        case 0: colorspace = TJCS_RGB; break;     // RGB colorspace
        case 1: colorspace = TJCS_YCbCr; break;   // YCbCr colorspace (most common)
        case 2: colorspace = TJCS_GRAY; break;    // Grayscale colorspace
        case 3: colorspace = TJCS_CMYK; break;    // CMYK colorspace
        case 4: colorspace = TJCS_YCCK; break;    // YCCK colorspace (CMYK transformation)
        default: colorspace = TJCS_YCbCr; break;
    }
    
    // Set colorspace parameter - this is the key configuration for j12init_color_converter
    tj3Set(compress_handle, TJPARAM_COLORSPACE, colorspace);
    
    // Choose pixel format based on colorspace
    // Different colorspaces support different pixel formats
    int pixelFormat;
    if (colorspace == TJCS_CMYK || colorspace == TJCS_YCCK) {
        // CMYK/YCCK colorspaces only support TJPF_CMYK pixel format
        pixelFormat = TJPF_CMYK;
    } else if (colorspace == TJCS_GRAY) {
        // Grayscale colorspace supports TJPF_GRAY
        pixelFormat = TJPF_GRAY;
    } else {
        // RGB and YCbCr colorspaces support various RGB pixel formats
        int rgb_format_idx = fdp.ConsumeIntegralInRange<int>(0, 5);
        enum TJPF rgb_formats[] = {
            TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, TJPF_XBGR, TJPF_XRGB
        };
        pixelFormat = rgb_formats[rgb_format_idx % 6];
    }
    
    // Choose chroma subsampling - different options for different colorspaces
    int subsamp_idx;
    if (colorspace == TJCS_GRAY) {
        // Grayscale only supports TJSAMP_GRAY
        subsamp_idx = 3; // TJSAMP_GRAY
    } else if (colorspace == TJCS_CMYK) {
        // CMYK supports 444 (no subsampling) only
        subsamp_idx = 0; // TJSAMP_444
    } else if (colorspace == TJCS_YCCK) {
        // YCCK supports 444, 422, 420
        subsamp_idx = fdp.ConsumeIntegralInRange<int>(0, 2);
    } else {
        // RGB and YCbCr support all subsampling options
        subsamp_idx = fdp.ConsumeIntegralInRange<int>(0, 3);
    }
    
    enum TJSAMP subsampling[] = {
        TJSAMP_444,    // No chroma subsampling
        TJSAMP_422,    // Horizontal 2:1 subsampling
        TJSAMP_420,    // 2:2 subsampling
        TJSAMP_GRAY,   // Grayscale (no color components)
    };
    enum TJSAMP subsamp = subsampling[subsamp_idx % 4];
    
    // Set subsampling parameter
    tj3Set(compress_handle, TJPARAM_SUBSAMP, subsamp);
    
    // Configure lossy vs lossless compression
    // Lossless compression uses different code paths in j12init_color_converter
    bool use_lossless = fdp.ConsumeBool();
    tj3Set(compress_handle, TJPARAM_LOSSLESS, use_lossless ? 1 : 0);
    
    // Configure progressive mode
    // Progressive mode affects Huffman table generation and scanning
    bool use_progressive = fdp.ConsumeBool();
    tj3Set(compress_handle, TJPARAM_PROGRESSIVE, use_progressive ? 1 : 0);
    
    // Set compression quality for lossy compression
    if (!use_lossless) {
        int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
        tj3Set(compress_handle, TJPARAM_QUALITY, quality);
    }
    
    // Calculate required buffer size for 12-bit source data
    int pixelSize = tjPixelSize[pixelFormat];
    int pitch = width * pixelSize;
    size_t srcBufSize = pitch * height;
    
    // Check if we have enough input data for source buffer
    if (fdp.remaining_bytes() < srcBufSize * sizeof(short)) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    // Allocate and populate 12-bit source buffer
    std::vector<short> srcBuf(srcBufSize);
    for (size_t i = 0; i < srcBufSize && fdp.remaining_bytes() > 0; ++i) {
        // Generate 12-bit values (0-4095) from fuzzer input
        srcBuf[i] = fdp.ConsumeIntegralInRange<short>(0, 4095);
    }
    
    // Allocate JPEG buffer - let TurboJPEG handle allocation
    unsigned char* jpegBuf = nullptr;
    size_t jpegSize = 0;
    
    // Call tj3Compress12 - this triggers jpeg_start_compress -> jinit_compress_master -> j12init_color_converter
    int compress_result = tj3Compress12(compress_handle, srcBuf.data(), width, pitch, height,
                                       pixelFormat, &jpegBuf, &jpegSize);
    
    // Clean up JPEG buffer if allocated
    if (jpegBuf) {
        tj3Free(jpegBuf);
    }
    
    // Destroy compression handle
    tj3Destroy(compress_handle);
    
    return 0;
}

} // namespace harness_021
