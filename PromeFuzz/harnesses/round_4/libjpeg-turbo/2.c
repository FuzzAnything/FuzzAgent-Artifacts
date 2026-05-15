// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
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
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
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

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    tjhandle handle = NULL;
    unsigned char *jpeg_buf = NULL;
    size_t jpeg_size = 0;
    unsigned char *dst_bufs[1] = {NULL};
    size_t dst_sizes[1] = {0};
    tjtransform transforms[1];
    int param_values[3] = {0};
    size_t buf_size = 0;
    void *alloc_buf = NULL;
    int i;

    handle = tj3Init(TJINIT_TRANSFORM);
    if (!handle) {
        goto cleanup;
    }

    if (Size > 0) {
        jpeg_buf = (unsigned char *)Data;
        jpeg_size = Size;
        tj3DecompressHeader(handle, jpeg_buf, jpeg_size);
    }

    for (i = 0; i < 3; i++) {
        param_values[i] = tj3Get(handle, TJPARAM_SUBSAMP + i);
    }

    tj3Set(handle, TJPARAM_QUALITY, 85);
    tj3Set(handle, TJPARAM_SUBSAMP, TJSAMP_420);
    tj3Set(handle, TJPARAM_FASTUPSAMPLE, 1);

    memset(&transforms[0], 0, sizeof(tjtransform));
    transforms[0].op = TJXOP_NONE;
    transforms[0].options = TJXOPT_TRIM;
    transforms[0].customFilter = dummy_custom_filter;

    buf_size = tj3TransformBufSize(handle, &transforms[0]);
    if (buf_size > 0) {
        alloc_buf = tj3Alloc(buf_size);
        dst_bufs[0] = (unsigned char *)alloc_buf;
        dst_sizes[0] = buf_size;
    }

    tj3Set(handle, TJPARAM_NOREALLOC, 1);

    if (jpeg_buf && dst_bufs[0]) {
        tj3Transform(handle, jpeg_buf, jpeg_size, 1, dst_bufs, dst_sizes, transforms);
    }

    transforms[0].op = TJXOP_ROT90;
    buf_size = tj3TransformBufSize(handle, &transforms[0]);
    if (buf_size > 0) {
        if (alloc_buf) {
            free(alloc_buf);
        }
        alloc_buf = tj3Alloc(buf_size);
        dst_bufs[0] = (unsigned char *)alloc_buf;
        dst_sizes[0] = buf_size;
    }

    if (jpeg_buf && dst_bufs[0]) {
        tj3Transform(handle, jpeg_buf, jpeg_size, 1, dst_bufs, dst_sizes, transforms);
    }

    transforms[0].op = TJXOP_ROT180;
    buf_size = tj3TransformBufSize(handle, &transforms[0]);
    if (buf_size > 0) {
        if (alloc_buf) {
            free(alloc_buf);
        }
        alloc_buf = tj3Alloc(buf_size);
        dst_bufs[0] = (unsigned char *)alloc_buf;
        dst_sizes[0] = buf_size;
    }

    if (jpeg_buf && dst_bufs[0]) {
        tj3Transform(handle, jpeg_buf, jpeg_size, 1, dst_bufs, dst_sizes, transforms);
    }

    tj3Set(handle, TJPARAM_BOTTOMUP, 1);

    transforms[0].op = TJXOP_ROT270;
    if (jpeg_buf && dst_bufs[0]) {
        tj3Transform(handle, jpeg_buf, jpeg_size, 1, dst_bufs, dst_sizes, transforms);
    }

cleanup:
    if (alloc_buf) {
        free(alloc_buf);
    }
    tj3Destroy(handle);
    return 0;
}