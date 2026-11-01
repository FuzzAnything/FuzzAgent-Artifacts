// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static int fuzz_decode_yuv(tjhandle handle, const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Extract parameters from fuzz data
    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int subsamp = (Size > 2) ? (Data[2] % 4) : TJSAMP_444;
    int align = 1 << ((Size > 3) ? (Data[3] % 4) : 0); // 1, 2, 4, or 8
    int pixelFormat = (Size > 4) ? (Data[4] % 6) : TJPF_RGB;

    // Calculate YUV buffer size
    size_t yuvSize = tj3YUVBufSize(width, align, height, subsamp);
    if (yuvSize == 0) return 0;

    // Allocate YUV buffer
    unsigned char *yuvBuf = (unsigned char *)tj3Alloc(yuvSize);
    if (!yuvBuf) return 0;

    // Fill YUV buffer with fuzz data (or pattern)
    size_t fillLen = (Size > 5) ? Size - 5 : 0;
    if (fillLen > 0) {
        memcpy(yuvBuf, Data + 5, (fillLen < yuvSize) ? fillLen : yuvSize);
    }

    // Calculate RGB buffer size
    int pitch = width * tjPixelSize[pixelFormat];
    size_t rgbSize = (size_t)height * pitch;
    unsigned char *rgbBuf = (unsigned char *)tj3Alloc(rgbSize);
    if (!rgbBuf) {
        tj3Free(yuvBuf);
        return 0;
    }

    // Decode YUV to RGB
    tj3DecodeYUV8(handle, yuvBuf, align, rgbBuf, width, pitch, height, pixelFormat);

    // Cleanup
    tj3Free(yuvBuf);
    tj3Free(rgbBuf);

    return 0;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Initialize TurboJPEG instance
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;

    // Extract scaling factor from data
    int scaleIdx = Data[0] % 8;
    tjscalingfactor scalingFactor = {0, 0};
    // Simple mapping to valid scaling factors (1/1, 1/2, 1/4, 1/8)
    switch (scaleIdx) {
        case 0: scalingFactor.num = 1; scalingFactor.denom = 1; break;
        case 1: scalingFactor.num = 1; scalingFactor.denom = 2; break;
        case 2: scalingFactor.num = 1; scalingFactor.denom = 4; break;
        case 3: scalingFactor.num = 1; scalingFactor.denom = 8; break;
        default: scalingFactor.num = 1; scalingFactor.denom = 1; break;
    }

    // Set scaling factor
    if (tj3SetScalingFactor(handle, scalingFactor) != 0) {
        tj3Destroy(handle);
        return 0;
    }

    // Extract JPEG dimensions from data
    int width = Data[1] + 1;
    int height = Data[2] + 1;
    int subsamp = (Size > 3) ? (Data[3] % 4) : TJSAMP_444;
    int align = 1 << ((Size > 4) ? (Data[4] % 4) : 0);

    // Calculate YUV buffer size
    size_t yuvSize = tj3YUVBufSize(width, align, height, subsamp);
    if (yuvSize == 0) {
        tj3Destroy(handle);
        return 0;
    }

    // Allocate YUV buffer (first tj3Alloc)
    unsigned char *yuvBuf = (unsigned char *)tj3Alloc(yuvSize);
    if (!yuvBuf) {
        tj3Destroy(handle);
        return 0;
    }

    // Allocate JPEG buffer (second tj3Alloc) - use remaining data as JPEG
    size_t jpegSize = (Size > 5) ? Size - 5 : 0;
    unsigned char *jpegBuf = (unsigned char *)tj3Alloc(jpegSize);
    if (!jpegBuf) {
        tj3Free(yuvBuf);
        tj3Destroy(handle);
        return 0;
    }

    if (jpegSize > 0) {
        memcpy(jpegBuf, Data + 5, jpegSize);
    }

    // Decompress to YUV
    if (tj3DecompressToYUV8(handle, jpegBuf, jpegSize, yuvBuf, align) == 0) {
        // If decompression succeeded, try to decode YUV
        fuzz_decode_yuv(handle, Data, Size);
    }

    // Cleanup
    tj3Free(jpegBuf);
    tj3Free(yuvBuf);
    tj3Destroy(handle);

    return 0;
}