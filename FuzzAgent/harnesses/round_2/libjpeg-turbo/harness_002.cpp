#include "turbojpeg.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_002 {
/*
 * Harness 002: JPEG transformation fuzzer using TurboJPEG API
 * This harness tests JPEG transformation operations including crop, flip,
 * rotate, and other transformations with various parameters.
 */

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for JPEG header
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize TurboJPEG transform handle
    tjhandle handle = tj3Init(TJINIT_TRANSFORM);
    if (handle == NULL) return 0;
    
    // Consume transformation parameters from fuzzer input
    bool arithmetic = fdp.ConsumeBool();
    bool progressive = fdp.ConsumeBool();
    bool optimize = fdp.ConsumeBool();
    bool noRealloc = fdp.ConsumeBool();
    int scanLimit = fdp.ConsumeIntegralInRange<int>(1, 1000);
    
    // Set transform parameters
    tj3Set(handle, TJPARAM_ARITHMETIC, arithmetic ? 1 : 0);
    tj3Set(handle, TJPARAM_PROGRESSIVE, progressive ? 1 : 0);
    tj3Set(handle, TJPARAM_OPTIMIZE, optimize ? 1 : 0);
    tj3Set(handle, TJPARAM_NOREALLOC, noRealloc ? 1 : 0);
    tj3Set(handle, TJPARAM_SCANLIMIT, scanLimit);
    
    // Try to get JPEG header information (ignore failures)
    tj3DecompressHeader(handle, data, size);
    
    int width = tj3Get(handle, TJPARAM_JPEGWIDTH);
    int height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
    int jpegSubsamp = tj3Get(handle, TJPARAM_SUBSAMP);
    
    // Skip invalid or too large images
    if (width < 1 || height < 1 || (uint64_t)width * height > 1048576) {
        tj3Destroy(handle);
        return 0;
    }
    
    // Validate subsampling
    if (jpegSubsamp < 0 || jpegSubsamp >= TJ_NUMSAMP) {
        jpegSubsamp = TJSAMP_444;
    }
    
    // Create transformation operations based on fuzzer input
    tjtransform transforms[2];
    memset(&transforms[0], 0, sizeof(tjtransform) * 2);
    
    // First transformation
    uint8_t transformOp1 = fdp.ConsumeIntegral<uint8_t>() % 8;
    transforms[0].op = static_cast<enum TJXOP>(transformOp1);
    
    // Consume transformation options
    uint16_t options1 = fdp.ConsumeIntegral<uint16_t>();
    transforms[0].options = options1;
    
    // Consume region parameters if crop option is enabled
    if ((transforms[0].options & TJXOPT_CROP) != 0) {
        // For crop operations, consume region parameters
        if (fdp.remaining_bytes() >= 16) {
            transforms[0].r.x = fdp.ConsumeIntegralInRange<int>(0, width - 1);
            transforms[0].r.y = fdp.ConsumeIntegralInRange<int>(0, height - 1);
            
            // Ensure region stays within bounds
            int maxW = width - transforms[0].r.x;
            int maxH = height - transforms[0].r.y;
            
            transforms[0].r.w = fdp.ConsumeIntegralInRange<int>(1, maxW > 0 ? maxW : 1);
            transforms[0].r.h = fdp.ConsumeIntegralInRange<int>(1, maxH > 0 ? maxH : 1);
        }
    }
    
    // Consume custom filter parameters
    bool useCustomFilter = fdp.ConsumeBool();
    if (useCustomFilter && fdp.remaining_bytes() >= 4) {
        // Simple filter that negates coefficients
        transforms[0].customFilter = [](short *coeffs, tjregion arrayRegion,
                                        tjregion planeRegion, int componentIndex,
                                        int transformIndex, tjtransform *transform) {
            for (int i = 0; i < arrayRegion.w * arrayRegion.h; i++) {
                coeffs[i] = -coeffs[i];
            }
            return 0;
        };
    }
    
    // Calculate buffer size for transformation
    size_t maxBufSize = tj3TransformBufSize(handle, &transforms[0]);
    if (maxBufSize == 0 || maxBufSize > 100 * 1024 * 1024) { // 100MB limit
        tj3Destroy(handle);
        return 0;
    }
    
    // Allocate destination buffer
    unsigned char* dstBuf = (unsigned char*)tj3Alloc(maxBufSize);
    if (dstBuf == NULL) {
        tj3Destroy(handle);
        return 0;
    }
    
    size_t dstSize = maxBufSize;
    unsigned char* dstBufs[1] = { dstBuf };
    size_t dstSizes[1] = { dstSize };
    
    // Set ICC profile if enough data
    if (size >= 34) {
        // Use some bytes for ICC profile
        size_t iccSize = fdp.ConsumeIntegralInRange<size_t>(1, 32);
        if (iccSize <= fdp.remaining_bytes()) {
            std::vector<uint8_t> iccData = fdp.ConsumeBytes<uint8_t>(iccSize);
            tj3SetICCProfile(handle, iccData.data(), iccSize);
        }
    }
    
    // Perform transformation
    int result = tj3Transform(handle, data, size, 1, dstBufs, dstSizes, transforms);
    
    // Note: We don't check result since we want to test error paths too
    
    // Clean up - must check if buffer was reallocated by tj3Transform
    // According to documentation: "Unless you have set #TJPARAM_NOREALLOC, 
    // you should always check dstBufs[i] upon return from this function, as it may have changed."
    if (noRealloc) {
        // When NOREALLOC is set, we must free the original buffer
        tj3Free(dstBuf);
    } else {
        // When NOREALLOC is not set, the buffer may have been reallocated
        // so we need to free the potentially new buffer pointer
        if (dstBufs[0] != NULL && dstBufs[0] != dstBuf) {
            tj3Free(dstBufs[0]);
        } else if (dstBufs[0] == dstBuf) {
            // Buffer wasn't reallocated, free the original
            tj3Free(dstBuf);
        }
        // If dstBufs[0] is NULL, tj3Transform failed or didn't allocate
    }
    
    tj3Destroy(handle);
    
    return 0;
}

} // namespace harness_002
