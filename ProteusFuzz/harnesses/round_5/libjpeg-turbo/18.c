// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void cleanup(tjhandle handle, unsigned char *buf) {
    if (handle) {
        tjDestroy(handle);
    }
    if (buf) {
        free(buf);
    }
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 5) return 0;

    // Extract parameters from input data to diversify test cases
    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int subsamp = (Data[2] % 5); // 0-4 covers common subsampling options
    int align = 1 << (Data[3] % 5); // 1, 2, 4, 8, 16
    int quality = (Data[3] % 100) + 1;
    int pixelFormat = (Data[4] % 3); // 0, 1, 2

    // Create handles
    tjhandle compressHandle = tjInitCompress();
    tjhandle decompressHandle = tjInitDecompress();

    if (!compressHandle || !decompressHandle) {
        if (compressHandle) tjDestroy(compressHandle);
        if (decompressHandle) tjDestroy(decompressHandle);
        return 0;
    }

    // Allocate buffers
    size_t yuvSize = tj3YUVBufSize(width, align, height, subsamp);
    if (yuvSize == 0) {
        cleanup(compressHandle, NULL);
        cleanup(decompressHandle, NULL);
        return 0;
    }

    unsigned char *yuvBuf = (unsigned char *)malloc(yuvSize);
    if (!yuvBuf) {
        cleanup(compressHandle, NULL);
        cleanup(decompressHandle, NULL);
        return 0;
    }

    // Initialize YUV buffer with fuzzing data
    memset(yuvBuf, 0, yuvSize);
    size_t copyLen = (Size > yuvSize) ? yuvSize : Size;
    memcpy(yuvBuf, Data, copyLen);

    // Prepare planes for planar functions
    unsigned char *planes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};

    // Calculate strides based on subsampling
    switch (subsamp) {
        case TJSAMP_GRAY:
            planes[0] = yuvBuf;
            strides[0] = width;
            break;
        case TJSAMP_444:
            planes[0] = yuvBuf;
            planes[1] = yuvBuf + (width * height);
            planes[2] = yuvBuf + (2 * width * height);
            strides[0] = width;
            strides[1] = width;
            strides[2] = width;
            break;
        case TJSAMP_422:
            planes[0] = yuvBuf;
            planes[1] = yuvBuf + (width * height);
            planes[2] = yuvBuf + (width * height + (width/2) * height);
            strides[0] = width;
            strides[1] = width / 2;
            strides[2] = width / 2;
            break;
        case TJSAMP_420:
        case TJSAMP_411:
            planes[0] = yuvBuf;
            planes[1] = yuvBuf + (width * height);
            planes[2] = yuvBuf + (width * height + (width/2) * (height/2));
            strides[0] = width;
            strides[1] = width / 2;
            strides[2] = width / 2;
            break;
        default:
            // Fallback for unknown subsamp, treat as 420
            planes[0] = yuvBuf;
            planes[1] = yuvBuf + (width * height);
            planes[2] = yuvBuf + (width * height + (width/2) * (height/2));
            strides[0] = width;
            strides[1] = width / 2;
            strides[2] = width / 2;
            break;
    }

    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    unsigned long jpegSizeLong = 0;

    // 1. tjCompressFromYUV
    // This function may allocate jpegBuf internally if NULL is passed.
    // We must ensure it is freed if allocation occurs.
    tjCompressFromYUV(compressHandle, yuvBuf, width, align, height, subsamp,
                      &jpegBuf, &jpegSizeLong, quality, 0);

    // 2. tjDecodeYUVPlanes
    unsigned char *rgbBuf = (unsigned char *)malloc(width * height * 4);
    if (rgbBuf) {
        tjDecodeYUVPlanes(compressHandle, (const unsigned char **)planes, strides, subsamp,
                          rgbBuf, width, width * 4, height, pixelFormat, 0);
        free(rgbBuf);
    }

    // 3. tj3DecompressToYUV8
    if (jpegBuf && jpegSizeLong > 0) {
        unsigned char *yuvDecompressed = (unsigned char *)malloc(yuvSize);
        if (yuvDecompressed) {
            tj3DecompressToYUV8(decompressHandle, jpegBuf, jpegSizeLong, yuvDecompressed, align);
            free(yuvDecompressed);
        }
    }

    // 4. tjDecompressToYUV2
    if (jpegBuf && jpegSizeLong > 0) {
        unsigned char *yuvDecompressed2 = (unsigned char *)malloc(yuvSize);
        if (yuvDecompressed2) {
            tjDecompressToYUV2(decompressHandle, jpegBuf, jpegSizeLong, yuvDecompressed2,
                               width, align, height, 0);
            free(yuvDecompressed2);
        }
    }

    // 5. tj3CompressFromYUVPlanes8
    unsigned char *jpegBuf2 = NULL;
    size_t jpegSize2 = 0;
    tj3CompressFromYUVPlanes8(compressHandle, (const unsigned char *const *)planes,
                              width, strides, height, &jpegBuf2, &jpegSize2);
    if (jpegBuf2) {
        tj3Free(jpegBuf2);
    }

    // Cleanup: Free the jpegBuf allocated by tjCompressFromYUV if it was allocated
    if (jpegBuf) {
        tj3Free(jpegBuf);
    }

    cleanup(compressHandle, yuvBuf);
    cleanup(decompressHandle, NULL);

    return 0;
}