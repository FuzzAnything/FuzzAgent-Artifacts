// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    tjhandle handle = tj3Init(TJINIT_DECOMPRESS | TJINIT_TRANSFORM);
    if (!handle) {
        return 0;
    }

    // Use input data as the JPEG buffer
    const unsigned char *jpegBuf = Data;
    size_t jpegSize = Size;

    // tj3DecompressHeader
    if (tj3DecompressHeader(handle, jpegBuf, jpegSize) != 0) {
        // If header parsing fails, we continue to test error handling paths
    }

    // tj3Get calls
    (void)tj3Get(handle, TJPARAM_QUALITY);
    (void)tj3Get(handle, TJPARAM_SUBSAMP);
    // Use TJPARAM_SAVEMARKERS instead of TJPARAM_ACCURACY
    (void)tj3Get(handle, TJPARAM_SAVEMARKERS);

    // tj3Set calls
    (void)tj3Set(handle, TJPARAM_QUALITY, 75);
    (void)tj3Set(handle, TJPARAM_SUBSAMP, TJSAMP_444);
    // Use TJPARAM_SAVEMARKERS
    (void)tj3Set(handle, TJPARAM_SAVEMARKERS, 1);
    (void)tj3Set(handle, TJPARAM_SAVEMARKERS, 2);

    // Prepare transform structure
    tjtransform transform = {0};
    // Initialize cropping region with safe defaults or fuzzed values if Size allows
    if (Size > 20) {
        transform.r.w = Data[10] % 1000;
        transform.r.h = Data[11] % 1000;
        transform.r.x = Data[12] % 1000;
        transform.r.y = Data[13] % 1000;
    } else {
        transform.r.w = 64;
        transform.r.h = 64;
        transform.r.x = 0;
        transform.r.y = 0;
    }
    transform.op = (Size > 15) ? (Data[15] % 8) : TJXOP_ROT90;
    transform.options = (Size > 16) ? (Data[16] % 16) : 0;
    transform.data = NULL;
    transform.customFilter = NULL;

    // tj3TransformBufSize
    size_t bufSize = tj3TransformBufSize(handle, &transform);
    if (bufSize == 0) {
        bufSize = 1; 
    }

    // tj3Alloc
    unsigned char *dstBuf = (unsigned char *)tj3Alloc(bufSize);
    size_t dstSize = bufSize;

    // tj3Set
    (void)tj3Set(handle, TJPARAM_NOREALLOC, 0);

    // tj3Transform
    unsigned char *dstBufs[1] = {dstBuf};
    size_t dstSizes[1] = {dstSize};
    (void)tj3Transform(handle, jpegBuf, jpegSize, 1, dstBufs, dstSizes, &transform);

    // tj3TransformBufSize (second time)
    bufSize = tj3TransformBufSize(handle, &transform);
    if (bufSize == 0) bufSize = 1;

    // tj3Alloc (second time)
    unsigned char *dstBuf2 = (unsigned char *)tj3Alloc(bufSize);
    size_t dstSize2 = bufSize;

    // tj3Transform (second time)
    unsigned char *dstBufs2[1] = {dstBuf2};
    size_t dstSizes2[1] = {dstSize2};
    (void)tj3Transform(handle, jpegBuf, jpegSize, 1, dstBufs2, dstSizes2, &transform);

    // tj3TransformBufSize (third time)
    bufSize = tj3TransformBufSize(handle, &transform);
    if (bufSize == 0) bufSize = 1;

    // tj3Alloc (third time)
    unsigned char *dstBuf3 = (unsigned char *)tj3Alloc(bufSize);
    size_t dstSize3 = bufSize;

    // tj3Transform (third time)
    unsigned char *dstBufs3[1] = {dstBuf3};
    size_t dstSizes3[1] = {dstSize3};
    (void)tj3Transform(handle, jpegBuf, jpegSize, 1, dstBufs3, dstSizes3, &transform);

    // tj3Set
    (void)tj3Set(handle, TJPARAM_NOREALLOC, 1);

    // tj3Transform (fourth time)
    unsigned char *dstBufs4[1] = {dstBuf3}; // Reuse buffer
    size_t dstSizes4[1] = {dstSize3};
    (void)tj3Transform(handle, jpegBuf, jpegSize, 1, dstBufs4, dstSizes4, &transform);

    // Cleanup
    if (dstBuf) tj3Free(dstBuf);
    if (dstBuf2) tj3Free(dstBuf2);
    if (dstBuf3) tj3Free(dstBuf3);
    tj3Destroy(handle);

    return 0;
}