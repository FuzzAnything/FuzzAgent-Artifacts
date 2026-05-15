#include "turbojpeg.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

namespace harness_002 {
/*
 * Fuzzing harness for libjpeg-turbo targeting JPEG transformation operations
 * This harness specifically targets tjTransform and tj3Transform APIs for
 * comprehensive testing of JPEG transformation operations: cropping, flipping,
 * rotating, transposing, etc.
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000.cpp: Focuses on RGB compression/decompression with basic transform testing
 * - harness_001.cpp: Focuses on YUV decompression APIs
 * - This harness: Focuses exclusively on transformation operations with both legacy (tjTransform) 
 *   and modern (tj3Transform) APIs, testing various transform operations and options
 */

// Custom filter callback for transform operations
static int customTransformFilter(short *coeffs, tjregion arrayRegion,
                                 tjregion planeRegion, int componentID,
                                 int transformID, struct tjtransform *transform) {
    // Simple custom filter that inverts DCT coefficients
    for (int i = 0; i < arrayRegion.w * arrayRegion.h; i++) {
        coeffs[i] = -coeffs[i];
    }
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for JPEG data + transform parameters
    if (size < 128) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Check if we have enough data for initial parameters (5 integers)
    if (fdp.remaining_bytes() < 5 * sizeof(int)) return 0;
    
    // Consume fixed-size parameters first
    int width = fdp.ConsumeIntegralInRange<int>(1, 1024);
    int height = fdp.ConsumeIntegralInRange<int>(1, 1024);
    int subsamp = fdp.ConsumeIntegralInRange<int>(0, 5); // TJSAMP enum values
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    int flags = fdp.ConsumeIntegral<int>();
    
    // Validate subsampling value
    if (subsamp < 0 || subsamp > 5) {
        subsamp = TJSAMP_420; // Default to 4:2:0
    }
    
    // Create a simple JPEG image for transformation testing
    // First, create compression handle
    tjhandle compressHandle = tjInitCompress();
    if (!compressHandle) return 0;
    
    // Create source image buffer
    int pixelFormat = TJPF_RGB;
    int bytesPerPixel = 3;
    size_t imageSize = width * height * bytesPerPixel;
    
    // Consume remaining data for source image and JPEG buffer
    // Ensure we have at least some data for the image
    size_t remaining = fdp.remaining_bytes();
    if (remaining < 64) {
        tjDestroy(compressHandle);
        return 0;
    }
    
    // Calculate maximum image size we can use (half of remaining bytes)
    size_t maxImageSize = remaining / 2;
    if (maxImageSize < imageSize) {
        // Adjust image dimensions to match available buffer size
        // Calculate new dimensions that maintain aspect ratio
        if (maxImageSize >= bytesPerPixel * 4) { // Need at least 4 pixels
            // Recalculate width and height based on available buffer size
            // Keep width/height ratio approximately the same
            double aspectRatio = (double)width / height;
            int newHeight = (int)sqrt(maxImageSize / (aspectRatio * bytesPerPixel));
            newHeight = std::max(1, std::min(newHeight, height));
            int newWidth = (int)(newHeight * aspectRatio);
            newWidth = std::max(1, std::min(newWidth, width));
            
            // Update dimensions and image size
            width = newWidth;
            height = newHeight;
            imageSize = width * height * bytesPerPixel;
        } else {
            // Not enough data even for minimal image
            tjDestroy(compressHandle);
            return 0;
        }
    }
    
    // Ensure we have enough bytes for the image
    if (fdp.remaining_bytes() < imageSize) {
        tjDestroy(compressHandle);
        return 0;
    }
    
    std::vector<uint8_t> srcBuffer = fdp.ConsumeBytes<uint8_t>(imageSize);
    
    // Compress to JPEG for transformation testing
    unsigned char* jpegBuf = nullptr;
    unsigned long jpegSize = 0;
    
    int compressResult = tjCompress2(compressHandle,
                                     srcBuffer.data(),
                                     width,
                                     width * bytesPerPixel,
                                     height,
                                     pixelFormat,
                                     &jpegBuf,
                                     &jpegSize,
                                     subsamp,
                                     quality,
                                     flags);
    
    if (compressResult != 0 || !jpegBuf || jpegSize == 0) {
        if (compressHandle) tjDestroy(compressHandle);
        return 0;
    }
    // Test 1: Legacy tjTransform API with various operations
    {
        tjhandle transformHandle = tjInitTransform();
        if (!transformHandle) {
            // Cleanup and continue to modern API test
            goto modern_test;
        }
        
        // Prepare transform structures
        const int numTransforms = 3;
        unsigned char* dstBufs[numTransforms] = {nullptr, nullptr, nullptr};
        unsigned long dstSizes[numTransforms] = {0, 0, 0};
        tjtransform transforms[numTransforms];
        
        memset(transforms, 0, sizeof(tjtransform) * numTransforms);
        
        // Transform 1: Simple cropping with no operation
        // Check if we have enough bytes for 4 integers
        if (fdp.remaining_bytes() < 4 * sizeof(int)) {
            tjDestroy(transformHandle);
            goto modern_test;
        }
        // Ensure max >= min for all range calculations
        transforms[0].r.x = fdp.ConsumeIntegralInRange<int>(0, std::max(0, width / 4));
        transforms[0].r.y = fdp.ConsumeIntegralInRange<int>(0, std::max(0, height / 4));
        transforms[0].r.w = fdp.ConsumeIntegralInRange<int>(1, std::max(1, width / 2));
        transforms[0].r.h = fdp.ConsumeIntegralInRange<int>(1, std::max(1, height / 2));
        transforms[0].op = TJXOP_NONE;
        transforms[0].options = TJXOPT_CROP;
        // Transform 2: Horizontal flip with trim
        transforms[1].r.x = 0;
        transforms[1].r.y = 0;
        transforms[1].r.w = width;
        transforms[1].r.h = height;
        transforms[1].op = TJXOP_HFLIP;
        transforms[1].options = TJXOPT_TRIM;
        // Transform 3: 90-degree rotation with custom filter
        transforms[2].r.x = 0;
        transforms[2].r.y = 0;
        transforms[2].r.w = width;
        transforms[2].r.h = height;
        transforms[2].op = TJXOP_ROT90;
        transforms[2].options = TJXOPT_OPTIMIZE;
        transforms[2].customFilter = customTransformFilter;
        
        // Execute transforms
        if (width > 1 && height > 1) {
            tjTransform(transformHandle,
                       jpegBuf,
                       jpegSize,
                       numTransforms,
                       dstBufs,
                       dstSizes,
                       transforms,
                       flags);
            
            // Cleanup allocated buffers
            for (int i = 0; i < numTransforms; i++) {
                if (dstBufs[i]) {
                    tjFree(dstBufs[i]);
                }
            }
        }
        
        tjDestroy(transformHandle);
    }
    
modern_test:
    // Test 2: Modern tj3Transform API with more complex operations
    {
        tjhandle transformHandle3 = tj3Init(TJINIT_TRANSFORM);
        if (!transformHandle3) {
            goto cleanup;
        }
        
        // Set JPEG header information
        tj3Set(transformHandle3, TJPARAM_JPEGWIDTH, width);
        tj3Set(transformHandle3, TJPARAM_JPEGHEIGHT, height);
        tj3Set(transformHandle3, TJPARAM_SUBSAMP, subsamp);
        
        // Configure transform options from fuzzer input
        // Check if we have enough bytes for 3 booleans
        if (fdp.remaining_bytes() < 3) {
            tj3Destroy(transformHandle3);
            goto cleanup;
        }
        int useArithmetic = fdp.ConsumeBool() ? 1 : 0;
        int useProgressive = fdp.ConsumeBool() ? 1 : 0;
        int useOptimize = fdp.ConsumeBool() ? 1 : 0;
        tj3Set(transformHandle3, TJPARAM_ARITHMETIC, useArithmetic);
        tj3Set(transformHandle3, TJPARAM_PROGRESSIVE, useProgressive);
        tj3Set(transformHandle3, TJPARAM_OPTIMIZE, useOptimize);
        
        // Test multiple transform operations
        const int numOps = 6;
        int transformOps[numOps] = {
            TJXOP_NONE, TJXOP_HFLIP, TJXOP_VFLIP,
            TJXOP_ROT90, TJXOP_ROT180, TJXOP_TRANSPOSE
        };
        
        // Test each operation with different options
        for (int i = 0; i < numOps && fdp.remaining_bytes() > 0; i++) {
            tjtransform transform;
            memset(&transform, 0, sizeof(tjtransform));
            
            // Configure transform from fuzzer input
            transform.op = transformOps[i];
            
            // Build options from various flags
            // Check if we have enough bytes for 4 booleans
            if (fdp.remaining_bytes() < 4) {
                break; // Not enough data for this iteration
            }
            int options = 0;
            if (fdp.ConsumeBool()) options |= TJXOPT_CROP;
            if (fdp.ConsumeBool()) options |= TJXOPT_TRIM;
            if (fdp.ConsumeBool()) options |= TJXOPT_GRAY;
            if (fdp.ConsumeBool()) options |= TJXOPT_OPTIMIZE;
            if (useProgressive) options |= TJXOPT_PROGRESSIVE;
            if (useArithmetic) options |= TJXOPT_ARITHMETIC;
            
            transform.options = options;
            
            // Configure cropping region if CROP option is set
            if (options & TJXOPT_CROP) {
                // Check if we have enough bytes for 4 integers
                if (fdp.remaining_bytes() < 4 * sizeof(int)) {
                    break; // Not enough data for crop region
                }
                transform.r.x = fdp.ConsumeIntegralInRange<int>(0, std::max(0, width / 2));
                transform.r.y = fdp.ConsumeIntegralInRange<int>(0, std::max(0, height / 2));
                transform.r.w = fdp.ConsumeIntegralInRange<int>(1, std::max(1, width - transform.r.x));
                transform.r.h = fdp.ConsumeIntegralInRange<int>(1, std::max(1, height - transform.r.y));
            }
            
            // Add custom filter for some operations
            if (fdp.ConsumeBool()) {
                transform.customFilter = customTransformFilter;
            }
            
            // Calculate buffer size and allocate
            size_t bufSize = tj3TransformBufSize(transformHandle3, &transform);
            if (bufSize > 0 && bufSize < 10 * 1024 * 1024) { // Limit to 10MB
                unsigned char* dstBuf = (unsigned char*)tj3Alloc(bufSize);
                if (dstBuf) {
                    size_t dstSize = bufSize;
                    
                    // Execute transform
                    int result = tj3Transform(transformHandle3,
                                             jpegBuf,
                                             jpegSize,
                                             1,
                                             &dstBuf,
                                             &dstSize,
                                             &transform);
                    
                    // Cleanup
                    tj3Free(dstBuf);
                }
            }
        }
        
        // Test multiple transforms in single call
        // Need at least 32 bytes for this test (4 integers + some buffer)
        if (fdp.remaining_bytes() >= 32) {
            tjtransform multiTransforms[2];
            unsigned char* multiDstBufs[2] = {nullptr, nullptr};
            size_t multiDstSizes[2] = {0, 0};
            
            memset(multiTransforms, 0, sizeof(tjtransform) * 2);
            
            // First transform: Crop and rotate
            // Check if we have enough bytes for 4 integers
            if (fdp.remaining_bytes() >= 4 * sizeof(int)) {
                multiTransforms[0].r.x = fdp.ConsumeIntegralInRange<int>(0, std::max(0, width / 4));
                multiTransforms[0].r.y = fdp.ConsumeIntegralInRange<int>(0, std::max(0, height / 4));
                multiTransforms[0].r.w = fdp.ConsumeIntegralInRange<int>(1, std::max(1, width / 2));
                multiTransforms[0].r.h = fdp.ConsumeIntegralInRange<int>(1, std::max(1, height / 2));
                multiTransforms[0].op = TJXOP_ROT180;
                multiTransforms[0].options = TJXOPT_CROP | TJXOPT_OPTIMIZE;
            } else {
                // Not enough data, skip this test
                tj3Destroy(transformHandle3);
                goto cleanup;
            }
            
            // Second transform: Flip with custom filter
            multiTransforms[1].r.x = 0;
            multiTransforms[1].r.y = 0;
            multiTransforms[1].r.w = width;
            multiTransforms[1].r.h = height;
            multiTransforms[1].op = TJXOP_VFLIP;
            multiTransforms[1].options = TJXOPT_TRIM;
            multiTransforms[1].customFilter = customTransformFilter;

            
            // Calculate buffer sizes
            size_t bufSize1 = tj3TransformBufSize(transformHandle3, &multiTransforms[0]);
            size_t bufSize2 = tj3TransformBufSize(transformHandle3, &multiTransforms[1]);
            
            if (bufSize1 > 0 && bufSize1 < 5 * 1024 * 1024 &&
                bufSize2 > 0 && bufSize2 < 5 * 1024 * 1024) {
                
                multiDstBufs[0] = (unsigned char*)tj3Alloc(bufSize1);
                multiDstBufs[1] = (unsigned char*)tj3Alloc(bufSize2);
                
                if (multiDstBufs[0] && multiDstBufs[1]) {
                    multiDstSizes[0] = bufSize1;
                    multiDstSizes[1] = bufSize2;
                    
                    tj3Transform(transformHandle3,
                                jpegBuf,
                                jpegSize,
                                2,
                                multiDstBufs,
                                multiDstSizes,
                                multiTransforms);
                    
                    // Cleanup
                    tj3Free(multiDstBufs[0]);
                    tj3Free(multiDstBufs[1]);
                }
            }
            
            tj3Destroy(transformHandle3);
        } else {
            tj3Destroy(transformHandle3);
        }
    }
    
cleanup:
    // Cleanup compression resources
    if (compressHandle) {
        tjDestroy(compressHandle);
    }
    if (jpegBuf) {
        tjFree(jpegBuf);
    }
    
    return 0;
}

} // namespace harness_002
