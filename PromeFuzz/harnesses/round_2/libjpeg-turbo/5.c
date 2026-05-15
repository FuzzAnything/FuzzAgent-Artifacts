// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // Initialize a TurboJPEG decompression instance
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        return 0;
    }

    // Use a fixed scaling factor (1/1 - no scaling) for simplicity
    // TJUNSCALED is defined as {1, 1}
    tjscalingfactor scalingFactor = TJUNSCALED;
    tj3SetScalingFactor(handle, scalingFactor);

    // Allocate buffer for JPEG data
    // Use minimum size 1 to avoid zero-size allocation
    size_t jpegBufSize = Size > 0 ? Size : 1;
    unsigned char *jpegBuf = (unsigned char *)tj3Alloc(jpegBufSize);
    if (!jpegBuf) {
        tj3Destroy(handle);
        return 0;
    }

    // Copy fuzzer data to JPEG buffer
    if (Size > 0) {
        memcpy(jpegBuf, Data, Size);
    }

    // Allocate buffer for YUV output (dummy size, will be resized later)
    unsigned char *yuvBuf = (unsigned char *)tj3Alloc(1024);
    if (!yuvBuf) {
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }

    // Calculate YUV buffer size with reasonable parameters
    // Use small dimensions to avoid excessive memory usage
    int width = 64;
    int height = 64;
    int align = 4;  // Must be power of 2
    int subsamp = TJSAMP_420;  // Common subsampling
    size_t yuvSize = tj3YUVBufSize(width, align, height, subsamp);

    // If calculated size is valid and fits our buffer, use it
    if (yuvSize > 0 && yuvSize <= 1024) {
        // Buffer already allocated with sufficient size
    }

    // Decompress JPEG to YUV
    // Use the actual JPEG data size
    tj3DecompressToYUV8(handle, jpegBuf, Size, yuvBuf, align);

    // Allocate buffer for RGB output
    int pixelFormat = TJPF_RGB;
    int pitch = width * tjPixelSize[pixelFormat];
    unsigned char *rgbBuf = (unsigned char *)malloc(pitch * height);
    if (!rgbBuf) {
        tj3Free(yuvBuf);
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }

    // Decode YUV to RGB
    tj3DecodeYUV8(handle, yuvBuf, align, rgbBuf, width, pitch, height, pixelFormat);

    // Cleanup
    free(rgbBuf);
    tj3Free(yuvBuf);
    tj3Free(jpegBuf);
    tj3Destroy(handle);

    return 0;
}