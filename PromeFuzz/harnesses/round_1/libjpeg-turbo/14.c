// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3DecompressToYUVPlanes8 at turbojpeg.c:2122:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_handles(tjhandle comp_handle, tjhandle decomp_handle) {
    if (comp_handle) tj3Destroy(comp_handle);
    if (decomp_handle) tj3Destroy(decomp_handle);
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    tjhandle comp_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle decomp_handle = tj3Init(TJINIT_DECOMPRESS);
    if (!comp_handle || !decomp_handle) {
        cleanup_handles(comp_handle, decomp_handle);
        return 0;
    }

    // Use first bytes to derive parameters
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int subsamp = Data[4] % 6; // TJSAMP values: 0-5
    int pixelFormat = Data[5] % 12; // TJPF values: 0-11
    int flags = Data[6] & 0xFF;
    int componentID = Data[7] % 3; // 0=Y, 1=U, 2=V
    int stride = (Data[8] << 8) | Data[9];

    // Clamp dimensions to reasonable values
    if (width <= 0) width = 1;
    if (height <= 0) height = 1;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    if (stride < 0) stride = 0;

    // Test helper functions
    size_t plane_size = tj3YUVPlaneSize(componentID, width, stride, height, subsamp);
    int plane_width = tj3YUVPlaneWidth(componentID, width, subsamp);
    int plane_height = tj3YUVPlaneHeight(componentID, height, subsamp);

    // Prepare YUV planes for compression
    unsigned char *planes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    size_t total_needed = 0;
    
    for (int i = 0; i < 3; i++) {
        size_t ps = tj3YUVPlaneSize(i, width, 0, height, subsamp);
        if (ps > 0 && ps < 1024 * 1024 * 10) { // Limit to 10MB
            planes[i] = (unsigned char *)malloc(ps);
            if (planes[i]) {
                size_t copy_size = Size > ps ? ps : Size;
                memcpy(planes[i], Data, copy_size);
                total_needed += ps;
            }
        }
        strides[i] = stride;
    }

    // Test tj3CompressFromYUVPlanes8
    if (total_needed > 0 && total_needed < 1024 * 1024 * 50) {
        unsigned char *jpegBuf = NULL;
        size_t jpegSize = 0;
        
        tj3CompressFromYUVPlanes8(comp_handle, (const unsigned char **)planes,
                                  width, strides, height, &jpegBuf, &jpegSize);
        
        if (jpegBuf && jpegSize > 0 && jpegSize < 1024 * 1024 * 100) {
            // Test tj3DecompressToYUVPlanes8 with the compressed data
            unsigned char *out_planes[3] = {NULL, NULL, NULL};
            int out_strides[3] = {0, 0, 0};
            
            for (int i = 0; i < 3; i++) {
                size_t ps = tj3YUVPlaneSize(i, width, 0, height, subsamp);
                if (ps > 0 && ps < 1024 * 1024 * 10) {
                    out_planes[i] = (unsigned char *)malloc(ps);
                }
            }
            
            tj3DecompressToYUVPlanes8(decomp_handle, jpegBuf, jpegSize,
                                      out_planes, out_strides);
            
            for (int i = 0; i < 3; i++) {
                if (out_planes[i]) free(out_planes[i]);
            }
            
            tj3Free(jpegBuf);
        }
    }

    // Test tjEncodeYUVPlanes with dummy RGB data
    if (Size > width * height * 3) {
        unsigned char *rgb_buf = (unsigned char *)malloc(width * height * 3);
        if (rgb_buf) {
            size_t copy_size = Size > width * height * 3 ? width * height * 3 : Size;
            memcpy(rgb_buf, Data, copy_size);
            
            unsigned char *dst_planes[3] = {NULL, NULL, NULL};
            int encode_strides[3] = {0, 0, 0};
            
            for (int i = 0; i < 3; i++) {
                size_t ps = tj3YUVPlaneSize(i, width, 0, height, subsamp);
                if (ps > 0 && ps < 1024 * 1024 * 10) {
                    dst_planes[i] = (unsigned char *)malloc(ps);
                }
            }
            
            tjEncodeYUVPlanes(comp_handle, rgb_buf, width, width * 3, height,
                              pixelFormat, dst_planes, encode_strides, subsamp, flags);
            
            for (int i = 0; i < 3; i++) {
                if (dst_planes[i]) free(dst_planes[i]);
            }
            free(rgb_buf);
        }
    }

    // Cleanup
    for (int i = 0; i < 3; i++) {
        if (planes[i]) free(planes[i]);
    }
    
    cleanup_handles(comp_handle, decomp_handle);
    return 0;
}