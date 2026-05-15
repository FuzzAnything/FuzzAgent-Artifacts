// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

static const int kWidth = 64;
static const int kHeight = 64;
static const int kSubsamp = TJSAMP_420;
static const int kPixelFormat = TJPF_RGB;
static const int kAlign = 1;
static const int kFlags = 0;

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_28(const uint8_t *data, size_t size) {
    if (size < 100) return 0;
    
    write_dummy_file(data, size);
    
    tjhandle comp_handle = tjInitCompress();
    tjhandle decomp_handle = tjInitDecompress();
    if (!comp_handle || !decomp_handle) {
        if (comp_handle) tjDestroy(comp_handle);
        if (decomp_handle) tjDestroy(decomp_handle);
        return 0;
    }
    
    // Allocate source and destination buffers
    int src_pitch = kWidth * tjPixelSize[kPixelFormat];
    size_t src_size = src_pitch * kHeight;
    unsigned char *src_buf = (unsigned char *)malloc(src_size);
    if (!src_buf) {
        tjDestroy(comp_handle);
        tjDestroy(decomp_handle);
        return 0;
    }
    
    // Fill source buffer with fuzzer data (cyclic)
    for (size_t i = 0; i < src_size; i++) {
        src_buf[i] = data[i % size];
    }
    
    // Test tjEncodeYUV (deprecated but still present)
    size_t yuv_size = tjBufSizeYUV(kWidth, kHeight, kSubsamp);
    unsigned char *dst_buf = (unsigned char *)malloc(yuv_size);
    if (dst_buf) {
        tjEncodeYUV(comp_handle, src_buf, kWidth, src_pitch, kHeight,
                    tjPixelSize[kPixelFormat], dst_buf, kSubsamp, kFlags);
        free(dst_buf);
    }
    
    // Test tj3EncodeYUVPlanes8
    int strides[3] = {0, 0, 0};
    unsigned char *planes[3];
    int plane_width = kWidth;
    int plane_height = kHeight;
    
    if (kSubsamp == TJSAMP_420) {
        plane_width = (kWidth + 1) / 2;
        plane_height = (kHeight + 1) / 2;
    } else if (kSubsamp == TJSAMP_422) {
        plane_width = (kWidth + 1) / 2;
    }
    
    planes[0] = (unsigned char *)malloc(kWidth * kHeight);
    planes[1] = (unsigned char *)malloc(plane_width * plane_height);
    planes[2] = (unsigned char *)malloc(plane_width * plane_height);
    
    if (planes[0] && planes[1] && planes[2]) {
        tj3EncodeYUVPlanes8(comp_handle, src_buf, kWidth, src_pitch, kHeight,
                           kPixelFormat, planes, strides);
    }
    
    if (planes[0]) free(planes[0]);
    if (planes[1]) free(planes[1]);
    if (planes[2]) free(planes[2]);
    
    // Test tjEncodeYUVPlanes
    planes[0] = (unsigned char *)malloc(kWidth * kHeight);
    planes[1] = (unsigned char *)malloc(plane_width * plane_height);
    planes[2] = (unsigned char *)malloc(plane_width * plane_height);
    
    if (planes[0] && planes[1] && planes[2]) {
        tjEncodeYUVPlanes(comp_handle, src_buf, kWidth, src_pitch, kHeight,
                         kPixelFormat, planes, strides, kSubsamp, kFlags);
    }
    
    if (planes[0]) free(planes[0]);
    if (planes[1]) free(planes[1]);
    if (planes[2]) free(planes[2]);
    
    // Test tjDecodeYUV
    size_t yuv_buf_size = tjBufSizeYUV2(kWidth, kAlign, kHeight, kSubsamp);
    unsigned char *yuv_buf = (unsigned char *)malloc(yuv_buf_size);
    if (yuv_buf) {
        // Fill YUV buffer with fuzzer data
        for (size_t i = 0; i < yuv_buf_size && i < size; i++) {
            yuv_buf[i] = data[i];
        }
        
        unsigned char *rgb_buf = (unsigned char *)malloc(src_size);
        if (rgb_buf) {
            tjDecodeYUV(decomp_handle, yuv_buf, kAlign, kSubsamp,
                       rgb_buf, kWidth, src_pitch, kHeight, kPixelFormat, kFlags);
            free(rgb_buf);
        }
        free(yuv_buf);
    }
    
    // Test tjEncodeYUV3
    size_t yuv3_size = tjBufSizeYUV2(kWidth, kAlign, kHeight, kSubsamp);
    unsigned char *yuv3_buf = (unsigned char *)malloc(yuv3_size);
    if (yuv3_buf) {
        tjEncodeYUV3(comp_handle, src_buf, kWidth, src_pitch, kHeight,
                    kPixelFormat, yuv3_buf, kAlign, kSubsamp, kFlags);
        free(yuv3_buf);
    }
    
    // Test tjEncodeYUV2
    size_t yuv2_size = tjBufSizeYUV2(kWidth, 1, kHeight, kSubsamp);
    unsigned char *yuv2_buf = (unsigned char *)malloc(yuv2_size);
    if (yuv2_buf) {
        tjEncodeYUV2(comp_handle, src_buf, kWidth, src_pitch, kHeight,
                    kPixelFormat, yuv2_buf, kSubsamp, kFlags);
        free(yuv2_buf);
    }
    
    // Cleanup
    free(src_buf);
    tjDestroy(comp_handle);
    tjDestroy(decomp_handle);
    
    return 0;
}