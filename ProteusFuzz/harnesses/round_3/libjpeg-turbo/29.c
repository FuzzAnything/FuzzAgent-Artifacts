// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// TJBUFSIZEYUV at turbojpeg.c:1013:25 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static int fuzzer_test(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int align = Data[2] % 16 + 1;
    int subsamp = Data[3] % 3; // 0, 1, or 2

    // Test tjBufSizeYUV2
    unsigned long bufSize2 = tjBufSizeYUV2(width, align, height, subsamp);
    if (bufSize2 == (unsigned long)-1) return 0;

    // Test tjBufSizeYUV
    unsigned long bufSize = tjBufSizeYUV(width, height, subsamp);
    if (bufSize == (unsigned long)-1) return 0;

    // Test TJBUFSIZEYUV
    unsigned long bufSizeMacro = TJBUFSIZEYUV(width, height, subsamp);
    if (bufSizeMacro == (unsigned long)-1) return 0;

    // Allocate buffers
    unsigned char *yuvBuf = (unsigned char *)malloc(bufSize2);
    unsigned char *yuvBufLegacy = (unsigned char *)malloc(bufSize);
    unsigned char *jpegBuf = (unsigned char *)malloc(Size);
    unsigned char *dstBuf = (unsigned char *)malloc(bufSize2);
    unsigned long jpegSize = Size;
    unsigned long compressedSize = 0;

    if (!yuvBuf || !yuvBufLegacy || !jpegBuf || !dstBuf) {
        free(yuvBuf);
        free(yuvBufLegacy);
        free(jpegBuf);
        free(dstBuf);
        return 0;
    }

    // Initialize handle for compression
    tjhandle handle = tjInitCompress();
    if (!handle) {
        free(yuvBuf);
        free(yuvBufLegacy);
        free(jpegBuf);
        free(dstBuf);
        return 0;
    }

    // Fill buffers with fuzz data
    memset(yuvBuf, 0, bufSize2);
    memset(yuvBufLegacy, 0, bufSize);
    memcpy(jpegBuf, Data, Size);

    // Test tjCompressFromYUV
    unsigned char *outJpegBuf = NULL;
    unsigned long outJpegSize = 0;
    tjCompressFromYUV(handle, yuvBuf, width, align, height, subsamp,
                      &outJpegBuf, &outJpegSize, 85, 0);
    if (outJpegBuf) {
        free(outJpegBuf);
    }

    // Test tjCompress (RGB to JPEG)
    int pitch = width * 3;
    int pixelSize = 3;
    tjCompress(handle, yuvBuf, width, pitch, height, pixelSize,
               jpegBuf, &compressedSize, subsamp, 85, 0);

    // Initialize handle for decompression
    tjhandle decompHandle = tjInitDecompress();
    if (!decompHandle) {
        tjDestroy(handle);
        free(yuvBuf);
        free(yuvBufLegacy);
        free(jpegBuf);
        free(dstBuf);
        return 0;
    }

    // Test tjDecompressToYUV2
    tjDecompressToYUV2(decompHandle, jpegBuf, Size, dstBuf,
                       width, align, height, 0);

    // Cleanup
    tjDestroy(handle);
    tjDestroy(decompHandle);
    free(yuvBuf);
    free(yuvBufLegacy);
    free(jpegBuf);
    free(dstBuf);

    return 0;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    return fuzzer_test(Data, Size);
}