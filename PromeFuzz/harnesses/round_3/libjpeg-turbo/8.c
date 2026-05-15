// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjCompressFromYUVPlanes at turbojpeg.c:1394:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
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

static void init_handle_and_params(tjhandle *handle, int subsamp, int quality) {
    *handle = tj3Init(TJINIT_COMPRESS);
    if (*handle) {
        tj3Set(*handle, TJPARAM_SUBSAMP, subsamp);
        tj3Set(*handle, TJPARAM_QUALITY, quality);
    }
}

static void cleanup_handle_and_buffers(tjhandle handle, unsigned char *jpegBuf, 
                                       unsigned char *yuvBuf, const unsigned char **planes) {
    if (jpegBuf) tj3Free(jpegBuf);
    if (yuvBuf) free(yuvBuf);
    if (planes) free((void*)planes);
    if (handle) tj3Destroy(handle);
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 32) return 0;
    
    // Extract parameters from fuzz data
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int subsamp = Data[4] % 5;  // 0-4 for TJSAMP_444, 422, 420, GRAY, 440
    int quality = Data[5] % 101; // 0-100
    int align = 1 << (Data[6] % 4); // 1,2,4,8
    int flags = Data[7] & 0xF;
    int use_strides = Data[8] & 1;
    
    // Clamp dimensions to reasonable values
    if (width <= 0) width = 1;
    if (height <= 0) height = 1;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    
    // Calculate required buffer sizes
    size_t yuvBufSize = tj3YUVBufSize(width, align, height, subsamp);
    size_t jpegBufSize = tj3JPEGBufSize(width, height, subsamp);
    
    if (yuvBufSize == 0 || jpegBufSize == 0 || yuvBufSize > 100 * 1024 * 1024) {
        return 0;
    }
    
    // Allocate and fill YUV buffer with fuzz data
    unsigned char *yuvBuf = (unsigned char*)malloc(yuvBufSize);
    if (!yuvBuf) return 0;
    
    size_t copySize = Size - 9;
    if (copySize > yuvBufSize) copySize = yuvBufSize;
    memcpy(yuvBuf, Data + 9, copySize);
    
    // Initialize TurboJPEG handle
    tjhandle handle = NULL;
    init_handle_and_params(&handle, subsamp, quality);
    if (!handle) {
        free(yuvBuf);
        return 0;
    }
    
    // Test tj3CompressFromYUV8
    {
        unsigned char *jpegBuf = NULL;
        size_t outSize = jpegBufSize;
        jpegBuf = (unsigned char*)tj3Alloc(jpegBufSize);
        
        if (jpegBuf) {
            tj3CompressFromYUV8(handle, yuvBuf, width, align, height, 
                               &jpegBuf, &outSize);
            tj3Free(jpegBuf);
        }
    }
    
    // Test tj3CompressFromYUVPlanes8
    {
        const unsigned char **planes = (const unsigned char**)malloc(3 * sizeof(unsigned char*));
        int *strides = NULL;
        
        if (planes) {
            // For simplicity, use same buffer for all planes
            planes[0] = yuvBuf;
            planes[1] = yuvBuf;
            planes[2] = yuvBuf;
            
            if (use_strides) {
                strides = (int*)malloc(3 * sizeof(int));
                if (strides) {
                    strides[0] = width;
                    strides[1] = width / (subsamp == TJSAMP_420 ? 2 : 1);
                    strides[2] = width / (subsamp == TJSAMP_420 ? 2 : 1);
                }
            }
            
            unsigned char *jpegBuf = NULL;
            size_t outSize = jpegBufSize;
            jpegBuf = (unsigned char*)tj3Alloc(jpegBufSize);
            
            if (jpegBuf) {
                tj3CompressFromYUVPlanes8(handle, planes, width, strides, height,
                                         &jpegBuf, &outSize);
                tj3Free(jpegBuf);
            }
            
            free(strides);
            free(planes);
        }
    }
    
    // Test tjCompressFromYUV (legacy API)
    {
        unsigned char *jpegBuf = NULL;
        unsigned long outSize = jpegBufSize;
        
        tjCompressFromYUV(handle, yuvBuf, width, align, height, subsamp,
                         &jpegBuf, &outSize, quality, flags);
        if (jpegBuf) tjFree(jpegBuf);
    }
    
    // Test tjCompressFromYUVPlanes (legacy API)
    {
        const unsigned char **planes = (const unsigned char**)malloc(3 * sizeof(unsigned char*));
        int *strides = NULL;
        
        if (planes) {
            planes[0] = yuvBuf;
            planes[1] = yuvBuf;
            planes[2] = yuvBuf;
            
            if (use_strides) {
                strides = (int*)malloc(3 * sizeof(int));
                if (strides) {
                    strides[0] = width;
                    strides[1] = width / (subsamp == TJSAMP_420 ? 2 : 1);
                    strides[2] = width / (subsamp == TJSAMP_420 ? 2 : 1);
                }
            }
            
            unsigned char *jpegBuf = NULL;
            unsigned long outSize = jpegBufSize;
            
            tjCompressFromYUVPlanes(handle, planes, width, strides, height, subsamp,
                                   &jpegBuf, &outSize, quality, flags);
            if (jpegBuf) tjFree(jpegBuf);
            
            free(strides);
            free(planes);
        }
    }
    
    // Test tj3Alloc and tj3JPEGBufSize
    {
        size_t testSize = tj3JPEGBufSize(width, height, subsamp);
        if (testSize > 0 && testSize < 10 * 1024 * 1024) {
            void *allocBuf = tj3Alloc(testSize);
            if (allocBuf) tj3Free(allocBuf);
        }
    }
    
    cleanup_handle_and_buffers(handle, NULL, yuvBuf, NULL);
    return 0;
}