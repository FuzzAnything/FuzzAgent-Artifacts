// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "turbojpeg.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_jpeg_buffer(unsigned char *jpegBuf, size_t jpegSize) {
    if (jpegBuf) {
        free(jpegBuf);
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize TurboJPEG instances
    tjhandle handle_old = tjInitCompress();
    tjhandle handle_new = tj3Init(TJINIT_COMPRESS);
    
    if (!handle_old || !handle_new) {
        if (handle_old) tjDestroy(handle_old);
        if (handle_new) tj3Destroy(handle_new);
        return 0;
    }

    // Extract parameters from fuzz data with bounds checking
    int width = 64;
    int height = 64;
    int align = 4;
    int subsamp = TJSAMP_420;
    int jpegQual = 85;
    int flags = 0;
    int pixelFormat = TJPF_RGB;
    
    if (Size >= 1) width = (Data[0] % 1024) + 1;
    if (Size >= 2) height = (Data[1] % 1024) + 1;
    if (Size >= 3) align = 1 << (Data[2] % 4);
    if (Size >= 4) subsamp = Data[3] % 5;
    if (Size >= 5) jpegQual = Data[4] % 101;
    if (Size >= 6) flags = Data[5] % 256;
    if (Size >= 7) pixelFormat = Data[6] % 12;

    // Ensure align is at least 1 and a power of 2
    if (align < 1) align = 1;
    if (align > 8) align = 8;

    // Calculate buffer sizes
    size_t yuvBufSize = tj3YUVBufSize(width, align, height, subsamp);
    size_t jpegBufSize = tj3JPEGBufSize(width, height, subsamp);
    unsigned long jpegBufSizeOld = tjBufSize(width, height, subsamp);
    
    // Prepare source buffers
    unsigned char *srcBuf = NULL;
    unsigned char *yuvBuf = NULL;
    const unsigned char *srcPlanes[3] = {NULL, NULL, NULL};
    
    if (yuvBufSize > 0 && yuvBufSize < 1000000) {
        yuvBuf = (unsigned char *)malloc(yuvBufSize);
        if (yuvBuf) {
            size_t copySize = Size < yuvBufSize ? Size : yuvBufSize;
            if (copySize > 0) {
                memcpy(yuvBuf, Data, copySize);
                if (copySize < yuvBufSize) {
                    memset(yuvBuf + copySize, 0, yuvBufSize - copySize);
                }
            } else {
                memset(yuvBuf, 0, yuvBufSize);
            }
        }
    }
    
    if (width > 0 && height > 0) {
        size_t srcSize = width * height * 3;
        if (srcSize > 0 && srcSize < 1000000) {
            srcBuf = (unsigned char *)malloc(srcSize);
            if (srcBuf) {
                size_t copySize = Size < srcSize ? Size : srcSize;
                if (copySize > 0) {
                    memcpy(srcBuf, Data, copySize);
                    if (copySize < srcSize) {
                        memset(srcBuf + copySize, 0, srcSize - copySize);
                    }
                } else {
                    memset(srcBuf, 0, srcSize);
                }
            }
        }
    }

    // Test tj3CompressFromYUV8
    if (yuvBuf) {
        unsigned char *jpegBuf = NULL;
        size_t jpegSize = jpegBufSize;
        
        tj3Set(handle_new, TJPARAM_SUBSAMP, subsamp);
        tj3Set(handle_new, TJPARAM_QUALITY, jpegQual);
        
        int result = tj3CompressFromYUV8(handle_new, yuvBuf, width, align, height, &jpegBuf, &jpegSize);
        int errorCode = tj3GetErrorCode(handle_new);
        (void)errorCode; // Use error code to avoid unused variable warning
        
        if (jpegBuf) {
            free(jpegBuf);
        }
    }

    // Test tjCompressFromYUV
    if (yuvBuf && handle_old) {
        unsigned char *jpegBuf = NULL;
        unsigned long jpegSize = jpegBufSizeOld;
        
        int result = tjCompressFromYUV(handle_old, yuvBuf, width, align, height, subsamp, 
                                      &jpegBuf, &jpegSize, jpegQual, flags);
        int errorCode = tjGetErrorCode(handle_old);
        (void)errorCode;
        
        if (jpegBuf) {
            free(jpegBuf);
        }
    }

    // Test tjCompress2
    if (srcBuf && handle_old) {
        unsigned char *jpegBuf = NULL;
        unsigned long jpegSize = jpegBufSizeOld;
        int pitch = width * tjPixelSize[pixelFormat];
        
        int result = tjCompress2(handle_old, srcBuf, width, pitch, height, pixelFormat,
                                &jpegBuf, &jpegSize, subsamp, jpegQual, flags);
        int errorCode = tjGetErrorCode(handle_old);
        (void)errorCode;
        
        if (jpegBuf) {
            free(jpegBuf);
        }
    }

    // Test tj3CompressFromYUVPlanes8
    if (yuvBuf && handle_new) {
        unsigned char *jpegBuf = NULL;
        size_t jpegSize = jpegBufSize;
        
        // Split YUV buffer into planes
        size_t ySize = width * height;
        size_t uvSize = ySize / (subsamp == TJSAMP_GRAY ? 1 : 4);
        
        if (yuvBufSize >= ySize + 2 * uvSize) {
            srcPlanes[0] = yuvBuf;
            srcPlanes[1] = yuvBuf + ySize;
            srcPlanes[2] = yuvBuf + ySize + uvSize;
            
            int strides[3] = {0, 0, 0};
            
            tj3Set(handle_new, TJPARAM_SUBSAMP, subsamp);
            tj3Set(handle_new, TJPARAM_QUALITY, jpegQual);
            
            int result = tj3CompressFromYUVPlanes8(handle_new, srcPlanes, width, strides, 
                                                  height, &jpegBuf, &jpegSize);
            int errorCode = tj3GetErrorCode(handle_new);
            (void)errorCode;
            
            if (jpegBuf) {
                free(jpegBuf);
            }
        }
    }

    // Cleanup
    if (yuvBuf) free(yuvBuf);
    if (srcBuf) free(srcBuf);
    
    if (handle_old) tjDestroy(handle_old);
    if (handle_new) tj3Destroy(handle_new);
    
    return 0;
}