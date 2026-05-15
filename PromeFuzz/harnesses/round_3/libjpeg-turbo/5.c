// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static tjhandle init_decompressor(void) {
    return tj3Init(TJINIT_DECOMPRESS);
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0; // Too small to be meaningful
    
    tjhandle handle = init_decompressor();
    if (!handle) return 0;
    
    // 1. Set scaling factor
    tjscalingfactor scalingFactor;
    scalingFactor.num = 1;
    scalingFactor.denom = 2; // 1/2 scaling
    tj3SetScalingFactor(handle, scalingFactor);
    
    // 2. Allocate JPEG buffer
    unsigned char *jpegBuf = (unsigned char *)tj3Alloc(Size);
    if (!jpegBuf) {
        tj3Destroy(handle);
        return 0;
    }
    memcpy(jpegBuf, Data, Size);
    
    // 3. Allocate YUV buffer (initial guess, will be resized)
    size_t initialYUVSize = Size * 2; // Conservative estimate
    unsigned char *yuvBuf = (unsigned char *)tj3Alloc(initialYUVSize);
    if (!yuvBuf) {
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    // 4. Calculate YUV buffer size
    int width = 640; // Default values
    int height = 480;
    int align = 4; // X Video compatible
    int subsamp = TJSAMP_420; // Common subsampling
    size_t yuvSize = tj3YUVBufSize(width, align, height, subsamp);
    
    // 5. Decompress to YUV8
    int decompressResult = tj3DecompressToYUV8(handle, jpegBuf, Size, yuvBuf, align);
    
    // 6. Decode YUV8 to RGB
    if (decompressResult == 0) {
        int pixelFormat = TJPF_RGB;
        int pitch = width * tjPixelSize[pixelFormat];
        unsigned char *rgbBuf = (unsigned char *)malloc(pitch * height);
        if (rgbBuf) {
            tj3DecodeYUV8(handle, yuvBuf, align, rgbBuf, width, pitch, height, pixelFormat);
            free(rgbBuf);
        }
    }
    
    // 7. Cleanup
    tj3Free(jpegBuf);
    tj3Free(yuvBuf);
    tj3Destroy(handle);
    
    return 0;
}