// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int read_int(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) return 0;
    int value;
    memcpy(&value, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    return value;
}

static size_t read_size_t(const uint8_t **data, size_t *size) {
    if (*size < sizeof(size_t)) return 0;
    size_t value;
    memcpy(&value, *data, sizeof(size_t));
    *data += sizeof(size_t);
    *size -= sizeof(size_t);
    return value;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    const uint8_t *input = Data;
    size_t remaining = Size;
    
    // Initialize TurboJPEG instance
    int initType = read_int(&input, &remaining);
    tjhandle handle = tj3Init(initType);
    if (!handle) return 0;
    
    // First 8 tj3Set calls
    for (int i = 0; i < 8; i++) {
        int param = read_int(&input, &remaining);
        int value = read_int(&input, &remaining);
        tj3Set(handle, param, value);
    }
    
    // Get parameters for buffer size calculations
    int width = read_int(&input, &remaining);
    int height = read_int(&input, &remaining);
    int jpegSubsamp = read_int(&input, &remaining);
    
    // Calculate JPEG buffer size and allocate
    size_t jpegBufSize = tj3JPEGBufSize(width, height, jpegSubsamp);
    unsigned char *jpegBuf = NULL;
    if (jpegBufSize > 0) {
        jpegBuf = (unsigned char *)tj3Alloc(jpegBufSize);
    }
    
    // Get YUV buffer parameters
    int yuvWidth = read_int(&input, &remaining);
    int align = read_int(&input, &remaining);
    int yuvHeight = read_int(&input, &remaining);
    int yuvSubsamp = read_int(&input, &remaining);
    
    // Calculate YUV buffer size
    size_t yuvBufSize = tj3YUVBufSize(yuvWidth, align, yuvHeight, yuvSubsamp);
    
    // Next 2 tj3Set calls
    for (int i = 0; i < 2; i++) {
        int param = read_int(&input, &remaining);
        int value = read_int(&input, &remaining);
        tj3Set(handle, param, value);
    }
    
    // Allocate YUV buffer if size is valid
    unsigned char *yuvBuf = NULL;
    if (yuvBufSize > 0) {
        yuvBuf = (unsigned char *)tj3Alloc(yuvBufSize);
    }
    
    // Prepare parameters for tj3EncodeYUV8
    int pitch = read_int(&input, &remaining);
    int pixelFormat = read_int(&input, &remaining);
    
    // Create source buffer from remaining data
    size_t srcSize = remaining;
    const unsigned char *srcBuf = input;
    
    // Encode YUV if we have valid buffers
    if (yuvBuf && srcBuf && srcSize > 0) {
        tj3EncodeYUV8(handle, srcBuf, yuvWidth, pitch, yuvHeight, 
                      pixelFormat, yuvBuf, align);
    }
    
    // Compress from YUV if we have valid buffers
    if (yuvBuf && jpegBuf && jpegBufSize > 0) {
        size_t actualJpegSize = jpegBufSize;
        tj3CompressFromYUV8(handle, yuvBuf, yuvWidth, align, yuvHeight, 
                            &jpegBuf, &actualJpegSize);
    }
    
    // Cleanup
    tj3Free(jpegBuf);
    tj3Free(yuvBuf);
    tj3Destroy(handle);
    
    return 0;
}