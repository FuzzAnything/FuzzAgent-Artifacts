// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
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

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    // Try tj3DecompressHeader
    if (Size > 0) {
        tj3DecompressHeader(handle, Data, Size);
    }

    // Try tjDecompressHeader2
    int width = 0, height = 0, jpegSubsamp = 0;
    if (Size > 0) {
        tjDecompressHeader2(handle, (unsigned char*)Data, (unsigned long)Size,
                            &width, &height, &jpegSubsamp);
    }

    // Try tjDecompressHeader3
    int jpegColorspace = 0;
    if (Size > 0) {
        tjDecompressHeader3(handle, Data, (unsigned long)Size,
                            &width, &height, &jpegSubsamp, &jpegColorspace);
    }

    // Try tjDecompress2 with dummy output buffer
    if (width > 0 && height > 0 && width < 4096 && height < 4096) {
        int pitch = width * 3; // RGB
        unsigned char *dstBuf = (unsigned char*)malloc(pitch * height);
        if (dstBuf) {
            tjDecompress2(handle, Data, (unsigned long)Size, dstBuf,
                          width, pitch, height, TJPF_RGB, 0);
            free(dstBuf);
        }
    }

    // Try tjDecodeYUVPlanes with dummy YUV data
    if (Size > 100) {
        const unsigned char *srcPlanes[3];
        int strides[3] = {16, 8, 8};
        int small_width = 16;
        int small_height = 16;
        int pitch = small_width * 3;
        unsigned char *dstBuf = (unsigned char*)malloc(pitch * small_height);
        if (dstBuf) {
            srcPlanes[0] = Data;
            srcPlanes[1] = Data + 256;
            srcPlanes[2] = Data + 320;
            tjDecodeYUVPlanes(handle, srcPlanes, strides, TJSAMP_420,
                              dstBuf, small_width, pitch, small_height,
                              TJPF_RGB, 0);
            free(dstBuf);
        }
    }

    // Try tjTransform
    if (Size > 0) {
        tjtransform transform;
        transform.r.x = 0;
        transform.r.y = 0;
        transform.r.w = 100;
        transform.r.h = 100;
        transform.op = TJXOP_NONE;
        transform.options = TJXOPT_PERFECT;
        transform.customFilter = dummy_custom_filter;
        transform.data = NULL;

        unsigned char *dstBuf = NULL;
        unsigned long dstSize = 0;
        tjTransform(handle, Data, (unsigned long)Size, 1,
                    &dstBuf, &dstSize, &transform, TJXOPT_PERFECT);
        if (dstBuf) {
            free(dstBuf);
        }
    }

    tjDestroy(handle);
    return 0;
}