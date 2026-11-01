// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// TJBUFSIZEYUV at turbojpeg.c:1013:25 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <turbojpeg.h>

static int fuzzer_test(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to safely read required bytes
    if (Size < 10) return 0;

    int width = (int)(Data[0] | (Data[1] << 8));
    int height = (int)(Data[2] | (Data[3] << 8));
    int subsamp = Data[4] % 5; // TJSAMP_444, 422, 420, 440, 422H
    int align = 1 << (Data[5] % 8); // Power of 2 alignment
    int componentID = Data[6] % 3; // 0=Y, 1=U, 2=V
    int quality = 1 + (Data[7] % 100);
    int stride = (int)((Data[8] << 8) | Data[9]);
    
    // Clamp dimensions to reasonable limits to avoid allocation failures or crashes
    if (width <= 0 || width > 16384) width = 1;
    if (height <= 0 || height > 16384) height = 1;
    if (align <= 0) align = 1;
    if (stride < 0) stride = 0;

    // Test tjBufSizeYUV2
    unsigned long bufSizeYUV2 = tjBufSizeYUV2(width, align, height, subsamp);
    if (bufSizeYUV2 != (unsigned long)-1) {
        unsigned char *yuvBuf = malloc(bufSizeYUV2);
        if (yuvBuf) {
            memset(yuvBuf, 0, bufSizeYUV2);
            free(yuvBuf);
        }
    }

    // Test TJBUFSIZEYUV
    unsigned long bufSizeYUV = TJBUFSIZEYUV(width, height, subsamp);
    if (bufSizeYUV != (unsigned long)-1) {
        unsigned char *yuvBuf2 = malloc(bufSizeYUV);
        if (yuvBuf2) {
            memset(yuvBuf2, 0, bufSizeYUV);
            free(yuvBuf2);
        }
    }

    // Test tjBufSize
    unsigned long jpegBufSize = tjBufSize(width, height, subsamp);
    if (jpegBufSize != (unsigned long)-1) {
        unsigned char *jpegBuf = malloc(jpegBufSize);
        if (jpegBuf) {
            memset(jpegBuf, 0, jpegBufSize);
            free(jpegBuf);
        }
    }

    // Test tj3YUVPlaneSize
    size_t planeSize = tj3YUVPlaneSize(componentID, width, stride, height, subsamp);
    if (planeSize > 0) {
        unsigned char *planeBuf = malloc(planeSize);
        if (planeBuf) {
            memset(planeBuf, 0, planeSize);
            free(planeBuf);
        }
    }

    // Test tj3YUVBufSize
    size_t yuvBufSize3 = tj3YUVBufSize(width, align, height, subsamp);
    if (yuvBufSize3 > 0) {
        unsigned char *yuvBuf3 = malloc(yuvBufSize3);
        if (yuvBuf3) {
            memset(yuvBuf3, 0, yuvBufSize3);
            free(yuvBuf3);
        }
    }

    // Test tjCompress
    // Initialize TurboJPEG instance
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    // Prepare source buffer (RGB)
    int pixelSize = 3; // RGB
    int pitch = width * pixelSize;
    size_t srcSize = (size_t)width * height * pixelSize;
    
    // Safety check for overflow
    if (srcSize > SIZE_MAX / 2) srcSize = SIZE_MAX / 2; 

    unsigned char *srcBuf = malloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return 0;
    }
    
    // Fill source buffer with fuzz data or pattern
    size_t copySize = (Size > srcSize) ? srcSize : Size;
    if (copySize > 0) {
        memcpy(srcBuf, Data, copySize);
        if (copySize < srcSize) {
            memset(srcBuf + copySize, 0, srcSize - copySize);
        }
    }

    unsigned char *dstBuf = malloc(jpegBufSize);
    if (!dstBuf) {
        free(srcBuf);
        tjDestroy(handle);
        return 0;
    }

    unsigned long compressedSize = 0;
    int flags = 0;
    
    int ret = tjCompress(handle, srcBuf, width, pitch, height, pixelSize,
                         dstBuf, &compressedSize, subsamp, quality, flags);
    
    // We don't care about the return value for fuzzing, just that it doesn't crash
    (void)ret;

    free(srcBuf);
    free(dstBuf);
    tjDestroy(handle);

    return 0;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    return fuzzer_test(Data, Size);
}