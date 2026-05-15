// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int dummy_callback(short *coeffs, tjregion arrayRegion,
                         tjregion planeRegion, int componentID, int transformID,
                         struct tjtransform *transform) {
    return 0;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;

    tjhandle handle = NULL;
    unsigned char *jpegBuf = NULL;
    unsigned char **dstBufs = NULL;
    size_t *dstSizes = NULL;
    tjtransform *transforms = NULL;
    int n = 2;
    int i;

    handle = tj3Init(TJINIT_TRANSFORM);
    if (!handle) return 0;

    jpegBuf = (unsigned char *)tj3Alloc(Size);
    if (!jpegBuf) goto cleanup_handle;
    memcpy(jpegBuf, Data, Size);

    tj3Set(handle, TJPARAM_NOREALLOC, 0);
    tj3Set(handle, TJPARAM_QUALITY, 85);
    tj3Set(handle, TJPARAM_SUBSAMP, TJSAMP_420);
    tj3Set(handle, TJPARAM_FASTDCT, 1);

    dstBufs = (unsigned char **)tj3Alloc(n * sizeof(unsigned char *));
    dstSizes = (size_t *)tj3Alloc(n * sizeof(size_t));
    transforms = (tjtransform *)tj3Alloc(n * sizeof(tjtransform));

    if (!dstBufs || !dstSizes || !transforms) goto cleanup_all;

    for (i = 0; i < n; i++) {
        transforms[i].r.x = Data[i * 4] % 100;
        transforms[i].r.y = Data[i * 4 + 1] % 100;
        transforms[i].r.w = Data[i * 4 + 2] % 100 + 1;
        transforms[i].r.h = Data[i * 4 + 3] % 100 + 1;
        transforms[i].op = (Data[i] % 8) ? TJXOP_NONE : TJXOP_HFLIP;
        transforms[i].options = (Data[i + 1] % 2) ? TJXOPT_PERFECT : 0;
        transforms[i].data = NULL;
        transforms[i].customFilter = dummy_callback;

        dstSizes[i] = tj3TransformBufSize(handle, &transforms[i]);
        if (dstSizes[i] > 0) {
            dstBufs[i] = (unsigned char *)tj3Alloc(dstSizes[i]);
        } else {
            dstBufs[i] = NULL;
        }
    }

    tj3Transform(handle, jpegBuf, Size, n, dstBufs, dstSizes, transforms);

    for (i = 0; i < n; i++) {
        if (dstBufs[i]) tj3Free(dstBufs[i]);
    }

    tjhandle old_handle = tjInitTransform();
    if (old_handle) {
        unsigned char *old_dstBufs[2] = {NULL, NULL};
        unsigned long old_dstSizes[2] = {0, 0};
        tjtransform old_transforms[2];

        for (i = 0; i < 2; i++) {
            old_transforms[i].r.x = Data[i * 4] % 50;
            old_transforms[i].r.y = Data[i * 4 + 1] % 50;
            old_transforms[i].r.w = Data[i * 4 + 2] % 50 + 1;
            old_transforms[i].r.h = Data[i * 4 + 3] % 50 + 1;
            old_transforms[i].op = (Data[i + 2] % 8) ? TJXOP_NONE : TJXOP_VFLIP;
            old_transforms[i].options = (Data[i + 3] % 2) ? TJXOPT_PERFECT : 0;
            old_transforms[i].data = NULL;
            old_transforms[i].customFilter = dummy_callback;

            old_dstSizes[i] = 1024;
            old_dstBufs[i] = (unsigned char *)tj3Alloc(old_dstSizes[i]);
        }

        tjTransform(old_handle, jpegBuf, (unsigned long)Size, 
                   2, old_dstBufs, old_dstSizes, old_transforms, TJXOPT_PERFECT);

        for (i = 0; i < 2; i++) {
            if (old_dstBufs[i]) tj3Free(old_dstBufs[i]);
        }

        tjDestroy(old_handle);
    }

cleanup_all:
    if (transforms) tj3Free(transforms);
    if (dstSizes) tj3Free(dstSizes);
    if (dstBufs) tj3Free(dstBufs);
    if (jpegBuf) tj3Free(jpegBuf);

cleanup_handle:
    if (handle) tj3Destroy(handle);

    void *test_alloc = tj3Alloc(Data[0] % 1024 + 1);
    if (test_alloc) tj3Free(test_alloc);

    tj3Free(NULL);

    return 0;
}