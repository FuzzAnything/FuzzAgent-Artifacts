#include <fuzzer/FuzzedDataProvider.h>
#include "turbojpeg.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string>

namespace harness_025 {
/*
 * Fuzzing harness for libjpeg-turbo 12-bit compression with comprehensive color space testing
 * targeting blocked conversion paths in j12init_color_converter (168 blocked branches, 82.4% blocked).
 * 
 * This harness specifically targets the 168 blocked branches in j12init_color_converter
 * by testing comprehensive color space conversions for 12-bit JPEG compression.
 * It follows the exact API sequence specified in coverage guidance to maximize coverage
 * of color conversion paths that remain untested.
 * 
 * Target APIs (per coverage guidance):
 * - tj3Compress12 (Primary Target: 12-bit compression with diverse color spaces)
 * - tj3Compress16 (Secondary Target: 16-bit compression for comparison)
 * - tj3Set (Required: Configure compression parameters)
 * - tj3SetScalingFactor (Required: Test scaling)
 * - tj3SetICCProfile (Required: Test ICC profile handling)
 * - tj3InitVersion (Required Helper: Initialize with version check)
 * - tj3Destroy (Required Helper: Cleanup)
 * - tj3Alloc (Required Helper: Memory allocation)
 * - tj3Free (Required Helper: Memory cleanup)
 * 
 * Invocation sequence (per coverage guidance):
 * 1. Call tj3InitVersion to initialize TurboJPEG instance.
 * 2. Call tj3Alloc to allocate image buffers.
 * 3. Call tj3Set to configure compression parameters (color space, subsampling, quality).
 * 4. Call tj3SetScalingFactor to test scaling.
 * 5. Call tj3SetICCProfile to test ICC profile handling.
 * 6. Call tj3Compress12 with various color spaces (RGB, BGR, RGBX, BGRX, XBGR, XRGB, CMYK, YCbCr).
 * 7. Call tj3Compress16 with 16-bit data for comparison.
 * 8. Call tj3Free and tj3Destroy.
 * 
 * Differentiation from existing harnesses:
 * - harness_024 tests 12-bit compression with 12 pixel formats but uses tj3Init and tj3Compress8
 * - This harness uses tj3InitVersion (as specified in guidance) and tests tj3Compress16
 * - This harness includes tj3SetICCProfile testing which is missing in harness_024
 * - This harness follows the exact invocation sequence from coverage guidance
 * - Focuses on comprehensive color space testing for 12-bit paths specifically
 * 
 * Implementation strategy:
 * 1. Use tj3InitVersion for initialization with version checking
 * 2. Test all 8 color spaces mentioned in guidance (RGB, BGR, RGBX, BGRX, XBGR, XRGB, CMYK, YCbCr)
 * 3. Generate source data from fuzzer input for both 12-bit and 16-bit compression
 * 4. Test with scaling factors to exercise different code paths
 * 5. Test with ICC profiles to cover ICC-related conversion paths
 * 6. Include proper error handling and resource cleanup
 * 7. Ensure semantic diversity by following exact guidance sequence
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for comprehensive testing
    // Required: dimensions (2 ints) + color space + subsampling + quality + 
    // scaling factors + ICC profile data + image data for 12-bit and 16-bit
    if (size < 2048) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Call tj3InitVersion to initialize TurboJPEG instance
    // Use TURBOJPEG_VERSION_NUMBER constant from turbojpeg.h
    tjhandle compress_handle = tj3InitVersion(TJINIT_COMPRESS, TURBOJPEG_VERSION_NUMBER);
    if (!compress_handle) {
        return 0;
    }
    
    // Consume image dimensions from fuzzer input
    // Use reasonable bounds for efficient fuzzing
    int width = fdp.ConsumeIntegralInRange<int>(16, 128);
    int height = fdp.ConsumeIntegralInRange<int>(16, 128);
    
    // Choose pixel format from the 8 color spaces mentioned in guidance
    // RGB, BGR, RGBX, BGRX, XBGR, XRGB, CMYK, YCbCr
    // Note: YCbCr is a colorspace (TJCS_YCbCr), not a pixel format
    // For YCbCr testing we'll use RGB pixel format but set colorspace to TJCS_YCbCr
    int pixel_format_idx = fdp.ConsumeIntegralInRange<int>(0, 7);
    int pixelFormats[] = {
        TJPF_RGB,    // 0: RGB (3 components)
        TJPF_BGR,    // 1: BGR (3 components)
        TJPF_RGBX,   // 2: RGBX (4 components)
        TJPF_BGRX,   // 3: BGRX (4 components)
        TJPF_XBGR,   // 4: XBGR (4 components)
        TJPF_XRGB,   // 5: XRGB (4 components)
        TJPF_CMYK,   // 6: CMYK (4 components)
        TJPF_RGB     // 7: For YCbCr testing - use RGB pixel format
    };
    
    // Handle YCbCr specially - for pixel format we use RGB, 
    // but we'll set colorspace to TJCS_YCbCr when pixel_format_idx == 7
    int pixelFormat = pixelFormats[pixel_format_idx];
    bool test_ycbcr = (pixel_format_idx == 7);
    
    int pixelSize = tjPixelSize[pixelFormat];
    
    // 2. Call tj3Alloc to allocate image buffers
    // Calculate worst-case buffer size using tj3JPEGBufSize
    size_t jpeg_buf_size = tj3JPEGBufSize(width, height, TJSAMP_444);
    unsigned char* jpeg_buf = (unsigned char*)tj3Alloc(jpeg_buf_size);
    if (!jpeg_buf) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    // 3. Call tj3Set to configure compression parameters
    
    // Set precision to 12-bit for tj3Compress12 (primary target)
    tj3Set(compress_handle, TJPARAM_PRECISION, 12);
    
    // Choose colorspace based on pixel format
    // For comprehensive testing of color conversion paths
    int colorspace_choice = fdp.ConsumeIntegralInRange<int>(0, 4);
    enum TJCS colorspaces[] = {
        TJCS_RGB,     // RGB colorspace
        TJCS_YCbCr,   // YCbCr colorspace
        TJCS_GRAY,    // Grayscale colorspace
        TJCS_CMYK,    // CMYK colorspace
        TJCS_YCCK     // YCCK colorspace
    };
    enum TJCS colorspace = colorspaces[colorspace_choice % 5];
    
    // Special handling for YCbCr testing
    if (pixel_format_idx == 7) {
        colorspace = TJCS_YCbCr;
    }
    
    // Special handling for CMYK pixel format
    if (pixelFormat == TJPF_CMYK) {
        colorspace = (fdp.ConsumeBool()) ? TJCS_CMYK : TJCS_YCCK;
    }
    
    tj3Set(compress_handle, TJPARAM_COLORSPACE, colorspace);
    
    // Choose subsampling - different subsampling affects color conversion
    int subsamp_idx = fdp.ConsumeIntegralInRange<int>(0, 5);
    enum TJSAMP subsampling[] = {
        TJSAMP_444,    // No chroma subsampling
        TJSAMP_422,    // Horizontal 2:1 subsampling
        TJSAMP_420,    // 2:2 subsampling
        TJSAMP_440,    // Vertical 2:1 subsampling
        TJSAMP_411,    // Horizontal 4:1 subsampling
        TJSAMP_441     // Vertical 4:1 subsampling
    };
    enum TJSAMP subsamp = subsampling[subsamp_idx % 6];
    tj3Set(compress_handle, TJPARAM_SUBSAMP, subsamp);
    
    // Set quality
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    tj3Set(compress_handle, TJPARAM_QUALITY, quality);
    
    // Set other compression parameters that may affect color conversion
    tj3Set(compress_handle, TJPARAM_FASTDCT, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_OPTIMIZE, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_PROGRESSIVE, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_ARITHMETIC, fdp.ConsumeBool());
    // 4. Call tj3SetScalingFactor to test scaling
    // Scaling factors affect how color conversion is applied
    tjscalingfactor scaling_factor;
    scaling_factor.num = fdp.ConsumeIntegralInRange<int>(1, 16);
    scaling_factor.denom = fdp.ConsumeIntegralInRange<int>(1, 16);
    tj3SetScalingFactor(compress_handle, scaling_factor);
    
    // 5. Call tj3SetICCProfile to test ICC profile handling
    // ICC profiles can affect color conversion paths
    if (fdp.remaining_bytes() >= 128) {
        size_t icc_profile_size = fdp.ConsumeIntegralInRange<size_t>(64, 512);
        if (fdp.remaining_bytes() >= icc_profile_size) {
            std::vector<unsigned char> icc_profile_data = 
                fdp.ConsumeBytes<unsigned char>(icc_profile_size);
            tj3SetICCProfile(compress_handle, icc_profile_data.data(), icc_profile_size);
        }
    }
    
    // Calculate image buffer size for source data
    size_t image_size = width * height * pixelSize;
    
    // Check if we have enough data for both 12-bit and 16-bit testing
    size_t required_bytes = image_size * sizeof(short) +  // 12-bit data
                           image_size * sizeof(unsigned short) +  // 16-bit data
                           128;  // Extra for safety
    
    if (fdp.remaining_bytes() < required_bytes) {
        tj3Free(jpeg_buf);
        tj3Destroy(compress_handle);
        return 0;
    }
    
    // Allocate source buffer for 12-bit data (short type for tj3Compress12)
    std::vector<short> src_buf_12bit(image_size);
    for (size_t i = 0; i < image_size; i++) {
        src_buf_12bit[i] = fdp.ConsumeIntegral<short>();
    }
    
    // Allocate source buffer for 16-bit data (unsigned short type for tj3Compress16)
    std::vector<unsigned short> src_buf_16bit(image_size);
    for (size_t i = 0; i < image_size; i++) {
        src_buf_16bit[i] = fdp.ConsumeIntegral<unsigned short>();
    }
    
    // Prepare JPEG buffer parameters
    size_t actual_jpeg_size = jpeg_buf_size;
    unsigned char* jpeg_buf_ptr = jpeg_buf;
    
    // 6. Call tj3Compress12 with various color spaces
    // Test primary target: 12-bit compression
    // Use pitch = width * pixelSize (0 would also work but explicit is clearer)
    int pitch = width * pixelSize;
    
    int compress12_result = tj3Compress12(
        compress_handle,
        src_buf_12bit.data(),
        width,
        pitch,
        height,
        pixelFormat,
        &jpeg_buf_ptr,
        &actual_jpeg_size
    );
    
    // Reset buffer for next compression test
    // Note: tj3Compress12 may have reallocated jpeg_buf_ptr
    // We need to ensure we're using the potentially reallocated buffer
    if (jpeg_buf_ptr != jpeg_buf) {
        // Buffer was reallocated, free the old one
        tj3Free(jpeg_buf);
        jpeg_buf = jpeg_buf_ptr;
    }
    
    // Reset size for next compression
    actual_jpeg_size = jpeg_buf_size;
    
    // 7. Call tj3Compress16 with 16-bit data for comparison
    // First, set precision to 16-bit for tj3Compress16
    tj3Set(compress_handle, TJPARAM_PRECISION, 16);
    
    int compress16_result = tj3Compress16(
        compress_handle,
        src_buf_16bit.data(),
        width,
        pitch,
        height,
        pixelFormat,
        &jpeg_buf_ptr,
        &actual_jpeg_size
    );
    
    // Handle potential buffer reallocation
    if (jpeg_buf_ptr != jpeg_buf) {
        tj3Free(jpeg_buf);
        jpeg_buf = jpeg_buf_ptr;
    }
    
    // 8. Call tj3Free and tj3Destroy
    tj3Free(jpeg_buf);
    tj3Destroy(compress_handle);
    
    return 0;
}

} // namespace harness_025
