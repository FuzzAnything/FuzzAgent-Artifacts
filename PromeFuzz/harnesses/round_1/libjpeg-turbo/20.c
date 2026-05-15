// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tj3DecompressToYUVPlanes8 at turbojpeg.c:2122:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_DIM 4096
#define MAX_BUF_SIZE (MAX_DIM * MAX_DIM * 4)

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

static int get_safe_dimension(int dim) {
    if (dim <= 0 || dim > MAX_DIM) {
        return 64;
    }
    return dim;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return 0;
    }

    int width = 0, height = 0, jpegSubsamp = 0;
    int flags = 0;
    
    // Use first 4 bytes to determine flags for variety
    if (Size >= 4) {
        flags = Data[0] & 0xF;
    }

    // Try to get header first
    int header_result = tjDecompressHeader2(handle, (unsigned char *)Data, 
                                          (unsigned long)Size, &width, 
                                          &height, &jpegSubsamp);
    
    if (header_result == 0) {
        // Valid header found, use actual dimensions
        width = get_safe_dimension(width);
        height = get_safe_dimension(height);
    } else {
        // Invalid header, use safe defaults
        width = 64;
        height = 64;
        jpegSubsamp = TJSAMP_420;
    }

    // Test tjDecompressToYUV
    {
        unsigned long yuv_size = tjBufSizeYUV(width, height, jpegSubsamp);
        if (yuv_size > 0 && yuv_size < MAX_BUF_SIZE) {
            unsigned char *dst_buf = (unsigned char *)malloc(yuv_size);
            if (dst_buf) {
                tjDecompressToYUV(handle, (unsigned char *)Data, 
                                (unsigned long)Size, dst_buf, flags);
                free(dst_buf);
            }
        }
    }

    // Test tj3DecompressToYUVPlanes8
    {
        int num_planes = (jpegSubsamp == TJSAMP_GRAY) ? 1 : 3;
        unsigned char *planes[3] = {NULL, NULL, NULL};
        int strides[3] = {0, 0, 0};
        
        for (int i = 0; i < num_planes; i++) {
            unsigned long plane_size = tjPlaneSizeYUV(i, width, 0, height, jpegSubsamp);
            if (plane_size > 0 && plane_size < MAX_BUF_SIZE) {
                planes[i] = (unsigned char *)malloc(plane_size);
                if (planes[i]) {
                    strides[i] = width;
                }
            }
        }
        
        if (planes[0] != NULL) {
            tj3DecompressToYUVPlanes8(handle, Data, Size, planes, strides);
        }
        
        for (int i = 0; i < 3; i++) {
            if (planes[i]) {
                free(planes[i]);
            }
        }
    }

    // Test tjDecompressToYUVPlanes
    {
        int num_planes = (jpegSubsamp == TJSAMP_GRAY) ? 1 : 3;
        unsigned char *planes[3] = {NULL, NULL, NULL};
        int strides[3] = {0, 0, 0};
        
        for (int i = 0; i < num_planes; i++) {
            unsigned long plane_size = tjPlaneSizeYUV(i, width, 0, height, jpegSubsamp);
            if (plane_size > 0 && plane_size < MAX_BUF_SIZE) {
                planes[i] = (unsigned char *)malloc(plane_size);
                if (planes[i]) {
                    strides[i] = width;
                }
            }
        }
        
        if (planes[0] != NULL) {
            tjDecompressToYUVPlanes(handle, Data, (unsigned long)Size, 
                                  planes, width, strides, height, flags);
        }
        
        for (int i = 0; i < 3; i++) {
            if (planes[i]) {
                free(planes[i]);
            }
        }
    }

    // Test tjDecompress2
    {
        int pixel_format = TJPF_RGB;
        int pitch = width * tjPixelSize[pixel_format];
        unsigned long buf_size = pitch * height;
        
        if (buf_size > 0 && buf_size < MAX_BUF_SIZE) {
            unsigned char *dst_buf = (unsigned char *)malloc(buf_size);
            if (dst_buf) {
                tjDecompress2(handle, (unsigned char *)Data, 
                            (unsigned long)Size, dst_buf, width, 
                            pitch, height, pixel_format, flags);
                free(dst_buf);
            }
        }
    }

    // Test tjDecompressToYUV2
    {
        int align = 4;
        unsigned long yuv_size = tjBufSizeYUV2(width, align, height, jpegSubsamp);
        if (yuv_size > 0 && yuv_size < MAX_BUF_SIZE) {
            unsigned char *dst_buf = (unsigned char *)malloc(yuv_size);
            if (dst_buf) {
                tjDecompressToYUV2(handle, (unsigned char *)Data, 
                                 (unsigned long)Size, dst_buf, 
                                 width, align, height, flags);
                free(dst_buf);
            }
        }
    }

    cleanup_handle(handle);
    return 0;
}