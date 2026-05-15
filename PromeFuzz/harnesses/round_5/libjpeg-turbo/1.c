// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
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
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
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

static int dummy_filter(short *coeffs, tjregion arrayRegion,
                        tjregion planeRegion, int componentID, int transformID,
                        struct tjtransform *transform) {
    return 0;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    tjhandle handle = NULL;
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = Size;
    int width = 0, height = 0, subsamp = 0;
    size_t bufSize = 0;
    unsigned char *dstBuf = NULL;
    size_t dstSize = 0;
    unsigned char *dstBufs[2] = {NULL, NULL};
    size_t dstSizes[2] = {0, 0};
    tjtransform transforms[2];
    int i;

    if (Size < 2) return 0;

    handle = tj3Init(TJINIT_TRANSFORM);
    if (!handle) goto cleanup;

    if (tj3DecompressHeader(handle, Data, Size) != 0) goto cleanup;

    tj3Get(handle, TJPARAM_JPEGWIDTH);
    tj3Get(handle, TJPARAM_JPEGHEIGHT);
    tj3Get(handle, TJPARAM_SUBSAMP);

    tj3Set(handle, TJPARAM_FASTUPSAMPLE, 1);
    tj3Set(handle, TJPARAM_FASTDCT, 1);
    tj3Set(handle, TJPARAM_OPTIMIZE, 1);
    tj3Set(handle, TJPARAM_PROGRESSIVE, 0);

    memset(&transforms[0], 0, sizeof(tjtransform));
    transforms[0].op = TJXOP_NONE;
    transforms[0].options = TJXOPT_CROP | TJXOPT_GRAY;
    transforms[0].customFilter = dummy_filter;

    bufSize = tj3TransformBufSize(handle, &transforms[0]);
    if (bufSize > 0) {
        dstBuf = tj3Alloc(bufSize);
    }

    tj3Set(handle, TJPARAM_NOREALLOC, 1);

    if (dstBuf) {
        dstSize = bufSize;
        tj3Transform(handle, Data, Size, 1, &dstBuf, &dstSize, &transforms[0]);
    }

    memset(&transforms[0], 0, sizeof(tjtransform));
    transforms[0].op = TJXOP_ROT90;
    transforms[0].options = TJXOPT_TRIM;
    transforms[1].op = TJXOP_HFLIP;
    transforms[1].options = TJXOPT_PERFECT;

    bufSize = tj3TransformBufSize(handle, &transforms[0]);
    if (bufSize > 0) {
        dstBufs[0] = tj3Alloc(bufSize);
        dstSizes[0] = bufSize;
    }

    bufSize = tj3TransformBufSize(handle, &transforms[1]);
    if (bufSize > 0) {
        dstBufs[1] = tj3Alloc(bufSize);
        dstSizes[1] = bufSize;
    }

    if (dstBufs[0] && dstBufs[1]) {
        tj3Transform(handle, Data, Size, 2, dstBufs, dstSizes, transforms);
    }

    memset(&transforms[0], 0, sizeof(tjtransform));
    transforms[0].op = TJXOP_TRANSPOSE;
    transforms[0].options = TJXOPT_COPYNONE;
    transforms[0].customFilter = dummy_filter;

    bufSize = tj3TransformBufSize(handle, &transforms[0]);
    if (bufSize > 0) {
        dstBuf = tj3Alloc(bufSize);
        dstSize = bufSize;
        tj3Set(handle, TJPARAM_NOREALLOC, 0);
        tj3Transform(handle, Data, Size, 1, &dstBuf, &dstSize, &transforms[0]);
    }

cleanup:
    if (dstBuf) {
        free(dstBuf);
    }
    for (i = 0; i < 2; i++) {
        if (dstBufs[i]) {
            free(dstBufs[i]);
        }
    }
    if (handle) {
        tj3Destroy(handle);
    }
    return 0;
}