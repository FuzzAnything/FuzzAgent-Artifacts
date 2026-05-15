// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int get_valid_dimension(int value) {
    if (value <= 0) return 1;
    if (value > 4096) return 4096;
    return value;
}

static int get_valid_subsamp(int value) {
    value = value % 6;
    if (value < 0) value += 6;
    return value;
}

static int get_valid_pixel_format(int value) {
    value = value % 12;
    if (value < 0) value += 12;
    return value;
}

static int get_valid_quality(int value) {
    value = value % 101;
    if (value < 0) value += 101;
    return value;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    tjhandle handle = NULL;
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    int width, height, pitch;
    int pixelFormat, subsamp, quality, flags;
    int align, strides[3];
    const unsigned char *srcPlanes[3];
    unsigned char *srcBuf = NULL;
    unsigned char *dstBuf = NULL;
    unsigned char *yuvBuf = NULL;
    int i;
    
    // Parse parameters from fuzz data
    width = get_valid_dimension((Data[0] << 8) | Data[1]);
    height = get_valid_dimension((Data[2] << 8) | Data[3]);
    pitch = width * 3;
    pixelFormat = get_valid_pixel_format(Data[4]);
    subsamp = get_valid_subsamp(Data[5]);
    quality = get_valid_quality(Data[6]);
    flags = Data[7];
    align = (Data[8] % 16) + 1;
    
    // Initialize handles
    handle = tjInitCompress();
    if (!handle) goto cleanup;
    
    tjhandle decompHandle = tjInitDecompress();
    if (!decompHandle) {
        tjDestroy(handle);
        goto cleanup;
    }
    
    // Allocate source buffer
    srcBuf = tjAlloc(width * height * 3);
    if (!srcBuf) goto cleanup;
    
    // Fill source buffer with fuzz data
    size_t srcSize = width * height * 3;
    size_t copySize = (Size - 9 > srcSize) ? srcSize : Size - 9;
    if (copySize > 0) {
        memcpy(srcBuf, Data + 9, copySize);
    }
    
    // Test tjCompress2
    if (tjCompress2(handle, srcBuf, width, pitch, height, pixelFormat,
                   &jpegBuf, &jpegSize, subsamp, quality, flags) == 0) {
        // Test tjDecompressToYUV2 with the compressed JPEG
        if (jpegSize > 0 && jpegBuf) {
            yuvBuf = tjAlloc(width * height * 2);
            if (yuvBuf) {
                tjDecompressToYUV2(decompHandle, jpegBuf, jpegSize, yuvBuf,
                                  width, align, height, flags);
                tjFree(yuvBuf);
                yuvBuf = NULL;
            }
        }
        
        // Test tjDecodeYUVPlanes if we have YUV data
        if (jpegSize > 0 && jpegBuf) {
            // Create dummy YUV planes
            unsigned char *yPlane = tjAlloc(width * height);
            unsigned char *uPlane = tjAlloc(width * height / 4);
            unsigned char *vPlane = tjAlloc(width * height / 4);
            
            if (yPlane && uPlane && vPlane) {
                srcPlanes[0] = yPlane;
                srcPlanes[1] = uPlane;
                srcPlanes[2] = vPlane;
                
                strides[0] = width;
                strides[1] = width / 2;
                strides[2] = width / 2;
                
                dstBuf = tjAlloc(width * height * 3);
                if (dstBuf) {
                    tjDecodeYUVPlanes(decompHandle, srcPlanes, strides, subsamp,
                                     dstBuf, width, pitch, height, pixelFormat, flags);
                    tjFree(dstBuf);
                    dstBuf = NULL;
                }
            }
            
            if (yPlane) tjFree(yPlane);
            if (uPlane) tjFree(uPlane);
            if (vPlane) tjFree(vPlane);
        }
    }
    
    // Test tjCompress
    if (srcBuf) {
        unsigned char *dstCompressed = tjAlloc(width * height * 2);
        if (dstCompressed) {
            unsigned long compressedSize = 0;
            tjCompress(handle, srcBuf, width, pitch, height, 3,
                      dstCompressed, &compressedSize, subsamp, quality, flags);
            tjFree(dstCompressed);
        }
    }
    
    // Clean up JPEG buffer
    if (jpegBuf) {
        tjFree(jpegBuf);
        jpegBuf = NULL;
    }
    
    tjDestroy(decompHandle);
    tjDestroy(handle);
    
cleanup:
    if (srcBuf) tjFree(srcBuf);
    if (dstBuf) tjFree(dstBuf);
    if (yuvBuf) tjFree(yuvBuf);
    if (jpegBuf) tjFree(jpegBuf);
    
    return 0;
}