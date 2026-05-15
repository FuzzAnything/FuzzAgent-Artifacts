// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_tj3Compress16(const uint8_t *data, size_t size) {
    if (size < 16) return;
    
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return;
    
    // Extract parameters from fuzz data
    int width = *(const int*)(data) % 1024 + 1;
    int height = *(const int*)(data + 4) % 1024 + 1;
    int pixelFormat = *(const int*)(data + 8) % (TJPF_CMYK + 1);
    int pitch = width * tjPixelSize[pixelFormat];
    
    // Calculate source buffer size
    size_t srcSize = pitch * height * sizeof(unsigned short);
    if (srcSize > size - 16 || srcSize == 0) {
        tj3Destroy(handle);
        return;
    }
    
    // Allocate and fill source buffer
    const unsigned short *srcBuf = (const unsigned short*)(data + 16);
    
    // Set precision parameter (13-16 bits)
    int precision = 13 + (*(const uint8_t*)(data + 12) % 4);
    tj3Set(handle, TJPARAM_PRECISION, precision);
    
    // Prepare output buffer
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    
    // Call target function
    tj3Compress16(handle, srcBuf, width, pitch, height, pixelFormat, &jpegBuf, &jpegSize);
    
    // Cleanup
    if (jpegBuf) tj3Free(jpegBuf);
    tj3Destroy(handle);
}

static void fuzz_tjCompress2(const uint8_t *data, size_t size) {
    if (size < 16) return;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return;
    
    // Extract parameters from fuzz data
    int width = *(const int*)(data) % 1024 + 1;
    int height = *(const int*)(data + 4) % 1024 + 1;
    int pixelFormat = *(const int*)(data + 8) % (TJPF_CMYK + 1);
    int jpegSubsamp = *(const int*)(data + 12) % (TJSAMP_440 + 1);
    int jpegQual = *(const uint8_t*)(data + 15) % 101;
    int flags = *(const uint8_t*)(data + 14) % 4;
    int pitch = width * tjPixelSize[pixelFormat];
    
    // Calculate source buffer size
    size_t srcSize = pitch * height;
    if (srcSize > size - 16 || srcSize == 0) {
        tjDestroy(handle);
        return;
    }
    
    // Allocate and fill source buffer
    const unsigned char *srcBuf = data + 16;
    
    // Prepare output buffer
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    
    // Call target function
    tjCompress2(handle, srcBuf, width, pitch, height, pixelFormat, &jpegBuf, &jpegSize, jpegSubsamp, jpegQual, flags);
    
    // Cleanup
    if (jpegBuf) tjFree(jpegBuf);
    tjDestroy(handle);
}

static void fuzz_tjBufSize(const uint8_t *data, size_t size) {
    if (size < 12) return;
    
    int width = *(const int*)(data) % 4096 + 1;
    int height = *(const int*)(data + 4) % 4096 + 1;
    int jpegSubsamp = *(const int*)(data + 8) % (TJSAMP_440 + 1);
    
    // Call target function
    tjBufSize(width, height, jpegSubsamp);
}

static void fuzz_tj3JPEGBufSize(const uint8_t *data, size_t size) {
    if (size < 12) return;
    
    int width = *(const int*)(data) % 4096 + 1;
    int height = *(const int*)(data + 4) % 4096 + 1;
    int jpegSubsamp = *(const int*)(data + 8) % (TJSAMP_440 + 1);
    
    // Call target function
    tj3JPEGBufSize(width, height, jpegSubsamp);
}

static void fuzz_tjCompress(const uint8_t *data, size_t size) {
    if (size < 20) return;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return;
    
    // Extract parameters from fuzz data
    int width = *(const int*)(data) % 1024 + 1;
    int height = *(const int*)(data + 4) % 1024 + 1;
    int pixelSize = *(const uint8_t*)(data + 8) % 4 + 1;
    int jpegSubsamp = *(const int*)(data + 12) % (TJSAMP_440 + 1);
    int jpegQual = *(const uint8_t*)(data + 16) % 101;
    int flags = *(const uint8_t*)(data + 17) % 4;
    int pitch = width * pixelSize;
    
    // Calculate source buffer size
    size_t srcSize = pitch * height;
    if (srcSize > size - 20 || srcSize == 0) {
        tjDestroy(handle);
        return;
    }
    
    // Allocate source buffer
    unsigned char *srcBuf = (unsigned char*)malloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return;
    }
    memcpy(srcBuf, data + 20, srcSize > size - 20 ? size - 20 : srcSize);
    
    // Calculate destination buffer size
    unsigned long dstSize = tjBufSize(width, height, jpegSubsamp);
    if (dstSize == 0) {
        free(srcBuf);
        tjDestroy(handle);
        return;
    }
    
    unsigned char *dstBuf = (unsigned char*)malloc(dstSize);
    if (!dstBuf) {
        free(srcBuf);
        tjDestroy(handle);
        return;
    }
    
    // Call target function
    tjCompress(handle, srcBuf, width, pitch, height, pixelSize, dstBuf, &dstSize, jpegSubsamp, jpegQual, flags);
    
    // Cleanup
    free(srcBuf);
    free(dstBuf);
    tjDestroy(handle);
}

static void fuzz_TJBUFSIZE(const uint8_t *data, size_t size) {
    if (size < 8) return;
    
    int width = *(const int*)(data) % 4096 + 1;
    int height = *(const int*)(data + 4) % 4096 + 1;
    
    // Call target macro/function
    TJBUFSIZE(width, height);
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Use first byte to select which function to fuzz
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0:
            fuzz_tj3Compress16(Data + 1, Size - 1);
            break;
        case 1:
            fuzz_tjCompress2(Data + 1, Size - 1);
            break;
        case 2:
            fuzz_tjBufSize(Data + 1, Size - 1);
            break;
        case 3:
            fuzz_tj3JPEGBufSize(Data + 1, Size - 1);
            break;
        case 4:
            fuzz_tjCompress(Data + 1, Size - 1);
            break;
        case 5:
            fuzz_TJBUFSIZE(Data + 1, Size - 1);
            break;
    }
    
    return 0;
}