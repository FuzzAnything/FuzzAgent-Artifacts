// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "src/turbojpeg.h"

static int get_byte(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return 0;
    return data[(*offset)++];
}

static int get_two_bytes(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 1 >= size) {
        *offset = size;
        return 0;
    }
    int val = (data[*offset] << 8) | data[*offset + 1];
    *offset += 2;
    return val;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    size_t offset = 0;
    
    // Initialize TurboJPEG instance
    int initType = get_byte(Data, Size, &offset) % 4; // TJINIT values: 0-3
    tjhandle handle = tj3Init(initType);
    if (!handle) return 0;
    
    // Set 8 parameters using fuzz data
    static const int param_list[] = {
        TJPARAM_QUALITY, TJPARAM_SUBSAMP, TJPARAM_FASTDCT, TJPARAM_OPTIMIZE,
        TJPARAM_PROGRESSIVE, TJPARAM_SCANLIMIT, TJPARAM_ARITHMETIC, TJPARAM_LOSSLESS,
        TJPARAM_COLORSPACE, TJPARAM_NOREALLOC
    };
    
    for (int i = 0; i < 8; i++) {
        int param = param_list[i % (sizeof(param_list)/sizeof(param_list[0]))];
        int value = get_byte(Data, Size, &offset);
        tj3Set(handle, param, value);
    }
    
    // Get JPEG buffer size
    int width = get_two_bytes(Data, Size, &offset) % 4096 + 1;
    int height = get_two_bytes(Data, Size, &offset) % 4096 + 1;
    int jpegSubsamp = get_byte(Data, Size, &offset) % 5; // TJSAMP values: 0-4
    
    size_t jpegBufSize = tj3JPEGBufSize(width, height, jpegSubsamp);
    if (jpegBufSize == 0) {
        tj3Destroy(handle);
        return 0;
    }
    
    // Allocate JPEG buffer
    unsigned char *jpegBuf = (unsigned char *)tj3Alloc(jpegBufSize);
    if (!jpegBuf) {
        tj3Destroy(handle);
        return 0;
    }
    
    // Get YUV buffer size
    int yuvWidth = get_two_bytes(Data, Size, &offset) % 4096 + 1;
    int align = 1 << (get_byte(Data, Size, &offset) % 4); // 1, 2, 4, 8
    int yuvHeight = get_two_bytes(Data, Size, &offset) % 4096 + 1;
    int yuvSubsamp = get_byte(Data, Size, &offset) % 5;
    
    size_t yuvBufSize = tj3YUVBufSize(yuvWidth, align, yuvHeight, yuvSubsamp);
    if (yuvBufSize == 0) {
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    // Allocate YUV buffer
    unsigned char *yuvBuf = (unsigned char *)tj3Alloc(yuvBufSize);
    if (!yuvBuf) {
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    // Set 2 more parameters - ensure TJPARAM_SUBSAMP matches yuvSubsamp for YUV encoding
    tj3Set(handle, TJPARAM_SUBSAMP, yuvSubsamp);
    int param = param_list[get_byte(Data, Size, &offset) % (sizeof(param_list)/sizeof(param_list[0]))];
    int value = get_byte(Data, Size, &offset);
    tj3Set(handle, param, value);
    
    // Create source buffer for YUV encoding with safe bounds
    int pixelFormat = get_byte(Data, Size, &offset) % 12; // TJPF values: 0-11
    int pixelSize = 3; // Default fallback
    if (pixelFormat >= 0 && pixelFormat < 12) {
        pixelSize = tjPixelSize[pixelFormat];
        if (pixelSize <= 0) pixelSize = 3;
    }
    
    // Ensure pitch calculation doesn't overflow
    if (pixelSize == 0 || yuvWidth > INT_MAX / pixelSize) {
        tj3Free(yuvBuf);
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }
    int pitch = yuvWidth * pixelSize;
    
    // Limit source buffer size to prevent excessive allocation
    if (yuvHeight > 0 && pitch > 0 && (size_t)yuvHeight > SIZE_MAX / (size_t)pitch) {
        tj3Free(yuvBuf);
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    size_t srcBufSize = (size_t)pitch * yuvHeight;
    if (srcBufSize > 10 * 1024 * 1024) { // Limit to 10MB
        tj3Free(yuvBuf);
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    unsigned char *srcBuf = NULL;
    if (srcBufSize > 0) {
        srcBuf = (unsigned char *)malloc(srcBufSize);
        if (srcBuf) {
            size_t to_copy = Size - offset;
            if (to_copy > srcBufSize) to_copy = srcBufSize;
            if (to_copy > 0) {
                memcpy(srcBuf, Data + offset, to_copy);
            }
            // Fill remaining with safe pattern
            for (size_t i = to_copy; i < srcBufSize; i++) {
                srcBuf[i] = (i % 256);
            }
        }
    }
    
    // Encode YUV if we have source buffer
    if (srcBuf) {
        tj3EncodeYUV8(handle, srcBuf, yuvWidth, pitch, yuvHeight, 
                      pixelFormat, yuvBuf, align);
        free(srcBuf);
    }
    
    // Compress from YUV
    size_t jpegSize = jpegBufSize;
    unsigned char *jpegBufPtr = jpegBuf;
    tj3CompressFromYUV8(handle, yuvBuf, yuvWidth, align, yuvHeight, 
                        &jpegBufPtr, &jpegSize);
    
    // Cleanup
    tj3Free(yuvBuf);
    tj3Free(jpegBufPtr);
    tj3Destroy(handle);
    
    return 0;
}