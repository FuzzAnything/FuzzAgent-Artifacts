// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3DecodeYUVPlanes8 at turbojpeg.c:2508:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static tjhandle create_decompressor() {
    return tj3Init(TJINIT_DECOMPRESS);
}

static tjhandle create_compressor() {
    return tj3Init(TJINIT_COMPRESS);
}

static void destroy_handle(tjhandle handle) {
    if (handle) {
        tj3Destroy(handle);
    }
}

static int get_subsampling_from_data(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset >= size) return TJSAMP_444;
    int subsamp = data[*offset] % 5;
    (*offset)++;
    return subsamp;
}

static int get_pixel_format_from_data(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset >= size) return TJPF_RGB;
    int pixelFormat = data[*offset] % 12;
    (*offset)++;
    return pixelFormat;
}

static int get_align_from_data(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset >= size) return 1;
    int align_pow = data[*offset] % 4;
    (*offset)++;
    return 1 << align_pow;
}

static int get_dimension_from_data(const uint8_t* data, size_t size, size_t* offset, int max_dim) {
    if (*offset + 1 >= size) return 16;
    int dim = (data[*offset] << 8) | data[*offset + 1];
    dim = dim % max_dim + 1;
    *offset += 2;
    return dim;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    size_t data_offset = 0;
    
    int width = get_dimension_from_data(Data, Size, &data_offset, 256);
    int height = get_dimension_from_data(Data, Size, &data_offset, 256);
    int align = get_align_from_data(Data, Size, &data_offset);
    int subsamp = get_subsampling_from_data(Data, Size, &data_offset);
    int pixelFormat = get_pixel_format_from_data(Data, Size, &data_offset);
    
    size_t yuv_buf_size = tj3YUVBufSize(width, align, height, subsamp);
    if (yuv_buf_size == 0) return 0;
    
    unsigned char* src_buf = (unsigned char*)malloc(yuv_buf_size);
    if (!src_buf) return 0;
    
    size_t copy_size = Size - data_offset;
    if (copy_size > yuv_buf_size) copy_size = yuv_buf_size;
    if (copy_size > 0) {
        memcpy(src_buf, Data + data_offset, copy_size);
    }
    if (copy_size < yuv_buf_size) {
        memset(src_buf + copy_size, 0, yuv_buf_size - copy_size);
    }
    
    int dst_pitch = width * tjPixelSize[pixelFormat];
    if (dst_pitch <= 0) {
        free(src_buf);
        return 0;
    }
    
    size_t dst_buf_size = dst_pitch * height;
    unsigned char* dst_buf = (unsigned char*)malloc(dst_buf_size);
    if (!dst_buf) {
        free(src_buf);
        return 0;
    }
    
    tjhandle decompressor = create_decompressor();
    if (decompressor) {
        tj3Set(decompressor, TJPARAM_SUBSAMP, subsamp);
        
        tj3DecodeYUV8(decompressor, src_buf, align, dst_buf, width, 0, height, pixelFormat);
        
        tjhandle old_decompressor = tjInitDecompress();
        if (old_decompressor) {
            tjDecodeYUV(old_decompressor, src_buf, align, subsamp, dst_buf, width, 0, height, pixelFormat, 0);
            tjDestroy(old_decompressor);
        }
        
        size_t y_plane_size = tj3YUVPlaneSize(0, width, align, height, subsamp);
        size_t u_plane_size = tj3YUVPlaneSize(1, width, align, height, subsamp);
        size_t v_plane_size = tj3YUVPlaneSize(2, width, align, height, subsamp);
        
        if (y_plane_size > 0 && u_plane_size > 0 && v_plane_size > 0) {
            unsigned char* y_plane = src_buf;
            unsigned char* u_plane = y_plane + y_plane_size;
            unsigned char* v_plane = u_plane + u_plane_size;
            
            const unsigned char* src_planes[3] = {y_plane, u_plane, v_plane};
            int strides[3] = {0, 0, 0};
            
            if (y_plane_size + u_plane_size + v_plane_size <= yuv_buf_size) {
                tj3DecodeYUVPlanes8(decompressor, src_planes, strides, dst_buf, width, 0, height, pixelFormat);
                
                old_decompressor = tjInitDecompress();
                if (old_decompressor) {
                    tjDecodeYUVPlanes(old_decompressor, src_planes, strides, subsamp, dst_buf, width, 0, height, pixelFormat, 0);
                    tjDestroy(old_decompressor);
                }
            }
        }
        
        destroy_handle(decompressor);
    }
    
    tjhandle compressor = create_compressor();
    if (compressor) {
        unsigned char* y_plane = (unsigned char*)malloc(tj3YUVPlaneSize(0, width, 1, height, subsamp));
        unsigned char* u_plane = (unsigned char*)malloc(tj3YUVPlaneSize(1, width, 1, height, subsamp));
        unsigned char* v_plane = (unsigned char*)malloc(tj3YUVPlaneSize(2, width, 1, height, subsamp));
        
        if (y_plane && u_plane && v_plane) {
            unsigned char* dst_planes[3] = {y_plane, u_plane, v_plane};
            int strides[3] = {0, 0, 0};
            
            tjEncodeYUVPlanes(compressor, dst_buf, width, 0, height, pixelFormat, dst_planes, strides, subsamp, 0);
        }
        
        free(y_plane);
        free(u_plane);
        free(v_plane);
        destroy_handle(compressor);
    }
    
    free(src_buf);
    free(dst_buf);
    
    return 0;
}