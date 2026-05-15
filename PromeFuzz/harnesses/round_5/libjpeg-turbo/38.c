// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitTransform at turbojpeg.c:2748:20 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjTransform at turbojpeg.c:3041:15 in turbojpeg.h
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

static int dummy_custom_filter(short *coeffs, tjregion arrayRegion,
                               tjregion planeRegion, int componentID,
                               int transformID, struct tjtransform *transform) {
    return 0;
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;

    tjhandle handle = tjInitTransform();
    if (!handle) return 0;

    unsigned char *jpegBuf = (unsigned char *)Data;
    size_t jpegSize = Size;

    tjtransform transform;
    transform.r.x = Data[0] % 100;
    transform.r.y = Data[1] % 100;
    transform.r.w = Data[2] % 100 + 1;
    transform.r.h = Data[3] % 100 + 1;
    transform.op = Data[4] % 10;
    transform.options = Data[5] % 256;
    transform.data = NULL;
    transform.customFilter = dummy_custom_filter;

    size_t buf_size = tj3TransformBufSize(handle, &transform);
    if (buf_size > 0) {
        unsigned char *allocated_buf = tj3Alloc(buf_size);
        if (allocated_buf) {
            tj3Free(allocated_buf);
        }
    }

    int n = 1 + (Data[6] % 4);
    unsigned char **dstBufs = (unsigned char **)malloc(n * sizeof(unsigned char *));
    size_t *dstSizes = (size_t *)malloc(n * sizeof(size_t));
    tjtransform *transforms = (tjtransform *)malloc(n * sizeof(tjtransform));

    if (!dstBufs || !dstSizes || !transforms) {
        free(dstBufs);
        free(dstSizes);
        free(transforms);
        tjDestroy(handle);
        return 0;
    }

    for (int i = 0; i < n; i++) {
        dstBufs[i] = NULL;
        dstSizes[i] = 0;
        transforms[i] = transform;
        transforms[i].r.x = Data[7 + i * 4] % 100;
        transforms[i].r.y = Data[8 + i * 4] % 100;
        transforms[i].r.w = Data[9 + i * 4] % 100 + 1;
        transforms[i].r.h = Data[10 + i * 4] % 100 + 1;
        transforms[i].op = Data[11 + i * 4] % 10;
    }

    int result = tj3Transform(handle, jpegBuf, jpegSize, n, dstBufs, dstSizes, transforms);
    if (result == 0) {
        for (int i = 0; i < n; i++) {
            if (dstBufs[i]) {
                tj3Free(dstBufs[i]);
            }
        }
    }

    free(dstBufs);
    free(dstSizes);
    free(transforms);

    unsigned char *old_dst_buf = NULL;
    unsigned long old_dst_size = 0;
    tjtransform old_transform = transform;
    int old_flags = TJXOPT_PERFECT;

    int old_result = tjTransform(handle, jpegBuf, (unsigned long)jpegSize, 1,
                                 &old_dst_buf, &old_dst_size, &old_transform, old_flags);
    if (old_result == 0 && old_dst_buf) {
        free(old_dst_buf);
    }

    tjDestroy(handle);
    return 0;
}