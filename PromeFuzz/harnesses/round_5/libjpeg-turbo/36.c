// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    // Use first few bytes to determine parameters
    if (Size < 16) return 0;
    
    // Extract parameters from fuzzer input
    int width = *(const int*)Data;
    int height = *(const int*)(Data + 4);
    int jpegSubsamp = *(const int*)(Data + 8) % 6;  // 0-5 for valid subsampling options
    int jpegQual = *(const int*)(Data + 12) % 101;  // 0-100 for quality
    
    // Test TJBUFSIZE
    unsigned long buf_size1 = TJBUFSIZE(width, height);
    
    // Test tj3JPEGBufSize
    size_t buf_size2 = tj3JPEGBufSize(width, height, jpegSubsamp);
    
    // Test tjBufSize
    unsigned long buf_size3 = tjBufSize(width, height, jpegSubsamp);
    
    // Test tj3Alloc with various sizes
    size_t alloc_size = buf_size2 > 0 ? buf_size2 : 1024;
    void *alloc_buf = tj3Alloc(alloc_size);
    if (alloc_buf) {
        tj3Free(alloc_buf);
    }
    
    // Test tjCompress2 if we have enough data
    if (Size >= 32 && width > 0 && height > 0 && width <= 10000 && height <= 10000) {
        tjhandle handle = tjInitCompress();
        if (handle) {
            int pixelFormat = TJPF_RGB;
            int pitch = width * tjPixelSize[pixelFormat];
            
            // Create source buffer from fuzzer data
            size_t src_size = (size_t)pitch * height;
            unsigned char *src_buf = NULL;
            if (src_size > 0 && src_size <= Size - 16) {
                src_buf = (unsigned char*)malloc(src_size);
                if (src_buf) {
                    size_t copy_size = src_size < (Size - 16) ? src_size : (Size - 16);
                    memcpy(src_buf, Data + 16, copy_size);
                    
                    unsigned char *jpeg_buf = NULL;
                    unsigned long jpeg_size = 0;
                    
                    // Test with auto allocation
                    int result = tjCompress2(handle, src_buf, width, pitch, height, 
                                            pixelFormat, &jpeg_buf, &jpeg_size, 
                                            jpegSubsamp, jpegQual, 0);
                    
                    if (jpeg_buf) {
                        tjFree(jpeg_buf);
                    }
                    
                    // Test with pre-allocated buffer
                    if (buf_size1 > 0 && buf_size1 < 10000000) {
                        jpeg_buf = (unsigned char*)malloc(buf_size1);
                        if (jpeg_buf) {
                            jpeg_size = buf_size1;
                            result = tjCompress2(handle, src_buf, width, pitch, height,
                                                pixelFormat, &jpeg_buf, &jpeg_size,
                                                jpegSubsamp, jpegQual, TJFLAG_NOREALLOC);
                            free(jpeg_buf);
                        }
                    }
                    
                    free(src_buf);
                }
            }
            tjDestroy(handle);
        }
    }
    
    // Test tj3Compress16 if we have enough data
    if (Size >= 48 && width > 0 && height > 0 && width <= 10000 && height <= 10000) {
        tjhandle handle = tj3Init(TJINIT_COMPRESS);
        if (handle) {
            int pixelFormat = TJPF_RGB;
            int pitch = width * tjPixelSize[pixelFormat];
            
            // Create 16-bit source buffer from fuzzer data
            size_t src_size = (size_t)pitch * height * sizeof(unsigned short);
            unsigned short *src_buf16 = NULL;
            if (src_size > 0 && src_size <= Size - 32) {
                src_buf16 = (unsigned short*)malloc(src_size);
                if (src_buf16) {
                    size_t copy_size = src_size < (Size - 32) ? src_size : (Size - 32);
                    memcpy(src_buf16, Data + 32, copy_size);
                    
                    unsigned char *jpeg_buf = NULL;
                    size_t jpeg_size = 0;
                    
                    // Test with auto allocation
                    int result = tj3Compress16(handle, src_buf16, width, pitch, height,
                                              pixelFormat, &jpeg_buf, &jpeg_size);
                    
                    if (jpeg_buf) {
                        tj3Free(jpeg_buf);
                    }
                    
                    // Test with pre-allocated buffer
                    if (buf_size2 > 0 && buf_size2 < 10000000) {
                        jpeg_buf = (unsigned char*)tj3Alloc(buf_size2);
                        if (jpeg_buf) {
                            jpeg_size = buf_size2;
                            result = tj3Compress16(handle, src_buf16, width, pitch, height,
                                                  pixelFormat, &jpeg_buf, &jpeg_size);
                            tj3Free(jpeg_buf);
                        }
                    }
                    
                    free(src_buf16);
                }
            }
            tj3Destroy(handle);
        }
    }
    
    return 0;
}