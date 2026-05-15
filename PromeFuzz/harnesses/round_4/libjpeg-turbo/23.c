// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "turbojpeg.h"

static void init_handle_and_buffers(tjhandle *handle, unsigned char **jpeg_buf, 
                                   size_t *jpeg_size, unsigned char **src_buf,
                                   size_t src_size, int width, int height,
                                   int align, int subsamp) {
    *handle = tj3Init(TJINIT_COMPRESS);
    if (!*handle) return;
    
    // Allocate source buffer if needed
    if (src_buf && *src_buf == NULL) {
        size_t yuv_size = tj3YUVBufSize(width, align, height, subsamp);
        if (yuv_size > 0 && yuv_size <= src_size) {
            *src_buf = (unsigned char *)malloc(yuv_size);
            if (*src_buf) {
                // Fill with some pattern
                for (size_t i = 0; i < yuv_size; i++) {
                    (*src_buf)[i] = (unsigned char)(i % 256);
                }
            }
        }
    }
    
    // Allocate JPEG buffer
    if (jpeg_buf && *jpeg_buf == NULL) {
        *jpeg_size = tj3JPEGBufSize(width, height, subsamp);
        if (*jpeg_size > 0) {
            *jpeg_buf = (unsigned char *)malloc(*jpeg_size);
        }
    }
}

static void cleanup(tjhandle handle, unsigned char *jpeg_buf, 
                   unsigned char *src_buf) {
    if (jpeg_buf) free(jpeg_buf);
    if (src_buf) free(src_buf);
    if (handle) tj3Destroy(handle);
}

static void fuzz_tj3CompressFromYUV8(const uint8_t *data, size_t size) {
    if (size < 16) return;
    
    int width = (data[0] << 8) | data[1];
    int height = (data[2] << 8) | data[3];
    int align = 1 << (data[4] % 4); // 1, 2, 4, or 8
    int subsamp = data[5] % 5; // 0-4 for subsampling options
    
    // Limit dimensions to avoid excessive memory usage
    if (width <= 0 || width > 1024) width = 64;
    if (height <= 0 || height > 1024) height = 64;
    if (align < 1 || align > 8) align = 1;
    
    tjhandle handle = NULL;
    unsigned char *src_buf = NULL;
    unsigned char *jpeg_buf = NULL;
    size_t jpeg_size = 0;
    
    init_handle_and_buffers(&handle, &jpeg_buf, &jpeg_size, &src_buf, 
                           size, width, height, align, subsamp);
    
    if (handle && src_buf && jpeg_buf) {
        // Set parameters
        tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
        tj3Set(handle, TJPARAM_QUALITY, 75);
        
        // Call target function
        int result = tj3CompressFromYUV8(handle, src_buf, width, align, 
                                        height, &jpeg_buf, &jpeg_size);
        
        // Check error code
        if (result == -1) {
            tj3GetErrorCode(handle);
        }
    }
    
    cleanup(handle, jpeg_buf, src_buf);
}

static void fuzz_tjCompressFromYUV(const uint8_t *data, size_t size) {
    if (size < 16) return;
    
    int width = (data[0] << 8) | data[1];
    int height = (data[2] << 8) | data[3];
    int align = 1 << (data[4] % 4);
    int subsamp = data[5] % 5;
    int jpeg_qual = (data[6] % 100) + 1;
    int flags = data[7] & 1 ? TJFLAG_NOREALLOC : 0;
    
    if (width <= 0 || width > 1024) width = 64;
    if (height <= 0 || height > 1024) height = 64;
    if (align < 1 || align > 8) align = 1;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return;
    
    size_t yuv_size = tjBufSizeYUV2(width, align, height, subsamp);
    if (yuv_size == 0 || yuv_size > size) {
        tjDestroy(handle);
        return;
    }
    
    unsigned char *src_buf = (unsigned char *)malloc(yuv_size);
    unsigned char *jpeg_buf = NULL;
    unsigned long jpeg_size = 0;
    
    if (src_buf) {
        // Fill with data from fuzzer input
        size_t copy_size = yuv_size < size ? yuv_size : size;
        memcpy(src_buf, data, copy_size);
        
        // Call target function
        int result = tjCompressFromYUV(handle, src_buf, width, align, 
                                      height, subsamp, &jpeg_buf, 
                                      &jpeg_size, jpeg_qual, flags);
        
        // Check error code
        if (result == -1) {
            tjGetErrorCode(handle);
        }
        
        if (jpeg_buf) tjFree(jpeg_buf);
        free(src_buf);
    }
    
    tjDestroy(handle);
}

static void fuzz_tjCompress2(const uint8_t *data, size_t size) {
    if (size < 16) return;
    
    int width = (data[0] << 8) | data[1];
    int height = (data[2] << 8) | data[3];
    int pitch = width * 3; // Assuming RGB
    int pixel_format = data[4] % 12; // 0-11 for pixel formats
    int jpeg_subsamp = data[5] % 5;
    int jpeg_qual = (data[6] % 100) + 1;
    int flags = data[7] & 1 ? TJFLAG_NOREALLOC : 0;
    
    if (width <= 0 || width > 1024) width = 64;
    if (height <= 0 || height > 1024) height = 64;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return;
    
    size_t src_size = pitch * height;
    if (src_size == 0 || src_size > size) {
        tjDestroy(handle);
        return;
    }
    
    unsigned char *src_buf = (unsigned char *)malloc(src_size);
    unsigned char *jpeg_buf = NULL;
    unsigned long jpeg_size = 0;
    
    if (src_buf) {
        size_t copy_size = src_size < size ? src_size : size;
        memcpy(src_buf, data, copy_size);
        
        // Call target function
        int result = tjCompress2(handle, src_buf, width, pitch, height,
                                pixel_format, &jpeg_buf, &jpeg_size,
                                jpeg_subsamp, jpeg_qual, flags);
        
        // Check error code
        if (result == -1) {
            tjGetErrorCode(handle);
        }
        
        if (jpeg_buf) tjFree(jpeg_buf);
        free(src_buf);
    }
    
    tjDestroy(handle);
}

static void fuzz_tj3CompressFromYUVPlanes8(const uint8_t *data, size_t size) {
    if (size < 16) return;
    
    int width = (data[0] << 8) | data[1];
    int height = (data[2] << 8) | data[3];
    int subsamp = data[4] % 5;
    
    if (width <= 0 || width > 1024) width = 64;
    if (height <= 0 || height > 1024) height = 64;
    
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return;
    
    // Set parameters
    tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(handle, TJPARAM_QUALITY, 75);
    
    // Calculate plane sizes
    size_t y_plane_size = tj3YUVPlaneSize(width, 0, height, 0, subsamp);
    size_t u_plane_size = tj3YUVPlaneSize(width, 1, height, 1, subsamp);
    size_t v_plane_size = tj3YUVPlaneSize(width, 2, height, 2, subsamp);
    
    if (y_plane_size == 0 || u_plane_size == 0 || v_plane_size == 0 ||
        y_plane_size > size/3 || u_plane_size > size/3 || v_plane_size > size/3) {
        tj3Destroy(handle);
        return;
    }
    
    unsigned char *y_plane = (unsigned char *)malloc(y_plane_size);
    unsigned char *u_plane = (unsigned char *)malloc(u_plane_size);
    unsigned char *v_plane = (unsigned char *)malloc(v_plane_size);
    const unsigned char *src_planes[3] = {y_plane, u_plane, v_plane};
    
    int strides[3] = {0, 0, 0}; // Let library calculate strides
    
    unsigned char *jpeg_buf = NULL;
    size_t jpeg_size = tj3JPEGBufSize(width, height, subsamp);
    
    if (y_plane && u_plane && v_plane && jpeg_size > 0) {
        jpeg_buf = (unsigned char *)malloc(jpeg_size);
        
        if (jpeg_buf) {
            // Fill planes with data
            size_t offset = 0;
            for (size_t i = 0; i < y_plane_size && offset < size; i++, offset++) {
                y_plane[i] = data[offset % size];
            }
            for (size_t i = 0; i < u_plane_size && offset < size; i++, offset++) {
                u_plane[i] = data[offset % size];
            }
            for (size_t i = 0; i < v_plane_size && offset < size; i++, offset++) {
                v_plane[i] = data[offset % size];
            }
            
            // Call target function
            int result = tj3CompressFromYUVPlanes8(handle, src_planes, width,
                                                  strides, height, &jpeg_buf,
                                                  &jpeg_size);
            
            // Check error code
            if (result == -1) {
                tj3GetErrorCode(handle);
            }
            
            free(jpeg_buf);
        }
    }
    
    if (y_plane) free(y_plane);
    if (u_plane) free(u_plane);
    if (v_plane) free(v_plane);
    tj3Destroy(handle);
}

int LLVMFuzzerTestOneInput_23(const uint8_t *data, size_t size) {
    // Use different parts of the input for different functions
    // to explore more states
    
    // First function
    fuzz_tj3CompressFromYUV8(data, size);
    
    // Second function with offset data
    if (size > 100) {
        fuzz_tjCompressFromYUV(data + 50, size - 50);
    }
    
    // Third function with different offset
    if (size > 200) {
        fuzz_tjCompress2(data + 100, size - 100);
    }
    
    // Fourth function with another offset
    if (size > 300) {
        fuzz_tj3CompressFromYUVPlanes8(data + 200, size - 200);
    }
    
    // Also test with the entire data for each function
    fuzz_tj3CompressFromYUV8(data, size);
    fuzz_tjCompressFromYUV(data, size);
    fuzz_tjCompress2(data, size);
    fuzz_tj3CompressFromYUVPlanes8(data, size);
    
    return 0;
}