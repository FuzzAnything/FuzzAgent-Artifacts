// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
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

static void dummy_func(void) {
    // Prevents the function from being optimized away if the linker is aggressive
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    dummy_func(); // Ensure symbol presence

    if (Size == 0) return 0;

    // 1. Prepare the environment
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS | TJINIT_TRANSFORM);
    if (handle == NULL) return 0;

    // Ensure we have enough data for a header and some payload
    if (Size < 2) {
        tj3Destroy(handle);
        return 0;
    }

    // 2. Invoke tj3DecompressHeader
    if (tj3DecompressHeader(handle, Data, Size) != 0) {
        // Header reading failed, but we continue to test other APIs
    }

    // 3. Invoke tj3Get (3 times)
    tj3Get(handle, TJPARAM_SUBSAMP);
    tj3Get(handle, TJPARAM_QUALITY);
    tj3Get(handle, TJPARAM_FASTDCT);

    // 4. Invoke tj3Set (4 times)
    tj3Set(handle, TJPARAM_FASTDCT, 1);
    tj3Set(handle, TJPARAM_SAVEMARKERS, 0);
    tj3Set(handle, TJPARAM_FASTDCT, 0);
    tj3Set(handle, TJPARAM_NOREALLOC, 0);

    // Prepare transform structures
    tjtransform transforms[1];
    memset(&transforms[0], 0, sizeof(tjtransform));
    transforms[0].op = TJXOP_NONE;
    transforms[0].options = 0;
    transforms[0].r.x = 0;
    transforms[0].r.y = 0;
    transforms[0].r.w = 0;
    transforms[0].r.h = 0;
    transforms[0].customFilter = NULL;
    transforms[0].data = NULL;

    unsigned char *dstBufs[1];
    size_t dstSizes[1];
    dstBufs[0] = NULL;
    dstSizes[0] = 0;

    // 5. Invoke tj3TransformBufSize
    size_t bufSize1 = tj3TransformBufSize(handle, &transforms[0]);
    
    // 6. Invoke tj3Alloc
    unsigned char *buf1 = NULL;
    if (bufSize1 > 0) {
        buf1 = tj3Alloc(bufSize1);
        dstBufs[0] = buf1;
        dstSizes[0] = bufSize1;
    }

    // 7. Invoke tj3Set (1 time)
    tj3Set(handle, TJPARAM_NOREALLOC, 0);

    // 8. Invoke tj3Transform (1st time)
    tj3Transform(handle, Data, Size, 1, dstBufs, dstSizes, transforms);

    // 9. Invoke tj3TransformBufSize (2nd time)
    size_t bufSize2 = tj3TransformBufSize(handle, &transforms[0]);

    // 10. Invoke tj3Alloc (2nd time)
    unsigned char *buf2 = NULL;
    if (bufSize2 > 0) {
        buf2 = tj3Alloc(bufSize2);
        dstBufs[0] = buf2;
        dstSizes[0] = bufSize2;
    }

    // 11. Invoke tj3Transform (2nd time)
    tj3Transform(handle, Data, Size, 1, dstBufs, dstSizes, transforms);

    // 12. Invoke tj3TransformBufSize (3rd time)
    size_t bufSize3 = tj3TransformBufSize(handle, &transforms[0]);

    // 13. Invoke tj3Alloc (3rd time)
    unsigned char *buf3 = NULL;
    if (bufSize3 > 0) {
        buf3 = tj3Alloc(bufSize3);
        dstBufs[0] = buf3;
        dstSizes[0] = bufSize3;
    }

    // 14. Invoke tj3Transform (3rd time)
    tj3Transform(handle, Data, Size, 1, dstBufs, dstSizes, transforms);

    // 15. Invoke tj3Set (2nd time in this block)
    tj3Set(handle, TJPARAM_NOREALLOC, 1);

    // 16. Invoke tj3Transform (4th time)
    tj3Transform(handle, Data, Size, 1, dstBufs, dstSizes, transforms);

    // Cleanup
    if (buf1) tj3Free(buf1);
    if (buf2) tj3Free(buf2);
    if (buf3) tj3Free(buf3);
    
    tj3Destroy(handle);

    return 0;
}