// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_jpeg_buffer(unsigned char *jpegBuf) {
    if (jpegBuf) {
        tjFree(jpegBuf);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 64) return 0;

    // Initialize handles
    tjhandle compress_handle = tjInitCompress();
    tjhandle decompress_handle = tjInitDecompress();
    tjhandle newapi_handle = tj3Init(TJINIT_COMPRESS);
    
    if (!compress_handle || !decompress_handle || !newapi_handle) {
        if (compress_handle) tjDestroy(compress_handle);
        if (decompress_handle) tjDestroy(decompress_handle);
        if (newapi_handle) tj3Destroy(newapi_handle);
        return 0;
    }

    // Extract parameters from fuzzer input
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int pitch = (Data[4] << 8) | Data[5];
    int pixelFormat = Data[6] % 12;  // TJPF values 0-11
    int jpegSubsamp = Data[7] % 6;   // TJSAMP values 0-5
    int jpegQual = Data[8] % 101;    // 0-100
    int flags = Data[9] % 256;
    int align = (Data[10] % 16) + 1; // Alignment 1-16
    int pixelSize = (Data[11] % 4) + 1; // 1-4 bytes per pixel
    
    // Ensure reasonable dimensions
    if (width <= 0) width = 16;
    if (height <= 0) height = 16;
    if (pitch <= 0) pitch = width * pixelSize;
    
    // Calculate required buffer sizes
    size_t srcBufSize = (size_t)pitch * height;
    size_t yuvPlaneSize = (size_t)width * height;
    size_t dstBufSize = (size_t)width * height * pixelSize;
    
    if (srcBufSize == 0 || dstBufSize == 0 || yuvPlaneSize == 0) {
        tjDestroy(compress_handle);
        tjDestroy(decompress_handle);
        tj3Destroy(newapi_handle);
        return 0;
    }

    // Allocate and initialize buffers
    unsigned char *srcBuf = (unsigned char *)malloc(srcBufSize);
    unsigned char *dstBuf = (unsigned char *)malloc(dstBufSize);
    unsigned char *yuvPlanes[3];
    
    if (!srcBuf || !dstBuf) {
        free(srcBuf);
        free(dstBuf);
        tjDestroy(compress_handle);
        tjDestroy(decompress_handle);
        tj3Destroy(newapi_handle);
        return 0;
    }
    
    // Fill source buffer with fuzzer data (wrapping if needed)
    for (size_t i = 0; i < srcBufSize; i++) {
        srcBuf[i] = Data[i % Size];
    }
    
    // Test tjCompress2
    unsigned char *jpegBuf1 = NULL;
    unsigned long jpegSize1 = 0;
    if (tjCompress2(compress_handle, srcBuf, width, pitch, height, pixelFormat,
                    &jpegBuf1, &jpegSize1, jpegSubsamp, jpegQual, flags) != 0) {
        tjGetErrorCode(compress_handle);
    }
    cleanup_jpeg_buffer(jpegBuf1);
    
    // Test tjCompressFromYUV
    unsigned char *jpegBuf2 = NULL;
    unsigned long jpegSize2 = 0;
    
    // Prepare YUV planes
    for (int i = 0; i < 3; i++) {
        yuvPlanes[i] = (unsigned char *)malloc(yuvPlaneSize);
        if (yuvPlanes[i]) {
            for (size_t j = 0; j < yuvPlaneSize; j++) {
                yuvPlanes[i][j] = Data[(i * yuvPlaneSize + j) % Size];
            }
        }
    }
    
    if (yuvPlanes[0] && yuvPlanes[1] && yuvPlanes[2]) {
        // Create a contiguous YUV buffer
        unsigned char *yuvBuf = (unsigned char *)malloc(yuvPlaneSize * 3);
        if (yuvBuf) {
            memcpy(yuvBuf, yuvPlanes[0], yuvPlaneSize);
            memcpy(yuvBuf + yuvPlaneSize, yuvPlanes[1], yuvPlaneSize);
            memcpy(yuvBuf + yuvPlaneSize * 2, yuvPlanes[2], yuvPlaneSize);
            
            if (tjCompressFromYUV(compress_handle, yuvBuf, width, align, height,
                                  jpegSubsamp, &jpegBuf2, &jpegSize2, jpegQual, flags) != 0) {
                tjGetErrorCode(compress_handle);
            }
            cleanup_jpeg_buffer(jpegBuf2);
            free(yuvBuf);
        }
    }
    
    // Test tjCompress
    unsigned long compressedSize = dstBufSize;
    if (tjCompress(compress_handle, srcBuf, width, pitch, height, pixelSize,
                   dstBuf, &compressedSize, jpegSubsamp, jpegQual, flags) != 0) {
        tjGetErrorCode(compress_handle);
    }
    
    // Test tjDecodeYUVPlanes
    const unsigned char *srcPlanes[3] = {yuvPlanes[0], yuvPlanes[1], yuvPlanes[2]};
    int strides[3] = {width, width/2, width/2};
    
    if (tjDecodeYUVPlanes(decompress_handle, srcPlanes, strides, jpegSubsamp,
                          dstBuf, width, pitch, height, pixelFormat, flags) != 0) {
        tjGetErrorCode(decompress_handle);
    }
    
    // Test tj3GetErrorCode with new API handle
    tj3GetErrorCode(newapi_handle);
    
    // Cleanup
    for (int i = 0; i < 3; i++) {
        free(yuvPlanes[i]);
    }
    free(srcBuf);
    free(dstBuf);
    
    tjDestroy(compress_handle);
    tjDestroy(decompress_handle);
    tj3Destroy(newapi_handle);
    
    return 0;
}