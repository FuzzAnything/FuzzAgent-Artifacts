// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Initialize TurboJPEG instance for transformation
    tjhandle handle = tj3Init(TJINIT_TRANSFORM);
    if (!handle) {
        return 0;
    }

    // Use input data as the JPEG source buffer
    const unsigned char *jpegBuf = Data;
    size_t jpegSize = Size;

    // Initialize transform structure
    tjtransform transform = {0};
    transform.r = (tjregion){0, 0, 0, 0};
    transform.op = 0; // Default: no operation
    transform.options = 0;
    transform.data = NULL;
    transform.customFilter = NULL;

    // Try to read header to ensure valid JPEG structure
    if (tj3DecompressHeader(handle, jpegBuf, jpegSize) != 0) {
        tjDestroy(handle);
        return 0;
    }

    // Calculate required buffer size for transformation
    size_t bufSize = tj3TransformBufSize(handle, &transform);
    if (bufSize == 0) {
        tjDestroy(handle);
        return 0;
    }

    // Allocate destination buffer
    unsigned char *dstBuf = (unsigned char *)tj3Alloc(bufSize);
    if (!dstBuf) {
        tjDestroy(handle);
        return 0;
    }

    size_t dstSize = bufSize;
    unsigned char *dstBufs[] = {dstBuf};
    size_t dstSizes[] = {dstSize};

    // Perform the transform
    tj3Transform(handle, jpegBuf, jpegSize, 1, dstBufs, dstSizes, &transform);
    
    // Cleanup
    if (dstBuf) {
        tjFree(dstBuf);
    }
    tjDestroy(handle);

    return 0;
}