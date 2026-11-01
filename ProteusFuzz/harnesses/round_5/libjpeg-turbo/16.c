// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

static void free_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

static tjhandle create_compression_handle(void) {
    return tjInitCompress();
}

static tjhandle create_decompression_handle(void) {
    return tjInitDecompress();
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 5) return 0;

    tjhandle compHandle = NULL;
    tjhandle decompHandle = NULL;
    unsigned char *srcBuf = NULL;
    unsigned char *dstBuf = NULL;
    unsigned char **dstPlanes = NULL;
    const unsigned char *srcPlanes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    
    int width = (Data[0] & 0x7F) + 1;
    int height = (Data[1] & 0x7F) + 1;
    int pixelFormat = Data[2] % 7; // 0 to 6
    int subsamp = Data[3] % 5;     // 0 to 4
    int align = 1 << (Data[4] & 0x7); // Power of 2 alignment
    int flags = 0;
    
    // Calculate pitch
    int pixelSize = tjPixelSize[pixelFormat];
    int pitch = width * pixelSize;
    
    // Allocate source buffer
    size_t srcSize = (size_t)height * pitch;
    if (srcSize > 0 && srcSize < (size_t)1024 * 1024 * 10) { // Limit size
        srcBuf = (unsigned char *)malloc(srcSize);
        if (!srcBuf) return 0;
        memset(srcBuf, 0, srcSize);
        if (Size > srcSize) Size = srcSize;
        memcpy(srcBuf, Data, Size);
    } else {
        return 0;
    }

    // Test tj3EncodeYUV8
    compHandle = create_compression_handle();
    if (compHandle) {
        int bufSize = tj3YUVBufSize(width, align, height, subsamp);
        if (bufSize > 0 && bufSize < (int)(1024 * 1024 * 10)) {
            dstBuf = (unsigned char *)malloc(bufSize);
            if (dstBuf) {
                memset(dstBuf, 0, bufSize);
                tj3EncodeYUV8(compHandle, srcBuf, width, pitch, height, pixelFormat, dstBuf, align);
                if (tj3GetErrorCode(compHandle) == TJERR_FATAL) {
                    // Handle error
                }
                free(dstBuf);
            }
        }
        tjDestroy(compHandle);
    }

    // Test tjEncodeYUV2
    compHandle = create_compression_handle();
    if (compHandle) {
        int bufSize = tjBufSize(width, height, subsamp);
        if (bufSize > 0 && bufSize < (int)(1024 * 1024 * 10)) {
            dstBuf = (unsigned char *)malloc(bufSize);
            if (dstBuf) {
                memset(dstBuf, 0, bufSize);
                tjEncodeYUV2(compHandle, srcBuf, width, pitch, height, pixelFormat, dstBuf, subsamp, flags);
                if (tjGetErrorCode(compHandle) == TJERR_FATAL) {
                    // Handle error
                }
                free(dstBuf);
            }
        }
        tjDestroy(compHandle);
    }

    // Test tj3EncodeYUVPlanes8
    compHandle = create_compression_handle();
    if (compHandle) {
        // tj3YUVPlaneSize signature: (componentID, width, stride, height, subsamp)
        int ySize = tj3YUVPlaneSize(0, width, 0, height, subsamp);
        int uSize = tj3YUVPlaneSize(1, width, 0, height, subsamp);
        int vSize = tj3YUVPlaneSize(2, width, 0, height, subsamp);
        
        if (ySize > 0 && uSize > 0 && vSize > 0 && 
            ySize < (int)(1024 * 1024 * 10) && 
            uSize < (int)(1024 * 1024 * 10) && 
            vSize < (int)(1024 * 1024 * 10)) {
            
            dstPlanes = (unsigned char **)malloc(3 * sizeof(unsigned char *));
            if (dstPlanes) {
                dstPlanes[0] = (unsigned char *)malloc(ySize);
                dstPlanes[1] = (unsigned char *)malloc(uSize);
                dstPlanes[2] = (unsigned char *)malloc(vSize);
                
                if (dstPlanes[0] && dstPlanes[1] && dstPlanes[2]) {
                    memset(dstPlanes[0], 0, ySize);
                    memset(dstPlanes[1], 0, uSize);
                    memset(dstPlanes[2], 0, vSize);
                    
                    strides[0] = 0;
                    strides[1] = 0;
                    strides[2] = 0;
                    
                    tj3EncodeYUVPlanes8(compHandle, srcBuf, width, pitch, height, pixelFormat, dstPlanes, strides);
                    if (tj3GetErrorCode(compHandle) == TJERR_FATAL) {
                        // Handle error
                    }
                }
                
                free(dstPlanes[0]);
                free(dstPlanes[1]);
                free(dstPlanes[2]);
                free(dstPlanes);
            }
        }
        tjDestroy(compHandle);
    }

    // Test tjDecodeYUVPlanes
    decompHandle = create_decompression_handle();
    if (decompHandle) {
        // tj3YUVPlaneSize signature: (componentID, width, stride, height, subsamp)
        int ySize = tj3YUVPlaneSize(0, width, 0, height, subsamp);
        int uSize = tj3YUVPlaneSize(1, width, 0, height, subsamp);
        int vSize = tj3YUVPlaneSize(2, width, 0, height, subsamp);
        
        if (ySize > 0 && uSize > 0 && vSize > 0 && 
            ySize < (int)(1024 * 1024 * 10) && 
            uSize < (int)(1024 * 1024 * 10) && 
            vSize < (int)(1024 * 1024 * 10)) {
            
            srcPlanes[0] = (const unsigned char *)malloc(ySize);
            srcPlanes[1] = (const unsigned char *)malloc(uSize);
            srcPlanes[2] = (const unsigned char *)malloc(vSize);
            
            if (srcPlanes[0] && srcPlanes[1] && srcPlanes[2]) {
                memset((void *)srcPlanes[0], 0, ySize);
                memset((void *)srcPlanes[1], 0, uSize);
                memset((void *)srcPlanes[2], 0, vSize);
                
                dstBuf = (unsigned char *)malloc(srcSize);
                if (dstBuf) {
                    memset(dstBuf, 0, srcSize);
                    strides[0] = 0;
                    strides[1] = 0;
                    strides[2] = 0;
                    
                    tjDecodeYUVPlanes(decompHandle, srcPlanes, strides, subsamp, dstBuf, width, pitch, height, pixelFormat, flags);
                    if (tjGetErrorCode(decompHandle) == TJERR_FATAL) {
                        // Handle error
                    }
                    free(dstBuf);
                }
            }
            
            free((void *)srcPlanes[0]);
            free((void *)srcPlanes[1]);
            free((void *)srcPlanes[2]);
        }
        tjDestroy(decompHandle);
    }

    free(srcBuf);
    return 0;
}