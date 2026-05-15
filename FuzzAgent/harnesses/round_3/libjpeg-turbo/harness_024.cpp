#include <fuzzer/FuzzedDataProvider.h>
#include "turbojpeg.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string>

namespace harness_024 {
/*
 * Fuzzing harness for libjpeg-turbo 12-bit compression with diverse color spaces
 * targeting blocked conversion paths in j12init_color_converter (168 blocked branches).
 * 
 * This harness specifically focuses on unlocking 168 blocked branches in j12init_color_converter
 * by targeting tj3Compress12 with 12 different pixel formats and color spaces to exercise
 * all possible color conversion paths. The blocked branches correspond to different
 * color space conversions for 12-bit compression.
 * 
 * Target APIs:
 * - tj3Compress12 (Primary Target: 12-bit compression with diverse pixel formats)
 * - tj3Compress8 (Helper: 8-bit compression for comparison and additional coverage)
 * - tj3Set (Required Helper: Configure compression parameters with various color formats)
 * - tj3SetCroppingRegion (Optional: Test with cropping regions)
 * - tj3SetScalingFactor (Optional: Test with scaling factors)
 * - tj3Destroy (Required Helper: Cleanup)
 * - tj3Free (Required Helper: Memory cleanup)
 * - tj3Alloc (Required Helper: Memory allocation for buffers)
 * 
 * Invocation sequence:
 * 1. Call tj3Alloc to allocate JPEG buffers for compression output
 * 2. Call tj3Init to initialize compression handle
 * 3. Call tj3Set to configure compression parameters with various color formats
 *    (TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, TJPF_XBGR, TJPF_XRGB, TJPF_GRAY,
 *     TJPF_RGBA, TJPF_BGRA, TJPF_ABGR, TJPF_ARGB, TJPF_CMYK)
 * 4. Optionally call tj3SetCroppingRegion and tj3SetScalingFactor
 * 5. Call tj3Compress12 with 12-bit source data in various color formats
 * 6. Call tj3Compress8 with 8-bit source data for comparison
 * 7. Call tj3Free and tj3Destroy for cleanup
 * 
 * Differentiation from existing harnesses:
 * - harness_020 tests 12-bit CMYK/YCCK compression with file operations
 * - harness_023 tests 12-bit and 16-bit CMYK compression
 * - This harness specifically tests ALL 12 pixel formats for 12-bit compression
 * - Focuses on diverse color space conversions for unlocking blocked branches
 * - Includes tj3SetCroppingRegion and tj3SetScalingFactor for additional coverage
 * - Tests both 12-bit (tj3Compress12) and 8-bit (tj3Compress8) compression paths
 * - Provides semantic diversity by exercising all pixel format conversion paths
 * 
 * Implementation strategy:
 * 1. Generate source data from fuzzer input for all pixel formats
 * 2. Test all 12 pixel formats to exercise different color conversion paths
 * 3. Use 12-bit precision for primary testing (tj3Compress12)
 * 4. Use 8-bit precision for comparison testing (tj3Compress8)
 * 5. Test with optional cropping regions and scaling factors
 * 6. Include proper error handling and resource cleanup
 * 7. Test various compression parameters (quality, subsampling, etc.)
 * 8. Ensure proper memory management with tj3Alloc/tj3Free
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for 12-bit compression with diverse pixel formats
    // Required: dimensions (2 ints) + pixel format + subsampling + quality + image data
    if (size < 1024) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize compression handle
    tjhandle compress_handle = tj3Init(TJINIT_COMPRESS);
    if (!compress_handle) {
        return 0;
    }
    
    // Consume image dimensions from fuzzer input
    // Use reasonable bounds for efficient fuzzing
    int width = fdp.ConsumeIntegralInRange<int>(8, 128);
    int height = fdp.ConsumeIntegralInRange<int>(8, 128);
    
    // Choose pixel format from all 12 available formats
    // This is critical for exercising different color conversion paths in j12init_color_converter
    int pixel_format_idx = fdp.ConsumeIntegralInRange<int>(0, 11);
    int pixelFormats[] = {
        TJPF_RGB,    // 0: RGB (3 components)
        TJPF_BGR,    // 1: BGR (3 components)
        TJPF_RGBX,   // 2: RGBX (4 components)
        TJPF_BGRX,   // 3: BGRX (4 components)
        TJPF_XBGR,   // 4: XBGR (4 components)
        TJPF_XRGB,   // 5: XRGB (4 components)
        TJPF_GRAY,   // 6: Grayscale (1 component)
        TJPF_RGBA,   // 7: RGBA (4 components)
        TJPF_BGRA,   // 8: BGRA (4 components)
        TJPF_ABGR,   // 9: ABGR (4 components)
        TJPF_ARGB,   // 10: ARGB (4 components)
        TJPF_CMYK    // 11: CMYK (4 components)
    };
    int pixelFormat = pixelFormats[pixel_format_idx % 12];
    int pixelSize = tjPixelSize[pixelFormat];
    
    // Set precision to 12-bit for tj3Compress12
    tj3Set(compress_handle, TJPARAM_PRECISION, 12);
    
    // Choose JPEG colorspace based on pixel format
    // Different pixel formats may work better with specific colorspaces
    int colorspace_choice = fdp.ConsumeIntegralInRange<int>(0, 4);
    enum TJCS colorspaces[] = {
        TJCS_RGB,     // RGB colorspace
        TJCS_YCbCr,   // YCbCr colorspace
        TJCS_GRAY,    // Grayscale colorspace
        TJCS_CMYK,    // CMYK colorspace (for CMYK pixel format)
        TJCS_YCCK     // YCCK colorspace (for CMYK pixel format)
    };
    enum TJCS colorspace = colorspaces[colorspace_choice % 5];
    
    // Special handling for CMYK pixel format - should use CMYK or YCCK colorspace
    if (pixelFormat == TJPF_CMYK) {
        colorspace = (fdp.ConsumeBool()) ? TJCS_CMYK : TJCS_YCCK;
    }
    
    // Set colorspace parameter
    tj3Set(compress_handle, TJPARAM_COLORSPACE, colorspace);
    
    // Choose subsampling based on pixel format and colorspace
    int subsamp_idx = fdp.ConsumeIntegralInRange<int>(0, 5);
    enum TJSAMP subsampling[] = {
        TJSAMP_444,    // No chroma subsampling
        TJSAMP_422,    // Horizontal 2:1 subsampling
        TJSAMP_420,    // 2:2 subsampling
        TJSAMP_440,    // Vertical 2:1 subsampling
        TJSAMP_411,    // Horizontal 4:1 subsampling
        TJSAMP_441     // Vertical 4:1 subsampling (special case)
    };
    enum TJSAMP subsamp = subsampling[subsamp_idx % 6];
    
    // Set subsampling parameter
    tj3Set(compress_handle, TJPARAM_SUBSAMP, subsamp);
    
    // Choose quality
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    tj3Set(compress_handle, TJPARAM_QUALITY, quality);
    
    // Set other compression parameters that may affect color conversion
    tj3Set(compress_handle, TJPARAM_FASTDCT, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_OPTIMIZE, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_PROGRESSIVE, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_ARITHMETIC, fdp.ConsumeBool());
    
    // Calculate image buffer size for source data
    // For 12-bit compression, we need short (16-bit) samples
    size_t image_size = width * height * pixelSize;
    
    // Check if we have enough data for the image
    if (fdp.remaining_bytes() < image_size * sizeof(short)) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    // Allocate source buffer for 12-bit data (short type)
    std::vector<short> src_buf_12bit(image_size);
    for (size_t i = 0; i < image_size && fdp.remaining_bytes() >= sizeof(short); i++) {
        src_buf_12bit[i] = fdp.ConsumeIntegral<short>();
    }
    
    // Allocate source buffer for 8-bit data (unsigned char type)
    std::vector<unsigned char> src_buf_8bit(image_size);
    for (size_t i = 0; i < image_size && fdp.remaining_bytes() >= sizeof(unsigned char); i++) {
        src_buf_8bit[i] = fdp.ConsumeIntegral<unsigned char>();
    }
    
    // Allocate JPEG buffer using tj3Alloc
    // Calculate worst-case buffer size using tj3JPEGBufSize
    size_t jpeg_buf_size = tj3JPEGBufSize(width, height, subsamp);
    unsigned char* jpeg_buf = (unsigned char*)tj3Alloc(jpeg_buf_size);
    if (!jpeg_buf) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    size_t actual_jpeg_size = jpeg_buf_size;
    
    // Optional: Test with cropping region
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= sizeof(int) * 4) {
        tjregion cropping_region;
        cropping_region.x = fdp.ConsumeIntegralInRange<int>(0, width / 2);
        cropping_region.y = fdp.ConsumeIntegralInRange<int>(0, height / 2);
        cropping_region.w = fdp.ConsumeIntegralInRange<int>(1, width - cropping_region.x);
        cropping_region.h = fdp.ConsumeIntegralInRange<int>(1, height - cropping_region.y);
        
        tj3SetCroppingRegion(compress_handle, cropping_region);
    }
    
    // Optional: Test with scaling factor
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= sizeof(int) * 2) {
        tjscalingfactor scaling_factor;
        scaling_factor.num = fdp.ConsumeIntegralInRange<int>(1, 16);
        scaling_factor.denom = fdp.ConsumeIntegralInRange<int>(1, 16);
        
        tj3SetScalingFactor(compress_handle, scaling_factor);
    }
    
    // Test 12-bit compression (primary target)
    int result_12bit = tj3Compress12(compress_handle,
                                     src_buf_12bit.data(),
                                     width,
                                     width * pixelSize,  // pitch
                                     height,
                                     pixelFormat,
                                     &jpeg_buf,
                                     &actual_jpeg_size);
    
    // Reset JPEG buffer for 8-bit compression test
    // We need to allocate a new buffer or reuse with proper size
    if (jpeg_buf) {
        tj3Free(jpeg_buf);
        jpeg_buf = nullptr;
    }
    
    // Re-allocate buffer for 8-bit compression
    jpeg_buf = (unsigned char*)tj3Alloc(jpeg_buf_size);
    if (!jpeg_buf) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    actual_jpeg_size = jpeg_buf_size;
    
    // Set precision to 8-bit for tj3Compress8
    tj3Set(compress_handle, TJPARAM_PRECISION, 8);
    
    // Test 8-bit compression (comparison)
    int result_8bit = tj3Compress8(compress_handle,
                                   src_buf_8bit.data(),
                                   width,
                                   width * pixelSize,  // pitch
                                   height,
                                   pixelFormat,
                                   &jpeg_buf,
                                   &actual_jpeg_size);
    
    // Clean up
    if (jpeg_buf) {
        tj3Free(jpeg_buf);
    }
    tj3Destroy(compress_handle);
    
    return 0;
}

} // namespace harness_024
