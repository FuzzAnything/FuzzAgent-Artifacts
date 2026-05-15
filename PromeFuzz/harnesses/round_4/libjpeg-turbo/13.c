// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3DecompressToYUVPlanes8 at turbojpeg.c:2122:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tj3DecodeYUVPlanes8 at turbojpeg.c:2508:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "src/turbojpeg.h"

#define WIDTH 64
#define HEIGHT 64
#define MAX_PLANES 3

static void fill_buffer(unsigned char *buf, size_t size, const uint8_t *data, size_t data_size) {
    if (data_size == 0) {
        memset(buf, 0, size);
    } else {
        for (size_t i = 0; i < size; i++) {
            buf[i] = data[i % data_size];
        }
    }
}

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Initialize handles
    tjhandle comp_handle = tjInitCompress();
    tjhandle decomp_handle = tjInitDecompress();
    tjhandle new_handle = tj3Init(TJINIT_DECOMPRESS);
    
    if (!comp_handle || !decomp_handle || !new_handle) {
        if (comp_handle) tjDestroy(comp_handle);
        if (decomp_handle) tjDestroy(decomp_handle);
        if (new_handle) tj3Destroy(new_handle);
        return 0;
    }
    
    // Fixed dimensions for testing
    int width = WIDTH;
    int height = HEIGHT;
    int pitch = width * tjPixelSize[TJPF_RGB];
    int align = 1;
    int subsamp = TJSAMP_420;
    int pixelFormat = TJPF_RGB;
    int flags = 0;
    
    // Allocate buffers
    size_t y_size = width * height;
    size_t uv_size = (width/2) * (height/2);
    
    unsigned char *src_buf = malloc(y_size + 2*uv_size);
    unsigned char *dst_buf = malloc(pitch * height);
    unsigned char *rgb_buf = malloc(pitch * height);
    
    unsigned char *planes[MAX_PLANES];
    int strides[MAX_PLANES];
    
    planes[0] = malloc(y_size);
    planes[1] = malloc(uv_size);
    planes[2] = malloc(uv_size);
    
    unsigned char *dst_planes[MAX_PLANES];
    dst_planes[0] = malloc(y_size);
    dst_planes[1] = malloc(uv_size);
    dst_planes[2] = malloc(uv_size);
    
    // Fill buffers with fuzz data
    if (src_buf && dst_buf && rgb_buf && planes[0] && planes[1] && planes[2] &&
        dst_planes[0] && dst_planes[1] && dst_planes[2]) {
        
        // Fill source buffers
        fill_buffer(src_buf, y_size + 2*uv_size, Data, Size);
        fill_buffer(rgb_buf, pitch * height, Data, Size);
        fill_buffer(planes[0], y_size, Data, Size);
        fill_buffer(planes[1], uv_size, Data, Size);
        fill_buffer(planes[2], uv_size, Data, Size);
        
        // Set strides
        for (int i = 0; i < MAX_PLANES; i++) {
            strides[i] = 0;
        }
        
        // Test tjDecodeYUV
        tjDecodeYUV(decomp_handle, src_buf, align, subsamp, dst_buf,
                   width, pitch, height, pixelFormat, flags);
        
        // Test tj3DecompressToYUVPlanes8
        write_dummy_file(Data, Size);
        FILE *fp = fopen("./dummy_file", "rb");
        if (fp) {
            fseek(fp, 0, SEEK_END);
            long jpeg_size = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            unsigned char *jpeg_buf = malloc(jpeg_size);
            if (jpeg_buf) {
                fread(jpeg_buf, 1, jpeg_size, fp);
                tj3DecompressToYUVPlanes8(new_handle, jpeg_buf, jpeg_size,
                                         dst_planes, strides);
                free(jpeg_buf);
            }
            fclose(fp);
        }
        
        // Test tjDecodeYUVPlanes
        const unsigned char *src_planes[MAX_PLANES];
        src_planes[0] = planes[0];
        src_planes[1] = planes[1];
        src_planes[2] = planes[2];
        
        tjDecodeYUVPlanes(decomp_handle, src_planes, strides, subsamp,
                         dst_buf, width, pitch, height, pixelFormat, flags);
        
        // Test tjEncodeYUVPlanes
        tjEncodeYUVPlanes(comp_handle, rgb_buf, width, pitch, height,
                         pixelFormat, dst_planes, strides, subsamp, flags);
        
        // Test tj3DecodeYUVPlanes8
        tj3DecodeYUVPlanes8(new_handle, src_planes, strides, dst_buf,
                           width, pitch, height, pixelFormat);
        
        // Test tjDecompressToYUVPlanes
        if (fp) {
            fp = fopen("./dummy_file", "rb");
            if (fp) {
                fseek(fp, 0, SEEK_END);
                long jpeg_size2 = ftell(fp);
                fseek(fp, 0, SEEK_SET);
                unsigned char *jpeg_buf2 = malloc(jpeg_size2);
                if (jpeg_buf2) {
                    fread(jpeg_buf2, 1, jpeg_size2, fp);
                    tjDecompressToYUVPlanes(decomp_handle, jpeg_buf2, jpeg_size2,
                                           dst_planes, width, strides, height, flags);
                    free(jpeg_buf2);
                }
                fclose(fp);
            }
        }
    }
    
    // Cleanup
    free(src_buf);
    free(dst_buf);
    free(rgb_buf);
    free(planes[0]);
    free(planes[1]);
    free(planes[2]);
    free(dst_planes[0]);
    free(dst_planes[1]);
    free(dst_planes[2]);
    
    tjDestroy(comp_handle);
    tjDestroy(decomp_handle);
    tj3Destroy(new_handle);
    
    return 0;
}