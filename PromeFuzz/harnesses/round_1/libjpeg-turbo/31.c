// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "src/turbojpeg.h"

static tjhandle init_compressor(void) {
    return tj3Init(TJINIT_COMPRESS);
}

static void cleanup_compressor(tjhandle handle) {
    tj3Destroy(handle);
}

static int get_random_subsamp(const uint8_t *data, size_t size, size_t offset) {
    if (offset >= size) return TJSAMP_444;
    return data[offset] % 5;  // 0-4 for TJSAMP_444, 422, 420, GRAY, 440
}

static int get_random_pixel_format(const uint8_t *data, size_t size, size_t offset) {
    if (offset >= size) return TJPF_RGB;
    return data[offset] % 12;  // 0-11 for various pixel formats
}

static int get_random_quality(const uint8_t *data, size_t size, size_t offset) {
    if (offset >= size) return 85;
    return (data[offset] % 100) + 1;  // 1-100
}

static int get_random_align(const uint8_t *data, size_t size, size_t offset) {
    if (offset >= size) return 1;
    int align = 1 << (data[offset] % 4);  // 1, 2, 4, or 8
    return align;
}

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;  // Need minimum data for parameters

    // Extract parameters from input data
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    width = width % 1024 + 1;   // Limit to reasonable size
    height = height % 1024 + 1;
    
    size_t param_offset = 4;
    int subsamp = get_random_subsamp(Data, Size, param_offset++);
    int pixel_format = get_random_pixel_format(Data, Size, param_offset++);
    int quality = get_random_quality(Data, Size, param_offset++);
    int align = get_random_align(Data, Size, param_offset++);
    int flags = (param_offset < Size) ? Data[param_offset++] : 0;
    
    // Initialize compressor
    tjhandle handle = init_compressor();
    if (!handle) return 0;
    
    // Set parameters
    tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
    
    // Prepare source buffers
    size_t yuv_size = tj3YUVBufSize(width, align, height, subsamp);
    size_t rgb_size = width * height * tjPixelSize[pixel_format];
    
    unsigned char *src_buf = NULL;
    unsigned char *yuv_buf = NULL;
    const unsigned char *src_planes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    
    if (Size > param_offset) {
        size_t available = Size - param_offset;
        
        // Allocate and fill source buffers
        src_buf = (unsigned char *)malloc(rgb_size);
        yuv_buf = (unsigned char *)malloc(yuv_size);
        
        if (src_buf && yuv_buf) {
            // Fill with data from fuzzer input
            size_t copy_size = (available < rgb_size) ? available : rgb_size;
            memcpy(src_buf, Data + param_offset, copy_size);
            if (copy_size < rgb_size) {
                memset(src_buf + copy_size, 0, rgb_size - copy_size);
            }
            
            copy_size = (available < yuv_size) ? available : yuv_size;
            memcpy(yuv_buf, Data + param_offset, copy_size);
            if (copy_size < yuv_size) {
                memset(yuv_buf + copy_size, 0, yuv_size - copy_size);
            }
            
            // Set up planes for planar YUV
            for (int i = 0; i < 3; i++) {
                strides[i] = width;
                src_planes[i] = yuv_buf;
            }
        }
    }
    
    // Prepare JPEG output buffers
    unsigned char *jpeg_buf = NULL;
    size_t jpeg_size = 0;
    unsigned long jpeg_size_old = 0;
    
    // Test tj3CompressFromYUVPlanes8
    if (src_planes[0] && yuv_buf) {
        jpeg_size = tj3JPEGBufSize(width, height, subsamp);
        jpeg_buf = (unsigned char *)tj3Alloc(jpeg_size);
        
        if (jpeg_buf) {
            tj3CompressFromYUVPlanes8(handle, src_planes, width, strides, 
                                     height, &jpeg_buf, &jpeg_size);
            tj3GetErrorCode(handle);
            tj3Free(jpeg_buf);
            jpeg_buf = NULL;
        }
    }
    
    // Test tj3CompressFromYUV8
    if (yuv_buf) {
        jpeg_size = tj3JPEGBufSize(width, height, subsamp);
        jpeg_buf = NULL;  // Let TurboJPEG allocate
        
        tj3CompressFromYUV8(handle, yuv_buf, width, align, height, 
                           &jpeg_buf, &jpeg_size);
        tj3GetErrorCode(handle);
        
        if (jpeg_buf) {
            tj3Free(jpeg_buf);
            jpeg_buf = NULL;
        }
    }
    
    // Test tjCompressFromYUV (old API)
    if (yuv_buf) {
        unsigned char *jpeg_buf_old = NULL;
        jpeg_size_old = 0;
        
        tjCompressFromYUV(handle, yuv_buf, width, align, height, subsamp,
                         &jpeg_buf_old, &jpeg_size_old, quality, flags);
        tjGetErrorCode(handle);
        
        if (jpeg_buf_old) {
            free(jpeg_buf_old);
        }
    }
    
    // Test tjCompress2 (old API)
    if (src_buf) {
        unsigned char *jpeg_buf_old = NULL;
        jpeg_size_old = 0;
        int pitch = width * tjPixelSize[pixel_format];
        
        tjCompress2(handle, src_buf, width, pitch, height, pixel_format,
                   &jpeg_buf_old, &jpeg_size_old, subsamp, quality, flags);
        tjGetErrorCode(handle);
        
        if (jpeg_buf_old) {
            free(jpeg_buf_old);
        }
    }
    
    // Test with dummy file
    create_dummy_file(Data, Size);
    
    // Cleanup
    if (src_buf) free(src_buf);
    if (yuv_buf) free(yuv_buf);
    cleanup_compressor(handle);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}