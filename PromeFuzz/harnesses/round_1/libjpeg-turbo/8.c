// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
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

static void fuzz_tj3Alloc(size_t bytes) {
    if (bytes > 1024 * 1024) return; // Limit allocation size
    void *buffer = tj3Alloc(bytes);
    if (buffer) {
        // Touch the buffer to potentially catch memory issues
        if (bytes > 0) {
            memset(buffer, 0, bytes > 16 ? 16 : bytes);
        }
        tj3Free(buffer);
    }
}

static void fuzz_tjCompressFromYUV(tjhandle handle, const uint8_t *data, size_t size) {
    if (size < 100) return;
    
    int width = (data[0] << 8) | data[1];
    int height = (data[2] << 8) | data[3];
    int align = 1 << (data[4] & 0x3); // 1, 2, 4, or 8
    int subsamp = data[5] % 6; // 0-5 for valid subsampling values
    int jpegQual = (data[6] % 100) + 1; // 1-100
    int flags = data[7];
    
    // Limit dimensions to avoid excessive memory usage
    if (width < 1) width = 1;
    if (width > 256) width = 256;
    if (height < 1) height = 1;
    if (height > 256) height = 256;
    
    // Calculate YUV buffer size needed
    size_t yuvSize = width * height;
    if (subsamp != TJSAMP_GRAY) {
        yuvSize += 2 * ((width + 1) / 2) * ((height + 1) / 2);
    }
    
    if (yuvSize > size - 8 || yuvSize == 0) return;
    
    const unsigned char *srcBuf = data + 8;
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    
    tjCompressFromYUV(handle, srcBuf, width, align, height, subsamp,
                     &jpegBuf, &jpegSize, jpegQual, flags);
    
    if (jpegBuf) {
        tj3Free(jpegBuf);
    }
}

static void fuzz_tj3CompressFromYUV8(tjhandle handle, const uint8_t *data, size_t size) {
    if (size < 100) return;
    
    int width = (data[0] << 8) | data[1];
    int height = (data[2] << 8) | data[3];
    int align = 1 << (data[4] & 0x3); // 1, 2, 4, or 8
    
    // Limit dimensions
    if (width < 1) width = 1;
    if (width > 256) width = 256;
    if (height < 1) height = 1;
    if (height > 256) height = 256;
    
    // Calculate YUV buffer size
    size_t yuvSize = tj3YUVBufSize(width, align, height, TJSAMP_420);
    if (yuvSize > size - 5 || yuvSize == 0) return;
    
    const unsigned char *srcBuf = data + 5;
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    
    tj3CompressFromYUV8(handle, srcBuf, width, align, height, &jpegBuf, &jpegSize);
    
    if (jpegBuf) {
        tj3Free(jpegBuf);
    }
}

static void fuzz_tjCompress2(tjhandle handle, const uint8_t *data, size_t size) {
    if (size < 100) return;
    
    int width = (data[0] << 8) | data[1];
    int height = (data[2] << 8) | data[3];
    int pixelFormat = data[4] % 12; // 0-11 for valid pixel formats
    int jpegSubsamp = data[5] % 6; // 0-5
    int jpegQual = (data[6] % 100) + 1; // 1-100
    int flags = data[7];
    
    // Limit dimensions
    if (width < 1) width = 1;
    if (width > 256) width = 256;
    if (height < 1) height = 1;
    if (height > 256) height = 256;
    
    int pitch = width * tjPixelSize[pixelFormat];
    size_t srcSize = pitch * height;
    
    if (srcSize > size - 8 || srcSize == 0) return;
    
    const unsigned char *srcBuf = data + 8;
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    
    tjCompress2(handle, srcBuf, width, pitch, height, pixelFormat,
                &jpegBuf, &jpegSize, jpegSubsamp, jpegQual, flags);
    
    if (jpegBuf) {
        tj3Free(jpegBuf);
    }
}

static void fuzz_tjCompress(tjhandle handle, const uint8_t *data, size_t size) {
    if (size < 100) return;
    
    int width = (data[0] << 8) | data[1];
    int height = (data[2] << 8) | data[3];
    int pixelSize = data[4] % 5; // 1-4 typically
    if (pixelSize == 0) pixelSize = 1;
    int jpegSubsamp = data[5] % 6; // 0-5
    int jpegQual = (data[6] % 100) + 1; // 1-100
    int flags = data[7];
    
    // Limit dimensions
    if (width < 1) width = 1;
    if (width > 256) width = 256;
    if (height < 1) height = 1;
    if (height > 256) height = 256;
    
    int pitch = width * pixelSize;
    size_t srcSize = pitch * height;
    size_t dstSize = tj3JPEGBufSize(width, height, jpegSubsamp);
    
    if (srcSize > size - 8 || srcSize == 0 || dstSize == 0) return;
    
    unsigned char *srcBuf = (unsigned char *)(data + 8);
    unsigned char *dstBuf = tj3Alloc(dstSize);
    if (!dstBuf) return;
    
    unsigned long compressedSize = dstSize;
    
    tjCompress(handle, srcBuf, width, pitch, height, pixelSize,
               dstBuf, &compressedSize, jpegSubsamp, jpegQual, flags);
    
    tj3Free(dstBuf);
}

static void fuzz_tj3JPEGBufSize(const uint8_t *data, size_t size) {
    if (size < 3) return;
    
    int width = (data[0] << 8) | data[1];
    int height = (data[2] << 8) | data[3];
    int jpegSubsamp = data[4] % 6; // 0-5
    
    // Test with various dimensions
    tj3JPEGBufSize(width, height, jpegSubsamp);
    tj3JPEGBufSize(width % 256, height % 256, jpegSubsamp);
    tj3JPEGBufSize(1, 1, jpegSubsamp);
    tj3JPEGBufSize(0, 0, jpegSubsamp); // Should return 0
    tj3JPEGBufSize(-1, -1, jpegSubsamp); // Should return 0
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize TurboJPEG handle
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;
    
    // Seed random based on input data
    unsigned int seed = 0;
    for (size_t i = 0; i < Size && i < 4; i++) {
        seed = (seed << 8) | Data[i];
    }
    srand(seed);
    
    // Call different functions based on input data
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0:
            fuzz_tj3Alloc(Size % 2048);
            break;
        case 1:
            fuzz_tjCompressFromYUV(handle, Data, Size);
            break;
        case 2:
            fuzz_tj3CompressFromYUV8(handle, Data, Size);
            break;
        case 3:
            fuzz_tjCompress2(handle, Data, Size);
            break;
        case 4:
            fuzz_tjCompress(handle, Data, Size);
            break;
        case 5:
            fuzz_tj3JPEGBufSize(Data, Size);
            break;
    }
    
    // Clean up
    tjDestroy(handle);
    
    return 0;
}