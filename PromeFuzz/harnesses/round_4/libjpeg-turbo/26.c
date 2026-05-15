// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
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

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    // Initialize handles
    tjhandle handle_old = tjInitCompress();
    tjhandle handle_new = tj3Init(TJINIT_COMPRESS);
    
    if (!handle_old || !handle_new) {
        if (handle_old) tjDestroy(handle_old);
        if (handle_new) tj3Destroy(handle_new);
        return 0;
    }

    // Extract parameters from fuzzer input
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int align = 1 << (Data[4] & 0x7); // Power of 2 alignment
    int subsamp = Data[5] % 5; // 0-4 for valid subsampling
    int jpegQual = Data[6] % 101; // 0-100
    int flags = Data[7];
    int pixelFormat = Data[8] % 12; // 0-11 for valid pixel formats
    
    // Ensure minimum dimensions
    if (width < 1) width = 1;
    if (height < 1) height = 1;
    if (align < 1) align = 1;
    
    // Calculate buffer sizes
    size_t yuvBufSize = tj3YUVBufSize(width, align, height, subsamp);
    size_t jpegBufSize = tj3JPEGBufSize(width, height, subsamp);
    size_t rawBufSize = width * height * 4; // Conservative estimate for raw buffer
    
    // Limit buffer sizes to prevent excessive memory usage
    if (yuvBufSize > 1024 * 1024) yuvBufSize = 1024 * 1024;
    if (jpegBufSize > 1024 * 1024) jpegBufSize = 1024 * 1024;
    if (rawBufSize > 1024 * 1024) rawBufSize = 1024 * 1024;
    
    // Allocate and fill buffers
    unsigned char *yuvBuf = NULL;
    unsigned char *rawBuf = NULL;
    unsigned char *jpegBuf1 = NULL;
    unsigned char *jpegBuf2 = NULL;
    unsigned char *jpegBuf3 = NULL;
    unsigned char *jpegBuf4 = NULL;
    
    if (yuvBufSize > 0) {
        yuvBuf = (unsigned char *)malloc(yuvBufSize);
        if (yuvBuf) {
            size_t copySize = Size > yuvBufSize ? yuvBufSize : Size;
            memcpy(yuvBuf, Data, copySize);
            // Fill remainder with pattern if needed
            for (size_t i = copySize; i < yuvBufSize; i++) {
                yuvBuf[i] = i % 256;
            }
        }
    }
    
    if (rawBufSize > 0) {
        rawBuf = (unsigned char *)malloc(rawBufSize);
        if (rawBuf) {
            size_t copySize = Size > rawBufSize ? rawBufSize : Size;
            memcpy(rawBuf, Data, copySize);
            // Fill remainder with pattern if needed
            for (size_t i = copySize; i < rawBufSize; i++) {
                rawBuf[i] = i % 256;
            }
        }
    }
    
    // Prepare YUV planes for tj3CompressFromYUVPlanes8
    const unsigned char *srcPlanes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    
    if (yuvBuf && yuvBufSize >= 3) {
        // Simple plane splitting for testing
        size_t planeSize = yuvBufSize / 3;
        srcPlanes[0] = yuvBuf;
        srcPlanes[1] = yuvBuf + planeSize;
        srcPlanes[2] = yuvBuf + planeSize * 2;
        strides[0] = width;
        strides[1] = width / 2;
        strides[2] = width / 2;
    }
    
    // Test tj3CompressFromYUV8
    if (yuvBuf) {
        size_t jpegSize1 = jpegBufSize;
        jpegBuf1 = (unsigned char *)tj3Alloc(jpegBufSize);
        
        if (jpegBuf1) {
            int result1 = tj3CompressFromYUV8(handle_new, yuvBuf, width, align, height, &jpegBuf1, &jpegSize1);
            // Get error code regardless of result
            int error1 = tj3GetErrorCode(handle_new);
            (void)error1; // Use error code to avoid unused variable warning
        }
    }
    
    // Test tjCompressFromYUV
    if (yuvBuf) {
        unsigned long jpegSize2 = jpegBufSize;
        jpegBuf2 = (unsigned char *)tjAlloc(jpegBufSize);
        
        if (jpegBuf2) {
            int result2 = tjCompressFromYUV(handle_old, yuvBuf, width, align, height, subsamp, &jpegBuf2, &jpegSize2, jpegQual, flags);
            // Get error code regardless of result
            int error2 = tjGetErrorCode(handle_old);
            (void)error2; // Use error code to avoid unused variable warning
        }
    }
    
    // Test tjCompress2
    if (rawBuf) {
        unsigned long jpegSize3 = jpegBufSize;
        jpegBuf3 = (unsigned char *)tjAlloc(jpegBufSize);
        
        if (jpegBuf3) {
            int pitch = width * 4; // Assuming 4 bytes per pixel for simplicity
            int result3 = tjCompress2(handle_old, rawBuf, width, pitch, height, pixelFormat, &jpegBuf3, &jpegSize3, subsamp, jpegQual, flags);
            // Get error code regardless of result
            int error3 = tjGetErrorCode(handle_old);
            (void)error3; // Use error code to avoid unused variable warning
        }
    }
    
    // Test tj3CompressFromYUVPlanes8
    if (srcPlanes[0]) {
        size_t jpegSize4 = jpegBufSize;
        jpegBuf4 = (unsigned char *)tj3Alloc(jpegBufSize);
        
        if (jpegBuf4) {
            int result4 = tj3CompressFromYUVPlanes8(handle_new, srcPlanes, width, strides, height, &jpegBuf4, &jpegSize4);
            // Get error code regardless of result
            int error4 = tj3GetErrorCode(handle_new);
            (void)error4; // Use error code to avoid unused variable warning
        }
    }
    
    // Test with file-based approach for additional coverage
    write_dummy_file(Data, Size > 1024 ? 1024 : Size);
    remove_dummy_file();
    
    // Cleanup
    if (yuvBuf) free(yuvBuf);
    if (rawBuf) free(rawBuf);
    if (jpegBuf1) tj3Free(jpegBuf1);
    if (jpegBuf2) tjFree(jpegBuf2);
    if (jpegBuf3) tjFree(jpegBuf3);
    if (jpegBuf4) tj3Free(jpegBuf4);
    
    tjDestroy(handle_old);
    tj3Destroy(handle_new);
    
    return 0;
}