// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "turbojpeg.h"

static int parse_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 4 > size) {
        return 0;
    }
    int value = (data[*offset] << 24) | (data[*offset + 1] << 16) |
                (data[*offset + 2] << 8) | data[*offset + 3];
    *offset += 4;
    return value;
}

static void cleanup_handle(tjhandle *handle) {
    if (*handle) {
        tj3Destroy(*handle);
        *handle = NULL;
    }
}

static void cleanup_buffers(unsigned char **buffers, int count) {
    for (int i = 0; i < count; i++) {
        if (buffers[i]) {
            free(buffers[i]);
            buffers[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 32) {
        return 0;
    }

    size_t offset = 0;
    int width = parse_int(Data, Size, &offset);
    int height = parse_int(Data, Size, &offset);
    int pixelFormat = parse_int(Data, Size, &offset);
    int subsamp = parse_int(Data, Size, &offset);
    int align = parse_int(Data, Size, &offset);
    int flags = parse_int(Data, Size, &offset);
    int pitch = parse_int(Data, Size, &offset);

    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        width = 64;
        height = 64;
    }
    if (pixelFormat < 0 || pixelFormat > TJPF_BGR) {
        pixelFormat = TJPF_RGB;
    }
    if (subsamp < 0 || subsamp > TJSAMP_440) {
        subsamp = TJSAMP_420;
    }
    if (align <= 0 || align > 256) {
        align = 1;
    }
    if (pitch < 0) {
        pitch = 0;
    }

    int pixelSize = tjPixelSize[pixelFormat];
    if (pixelSize <= 0) {
        pixelSize = 3;
    }
    int src_pitch = pitch ? pitch : width * pixelSize;
    size_t src_size = src_pitch * height;
    if (src_size == 0 || offset + src_size > Size) {
        return 0;
    }

    unsigned char *src_buf = (unsigned char *)malloc(src_size);
    if (!src_buf) {
        return 0;
    }
    memcpy(src_buf, Data + offset, src_size);
    offset += src_size;

    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) {
        free(src_buf);
        return 0;
    }

    tj3Set(handle, TJPARAM_SUBSAMP, subsamp);

    // Test tj3EncodeYUVPlanes8
    {
        int plane_count = (subsamp == TJSAMP_GRAY) ? 1 : 3;
        unsigned char *dst_planes[3] = {NULL, NULL, NULL};
        int strides[3] = {0, 0, 0};
        int plane_width[3], plane_height[3];
        
        for (int i = 0; i < plane_count; i++) {
            plane_width[i] = tj3YUVPlaneWidth(i, width, subsamp);
            plane_height[i] = tj3YUVPlaneHeight(i, height, subsamp);
            size_t plane_size = tj3YUVPlaneSize(i, width, strides[i], height, subsamp);
            if (plane_size > 0) {
                dst_planes[i] = (unsigned char *)malloc(plane_size);
            }
        }
        
        tj3EncodeYUVPlanes8(handle, src_buf, width, pitch, height,
                           pixelFormat, dst_planes, strides);
        
        cleanup_buffers(dst_planes, 3);
    }

    // Test tjEncodeYUV
    {
        unsigned long dst_size = tjBufSizeYUV2(width, align, height, subsamp);
        if (dst_size > 0 && dst_size < 100 * 1024 * 1024) {
            unsigned char *dst_buf = (unsigned char *)malloc(dst_size);
            if (dst_buf) {
                tjEncodeYUV(handle, src_buf, width, pitch, height,
                           pixelSize, dst_buf, subsamp, flags);
                free(dst_buf);
            }
        }
    }

    // Test tjEncodeYUV3
    {
        unsigned long dst_size = tjBufSizeYUV2(width, align, height, subsamp);
        if (dst_size > 0 && dst_size < 100 * 1024 * 1024) {
            unsigned char *dst_buf = (unsigned char *)malloc(dst_size);
            if (dst_buf) {
                tjEncodeYUV3(handle, src_buf, width, pitch, height,
                            pixelFormat, dst_buf, align, subsamp, flags);
                free(dst_buf);
            }
        }
    }

    // Test tj3EncodeYUV8
    {
        size_t dst_size = tj3YUVBufSize(width, align, height, subsamp);
        if (dst_size > 0 && dst_size < 100 * 1024 * 1024) {
            unsigned char *dst_buf = (unsigned char *)malloc(dst_size);
            if (dst_buf) {
                tj3EncodeYUV8(handle, src_buf, width, pitch, height,
                             pixelFormat, dst_buf, align);
                free(dst_buf);
            }
        }
    }

    // Test tjEncodeYUVPlanes
    {
        int plane_count = (subsamp == TJSAMP_GRAY) ? 1 : 3;
        unsigned char *dst_planes[3] = {NULL, NULL, NULL};
        int strides[3] = {0, 0, 0};
        
        for (int i = 0; i < plane_count; i++) {
            int plane_width = tjPlaneWidth(i, width, subsamp);
            strides[i] = plane_width;
            unsigned long plane_size = tjPlaneSizeYUV(i, width, strides[i], height, subsamp);
            if (plane_size > 0 && plane_size < 50 * 1024 * 1024) {
                dst_planes[i] = (unsigned char *)malloc(plane_size);
            }
        }
        
        tjEncodeYUVPlanes(handle, src_buf, width, pitch, height,
                         pixelFormat, dst_planes, strides, subsamp, flags);
        
        cleanup_buffers(dst_planes, 3);
    }

    // Test tjEncodeYUV2
    {
        unsigned long dst_size = tjBufSizeYUV2(width, align, height, subsamp);
        if (dst_size > 0 && dst_size < 100 * 1024 * 1024) {
            unsigned char *dst_buf = (unsigned char *)malloc(dst_size);
            if (dst_buf) {
                tjEncodeYUV2(handle, src_buf, width, pitch, height,
                            pixelFormat, dst_buf, subsamp, flags);
                free(dst_buf);
            }
        }
    }

    cleanup_handle(&handle);
    free(src_buf);
    return 0;
}