// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "src/turbojpeg.h"

static void fuzz_tj3Alloc(size_t bytes) {
    // Cap allocation size to avoid excessive memory usage during fuzzing
    if (bytes > 1024 * 1024) bytes = 1024 * 1024;
    
    void *buffer = tj3Alloc(bytes);
    if (buffer != NULL) {
        // Try to write to the allocated memory to ensure it's usable
        if (bytes > 0) {
            memset(buffer, 0, bytes > 1024 ? 1024 : bytes);
        }
        tj3Free(buffer);
    }
}

static void fuzz_tj3JPEGBufSize(const uint8_t *data, size_t size) {
    if (size < 8) return;
    
    int width = (data[0] << 8) | data[1];
    int height = (data[2] << 8) | data[3];
    int jpegSubsamp = data[4] % 6;  // TJSAMP values: 0-5
    
    // Test with various width/height values
    size_t buf_size = tj3JPEGBufSize(width, height, jpegSubsamp);
    
    // Also test with extreme values
    tj3JPEGBufSize(0, 0, jpegSubsamp);
    tj3JPEGBufSize(-1, -1, jpegSubsamp);
    tj3JPEGBufSize(32767, 32767, jpegSubsamp);
}

static void fuzz_tj3YUVBufSize(const uint8_t *data, size_t size) {
    if (size < 8) return;
    
    int width = (data[0] << 8) | data[1];
    int align = 1 << (data[2] % 5);  // Power of 2: 1, 2, 4, 8, 16
    int height = (data[3] << 8) | data[4];
    int subsamp = data[5] % 6;  // TJSAMP values: 0-5
    
    size_t yuv_size = tj3YUVBufSize(width, align, height, subsamp);
    
    // Test edge cases
    tj3YUVBufSize(0, 1, 0, subsamp);
    tj3YUVBufSize(width, 0, height, subsamp);  // Invalid align
    tj3YUVBufSize(32767, 1, 32767, subsamp);
}

static void fuzz_tj3EncodeYUV8(const uint8_t *data, size_t size) {
    if (size < 32) return;
    
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return;
    
    // Extract parameters from fuzz data
    int width = (data[0] << 8) | data[1];
    if (width <= 0) width = 64;
    int height = (data[2] << 8) | data[3];
    if (height <= 0) height = 64;
    int pitch = width * 3;  // For TJPF_RGB
    int pixelFormat = data[4] % 12;  // TJPF values: 0-11
    int align = 1 << (data[5] % 5);  // Power of 2: 1, 2, 4, 8, 16
    
    // Set subsampling parameter
    int subsamp = data[6] % 6;
    tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
    
    // Calculate required buffer sizes
    size_t src_size = pitch * height;
    size_t dst_size = tj3YUVBufSize(width, align, height, subsamp);
    
    if (src_size == 0 || dst_size == 0) {
        tj3Destroy(handle);
        return;
    }
    
    // Cap buffer sizes to avoid excessive memory usage
    if (src_size > 1024 * 1024) src_size = 1024 * 1024;
    if (dst_size > 1024 * 1024) dst_size = 1024 * 1024;
    
    // Allocate buffers
    unsigned char *srcBuf = NULL;
    unsigned char *dstBuf = NULL;
    
    if (size >= 32) {
        srcBuf = (unsigned char *)tj3Alloc(src_size);
        dstBuf = (unsigned char *)tj3Alloc(dst_size);
        
        if (srcBuf && dstBuf) {
            // Copy fuzz data to source buffer
            size_t copy_size = src_size < (size - 32) ? src_size : (size - 32);
            memcpy(srcBuf, data + 32, copy_size);
            
            // Call the target function
            tj3EncodeYUV8(handle, srcBuf, width, pitch, height, 
                         pixelFormat, dstBuf, align);
        }
    }
    
    // Cleanup
    if (srcBuf) tj3Free(srcBuf);
    if (dstBuf) tj3Free(dstBuf);
    tj3Destroy(handle);
}

static void fuzz_tj3Set(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return;
    
    // Test various parameters
    int param = data[0] % 32;  // TJPARAM values
    int value = (data[1] << 8) | data[2];
    
    tj3Set(handle, param, value);
    
    // Test with extreme values
    tj3Set(handle, param, 32767);
    tj3Set(handle, param, -32768);
    tj3Set(handle, param, 0);
    
    tj3Destroy(handle);
}

static void fuzz_tj3CompressFromYUV8(const uint8_t *data, size_t size) {
    if (size < 32) return;
    
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return;
    
    // Extract parameters
    int width = (data[0] << 8) | data[1];
    if (width <= 0) width = 64;
    int align = 1 << (data[2] % 5);
    int height = (data[3] << 8) | data[4];
    if (height <= 0) height = 64;
    int subsamp = data[5] % 6;
    
    // Set parameters
    tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(handle, TJPARAM_QUALITY, data[6] % 101);  // Quality 0-100
    
    // Calculate buffer sizes
    size_t src_size = tj3YUVBufSize(width, align, height, subsamp);
    size_t jpeg_buf_size = tj3JPEGBufSize(width, height, subsamp);
    
    if (src_size == 0 || jpeg_buf_size == 0) {
        tj3Destroy(handle);
        return;
    }
    
    // Cap buffer sizes to avoid excessive memory usage
    if (src_size > 1024 * 1024) src_size = 1024 * 1024;
    if (jpeg_buf_size > 1024 * 1024) jpeg_buf_size = 1024 * 1024;
    
    // Allocate buffers
    unsigned char *srcBuf = NULL;
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = jpeg_buf_size;
    
    if (size >= 32) {
        srcBuf = (unsigned char *)tj3Alloc(src_size);
        
        if (srcBuf) {
            // Copy fuzz data to source buffer
            size_t copy_size = src_size < (size - 32) ? src_size : (size - 32);
            memcpy(srcBuf, data + 32, copy_size);
            
            // Test different allocation strategies
            int strategy = data[7] % 3;
            
            switch (strategy) {
                case 0:  // Pre-allocate with tj3Alloc
                    jpegBuf = (unsigned char *)tj3Alloc(jpegSize);
                    if (jpegBuf) {
                        tj3CompressFromYUV8(handle, srcBuf, width, align, 
                                           height, &jpegBuf, &jpegSize);
                        tj3Free(jpegBuf);
                    }
                    break;
                    
                case 1:  // Let library allocate
                    jpegBuf = NULL;
                    tj3CompressFromYUV8(handle, srcBuf, width, align, 
                                       height, &jpegBuf, &jpegSize);
                    if (jpegBuf) {
                        tj3Free(jpegBuf);
                    }
                    break;
                    
                case 2:  // Pre-allocate worst-case size
                    jpegBuf = (unsigned char *)tj3Alloc(jpeg_buf_size + 4096);  // Extra for ICC
                    if (jpegBuf) {
                        jpegSize = jpeg_buf_size + 4096;
                        tj3CompressFromYUV8(handle, srcBuf, width, align, 
                                           height, &jpegBuf, &jpegSize);
                        tj3Free(jpegBuf);
                    }
                    break;
            }
        }
    }
    
    // Cleanup
    if (srcBuf) tj3Free(srcBuf);
    tj3Destroy(handle);
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Use first byte to decide which function(s) to test
    int test_case = Data[0] % 6;
    
    switch (test_case) {
        case 0:
            fuzz_tj3Alloc(Size);
            break;
        case 1:
            fuzz_tj3JPEGBufSize(Data, Size);
            break;
        case 2:
            fuzz_tj3YUVBufSize(Data, Size);
            break;
        case 3:
            fuzz_tj3EncodeYUV8(Data, Size);
            break;
        case 4:
            fuzz_tj3Set(Data, Size);
            break;
        case 5:
            fuzz_tj3CompressFromYUV8(Data, Size);
            break;
    }
    
    // Also test combinations with bounds checking
    if (Size > 1) {
        size_t alloc_size = Data[1] * 256;
        if (Size > 2) {
            alloc_size += Data[2];
        }
        fuzz_tj3Alloc(alloc_size);
        
        if (Size >= 8) {
            int width = (Data[3] << 8) | Data[4];
            int height = (Data[5] << 8) | Data[6];
            int subsamp = Data[7] % 6;
            size_t buf_size = tj3JPEGBufSize(width, height, subsamp);
            
            if (buf_size > 0) {
                void *buffer = tj3Alloc(buf_size);
                if (buffer) {
                    if (buf_size > 0) {
                        memset(buffer, 0, buf_size > 1024 ? 1024 : buf_size);
                    }
                    tj3Free(buffer);
                }
            }
        }
    }
    
    return 0;
}