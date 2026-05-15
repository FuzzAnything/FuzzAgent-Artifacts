// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "src/turbojpeg.h"

static void cleanup_jpeg_buffer(unsigned char **jpegBuf, size_t *jpegSize) {
    if (jpegBuf && *jpegBuf) {
        tj3Free(*jpegBuf);
        *jpegBuf = NULL;
    }
    if (jpegSize) {
        *jpegSize = 0;
    }
}

static int extract_int(const uint8_t *data, size_t size, size_t *offset, int min_val, int max_val) {
    if (*offset + sizeof(int) > size) {
        return min_val;
    }
    int val = *(int*)(data + *offset);
    *offset += sizeof(int);
    
    if (val < min_val) return min_val;
    if (val > max_val) return max_val;
    return val;
}

static unsigned char extract_byte(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) {
        return 0;
    }
    return data[(*offset)++];
}

static int is_power_of_two(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    size_t offset = 0;
    
    // Extract parameters with bounds checking
    int width = extract_int(Data, Size, &offset, 1, 4096);
    int height = extract_int(Data, Size, &offset, 1, 4096);
    int align = extract_int(Data, Size, &offset, 1, 256);
    int jpegSubsamp = extract_int(Data, Size, &offset, 0, 4);
    int pixelFormat = extract_int(Data, Size, &offset, 0, 11);
    int jpegQual = extract_int(Data, Size, &offset, 0, 100);
    int flags = extract_int(Data, Size, &offset, 0, 15);
    
    // Ensure align is power of 2
    if (!is_power_of_two(align)) {
        align = 1;
    }
    
    // 1. Test TJBUFSIZE
    unsigned long buf_size1 = TJBUFSIZE(width, height);
    
    // 2. Test tj3JPEGBufSize
    size_t buf_size2 = tj3JPEGBufSize(width, height, jpegSubsamp);
    
    // 3. Test tjBufSize
    unsigned long buf_size3 = tjBufSize(width, height, jpegSubsamp);
    
    // Create compressor handle
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) {
        return 0;
    }
    
    // Set some parameters
    tj3Set(handle, TJPARAM_SUBSAMP, jpegSubsamp);
    tj3Set(handle, TJPARAM_QUALITY, jpegQual);
    
    // Test tj3CompressFromYUV8
    size_t yuv_size = tj3YUVBufSize(width, align, height, jpegSubsamp);
    if (yuv_size > 0 && yuv_size <= 100 * 1024 * 1024) { // Limit to 100MB
        unsigned char *yuv_buf = (unsigned char *)malloc(yuv_size);
        if (yuv_buf) {
            // Fill with fuzzer data
            size_t copy_size = Size - offset;
            if (copy_size > yuv_size) copy_size = yuv_size;
            if (copy_size > 0) {
                memcpy(yuv_buf, Data + offset, copy_size);
            }
            // Pad with zeros if needed
            if (copy_size < yuv_size) {
                memset(yuv_buf + copy_size, 0, yuv_size - copy_size);
            }
            
            unsigned char *jpeg_buf1 = NULL;
            size_t jpeg_size1 = 0;
            
            int result1 = tj3CompressFromYUV8(handle, yuv_buf, width, align, height, 
                                             &jpeg_buf1, &jpeg_size1);
            
            cleanup_jpeg_buffer(&jpeg_buf1, &jpeg_size1);
            free(yuv_buf);
        }
    }
    
    // Test tjCompress2
    int pitch = width * tjPixelSize[pixelFormat];
    size_t src_size = (size_t)pitch * height;
    if (src_size > 0 && src_size <= 100 * 1024 * 1024) { // Limit to 100MB
        unsigned char *src_buf = (unsigned char *)malloc(src_size);
        if (src_buf) {
            // Fill with fuzzer data
            size_t copy_size = Size - offset;
            if (copy_size > src_size) copy_size = src_size;
            if (copy_size > 0) {
                memcpy(src_buf, Data + offset, copy_size);
            }
            // Pad with zeros if needed
            if (copy_size < src_size) {
                memset(src_buf + copy_size, 0, src_size - copy_size);
            }
            
            unsigned char *jpeg_buf2 = NULL;
            unsigned long jpeg_size2 = 0;
            
            int result2 = tjCompress2(handle, src_buf, width, pitch, height, pixelFormat,
                                     &jpeg_buf2, &jpeg_size2, jpegSubsamp, jpegQual, flags);
            
            if (jpeg_buf2) {
                tjFree(jpeg_buf2);
            }
            free(src_buf);
        }
    }
    
    // Test tjCompress
    int pixelSize = tjPixelSize[pixelFormat];
    unsigned long comp_size = buf_size3 > 0 ? buf_size3 : 1024;
    if (comp_size <= 100 * 1024 * 1024) { // Limit to 100MB
        unsigned char *src_buf2 = (unsigned char *)malloc(width * height * pixelSize);
        unsigned char *dst_buf = (unsigned char *)malloc(comp_size);
        
        if (src_buf2 && dst_buf) {
            // Fill with fuzzer data
            size_t copy_size = Size - offset;
            size_t max_copy = (size_t)width * height * pixelSize;
            if (copy_size > max_copy) copy_size = max_copy;
            if (copy_size > 0) {
                memcpy(src_buf2, Data + offset, copy_size);
            }
            
            unsigned long compressed_size = comp_size;
            int result3 = tjCompress(handle, src_buf2, width, width * pixelSize, height,
                                    pixelSize, dst_buf, &compressed_size, 
                                    jpegSubsamp, jpegQual, flags);
        }
        
        if (src_buf2) free(src_buf2);
        if (dst_buf) free(dst_buf);
    }
    
    // Cleanup
    tj3Destroy(handle);
    
    return 0;
}