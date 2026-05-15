// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
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
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
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

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    tjhandle handle = NULL;
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    unsigned char *dstBuf1 = NULL, *dstBuf2 = NULL, *dstBuf3 = NULL;
    size_t dstSize1 = 0, dstSize2 = 0, dstSize3 = 0;
    tjtransform transforms[3];
    unsigned char *dstBufs[3];
    size_t dstSizes[3];
    int width = 0, height = 0;
    size_t bufSize = 0;

    if (Size < 1) {
        return 0;
    }

    handle = tj3Init(TJINIT_TRANSFORM);
    if (!handle) {
        goto cleanup;
    }

    jpegBuf = (unsigned char *)Data;
    jpegSize = Size;

    if (tj3DecompressHeader(handle, jpegBuf, jpegSize) != 0) {
        goto cleanup;
    }

    tj3Get(handle, TJPARAM_JPEGWIDTH);
    tj3Get(handle, TJPARAM_JPEGHEIGHT);
    tj3Get(handle, TJPARAM_SUBSAMP);

    tj3Set(handle, TJPARAM_FASTUPSAMPLE, 1);
    tj3Set(handle, TJPARAM_BOTTOMUP, 0);
    tj3Set(handle, TJPARAM_COLORSPACE, TJCS_RGB);
    tj3Set(handle, TJPARAM_NOREALLOC, 0);

    width = tj3Get(handle, TJPARAM_JPEGWIDTH);
    height = tj3Get(handle, TJPARAM_JPEGHEIGHT);

    transforms[0].r.x = 0;
    transforms[0].r.y = 0;
    transforms[0].r.w = width > 0 ? width / 2 : 0;
    transforms[0].r.h = height > 0 ? height / 2 : 0;
    transforms[0].op = TJXOP_NONE;
    transforms[0].options = 0;
    transforms[0].data = NULL;
    transforms[0].customFilter = dummy_custom_filter;

    bufSize = tj3TransformBufSize(handle, &transforms[0]);
    if (bufSize > 0) {
        dstBuf1 = (unsigned char *)tj3Alloc(bufSize);
    }

    tj3Set(handle, TJPARAM_QUALITY, 85);

    dstBufs[0] = dstBuf1;
    dstSizes[0] = bufSize;
    if (dstBuf1) {
        tj3Transform(handle, jpegBuf, jpegSize, 1, dstBufs, dstSizes, transforms);
    }

    transforms[1].r.x = width > 0 ? width / 4 : 0;
    transforms[1].r.y = height > 0 ? height / 4 : 0;
    transforms[1].r.w = width > 0 ? width / 2 : 0;
    transforms[1].r.h = height > 0 ? height / 2 : 0;
    transforms[1].op = TJXOP_ROT90;
    transforms[1].options = TJXOPT_CROP;
    transforms[1].data = NULL;
    transforms[1].customFilter = NULL;

    bufSize = tj3TransformBufSize(handle, &transforms[1]);
    if (bufSize > 0) {
        dstBuf2 = (unsigned char *)tj3Alloc(bufSize);
    }

    dstBufs[0] = dstBuf2;
    dstSizes[0] = bufSize;
    if (dstBuf2) {
        tj3Transform(handle, jpegBuf, jpegSize, 1, dstBufs, dstSizes, transforms + 1);
    }

    transforms[2].r.x = 0;
    transforms[2].r.y = 0;
    transforms[2].r.w = width > 0 ? width : 0;
    transforms[2].r.h = height > 0 ? height : 0;
    transforms[2].op = TJXOP_HFLIP;
    transforms[2].options = 0;
    transforms[2].data = NULL;
    transforms[2].customFilter = dummy_custom_filter;

    bufSize = tj3TransformBufSize(handle, &transforms[2]);
    if (bufSize > 0) {
        dstBuf3 = (unsigned char *)tj3Alloc(bufSize);
    }

    tj3Set(handle, TJPARAM_FASTDCT, 1);

    dstBufs[0] = dstBuf3;
    dstSizes[0] = bufSize;
    if (dstBuf3) {
        tj3Transform(handle, jpegBuf, jpegSize, 1, dstBufs, dstSizes, transforms + 2);
    }

cleanup:
    if (dstBuf1) {
        tj3Free(dstBuf1);
    }
    if (dstBuf2) {
        tj3Free(dstBuf2);
    }
    if (dstBuf3) {
        tj3Free(dstBuf3);
    }
    if (handle) {
        tj3Destroy(handle);
    }
    return 0;
}