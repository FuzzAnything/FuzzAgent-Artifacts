// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tj3DecodeYUVPlanes8 at turbojpeg.c:2508:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
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

#define MIN_DIMENSION 1
#define MAX_DIMENSION 256
#define MAX_PLANES 3
#define DUMMY_FILE "./dummy_file"

static void cleanup_handle(tjhandle *handle) {
    if (*handle) {
        tjDestroy(*handle);
        *handle = NULL;
    }
}

static int get_valid_dimension(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 1 > size) return MIN_DIMENSION;
    int dim = MIN_DIMENSION + (data[*offset] % (MAX_DIMENSION - MIN_DIMENSION + 1));
    (*offset)++;
    return dim;
}

static int get_valid_pixel_format(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 1 > size) return TJPF_RGB;
    int pf = data[*offset] % (TJPF_CMYK + 1);
    (*offset)++;
    return pf;
}

static int get_valid_subsamp(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 1 > size) return TJSAMP_444;
    int subsamp = data[*offset] % (TJSAMP_GRAY + 1);
    (*offset)++;
    return subsamp;
}

static int get_valid_align(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 1 > size) return 1;
    int align = 1 << (data[*offset] % 4); // 1, 2, 4, 8
    (*offset)++;
    return align;
}

static int get_valid_strides(const uint8_t *data, size_t *offset, size_t size, int *strides, int num_planes, int width, int subsamp) {
    for (int i = 0; i < num_planes; i++) {
        if (*offset + 1 > size) {
            strides[i] = 0; // Let library compute default
        } else {
            strides[i] = (data[*offset] % 32) * 4; // Multiple of 4 up to 124
            (*offset)++;
        }
    }
    return 0;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    size_t offset = 0;
    int width = get_valid_dimension(Data, &offset, Size);
    int height = get_valid_dimension(Data, &offset, Size);
    int pixel_format = get_valid_pixel_format(Data, &offset, Size);
    int subsamp = get_valid_subsamp(Data, &offset, Size);
    int align = get_valid_align(Data, &offset, Size);
    
    // Ensure valid dimensions for subsampling
    if (subsamp == TJSAMP_420 || subsamp == TJSAMP_422 || subsamp == TJSAMP_440) {
        if (width % 2 != 0) width++;
        if (height % 2 != 0 && subsamp == TJSAMP_420) height++;
    }
    if (subsamp == TJSAMP_411) {
        if (width % 4 != 0) width += (4 - (width % 4));
    }
    
    // Create handles
    tjhandle comp_handle = tjInitCompress();
    tjhandle decomp_handle = tjInitDecompress();
    tjhandle decomp_handle3 = tj3Init(TJINIT_DECOMPRESS);
    
    if (!comp_handle || !decomp_handle || !decomp_handle3) {
        cleanup_handle(&comp_handle);
        cleanup_handle(&decomp_handle);
        if (decomp_handle3) tj3Destroy(decomp_handle3);
        return 0;
    }
    
    // Set parameters for tj3 functions
    tj3Set(decomp_handle3, TJPARAM_SUBSAMP, subsamp);
    
    // Allocate source and destination buffers
    int src_size = width * height * tjPixelSize[pixel_format];
    if (src_size <= 0) src_size = width * height * 4;
    unsigned char *src_buf = (unsigned char *)malloc(src_size);
    if (!src_buf) goto cleanup;
    
    // Fill source buffer with fuzzer data
    size_t copy_size = Size - offset;
    if (copy_size > (size_t)src_size) copy_size = src_size;
    if (copy_size > 0) {
        memcpy(src_buf, Data + offset, copy_size);
    }
    
    // Test tjEncodeYUVPlanes
    {
        int strides[MAX_PLANES];
        get_valid_strides(Data, &offset, Size, strides, MAX_PLANES, width, subsamp);
        
        unsigned char *dst_planes[MAX_PLANES];
        int plane_sizes[MAX_PLANES];
        for (int i = 0; i < MAX_PLANES; i++) {
            plane_sizes[i] = tjPlaneSizeYUV(i, width, strides[i], height, subsamp);
            if (plane_sizes[i] > 0) {
                dst_planes[i] = (unsigned char *)malloc(plane_sizes[i]);
                if (dst_planes[i]) memset(dst_planes[i], 0, plane_sizes[i]);
            } else {
                dst_planes[i] = NULL;
            }
        }
        
        tjEncodeYUVPlanes(comp_handle, src_buf, width, width * tjPixelSize[pixel_format],
                         height, pixel_format, dst_planes, strides, subsamp, 0);
        
        for (int i = 0; i < MAX_PLANES; i++) {
            free(dst_planes[i]);
        }
    }
    
    // Test tj3DecodeYUV8
    {
        unsigned long yuv_size = tj3YUVBufSize(width, align, height, subsamp);
        if (yuv_size > 0) {
            unsigned char *yuv_buf = (unsigned char *)malloc(yuv_size);
            if (yuv_buf) {
                // Fill with some data
                size_t yuv_copy = Size - (offset % Size);
                if (yuv_copy > yuv_size) yuv_copy = yuv_size;
                if (yuv_copy > 0) {
                    memcpy(yuv_buf, Data + (offset % Size), yuv_copy);
                }
                
                int dst_pitch = width * tjPixelSize[pixel_format];
                unsigned char *dst_buf = (unsigned char *)malloc(dst_pitch * height);
                if (dst_buf) {
                    tj3DecodeYUV8(decomp_handle3, yuv_buf, align, dst_buf, 
                                 width, dst_pitch, height, pixel_format);
                    free(dst_buf);
                }
                free(yuv_buf);
            }
        }
    }
    
    // Test tjDecompressToYUVPlanes with dummy JPEG file
    {
        FILE *fp = fopen(DUMMY_FILE, "wb");
        if (fp) {
            size_t write_size = Size - (offset % Size);
            if (write_size > 4096) write_size = 4096;
            fwrite(Data + (offset % Size), 1, write_size, fp);
            fclose(fp);
            
            fp = fopen(DUMMY_FILE, "rb");
            if (fp) {
                fseek(fp, 0, SEEK_END);
                unsigned long jpeg_size = ftell(fp);
                fseek(fp, 0, SEEK_SET);
                
                unsigned char *jpeg_buf = (unsigned char *)malloc(jpeg_size);
                if (jpeg_buf) {
                    fread(jpeg_buf, 1, jpeg_size, fp);
                    
                    int strides[MAX_PLANES];
                    get_valid_strides(Data, &offset, Size, strides, MAX_PLANES, width, subsamp);
                    
                    unsigned char *dst_planes[MAX_PLANES];
                    for (int i = 0; i < MAX_PLANES; i++) {
                        int plane_width = (i == 0) ? width : tjPlaneWidth(i, width, subsamp);
                        int plane_height = (i == 0) ? height : tjPlaneHeight(i, height, subsamp);
                        int plane_stride = strides[i] > 0 ? strides[i] : plane_width;
                        dst_planes[i] = (unsigned char *)malloc(plane_stride * plane_height);
                        if (dst_planes[i]) memset(dst_planes[i], 0, plane_stride * plane_height);
                    }
                    
                    tjDecompressToYUVPlanes(decomp_handle, jpeg_buf, jpeg_size,
                                           dst_planes, width, strides, height, 0);
                    
                    for (int i = 0; i < MAX_PLANES; i++) {
                        free(dst_planes[i]);
                    }
                    free(jpeg_buf);
                }
                fclose(fp);
                remove(DUMMY_FILE);
            }
        }
    }
    
    // Test tjDecodeYUV
    {
        unsigned long yuv_size = tjBufSizeYUV(width, height, subsamp);
        if (yuv_size > 0) {
            unsigned char *yuv_buf = (unsigned char *)malloc(yuv_size);
            if (yuv_buf) {
                size_t yuv_copy = Size - (offset % Size);
                if (yuv_copy > yuv_size) yuv_copy = yuv_size;
                if (yuv_copy > 0) {
                    memcpy(yuv_buf, Data + (offset % Size), yuv_copy);
                }
                
                int dst_pitch = width * tjPixelSize[pixel_format];
                unsigned char *dst_buf = (unsigned char *)malloc(dst_pitch * height);
                if (dst_buf) {
                    tjDecodeYUV(decomp_handle, yuv_buf, align, subsamp, dst_buf,
                               width, dst_pitch, height, pixel_format, 0);
                    free(dst_buf);
                }
                free(yuv_buf);
            }
        }
    }
    
    // Test tjDecodeYUVPlanes
    {
        const unsigned char *src_planes[MAX_PLANES];
        int strides[MAX_PLANES];
        get_valid_strides(Data, &offset, Size, strides, MAX_PLANES, width, subsamp);
        
        // Allocate and fill planes
        for (int i = 0; i < MAX_PLANES; i++) {
            int plane_width = (i == 0) ? width : tjPlaneWidth(i, width, subsamp);
            int plane_height = (i == 0) ? height : tjPlaneHeight(i, height, subsamp);
            int plane_stride = strides[i] > 0 ? strides[i] : plane_width;
            unsigned char *plane = (unsigned char *)malloc(plane_stride * plane_height);
            if (plane) {
                size_t plane_copy = Size - (offset % Size);
                if (plane_copy > (size_t)(plane_stride * plane_height)) {
                    plane_copy = plane_stride * plane_height;
                }
                if (plane_copy > 0) {
                    memcpy(plane, Data + (offset % Size), plane_copy);
                }
                src_planes[i] = plane;
            } else {
                src_planes[i] = NULL;
            }
        }
        
        int dst_pitch = width * tjPixelSize[pixel_format];
        unsigned char *dst_buf = (unsigned char *)malloc(dst_pitch * height);
        if (dst_buf) {
            tjDecodeYUVPlanes(decomp_handle, src_planes, strides, subsamp, dst_buf,
                             width, dst_pitch, height, pixel_format, 0);
            free(dst_buf);
        }
        
        for (int i = 0; i < MAX_PLANES; i++) {
            free((void *)src_planes[i]);
        }
    }
    
    // Test tj3DecodeYUVPlanes8
    {
        const unsigned char *src_planes[MAX_PLANES];
        int strides[MAX_PLANES];
        get_valid_strides(Data, &offset, Size, strides, MAX_PLANES, width, subsamp);
        
        // Allocate and fill planes
        for (int i = 0; i < MAX_PLANES; i++) {
            int plane_width = (i == 0) ? width : tjPlaneWidth(i, width, subsamp);
            int plane_height = (i == 0) ? height : tjPlaneHeight(i, height, subsamp);
            int plane_stride = strides[i] > 0 ? strides[i] : plane_width;
            unsigned char *plane = (unsigned char *)malloc(plane_stride * plane_height);
            if (plane) {
                size_t plane_copy = Size - (offset % Size);
                if (plane_copy > (size_t)(plane_stride * plane_height)) {
                    plane_copy = plane_stride * plane_height;
                }
                if (plane_copy > 0) {
                    memcpy(plane, Data + (offset % Size), plane_copy);
                }
                src_planes[i] = plane;
            } else {
                src_planes[i] = NULL;
            }
        }
        
        int dst_pitch = width * tjPixelSize[pixel_format];
        unsigned char *dst_buf = (unsigned char *)malloc(dst_pitch * height);
        if (dst_buf) {
            tj3DecodeYUVPlanes8(decomp_handle3, src_planes, strides, dst_buf,
                               width, dst_pitch, height, pixel_format);
            free(dst_buf);
        }
        
        for (int i = 0; i < MAX_PLANES; i++) {
            free((void *)src_planes[i]);
        }
    }
    
cleanup:
    free(src_buf);
    cleanup_handle(&comp_handle);
    cleanup_handle(&decomp_handle);
    if (decomp_handle3) tj3Destroy(decomp_handle3);
    
    return 0;
}