// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    // Create decompressor handles
    tjhandle handle_old = tjInitDecompress();
    tjhandle handle_new = tj3Init(TJINIT_DECOMPRESS);
    
    if (!handle_old || !handle_new) {
        if (handle_old) tjDestroy(handle_old);
        if (handle_new) tjDestroy(handle_new);
        return 0;
    }
    
    // Ensure we have some data
    if (Size == 0) {
        tjDestroy(handle_old);
        tjDestroy(handle_new);
        return 0;
    }
    
    // Copy fuzz data to buffer
    unsigned char *jpegBuf = (unsigned char *)malloc(Size);
    if (!jpegBuf) {
        tjDestroy(handle_old);
        tjDestroy(handle_new);
        return 0;
    }
    memcpy(jpegBuf, Data, Size);
    
    int width = 0, height = 0, jpegSubsamp = 0;
    unsigned long jpegSize = (unsigned long)Size;
    
    // Test tjDecompressHeader
    int result1 = tjDecompressHeader(handle_old, jpegBuf, jpegSize, &width, &height);
    
    // Test tjDecompressHeader2
    int result2 = tjDecompressHeader2(handle_old, jpegBuf, jpegSize, &width, &height, &jpegSubsamp);
    
    // If header decompression succeeded, test decompression functions
    if (result1 == 0 && result2 == 0 && width > 0 && height > 0) {
        // Calculate pitch and allocate destination buffer for tjDecompress
        int pitch = width * 3; // RGB
        int pixelSize = 3; // RGB
        unsigned char *dstBuf = (unsigned char *)malloc((size_t)pitch * height);
        
        if (dstBuf) {
            // Test tjDecompress with various flags
            int flags = 0;
            tjDecompress(handle_old, jpegBuf, jpegSize, dstBuf, width, pitch, height, pixelSize, flags);
            
            flags = TJFLAG_FASTDCT;
            tjDecompress(handle_old, jpegBuf, jpegSize, dstBuf, width, pitch, height, pixelSize, flags);
            
            flags = TJFLAG_ACCURATEDCT;
            tjDecompress(handle_old, jpegBuf, jpegSize, dstBuf, width, pitch, height, pixelSize, flags);
            
            free(dstBuf);
        }
        
        // Test tjDecompressToYUV2
        int align = 4;
        unsigned char *yuvBuf = (unsigned char *)malloc((size_t)width * height * 2); // Rough estimate for YUV
        if (yuvBuf) {
            int flags = 0;
            tjDecompressToYUV2(handle_old, jpegBuf, jpegSize, yuvBuf, width, align, height, flags);
            
            flags = TJFLAG_FASTDCT;
            tjDecompressToYUV2(handle_old, jpegBuf, jpegSize, yuvBuf, width, align, height, flags);
            
            free(yuvBuf);
        }
    }
    
    // Test tj3SetScalingFactor with different scaling factors
    tjscalingfactor scalingFactors[] = {
        {1, 1},   // No scaling
        {1, 2},   // 1/2 scaling
        {1, 4},   // 1/4 scaling
        {1, 8},   // 1/8 scaling
        {3, 4},   // 3/4 scaling
        {3, 8}    // 3/8 scaling
    };
    
    for (int i = 0; i < 6; i++) {
        tj3SetScalingFactor(handle_new, scalingFactors[i]);
        
        // After setting scaling factor, try to decompress headers again
        if (Size > 0) {
            tjDecompressHeader2(handle_old, jpegBuf, jpegSize, &width, &height, &jpegSubsamp);
        }
    }
    
    // Cleanup
    free(jpegBuf);
    tjDestroy(handle_old);
    tjDestroy(handle_new);
    
    return 0;
}

#ifdef __cplusplus
}
#endif