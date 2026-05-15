#include <fuzzer/FuzzedDataProvider.h>
#include "turbojpeg.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

namespace harness_018 {
/*
 * Fuzzing harness for libjpeg-turbo 12-bit CMYK image compression targeting
 * blocked color conversion paths in j12init_color_converter (164 blocked branches).
 * 
 * This harness specifically focuses on unlocking 164 blocked branches in j12init_color_converter
 * by targeting tj3Compress12 with CMYK color space configurations. The blocked branches
 * correspond to different color space conversions, particularly CMYK to YCCK conversion.
 * 
 * Target APIs:
 * - tj3Compress12 (Primary Target: 12-bit compression with CMYK color space)
 * - tj3Set (Required Helper: Configure color space and other parameters)
 * - tj3Init (Required Helper: Initialize compressor instance)
 * - tj3Destroy (Required Helper: Cleanup)
 * - tj3Alloc (Required Helper: Memory allocation for 12-bit image data)
 * - tj3Free (Required Helper: Memory cleanup)
 * 
 * Invocation sequence:
 * 1. Call tj3Init to create a compressor instance
 * 2. Call tj3Set with color space parameters (specifically target CMYK color space)
 * 3. Call tj3Alloc to allocate memory for 12-bit CMYK source image data
 * 4. Call tj3Compress12 with appropriate parameters for 12-bit CMYK compression
 * 5. Call tj3Free to free allocated memory
 * 6. Call tj3Destroy to clean up compressor instance
 * 
 * Differentiation from existing harnesses:
 * - harness_015 tests 12-bit precision with various pixel formats including CMYK
 * - This harness specifically focuses on CMYK color space to target the 164 blocked branches
 * - Focuses on CMYK-specific color conversion paths in j12init_color_converter
 * - Tests various combinations of colorspace settings (TJCS_CMYK, TJCS_YCCK) for CMYK data
 * 
 * Implementation strategy:
 * 1. Test TJPF_CMYK pixel format with different JPEG colorspaces (TJCS_CMYK, TJCS_YCCK)
 * 2. Test various subsampling options applicable to CMYK (444, 422, 420, GRAY)
 * 3. Test different quality levels and compression options
 * 4. Generate 12-bit CMYK source data from fuzzer input
 * 5. Test both regular and lossless compression modes for CMYK
 * 6. Include proper error handling and resource cleanup
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for 12-bit CMYK testing
    // Required: dimensions (2 ints) + colorspace + subsampling + quality + 
    // lossless flag + other params + image data
    if (size < 256) return 0;
    
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
    
    // Fixed pixel format: TJPF_CMYK (4 components per pixel)
    // This is the key difference from other harnesses - focusing specifically on CMYK
    int pixelFormat = TJPF_CMYK;
    int pixelSize = tjPixelSize[pixelFormat];  // Should be 4 for CMYK
    
    // Set precision to 12-bit explicitly
    tj3Set(compress_handle, TJPARAM_PRECISION, 12);
    
    // Choose JPEG colorspace for CMYK data
    // Critical: CMYK data can use TJCS_CMYK or TJCS_YCCK colorspace
    // Different colorspaces trigger different color conversion paths
    int colorspace_idx = fdp.ConsumeIntegralInRange<int>(0, 3);
    enum TJCS colorspaces[] = {
        TJCS_CMYK,       // Direct CMYK encoding
        TJCS_YCCK,       // CMYK converted to YCCK (luminance-chrominance)
        TJCS_RGB,        // RGB colorspace (should trigger conversion or error)
        TJCS_DEFAULT     // Let library choose based on pixel format
    };
    enum TJCS colorspace = colorspaces[colorspace_idx % 4];
    
    // Set colorspace parameter
    tj3Set(compress_handle, TJPARAM_COLORSPACE, colorspace);
    
    // Choose subsampling - some options may not be valid for CMYK
    // Testing various options to explore different conversion paths
    int subsamp_idx = fdp.ConsumeIntegralInRange<int>(0, 5);
    enum TJSAMP subsampling[] = {
        TJSAMP_444,    // No chroma subsampling - valid for CMYK
        TJSAMP_422,    // Horizontal 2:1 subsampling - may trigger special handling
        TJSAMP_420,    // 2:2 subsampling - may trigger special handling
        TJSAMP_GRAY,   // Grayscale - should trigger conversion or error
        TJSAMP_440,    // Vertical 2:1 subsampling
        TJSAMP_411     // 4:1:1 subsampling
    };
    enum TJSAMP subsamp = subsampling[subsamp_idx % 6];
    
    // Set subsampling parameter
    tj3Set(compress_handle, TJPARAM_SUBSAMP, subsamp);
    
    // Choose quality - affects quantization tables
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    tj3Set(compress_handle, TJPARAM_QUALITY, quality);
    
    // Test lossless compression mode for CMYK
    // Lossless mode uses different code paths and may affect color conversion
    bool lossless = fdp.ConsumeBool();
    if (lossless) {
        tj3Set(compress_handle, TJPARAM_LOSSLESS, 1);
        // Set lossless predictor selection value
        int lossless_psv = fdp.ConsumeIntegralInRange<int>(1, 7);
        tj3Set(compress_handle, TJPARAM_LOSSLESSPSV, lossless_psv);
        // Set lossless point transform
        int lossless_pt = fdp.ConsumeIntegralInRange<int>(0, 15);
        tj3Set(compress_handle, TJPARAM_LOSSLESSPT, lossless_pt);
    }
    
    // Set other compression parameters
    // These affect DCT processing and may interact with color conversion
    tj3Set(compress_handle, TJPARAM_FASTDCT, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_OPTIMIZE, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_PROGRESSIVE, fdp.ConsumeBool());
    tj3Set(compress_handle, TJPARAM_ARITHMETIC, fdp.ConsumeBool());
    
    // Set restart markers - can affect error resilience
    bool use_restart_blocks = fdp.ConsumeBool();
    if (use_restart_blocks) {
        int restart_blocks = fdp.ConsumeIntegralInRange<int>(1, 64);
        tj3Set(compress_handle, TJPARAM_RESTARTBLOCKS, restart_blocks);
    } else {
        int restart_rows = fdp.ConsumeIntegralInRange<int>(0, 16);
        tj3Set(compress_handle, TJPARAM_RESTARTROWS, restart_rows);
    }
    
    // Generate 12-bit CMYK source image data
    // CMYK uses 4 components per pixel, each component is 12-bit (stored in 16-bit short)
    size_t image_samples = (size_t)width * height * pixelSize;  // pixelSize should be 4 for CMYK
    size_t image_bytes = image_samples * sizeof(short);  // 12-bit data stored in short
    
    if (image_samples == 0 || image_bytes == 0) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    // Allocate source image buffer for 12-bit CMYK data
    std::vector<short> src_buf(image_samples);
    if (src_buf.empty()) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    // Fill with fuzzer data for CMYK components
    // CMYK has 4 components: Cyan, Magenta, Yellow, Key (black)
    // Each component is 12-bit (0-4095 range)
    auto remaining = fdp.ConsumeRemainingBytes<uint8_t>();
    size_t bytes_to_copy = std::min(src_buf.size() * sizeof(short), remaining.size());
    
    if (bytes_to_copy > 0) {
        // Copy fuzzer data into source buffer
        memcpy(src_buf.data(), remaining.data(), bytes_to_copy);
        
        // Ensure 12-bit range for valid data
        // Mask to 12-bit range (0-4095) to avoid out-of-range values
        for (size_t i = 0; i < src_buf.size(); i++) {
            src_buf[i] = src_buf[i] & 0x0FFF;  // Keep only 12 bits
        }
    }
    
    // Calculate worst-case JPEG buffer size
    size_t jpeg_buf_size = tj3JPEGBufSize(width, height, subsamp);
    if (jpeg_buf_size == 0) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    // For CMYK, we may need additional space for color conversion overhead
    // Add some padding to be safe
    jpeg_buf_size += 1024;
    
    // Allocate JPEG buffer using tj3Alloc
    unsigned char* jpeg_buf = (unsigned char*)tj3Alloc(jpeg_buf_size);
    if (!jpeg_buf) {
        tj3Destroy(compress_handle);
        return 0;
    }
    
    size_t compressed_size = jpeg_buf_size;
    
    // Perform 12-bit CMYK compression
    // This is the key API call that should exercise j12init_color_converter
    // Pitch is in samples, not bytes: width * pixelSize (4 for CMYK)
    int pitch = width * pixelSize;  // Samples per row
    
    int compress_result = tj3Compress12(compress_handle,
                                        src_buf.data(),
                                        width,
                                        pitch,  // Samples per row
                                        height,
                                        pixelFormat,
                                        &jpeg_buf,  // Note: pointer may be reallocated
                                        &compressed_size);
    
    // Note: tj3Compress12 may reallocate jpeg_buf if TJPARAM_NOREALLOC is not set
    // We need to free the potentially reallocated buffer
    unsigned char* buffer_to_free = jpeg_buf;
    
    // Free allocated JPEG buffer (use the potentially reallocated pointer)
    if (buffer_to_free) {
        tj3Free(buffer_to_free);
    }
    
    // Clean up compressor handle
    tj3Destroy(compress_handle);
    
    return 0;
}

} // namespace harness_018
