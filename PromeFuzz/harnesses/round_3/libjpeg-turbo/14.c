// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3GetICCProfile at turbojpeg.c:1926:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3TransformBufSize at turbojpeg.c:2828:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Transform at turbojpeg.c:2867:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "turbojpeg.h"

static int dummy_custom_filter(short *coeffs, tjregion arrayRegion,
                               tjregion planeRegion, int componentID,
                               int transformID, struct tjtransform *transform) {
    return 0;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    tjhandle decomp_handle = NULL;
    tjhandle trans_handle = NULL;
    tjhandle comp_handle = NULL;

    decomp_handle = tj3Init(TJINIT_DECOMPRESS);
    trans_handle = tj3Init(TJINIT_TRANSFORM);
    comp_handle = tj3Init(TJINIT_COMPRESS);

    if (!decomp_handle || !trans_handle || !comp_handle) {
        goto cleanup;
    }

    tj3Set(decomp_handle, TJPARAM_SAVEMARKERS, 2);

    int width, height, jpegSubsamp, jpegColorspace;
    unsigned char *jpegBuf = (unsigned char *)Data;
    size_t jpegSize = Size;

    if (tj3DecompressHeader(decomp_handle, jpegBuf, jpegSize) == 0) {
        width = tj3Get(decomp_handle, TJPARAM_JPEGWIDTH);
        height = tj3Get(decomp_handle, TJPARAM_JPEGHEIGHT);
        jpegSubsamp = tj3Get(decomp_handle, TJPARAM_SUBSAMP);
        jpegColorspace = tj3Get(decomp_handle, TJPARAM_COLORSPACE);

        unsigned char *iccBuf = NULL;
        size_t iccSize = 0;

        tj3GetICCProfile(decomp_handle, &iccBuf, &iccSize);

        if (iccBuf && iccSize > 0) {
            tj3SetICCProfile(comp_handle, iccBuf, iccSize);
            tj3Free(iccBuf);
        }

        tjtransform transform;
        memset(&transform, 0, sizeof(transform));
        transform.op = TJXOP_NONE;
        transform.options = 0;
        transform.customFilter = dummy_custom_filter;

        size_t bufSize = tj3TransformBufSize(trans_handle, &transform);

        if (bufSize > 0) {
            unsigned char *dstBuf = tj3Alloc(bufSize);
            size_t dstSize = bufSize;
            unsigned char *dstBufs[1] = {dstBuf};
            size_t dstSizes[1] = {dstSize};

            if (dstBuf) {
                tj3Transform(trans_handle, jpegBuf, jpegSize, 1, dstBufs, dstSizes, &transform);
                tj3Free(dstBuf);
            }
        }

        unsigned char *allocBuf = tj3Alloc(Size % 1024 + 1);
        if (allocBuf) {
            tj3Free(allocBuf);
        }
    }

cleanup:
    if (decomp_handle) {
        tj3Destroy(decomp_handle);
    }
    if (trans_handle) {
        tj3Destroy(trans_handle);
    }
    if (comp_handle) {
        tj3Destroy(comp_handle);
    }

    return 0;
}