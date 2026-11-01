// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
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

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tjhandle handle = tj3Init(TJINIT_TRANSFORM);
    if (!handle) return 0;

    const unsigned char *jpegBuf = Data;
    size_t jpegSize = Size;

    // 1. tj3DecompressHeader
    tj3DecompressHeader(handle, jpegBuf, jpegSize);

    // 2. tj3Get (3 times)
    tj3Get(handle, TJPARAM_SUBSAMP);
    tj3Get(handle, TJPARAM_COLORSPACE);
    tj3Get(handle, TJPARAM_JPEGWIDTH);

    // 3. tj3Set (4 times)
    int val1 = (Size > 0) ? Data[0] % 10 : 0;
    int val2 = (Size > 1) ? Data[1] % 10 : 0;
    int val3 = (Size > 2) ? Data[2] % 10 : 0;
    int val4 = (Size > 3) ? Data[3] % 10 : 0;

    tj3Set(handle, TJPARAM_SAVEMARKERS, val1);
    tj3Set(handle, TJPARAM_JPEGWIDTH, val2);
    tj3Set(handle, TJPARAM_JPEGHEIGHT, val3);
    tj3Set(handle, TJPARAM_NOREALLOC, val4);

    // Prepare transform structure
    tjtransform transform = {0};
    transform.r.x = (Size > 4) ? Data[4] % 100 : 0;
    transform.r.y = (Size > 5) ? Data[5] % 100 : 0;
    transform.r.w = (Size > 6) ? Data[6] % 100 : 0;
    transform.r.h = (Size > 7) ? Data[7] % 100 : 0;
    transform.op = (Size > 8) ? Data[8] % 10 : 0;
    transform.options = (Size > 9) ? Data[9] % 100 : 0;
    transform.data = NULL;
    transform.customFilter = NULL;

    // 4. tj3TransformBufSize
    size_t bufSize1 = tj3TransformBufSize(handle, &transform);
    
    // 5. tj3Alloc
    unsigned char *dstBuf1 = NULL;
    size_t dstSize1 = 0;
    if (bufSize1 > 0) {
        dstBuf1 = (unsigned char *)tj3Alloc(bufSize1);
        if (dstBuf1) dstSize1 = bufSize1;
    }

    // 6. tj3Set (1 time)
    int val5 = (Size > 10) ? Data[10] % 10 : 0;
    tj3Set(handle, TJPARAM_NOREALLOC, val5);

    // 7. tj3Transform
    unsigned char *dstBufs1[1] = {dstBuf1};
    size_t dstSizes1[1] = {dstSize1};
    tjtransform transforms1[1] = {transform};
    
    tj3Transform(handle, jpegBuf, jpegSize, 1, dstBufs1, dstSizes1, transforms1);

    // 8. tj3TransformBufSize (2nd time)
    size_t bufSize2 = tj3TransformBufSize(handle, &transform);

    // 9. tj3Alloc (2nd time)
    unsigned char *dstBuf2 = NULL;
    size_t dstSize2 = 0;
    if (bufSize2 > 0) {
        dstBuf2 = (unsigned char *)tj3Alloc(bufSize2);
        if (dstBuf2) dstSize2 = bufSize2;
    }

    // 10. tj3Transform (2nd time)
    unsigned char *dstBufs2[1] = {dstBuf2};
    size_t dstSizes2[1] = {dstSize2};
    tjtransform transforms2[1] = {transform};
    
    tj3Transform(handle, jpegBuf, jpegSize, 1, dstBufs2, dstSizes2, transforms2);

    // 11. tj3TransformBufSize (3rd time)
    size_t bufSize3 = tj3TransformBufSize(handle, &transform);

    // 12. tj3Alloc (3rd time)
    unsigned char *dstBuf3 = NULL;
    size_t dstSize3 = 0;
    if (bufSize3 > 0) {
        dstBuf3 = (unsigned char *)tj3Alloc(bufSize3);
        if (dstBuf3) dstSize3 = bufSize3;
    }

    // 13. tj3Transform (3rd time)
    unsigned char *dstBufs3[1] = {dstBuf3};
    size_t dstSizes3[1] = {dstSize3};
    tjtransform transforms3[1] = {transform};
    
    tj3Transform(handle, jpegBuf, jpegSize, 1, dstBufs3, dstSizes3, transforms3);

    // 14. tj3Set (2nd time)
    int val6 = (Size > 11) ? Data[11] % 10 : 0;
    tj3Set(handle, TJPARAM_SAVEMARKERS, val6);

    // 15. tj3Transform (4th time)
    unsigned char *dstBufs4[1] = {NULL};
    size_t dstSizes4[1] = {0};
    tjtransform transforms4[1] = {transform};
    
    tj3Transform(handle, jpegBuf, jpegSize, 1, dstBufs4, dstSizes4, transforms4);

    // Cleanup
    if (dstBufs1[0]) tj3Free(dstBufs1[0]);
    if (dstBufs2[0]) tj3Free(dstBufs2[0]);
    if (dstBufs3[0]) tj3Free(dstBufs3[0]);
    if (dstBufs4[0]) tj3Free(dstBufs4[0]);

    tj3Destroy(handle);

    return 0;
}