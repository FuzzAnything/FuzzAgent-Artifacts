// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int get_valid_subsamp(uint8_t value) {
    const int subsamps[] = {TJSAMP_444, TJSAMP_422, TJSAMP_420, TJSAMP_GRAY, TJSAMP_440, TJSAMP_411};
    return subsamps[value % 6];
}

static int get_valid_pixel_format(uint8_t value) {
    const int formats[] = {TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, TJPF_XBGR, TJPF_XRGB,
                          TJPF_GRAY, TJPF_RGBA, TJPF_BGRA, TJPF_ABGR, TJPF_ARGB};
    return formats[value % 11];
}

static int get_valid_align(uint8_t value) {
    const int aligns[] = {1, 2, 4, 8, 16};
    return aligns[value % 5];
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    uint8_t selector = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;

    switch (selector % 6) {
        case 0: {  // tjDecodeYUV
            int width = 16 + (fuzz_data[0] % 64);
            int height = 16 + (fuzz_data[1] % 64);
            int align = get_valid_align(fuzz_data[2]);
            int subsamp = get_valid_subsamp(fuzz_data[3]);
            int pixel_format = get_valid_pixel_format(fuzz_data[4]);
            int pitch = width * tjPixelSize[pixel_format];
            
            size_t yuv_size = tjBufSizeYUV2(width, align, height, subsamp);
            if (yuv_size > fuzz_size) {
                yuv_size = fuzz_size;
            }
            
            unsigned char *dst_buf = tjAlloc(width * height * tjPixelSize[pixel_format]);
            if (dst_buf) {
                tjDecodeYUV(handle, fuzz_data, align, subsamp, dst_buf,
                           width, pitch, height, pixel_format, 0);
                tjFree(dst_buf);
            }
            break;
        }
        
        case 1: {  // tjAlloc and tjFree
            int alloc_size = 16 + (fuzz_data[0] % 1024);
            unsigned char *buf = tjAlloc(alloc_size);
            if (buf) {
                tjFree(buf);
            }
            break;
        }
        
        case 2: {  // tjDecompressToYUV2
            int width = 16 + (fuzz_data[0] % 64);
            int height = 16 + (fuzz_data[1] % 64);
            int align = get_valid_align(fuzz_data[2]);
            
            size_t yuv_size = tjBufSizeYUV2(width, align, height, TJSAMP_420);
            unsigned char *dst_buf = tjAlloc(yuv_size);
            if (dst_buf) {
                tjDecompressToYUV2(handle, fuzz_data, fuzz_size, dst_buf,
                                  width, align, height, 0);
                tjFree(dst_buf);
            }
            break;
        }
        
        case 3: {  // tjDecodeYUVPlanes
            int width = 16 + (fuzz_data[0] % 64);
            int height = 16 + (fuzz_data[1] % 64);
            int subsamp = get_valid_subsamp(fuzz_data[2]);
            int pixel_format = get_valid_pixel_format(fuzz_data[3]);
            int pitch = width * tjPixelSize[pixel_format];
            
            const unsigned char *src_planes[3];
            int strides[3];
            
            int y_plane_size = width * height;
            int uv_width = tjPlaneWidth(0, width, subsamp);
            int uv_height = tjPlaneHeight(0, height, subsamp);
            int uv_plane_size = uv_width * uv_height;
            
            size_t total_plane_size = y_plane_size + 2 * uv_plane_size;
            if (total_plane_size > fuzz_size) {
                total_plane_size = fuzz_size;
            }
            
            size_t offset = 0;
            for (int i = 0; i < 3; i++) {
                src_planes[i] = fuzz_data + offset;
                strides[i] = (i == 0) ? width : uv_width;
                offset += (i == 0) ? y_plane_size : uv_plane_size;
                if (offset > total_plane_size) offset = total_plane_size;
            }
            
            unsigned char *dst_buf = tjAlloc(width * height * tjPixelSize[pixel_format]);
            if (dst_buf) {
                tjDecodeYUVPlanes(handle, src_planes, strides, subsamp,
                                 dst_buf, width, pitch, height, pixel_format, 0);
                tjFree(dst_buf);
            }
            break;
        }
        
        case 4: {  // tjEncodeYUVPlanes
            int width = 16 + (fuzz_data[0] % 64);
            int height = 16 + (fuzz_data[1] % 64);
            int pixel_format = get_valid_pixel_format(fuzz_data[2]);
            int subsamp = get_valid_subsamp(fuzz_data[3]);
            int pitch = width * tjPixelSize[pixel_format];
            
            size_t src_size = width * height * tjPixelSize[pixel_format];
            if (src_size > fuzz_size) {
                src_size = fuzz_size;
            }
            
            int y_plane_size = tjPlaneSizeYUV(0, width, 1, height, subsamp);
            int u_plane_size = tjPlaneSizeYUV(1, width, 1, height, subsamp);
            int v_plane_size = tjPlaneSizeYUV(2, width, 1, height, subsamp);
            
            unsigned char *y_plane = tjAlloc(y_plane_size);
            unsigned char *u_plane = tjAlloc(u_plane_size);
            unsigned char *v_plane = tjAlloc(v_plane_size);
            unsigned char *dst_planes[3] = {y_plane, u_plane, v_plane};
            int strides[3] = {tjPlaneWidth(0, width, subsamp),
                             tjPlaneWidth(1, width, subsamp),
                             tjPlaneWidth(2, width, subsamp)};
            
            if (y_plane && u_plane && v_plane) {
                tjEncodeYUVPlanes(handle, fuzz_data, width, pitch, height,
                                 pixel_format, dst_planes, strides, subsamp, 0);
            }
            
            if (y_plane) tjFree(y_plane);
            if (u_plane) tjFree(u_plane);
            if (v_plane) tjFree(v_plane);
            break;
        }
        
        case 5: {  // Combined test with multiple functions
            int width = 16 + (fuzz_data[0] % 64);
            int height = 16 + (fuzz_data[1] % 64);
            int align = get_valid_align(fuzz_data[2]);
            int subsamp = get_valid_subsamp(fuzz_data[3]);
            
            size_t yuv_size = tjBufSizeYUV2(width, align, height, subsamp);
            unsigned char *yuv_buf = tjAlloc(yuv_size);
            if (yuv_buf) {
                if (fuzz_size >= yuv_size) {
                    memcpy(yuv_buf, fuzz_data, yuv_size);
                    
                    int pixel_format = get_valid_pixel_format(fuzz_data[4]);
                    int pitch = width * tjPixelSize[pixel_format];
                    unsigned char *rgb_buf = tjAlloc(width * height * tjPixelSize[pixel_format]);
                    
                    if (rgb_buf) {
                        tjDecodeYUV(handle, yuv_buf, align, subsamp, rgb_buf,
                                   width, pitch, height, pixel_format, 0);
                        tjFree(rgb_buf);
                    }
                }
                tjFree(yuv_buf);
            }
            break;
        }
    }
    
    tjDestroy(handle);
    return 0;
}