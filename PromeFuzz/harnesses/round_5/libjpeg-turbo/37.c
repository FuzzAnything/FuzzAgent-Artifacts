// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fill_random_buffer(unsigned char *buf, size_t size, const uint8_t *data, size_t data_size) {
    for (size_t i = 0; i < size; i++) {
        buf[i] = data[i % data_size];
    }
}

static void fill_random_buffer_16(unsigned short *buf, size_t size, const uint8_t *data, size_t data_size) {
    for (size_t i = 0; i < size; i++) {
        buf[i] = (data[i % data_size] << 8) | data[(i + 1) % data_size];
    }
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    // Extract parameters from fuzzer input
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int jpegSubsamp = Data[4] % 6;  // 0-5 for valid subsampling options
    int jpegQual = Data[5] % 101;   // 0-100 for quality
    int pixelFormat = Data[6] % 12; // 0-11 for pixel formats
    int flags = Data[7];
    
    // Ensure reasonable dimensions to avoid excessive memory usage
    if (width <= 0) width = 1;
    if (height <= 0) height = 1;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    // Test TJBUFSIZE
    unsigned long bufsize1 = TJBUFSIZE(width, height);
    
    // Test tj3JPEGBufSize
    size_t bufsize2 = tj3JPEGBufSize(width, height, jpegSubsamp);
    
    // Test tjBufSize
    unsigned long bufsize3 = tjBufSize(width, height, jpegSubsamp);
    
    // Test tj3Alloc with various sizes
    void *alloc1 = NULL;
    void *alloc2 = NULL;
    void *alloc3 = NULL;
    
    if (bufsize1 > 0 && bufsize1 < 1000000) {
        alloc1 = tj3Alloc(bufsize1);
    }
    
    if (bufsize2 > 0 && bufsize2 < 1000000) {
        alloc2 = tj3Alloc(bufsize2);
    }
    
    // Allocate a small buffer for testing
    size_t small_alloc_size = (Size % 10000) + 1;
    alloc3 = tj3Alloc(small_alloc_size);
    
    // Test tjCompress2
    tjhandle handle = tjInitCompress();
    if (handle) {
        unsigned char *jpegBuf = NULL;
        unsigned long jpegSize = 0;
        
        // Prepare source buffer
        int pitch = width * tjPixelSize[pixelFormat];
        if (pitch <= 0) pitch = width * 3; // Default to RGB if invalid
        
        size_t srcBufSize = (size_t)pitch * (size_t)height;
        if (srcBufSize > 0 && srcBufSize < 10000000) {
            unsigned char *srcBuf = (unsigned char *)malloc(srcBufSize);
            if (srcBuf) {
                fill_random_buffer(srcBuf, srcBufSize, Data, Size);
                
                // Try compression with auto-allocation
                tjCompress2(handle, srcBuf, width, pitch, height, pixelFormat,
                           &jpegBuf, &jpegSize, jpegSubsamp, jpegQual, flags);
                
                if (jpegBuf) {
                    tjFree(jpegBuf);
                    jpegBuf = NULL;
                }
                
                // Try compression with pre-allocated buffer
                if (bufsize3 > 0 && bufsize3 < 1000000) {
                    jpegBuf = (unsigned char *)tj3Alloc(bufsize3);
                    jpegSize = bufsize3;
                    tjCompress2(handle, srcBuf, width, pitch, height, pixelFormat,
                               &jpegBuf, &jpegSize, jpegSubsamp, jpegQual, 
                               flags | TJFLAG_NOREALLOC);
                    
                    if (jpegBuf) {
                        tj3Free(jpegBuf);
                        jpegBuf = NULL;
                    }
                }
                
                free(srcBuf);
            }
        }
        
        tjDestroy(handle);
    }
    
    // Test tj3Compress16
    tjhandle handle16 = tj3Init(TJINIT_COMPRESS);
    if (handle16) {
        unsigned char *jpegBuf16 = NULL;
        size_t jpegSize16 = 0;
        
        // Prepare 16-bit source buffer
        int pitch16 = width * tjPixelSize[pixelFormat];
        if (pitch16 <= 0) pitch16 = width * 3;
        
        size_t srcBufSize16 = (size_t)pitch16 * (size_t)height;
        if (srcBufSize16 > 0 && srcBufSize16 < 10000000) {
            unsigned short *srcBuf16 = (unsigned short *)malloc(srcBufSize16 * sizeof(unsigned short));
            if (srcBuf16) {
                fill_random_buffer_16(srcBuf16, srcBufSize16, Data, Size);
                
                // Try with auto-allocation
                tj3Compress16(handle16, srcBuf16, width, pitch16, height, pixelFormat,
                             &jpegBuf16, &jpegSize16);
                
                if (jpegBuf16) {
                    tj3Free(jpegBuf16);
                    jpegBuf16 = NULL;
                }
                
                // Try with pre-allocated buffer
                if (bufsize2 > 0 && bufsize2 < 1000000) {
                    jpegBuf16 = (unsigned char *)tj3Alloc(bufsize2);
                    jpegSize16 = bufsize2;
                    tj3Set(handle16, TJPARAM_NOREALLOC, 1);
                    tj3Compress16(handle16, srcBuf16, width, pitch16, height, pixelFormat,
                                 &jpegBuf16, &jpegSize16);
                    
                    if (jpegBuf16) {
                        tj3Free(jpegBuf16);
                        jpegBuf16 = NULL;
                    }
                    tj3Set(handle16, TJPARAM_NOREALLOC, 0);
                }
                
                free(srcBuf16);
            }
        }
        
        tj3Destroy(handle16);
    }
    
    // Clean up allocated memory
    if (alloc1) tj3Free(alloc1);
    if (alloc2) tj3Free(alloc2);
    if (alloc3) tj3Free(alloc3);
    
    return 0;
}