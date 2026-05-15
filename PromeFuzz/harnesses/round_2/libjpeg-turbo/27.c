// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
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

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

static void cleanup_buffer(unsigned char *buf) {
    if (buf) {
        tjFree(buf);
    }
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    // Initialize TurboJPEG handle for compression
    tjhandle handle = tjInitCompress();
    if (!handle) {
        return 0;
    }

    // Extract parameters from fuzzer input
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int align = Data[4];
    int pitch = (Data[5] << 8) | Data[6];
    int pixelFormat = Data[7] & 0x0F;
    int jpegSubsamp = (Data[8] >> 4) & 0x0F;
    int jpegQual = Data[9];
    int flags = Data[10];
    int subsamp = Data[11] & 0x0F;
    
    // Ensure reasonable dimensions to avoid excessive memory usage
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        width = 64;
        height = 64;
    }
    
    // Ensure alignment is power of 2 and reasonable
    if (align <= 0 || align > 256) {
        align = 1;
    }
    while (align & (align - 1)) {
        align++;
    }
    
    // Ensure pitch is at least width
    if (pitch < width) {
        pitch = width;
    }
    
    // Clamp quality
    if (jpegQual < 1) jpegQual = 1;
    if (jpegQual > 100) jpegQual = 100;
    
    // Clamp subsampling values
    if (jpegSubsamp > TJSAMP_GRAY) jpegSubsamp = TJSAMP_420;
    if (subsamp > TJSAMP_GRAY) subsamp = TJSAMP_420;
    
    // Clamp pixel format
    if (pixelFormat > TJCS_YCCK) pixelFormat = TJPF_RGB;
    
    // Calculate buffer sizes
    size_t yuvBufSize = tj3YUVBufSize(width, align, height, jpegSubsamp);
    size_t jpegBufSize = tj3JPEGBufSize(width, height, jpegSubsamp);
    
    // Allocate buffers
    unsigned char *srcBuf = NULL;
    unsigned char *srcPlanes[3] = {NULL, NULL, NULL};
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    unsigned long jpegSizeLong = 0;
    
    // Prepare source buffers from fuzzer data
    size_t dataOffset = 12;
    size_t remainingData = Size - dataOffset;
    
    // Test tj3CompressFromYUVPlanes8
    // Calculate plane sizes for Y, U, and V planes
    size_t yPlaneSize = tj3YUVPlaneSize(0, width, 0, height, jpegSubsamp);
    size_t uPlaneSize = tj3YUVPlaneSize(1, width, 0, height, jpegSubsamp);
    size_t vPlaneSize = tj3YUVPlaneSize(2, width, 0, height, jpegSubsamp);
    
    if (remainingData >= yPlaneSize + uPlaneSize + vPlaneSize) {
        srcPlanes[0] = (unsigned char*)malloc(yPlaneSize);
        srcPlanes[1] = (unsigned char*)malloc(uPlaneSize);
        srcPlanes[2] = (unsigned char*)malloc(vPlaneSize);
        
        if (srcPlanes[0] && srcPlanes[1] && srcPlanes[2]) {
            size_t offset = 0;
            memcpy(srcPlanes[0], Data + dataOffset + offset, yPlaneSize);
            offset += yPlaneSize;
            memcpy(srcPlanes[1], Data + dataOffset + offset, uPlaneSize);
            offset += uPlaneSize;
            memcpy(srcPlanes[2], Data + dataOffset + offset, vPlaneSize);
            
            int strides[3] = {0, 0, 0};
            jpegSize = jpegBufSize;
            jpegBuf = tj3Alloc(jpegSize);
            
            if (jpegBuf) {
                tj3CompressFromYUVPlanes8(handle, srcPlanes, width, strides, 
                                         height, &jpegBuf, &jpegSize);
                
                // Check error codes
                tjGetErrorCode(handle);
                tj3GetErrorCode(handle);
            }
            
            cleanup_buffer(jpegBuf);
        }
        
        for (int i = 0; i < 3; i++) {
            free(srcPlanes[i]);
        }
    }
    
    // Test tj3CompressFromYUV8
    if (remainingData >= yuvBufSize) {
        srcBuf = (unsigned char*)malloc(yuvBufSize);
        if (srcBuf) {
            memcpy(srcBuf, Data + dataOffset, yuvBufSize < remainingData ? yuvBufSize : remainingData);
            
            jpegSize = jpegBufSize;
            jpegBuf = tj3Alloc(jpegSize);
            
            if (jpegBuf) {
                tj3CompressFromYUV8(handle, srcBuf, width, align, height, 
                                   &jpegBuf, &jpegSize);
                
                // Check error codes
                tjGetErrorCode(handle);
                tj3GetErrorCode(handle);
            }
            
            cleanup_buffer(jpegBuf);
            free(srcBuf);
        }
    }
    
    // Test tjCompress2
    size_t srcBufSize = pitch * height * tjPixelSize[pixelFormat];
    if (remainingData >= srcBufSize) {
        srcBuf = (unsigned char*)malloc(srcBufSize);
        if (srcBuf) {
            memcpy(srcBuf, Data + dataOffset, srcBufSize < remainingData ? srcBufSize : remainingData);
            
            jpegBuf = NULL;
            jpegSizeLong = 0;
            
            tjCompress2(handle, srcBuf, width, pitch, height, pixelFormat,
                       &jpegBuf, &jpegSizeLong, jpegSubsamp, jpegQual, flags);
            
            // Check error codes
            tjGetErrorCode(handle);
            tj3GetErrorCode(handle);
            
            cleanup_buffer(jpegBuf);
            free(srcBuf);
        }
    }
    
    // Test tjCompressFromYUV
    size_t yuvSrcBufSize = tjBufSizeYUV2(width, align, height, subsamp);
    if (remainingData >= yuvSrcBufSize) {
        srcBuf = (unsigned char*)malloc(yuvSrcBufSize);
        if (srcBuf) {
            memcpy(srcBuf, Data + dataOffset, yuvSrcBufSize < remainingData ? yuvSrcBufSize : remainingData);
            
            jpegBuf = NULL;
            jpegSizeLong = 0;
            
            tjCompressFromYUV(handle, srcBuf, width, align, height, subsamp,
                             &jpegBuf, &jpegSizeLong, jpegQual, flags);
            
            // Check error codes
            tjGetErrorCode(handle);
            tj3GetErrorCode(handle);
            
            cleanup_buffer(jpegBuf);
            free(srcBuf);
        }
    }
    
    // Cleanup
    cleanup_handle(handle);
    
    return 0;
}