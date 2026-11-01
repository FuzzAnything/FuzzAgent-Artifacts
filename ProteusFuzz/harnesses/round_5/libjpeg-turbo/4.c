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
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    // 1. Prepare the environment
    tjhandle handle = tj3Init(TJINIT_TRANSFORM);
    if (!handle) {
        return 0;
    }

    // Ensure we have enough data to simulate a JPEG buffer
    const unsigned char *jpegBuf = Data;
    size_t jpegSize = Size;

    // 2. Invoke target functions in order

    // tj3DecompressHeader
    tj3DecompressHeader(handle, jpegBuf, jpegSize);

    // tj3Get (3 times)
    int val1 = tj3Get(handle, TJPARAM_JPEGWIDTH);
    int val2 = tj3Get(handle, TJPARAM_JPEGHEIGHT);
    int val3 = tj3Get(handle, TJPARAM_SUBSAMP);

    // tj3Set (4 times)
    // We use values derived from input data to diversify states
    int setVal1 = (Size > 0) ? (Data[0] % 100) : 0;
    int setVal2 = (Size > 1) ? (Data[1] % 4) : 0;
    // Replaced TJPARAM_ACCURACY with TJPARAM_NOREALLOC to ensure compatibility
    int setVal3 = (Size > 2) ? (Data[2] % 2) : 0;
    int setVal4 = (Size > 3) ? (Data[3] % 2) : 0;

    tj3Set(handle, TJPARAM_QUALITY, setVal1);
    tj3Set(handle, TJPARAM_SUBSAMP, setVal2);
    tj3Set(handle, TJPARAM_NOREALLOC, setVal3);
    tj3Set(handle, TJPARAM_NOREALLOC, setVal4);

    // Prepare transform structure
    tjtransform transform;
    memset(&transform, 0, sizeof(transform));
    transform.r.x = 0;
    transform.r.y = 0;
    transform.r.w = (Size > 4) ? (Data[4] % 256) : 0;
    transform.r.h = (Size > 5) ? (Data[5] % 256) : 0;
    transform.op = (Size > 6) ? (Data[6] % 8) : 0;
    transform.options = (Size > 7) ? (Data[7] % 256) : 0;
    transform.data = NULL;
    transform.customFilter = NULL;

    // tj3TransformBufSize
    size_t bufSize1 = tj3TransformBufSize(handle, &transform);

    // tj3Alloc
    unsigned char *dstBuf1 = NULL;
    if (bufSize1 > 0) {
        dstBuf1 = (unsigned char *)tj3Alloc(bufSize1);
    }

    // tj3Set (1 time)
    int setVal5 = (Size > 8) ? (Data[8] % 2) : 0;
    tj3Set(handle, TJPARAM_NOREALLOC, setVal5);

    // tj3Transform
    size_t dstSize1 = 0;
    if (dstBuf1 && bufSize1 > 0) {
        dstSize1 = bufSize1;
    }
    tj3Transform(handle, jpegBuf, jpegSize, 1, &dstBuf1, &dstSize1, &transform);

    // tj3TransformBufSize (2nd call)
    size_t bufSize2 = tj3TransformBufSize(handle, &transform);

    // tj3Alloc (2nd call)
    unsigned char *dstBuf2 = NULL;
    if (bufSize2 > 0) {
        dstBuf2 = (unsigned char *)tj3Alloc(bufSize2);
    }

    // tj3Transform (2nd call)
    size_t dstSize2 = 0;
    if (dstBuf2 && bufSize2 > 0) {
        dstSize2 = bufSize2;
    }
    tj3Transform(handle, jpegBuf, jpegSize, 1, &dstBuf2, &dstSize2, &transform);

    // tj3TransformBufSize (3rd call)
    size_t bufSize3 = tj3TransformBufSize(handle, &transform);

    // tj3Alloc (3rd call)
    unsigned char *dstBuf3 = NULL;
    if (bufSize3 > 0) {
        dstBuf3 = (unsigned char *)tj3Alloc(bufSize3);
    }

    // tj3Transform (3rd call)
    size_t dstSize3 = 0;
    if (dstBuf3 && bufSize3 > 0) {
        dstSize3 = bufSize3;
    }
    tj3Transform(handle, jpegBuf, jpegSize, 1, &dstBuf3, &dstSize3, &transform);

    // tj3Set (2nd batch)
    int setVal6 = (Size > 9) ? (Data[9] % 2) : 0;
    tj3Set(handle, TJPARAM_NOREALLOC, setVal6);

    // tj3Transform (4th call)
    tj3Transform(handle, jpegBuf, jpegSize, 1, &dstBuf3, &dstSize3, &transform);

    // 3. Cleanup
    if (dstBuf1) {
        tj3Free(dstBuf1);
    }
    if (dstBuf2) {
        tj3Free(dstBuf2);
    }
    if (dstBuf3) {
        tj3Free(dstBuf3);
    }

    tj3Destroy(handle);

    return 0;
}