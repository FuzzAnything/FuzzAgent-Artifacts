// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int test_encode_decode_yuv(tjhandle handle, const uint8_t *data, size_t size) {
    if (!handle) return 0;

    // Extract parameters from fuzz input
    if (size < 24) return 0;

    int width = *(int32_t *)(data + 0);
    int height = *(int32_t *)(data + 4);
    int pixelFormat = *(int32_t *)(data + 8);
    int subsampling = *(int32_t *)(data + 12);
    int align = *(int32_t *)(data + 16);
    int quality = *(int32_t *)(data + 20);

    // Validate and sanitize dimensions
    if (width <= 0 || height <= 0 || width > 8192 || height > 8192) return 0;
    if (pixelFormat < 0 || pixelFormat > 10) pixelFormat = TJPF_RGB;
    if (subsampling < 0 || subsampling > 3) subsampling = TJSAMP_420;
    if (align <= 0) align = 1;
    if (align & (align - 1)) align = 1; // Force power of 2
    if (quality < 1 || quality > 100) quality = 75;

    // Set parameters
    tj3Set(handle, TJPARAM_SUBSAMP, subsampling);
    tj3Set(handle, TJPARAM_QUALITY, quality);

    // Calculate buffer sizes
    int pitch = width * tjPixelSize[pixelFormat];
    size_t srcSize = (size_t)width * height * tjPixelSize[pixelFormat];
    size_t yuvSize = tj3YUVBufSize(width, height, align, subsampling);
    size_t jpegSize = 0;
    unsigned char *jpegBuf = NULL;

    // Allocate buffers
    unsigned char *srcBuf = malloc(srcSize);
    unsigned char *yuvBuf = malloc(yuvSize);
    unsigned char *dstBuf = malloc(srcSize);

    if (!srcBuf || !yuvBuf || !dstBuf) {
        free(srcBuf); free(yuvBuf); free(dstBuf);
        return 0;
    }

    // Fill source buffer with fuzz data safely
    memset(srcBuf, 0, srcSize);
    size_t availableData = size - 24;
    if (availableData > srcSize) availableData = srcSize;
    if (availableData > 0) {
        memcpy(srcBuf, data + 24, availableData);
    }

    // 1. Encode RGB -> YUV
    if (tj3EncodeYUV8(handle, srcBuf, width, pitch, height, pixelFormat, yuvBuf, align) == 0) {
        // 2. Compress YUV -> JPEG
        if (tj3CompressFromYUV8(handle, yuvBuf, width, align, height, &jpegBuf, &jpegSize) == 0) {
            // FIX: Ensure jpegBuf is not NULL and size is valid before decompression
            if (jpegBuf != NULL && jpegSize > 0) {
                // 3. Decompress JPEG -> YUV
                unsigned char *yuvBuf2 = malloc(yuvSize);
                if (yuvBuf2) {
                    if (tj3DecompressToYUV8(handle, jpegBuf, jpegSize, yuvBuf2, align) == 0) {
                        // 4. Decode YUV -> RGB
                        tj3DecodeYUV8(handle, yuvBuf2, align, dstBuf, width, pitch, height, pixelFormat);
                    }
                    free(yuvBuf2);
                }
            }
        }
    }

    // Cleanup
    free(srcBuf);
    free(yuvBuf);
    free(dstBuf);
    if (jpegBuf) tj3Free(jpegBuf);

    return 0;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create compression instance
    tjhandle handleC = tj3Init(TJINIT_COMPRESS);
    if (handleC) {
        test_encode_decode_yuv(handleC, Data, Size);
        tj3Destroy(handleC);
    }

    // Create decompression instance
    tjhandle handleD = tj3Init(TJINIT_DECOMPRESS);
    if (handleD) {
        // For decompression path, we might need a valid JPEG, but the fuzzer
        // will likely pass garbage. The library handles invalid JPEGs gracefully.
        // We reuse the test logic which attempts the full pipeline.
        // Note: tj3DecompressToYUV8 expects a JPEG, but our test logic tries to
        // compress first. If compression fails (due to bad input), it skips.
        // To specifically test decompression with raw fuzz data as JPEG:
        if (Size > 10) {
            size_t yuvSize = tj3YUVBufSize(64, 64, 1, TJSAMP_420);
            unsigned char *yuvBuf = malloc(yuvSize);
            if (yuvBuf) {
                // Try to decompress raw fuzz data as JPEG directly
                tj3DecompressToYUV8(handleD, Data, Size, yuvBuf, 1);
                free(yuvBuf);
            }
        }
        tj3Destroy(handleD);
    }

    return 0;
}