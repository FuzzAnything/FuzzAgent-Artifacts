// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjCompressFromYUVPlanes at turbojpeg.c:1394:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const int MAX_DIMENSION = 10000;
static const int MAX_ALIGN = 256;
static const int MAX_SUBSAMP = 5;

static int get_valid_dimension(int value) {
    if (value < 1) return 1;
    if (value > MAX_DIMENSION) return MAX_DIMENSION;
    return value;
}

static int get_valid_align(int value) {
    if (value < 1) return 1;
    if (value > MAX_ALIGN) return MAX_ALIGN;
    // Ensure power of 2
    int align = 1;
    while (align < value) align <<= 1;
    if (align > MAX_ALIGN) align = MAX_ALIGN;
    return align;
}

static int get_valid_subsamp(int value) {
    if (value < 0) return 0;
    if (value > MAX_SUBSAMP) return MAX_SUBSAMP;
    return value;
}

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *data, size_t size) {
    if (size < 16) return 0;
    
    // Extract parameters from input
    int width = get_valid_dimension(*(int*)data);
    int height = get_valid_dimension(*(int*)(data + 4));
    int align = get_valid_align(*(int*)(data + 8));
    int subsamp = get_valid_subsamp(*(int*)(data + 12));
    
    // Test tj3YUVBufSize
    size_t yuv_size = tj3YUVBufSize(width, align, height, subsamp);
    
    // Test tj3JPEGBufSize
    size_t jpeg_buf_size = tj3JPEGBufSize(width, height, subsamp);
    
    // Initialize TurboJPEG instance for compression
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return 0;
    
    // Test tj3CompressFromYUV8
    if (yuv_size > 0 && yuv_size <= 1000000) { // Limit to reasonable size
        unsigned char *yuv_buf = (unsigned char*)malloc(yuv_size);
        if (yuv_buf) {
            // Fill buffer with fuzzer data (repeat if needed)
            size_t data_offset = 16;
            for (size_t i = 0; i < yuv_size; i++) {
                yuv_buf[i] = data[(data_offset + i) % size];
            }
            
            unsigned char *jpeg_buf = NULL;
            size_t jpeg_size = 0;
            if (jpeg_buf_size > 0) {
                jpeg_buf = (unsigned char*)tj3Alloc(jpeg_buf_size);
                jpeg_size = jpeg_buf_size;
            }
            
            int result = tj3CompressFromYUV8(handle, yuv_buf, width, align, height, 
                                           &jpeg_buf, &jpeg_size);
            
            if (jpeg_buf) tj3Free(jpeg_buf);
            free(yuv_buf);
        }
    }
    
    // Test tj3CompressFromYUVPlanes8
    if (yuv_size > 0 && yuv_size <= 1000000) {
        // Create plane pointers
        const unsigned char *planes[3];
        unsigned char *plane_buf = (unsigned char*)malloc(yuv_size);
        if (plane_buf) {
            // Fill buffer with fuzzer data
            size_t data_offset = 16;
            for (size_t i = 0; i < yuv_size; i++) {
                plane_buf[i] = data[(data_offset + i) % size];
            }
            
            // Split into planes (simplified - actual layout depends on subsampling)
            planes[0] = plane_buf;
            planes[1] = plane_buf + (yuv_size / 3);
            planes[2] = plane_buf + (2 * yuv_size / 3);
            
            int strides[3] = {0, 0, 0}; // Let library calculate strides
            
            unsigned char *jpeg_buf = NULL;
            size_t jpeg_size = 0;
            if (jpeg_buf_size > 0) {
                jpeg_buf = (unsigned char*)tj3Alloc(jpeg_buf_size);
                jpeg_size = jpeg_buf_size;
            }
            
            int result = tj3CompressFromYUVPlanes8(handle, planes, width, strides, height,
                                                  &jpeg_buf, &jpeg_size);
            
            if (jpeg_buf) tj3Free(jpeg_buf);
            free(plane_buf);
        }
    }
    
    // Test legacy functions if enough data
    if (size >= 32) {
        int jpeg_qual = *(int*)(data + 16) % 101; // 0-100
        int flags = *(int*)(data + 20) & 0xFF;
        
        // Test tjCompressFromYUV
        if (yuv_size > 0 && yuv_size <= 1000000) {
            unsigned char *yuv_buf = (unsigned char*)malloc(yuv_size);
            if (yuv_buf) {
                size_t data_offset = 24;
                for (size_t i = 0; i < yuv_size; i++) {
                    yuv_buf[i] = data[(data_offset + i) % size];
                }
                
                unsigned char *jpeg_buf = NULL;
                unsigned long jpeg_size = 0;
                
                int result = tjCompressFromYUV(handle, yuv_buf, width, align, height,
                                             subsamp, &jpeg_buf, &jpeg_size, jpeg_qual, flags);
                
                if (jpeg_buf) free(jpeg_buf);
                free(yuv_buf);
            }
        }
        
        // Test tjCompressFromYUVPlanes
        if (yuv_size > 0 && yuv_size <= 1000000) {
            unsigned char *plane_buf = (unsigned char*)malloc(yuv_size);
            if (plane_buf) {
                size_t data_offset = 24;
                for (size_t i = 0; i < yuv_size; i++) {
                    plane_buf[i] = data[(data_offset + i) % size];
                }
                
                const unsigned char *planes[3];
                planes[0] = plane_buf;
                planes[1] = plane_buf + (yuv_size / 3);
                planes[2] = plane_buf + (2 * yuv_size / 3);
                
                int strides[3] = {0, 0, 0};
                
                unsigned char *jpeg_buf = NULL;
                unsigned long jpeg_size = 0;
                
                int result = tjCompressFromYUVPlanes(handle, planes, width, strides, height,
                                                   subsamp, &jpeg_buf, &jpeg_size, jpeg_qual, flags);
                
                if (jpeg_buf) free(jpeg_buf);
                free(plane_buf);
            }
        }
    }
    
    // Write dummy file for any file-based operations
    write_dummy_file(data, size > 1024 ? 1024 : size);
    
    // Cleanup
    tj3Destroy(handle);
    
    return 0;
}