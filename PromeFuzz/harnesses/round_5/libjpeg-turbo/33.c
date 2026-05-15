// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int dummy_custom_filter(short *coeffs, tjregion arrayRegion,
                               tjregion planeRegion, int componentID,
                               int transformID, struct tjtransform *transform) {
    // Simple filter that does nothing - just returns success
    (void)coeffs;
    (void)arrayRegion;
    (void)planeRegion;
    (void)componentID;
    (void)transformID;
    (void)transform;
    return 0;
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for basic operations
    if (Size < 100) {
        return 0;
    }

    // Initialize transformer instance
    tjhandle handle = tjInitTransform();
    if (!handle) {
        return 0;
    }

    // Try to decompress header first (required for tj3TransformBufSize)
    // We'll use a dummy JPEG buffer from fuzzer data
    const unsigned char *jpegBuf = Data;
    size_t jpegSize = Size;
    
    // Read JPEG header if possible
    int width, height, jpegSubsamp;
    if (tjDecompressHeader2(handle, (unsigned char *)jpegBuf, (unsigned long)jpegSize,
                            &width, &height, &jpegSubsamp) == 0) {
        // Header read successfully - now we can test tj3TransformBufSize
        tjtransform transform;
        
        // Initialize transform structure with fuzzer-derived values
        transform.r.x = Data[0] % (width > 0 ? width : 1);
        transform.r.y = Data[1] % (height > 0 ? height : 1);
        transform.r.w = Data[2] % (width > transform.r.x ? width - transform.r.x : 1);
        transform.r.h = Data[3] % (height > transform.r.y ? height - transform.r.y : 1);
        transform.op = Data[4] % 8;  // 8 transform operations defined in turbojpeg.h
        transform.options = Data[5] & 0xFF;
        
        // Sometimes set a custom filter, sometimes not
        if (Data[6] & 0x1) {
            transform.customFilter = dummy_custom_filter;
        } else {
            transform.customFilter = NULL;
        }
        transform.data = NULL;
        
        // Test tj3TransformBufSize
        size_t bufSize = tj3TransformBufSize(handle, &transform);
        (void)bufSize;  // Use result to avoid compiler warnings
        
        // Test tj3Transform with single transform
        unsigned char *dstBuf = NULL;
        size_t dstSize = 0;
        
        // Allocate buffer using tj3Alloc if we have a size estimate
        if (bufSize > 0 && bufSize < 100 * 1024 * 1024) {  // Limit to 100MB
            dstBuf = (unsigned char *)tj3Alloc(bufSize);
            dstSize = bufSize;
        }
        
        // Try transformation
        if (dstBuf) {
            int result = tj3Transform(handle, jpegBuf, jpegSize, 1, &dstBuf, &dstSize, &transform);
            (void)result;  // Use result to avoid compiler warnings
            
            // Free the buffer
            tj3Free(dstBuf);
        } else {
            // Try with NULL buffer (library should allocate)
            dstBuf = NULL;
            dstSize = 0;
            int result = tj3Transform(handle, jpegBuf, jpegSize, 1, &dstBuf, &dstSize, &transform);
            (void)result;
            
            if (dstBuf) {
                tj3Free(dstBuf);
            }
        }
    }
    
    // Test older tjTransform API with multiple transforms
    // Prepare for up to 2 transforms
    int n = 2;
    unsigned char *dstBufs[2] = {NULL, NULL};
    unsigned long dstSizes[2] = {0, 0};
    tjtransform transforms[2];
    
    // Initialize transform structures
    for (int i = 0; i < n; i++) {
        transforms[i].r.x = Data[i * 4] % 100;
        transforms[i].r.y = Data[i * 4 + 1] % 100;
        transforms[i].r.w = Data[i * 4 + 2] % 100;
        transforms[i].r.h = Data[i * 4 + 3] % 100;
        transforms[i].op = Data[i + 8] % 8;
        transforms[i].options = TJXOPT_PERFECT;
        
        if (Data[i + 10] & 0x1) {
            transforms[i].customFilter = dummy_custom_filter;
        } else {
            transforms[i].customFilter = NULL;
        }
        transforms[i].data = NULL;
    }
    
    // Try transformation with older API
    int transform_result = tjTransform(handle, jpegBuf, (unsigned long)jpegSize,
                                       n, dstBufs, dstSizes, transforms, TJXOPT_PERFECT);
    (void)transform_result;
    
    // Free any allocated buffers
    for (int i = 0; i < n; i++) {
        if (dstBufs[i]) {
            tjFree(dstBufs[i]);
        }
    }
    
    // Clean up
    tjDestroy(handle);
    
    return 0;
}