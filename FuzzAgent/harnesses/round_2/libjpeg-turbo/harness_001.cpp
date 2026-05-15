#include "turbojpeg.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_001 {
/*
 * Harness 001: JPEG compression fuzzer using TurboJPEG API
 * This harness tests JPEG compression with various parameters including
 * different subsampling modes, quality levels, and compression options.
 */


int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for image parameters
    if (size < 200) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize TurboJPEG compression handle
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (handle == NULL) return 0;
    
    // Consume compression parameters from fuzzer input
    bool bottomUp = fdp.ConsumeBool();
    bool fastDCT = fdp.ConsumeBool();
    bool optimize = fdp.ConsumeBool();
    bool progressive = fdp.ConsumeBool();
    bool arithmetic = fdp.ConsumeBool();
    bool noRealloc = fdp.ConsumeBool();
    int restartRows = fdp.ConsumeIntegralInRange<int>(0, 100);
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    
    // Get colorspace from input
    uint8_t csIndex = fdp.ConsumeIntegral<uint8_t>() % 6;
    enum TJCS colorspaces[6] = {
        TJCS_RGB, TJCS_YCbCr, TJCS_GRAY, TJCS_CMYK, TJCS_YCCK, TJCS_DEFAULT
    };
    enum TJCS colorspace = colorspaces[csIndex];
    
    // Get subsampling from input
    uint8_t ssIndex = fdp.ConsumeIntegral<uint8_t>() % 6;
    enum TJSAMP subsampling[6] = {
        TJSAMP_444, TJSAMP_422, TJSAMP_420, TJSAMP_440, TJSAMP_411, TJSAMP_GRAY
    };
    enum TJSAMP subsamp = subsampling[ssIndex];
    
    // Get pixel format from input
    uint8_t pfIndex = fdp.ConsumeIntegral<uint8_t>() % 11;
    enum TJPF pixelFormats[11] = {
        TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, TJPF_XBGR, TJPF_XRGB,
        TJPF_GRAY, TJPF_RGBA, TJPF_BGRA, TJPF_ABGR, TJPF_ARGB
    };
    enum TJPF pixelFormat = pixelFormats[pfIndex];
    
    // Set compression parameters
    tj3Set(handle, TJPARAM_BOTTOMUP, bottomUp ? 1 : 0);
    tj3Set(handle, TJPARAM_COLORSPACE, colorspace);
    tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(handle, TJPARAM_FASTDCT, fastDCT ? 1 : 0);
    tj3Set(handle, TJPARAM_OPTIMIZE, optimize ? 1 : 0);
    tj3Set(handle, TJPARAM_PROGRESSIVE, progressive ? 1 : 0);
    tj3Set(handle, TJPARAM_ARITHMETIC, arithmetic ? 1 : 0);
    tj3Set(handle, TJPARAM_NOREALLOC, noRealloc ? 1 : 0);
    tj3Set(handle, TJPARAM_RESTARTROWS, restartRows);
    tj3Set(handle, TJPARAM_QUALITY, quality);
    
    // Limit maximum image size to prevent excessive memory usage
    tj3Set(handle, TJPARAM_MAXPIXELS, 1048576); // 1 megapixel
    
    // Create synthetic test image data based on fuzzer input
    int width = fdp.ConsumeIntegralInRange<int>(1, 512);
    int height = fdp.ConsumeIntegralInRange<int>(1, 512);
    
    // Skip if image would be too large
    if ((uint64_t)width * height > 1048576) {
        tj3Destroy(handle);
        return 0;
    }
    
    // Calculate required buffer size
    int pixelSize = tjPixelSize[pixelFormat];
    size_t srcBufSize = (size_t)width * height * pixelSize;
    
    // Limit buffer size to reasonable amount
    if (srcBufSize > 10 * 1024 * 1024) { // 10MB limit
        tj3Destroy(handle);
        return 0;
    }
    
    // Check if we have enough data for the image
    if (fdp.remaining_bytes() < srcBufSize) {
        tj3Destroy(handle);
        return 0;
    }
    
    // Create source image buffer from fuzzer data
    std::vector<unsigned char> srcBuf(srcBufSize);
    auto consumed = fdp.ConsumeBytes<unsigned char>(srcBufSize);
    if (consumed.size() < srcBufSize) {
        tj3Destroy(handle);
        return 0;
    }
    std::copy(consumed.begin(), consumed.end(), srcBuf.begin());
    
    // Calculate maximum compressed size
    size_t maxBufSize = tj3JPEGBufSize(width, height, subsamp);
    if (maxBufSize == 0 || maxBufSize > 100 * 1024 * 1024) { // 100MB limit
        tj3Destroy(handle);
        return 0;
    }
    
    // Prepare destination buffer based on noRealloc flag
    unsigned char* jpegBuf = nullptr;
    size_t jpegSize = maxBufSize;
    
    if (noRealloc) {
        // Allocate buffer if no reallocation is allowed
        jpegBuf = (unsigned char*)tj3Alloc(maxBufSize);
        if (jpegBuf == NULL) {
            tj3Destroy(handle);
            return 0;
        }
    } else {
        // Let TurboJPEG allocate buffer
        jpegBuf = NULL;
        jpegSize = 0; // Let library determine size
    }
    
    // Perform compression
    unsigned char* dstPtr = jpegBuf;
    size_t dstSize = jpegSize;
    int result = tj3Compress8(handle, srcBuf.data(), width, 0, height, pixelFormat,
                             &dstPtr, &dstSize);
    
    // Note: We don't check result since we want to test error paths too
    
    // Clean up
    if (dstPtr != NULL && dstPtr != jpegBuf) {
        // Library allocated new buffer
        tj3Free(dstPtr);
    } else if (jpegBuf != NULL) {
        // We allocated the buffer
        tj3Free(jpegBuf);
    }
    
    tj3Destroy(handle);
    
    return 0;
}

} // namespace harness_001
