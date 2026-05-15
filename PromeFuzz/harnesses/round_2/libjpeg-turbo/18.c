// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3DecodeYUVPlanes8 at turbojpeg.c:2508:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tj3DecompressToYUVPlanes8 at turbojpeg.c:2122:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "src/turbojpeg.h"

#define WIDTH 64
#define HEIGHT 64
#define SUBSAMP TJSAMP_420
#define PIXEL_FORMAT TJPF_RGB

static void init_planes(unsigned char **planes, int *strides, int width, int height, int subsamp) {
    int i;
    for (i = 0; i < 3; i++) {
        int plane_width = (i == 0) ? width : width / 2;
        int plane_height = (i == 0) ? height : height / 2;
        strides[i] = plane_width;
        planes[i] = (unsigned char *)malloc(plane_width * plane_height);
        if (planes[i]) {
            memset(planes[i], 0x80, plane_width * plane_height);
        }
    }
}

static void free_planes(unsigned char **planes, int num_planes) {
    int i;
    for (i = 0; i < num_planes; i++) {
        free(planes[i]);
    }
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    tjhandle handle = NULL;
    unsigned char *dst_buf = NULL;
    unsigned char *src_buf = NULL;
    unsigned char *planes[3] = {NULL, NULL, NULL};
    int strides[3] = {0};
    int width = WIDTH;
    int height = HEIGHT;
    int pitch = width * tjPixelSize[PIXEL_FORMAT];
    int subsamp = SUBSAMP;
    int flags = 0;
    int align = 1;
    int ret = 0;

    if (Size < 1) {
        return 0;
    }

    handle = tjInitDecompress();
    if (!handle) {
        goto cleanup;
    }

    dst_buf = (unsigned char *)malloc(width * height * tjPixelSize[PIXEL_FORMAT]);
    if (!dst_buf) {
        goto cleanup;
    }

    src_buf = (unsigned char *)malloc(Size);
    if (!src_buf) {
        goto cleanup;
    }
    memcpy(src_buf, Data, Size);

    init_planes(planes, strides, width, height, subsamp);

    /* Test tj3DecodeYUVPlanes8 */
    ret = tj3DecodeYUVPlanes8(handle, (const unsigned char *const *)planes,
                              strides, dst_buf, width, pitch, height, PIXEL_FORMAT);
    (void)ret;

    /* Test tjDecompressToYUVPlanes */
    ret = tjDecompressToYUVPlanes(handle, src_buf, (unsigned long)Size,
                                  planes, width, strides, height, flags);
    (void)ret;

    /* Test tj3DecompressToYUVPlanes8 */
    ret = tj3DecompressToYUVPlanes8(handle, src_buf, Size, planes, strides);
    (void)ret;

    /* Test tjDecodeYUVPlanes */
    ret = tjDecodeYUVPlanes(handle, (const unsigned char **)planes,
                            strides, subsamp, dst_buf, width, pitch,
                            height, PIXEL_FORMAT, flags);
    (void)ret;

    /* Test tjDecodeYUV */
    {
        unsigned char *yuv_buf = (unsigned char *)malloc(width * height * 2);
        if (yuv_buf) {
            memset(yuv_buf, 0x80, width * height * 2);
            ret = tjDecodeYUV(handle, yuv_buf, align, subsamp, dst_buf,
                              width, pitch, height, PIXEL_FORMAT, flags);
            free(yuv_buf);
        }
    }

    /* Test tjEncodeYUVPlanes */
    {
        tjhandle enc_handle = tjInitCompress();
        if (enc_handle) {
            memset(dst_buf, 0, width * height * tjPixelSize[PIXEL_FORMAT]);
            ret = tjEncodeYUVPlanes(enc_handle, dst_buf, width, pitch, height,
                                    PIXEL_FORMAT, planes, strides, subsamp, flags);
            tjDestroy(enc_handle);
        }
    }

cleanup:
    free(dst_buf);
    free(src_buf);
    free_planes(planes, 3);
    if (handle) {
        tjDestroy(handle);
    }
    return 0;
}