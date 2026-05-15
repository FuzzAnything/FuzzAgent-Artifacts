// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_tj3Alloc(const uint8_t *data, size_t size) {
    if (size < sizeof(size_t)) return;
    
    size_t alloc_size;
    memcpy(&alloc_size, data, sizeof(size_t));
    if (alloc_size > 1024 * 1024) return; // Limit allocation size
    
    void *buffer = tj3Alloc(alloc_size);
    if (buffer) {
        tj3Free(buffer);
    }
}

static void fuzz_tj3CompressFromYUV8(const uint8_t *data, size_t size) {
    if (size < 20) return;
    
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return;
    
    int width, height, align;
    memcpy(&width, data, sizeof(int));
    memcpy(&height, data + sizeof(int), sizeof(int));
    memcpy(&align, data + 2 * sizeof(int), sizeof(int));
    
    width = abs(width) % 1024 + 1;
    height = abs(height) % 1024 + 1;
    align = (abs(align) % 16) + 1;
    
    size_t yuv_size = tj3YUVBufSize(width, align, height, TJSAMP_444);
    if (yuv_size == 0 || yuv_size > size - 12) {
        tj3Destroy(handle);
        return;
    }
    
    unsigned char *src_buf = (unsigned char *)data + 12;
    unsigned char *jpeg_buf = NULL;
    size_t jpeg_size = 0;
    
    tj3CompressFromYUV8(handle, src_buf, width, align, height, &jpeg_buf, &jpeg_size);
    
    if (jpeg_buf) {
        tj3Free(jpeg_buf);
    }
    tj3Destroy(handle);
}

static void fuzz_tjCompress(const uint8_t *data, size_t size) {
    if (size < 40) return;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return;
    
    int width, height, pitch, pixelSize, jpegSubsamp, jpegQual, flags;
    memcpy(&width, data, sizeof(int));
    memcpy(&height, data + sizeof(int), sizeof(int));
    memcpy(&pitch, data + 2 * sizeof(int), sizeof(int));
    memcpy(&pixelSize, data + 3 * sizeof(int), sizeof(int));
    memcpy(&jpegSubsamp, data + 4 * sizeof(int), sizeof(int));
    memcpy(&jpegQual, data + 5 * sizeof(int), sizeof(int));
    memcpy(&flags, data + 6 * sizeof(int), sizeof(int));
    
    width = abs(width) % 1024 + 1;
    height = abs(height) % 1024 + 1;
    pitch = abs(pitch) % 2048 + 1;
    pixelSize = abs(pixelSize) % 4 + 1;
    jpegSubsamp = abs(jpegSubsamp) % 5;
    jpegQual = abs(jpegQual) % 101;
    flags = abs(flags) % 4;
    
    size_t src_size = height * pitch * pixelSize;
    if (src_size > size - 28 * sizeof(int)) {
        tjDestroy(handle);
        return;
    }
    
    unsigned char *src_buf = (unsigned char *)data + 28 * sizeof(int);
    unsigned char *dst_buf = NULL;
    unsigned long compressed_size = 0;
    
    size_t max_size = tjBufSize(width, height, jpegSubsamp);
    if (max_size > 0) {
        dst_buf = (unsigned char *)malloc(max_size);
        if (dst_buf) {
            tjCompress(handle, src_buf, width, pitch, height, pixelSize,
                      dst_buf, &compressed_size, jpegSubsamp, jpegQual, flags);
            free(dst_buf);
        }
    }
    
    tjDestroy(handle);
}

static void fuzz_tj3JPEGBufSize(const uint8_t *data, size_t size) {
    if (size < 12) return;
    
    int width, height, jpegSubsamp;
    memcpy(&width, data, sizeof(int));
    memcpy(&height, data + sizeof(int), sizeof(int));
    memcpy(&jpegSubsamp, data + 2 * sizeof(int), sizeof(int));
    
    width = abs(width) % 4096;
    height = abs(height) % 4096;
    jpegSubsamp = abs(jpegSubsamp) % 6;
    
    tj3JPEGBufSize(width, height, jpegSubsamp);
}

static void fuzz_tjCompress2(const uint8_t *data, size_t size) {
    if (size < 44) return;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return;
    
    int width, height, pitch, pixelFormat, jpegSubsamp, jpegQual, flags;
    memcpy(&width, data, sizeof(int));
    memcpy(&height, data + sizeof(int), sizeof(int));
    memcpy(&pitch, data + 2 * sizeof(int), sizeof(int));
    memcpy(&pixelFormat, data + 3 * sizeof(int), sizeof(int));
    memcpy(&jpegSubsamp, data + 4 * sizeof(int), sizeof(int));
    memcpy(&jpegQual, data + 5 * sizeof(int), sizeof(int));
    memcpy(&flags, data + 6 * sizeof(int), sizeof(int));
    
    width = abs(width) % 1024 + 1;
    height = abs(height) % 1024 + 1;
    pitch = abs(pitch) % 2048 + 1;
    pixelFormat = abs(pixelFormat) % 12;
    jpegSubsamp = abs(jpegSubsamp) % 5;
    jpegQual = abs(jpegQual) % 101;
    flags = abs(flags) % 4;
    
    size_t src_size = height * pitch * tjPixelSize[pixelFormat];
    if (src_size > size - 28 * sizeof(int)) {
        tjDestroy(handle);
        return;
    }
    
    const unsigned char *src_buf = data + 28 * sizeof(int);
    unsigned char *jpeg_buf = NULL;
    unsigned long jpeg_size = 0;
    
    tjCompress2(handle, src_buf, width, pitch, height, pixelFormat,
                &jpeg_buf, &jpeg_size, jpegSubsamp, jpegQual, flags);
    
    if (jpeg_buf) {
        tjFree(jpeg_buf);
    }
    tjDestroy(handle);
}

static void fuzz_tjCompressFromYUV(const uint8_t *data, size_t size) {
    if (size < 24) return;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return;
    
    int width, align, height, subsamp, jpegQual, flags;
    memcpy(&width, data, sizeof(int));
    memcpy(&align, data + sizeof(int), sizeof(int));
    memcpy(&height, data + 2 * sizeof(int), sizeof(int));
    memcpy(&subsamp, data + 3 * sizeof(int), sizeof(int));
    memcpy(&jpegQual, data + 4 * sizeof(int), sizeof(int));
    memcpy(&flags, data + 5 * sizeof(int), sizeof(int));
    
    width = abs(width) % 1024 + 1;
    height = abs(height) % 1024 + 1;
    subsamp = abs(subsamp) % 5;
    jpegQual = abs(jpegQual) % 101;
    flags = abs(flags) % 4;
    
    // Ensure align is a power of 2 between 1 and 16
    int align_value = (abs(align) % 5);
    align = 1 << align_value; // 1, 2, 4, 8, or 16
    
    // Calculate required buffer size using tjBufSizeYUV2
    size_t yuv_size = tjBufSizeYUV2(width, align, height, subsamp);
    if (yuv_size == 0 || yuv_size > size - 24) {
        tjDestroy(handle);
        return;
    }
    
    // Ensure we don't read beyond available data
    const unsigned char *src_buf = data + 24;
    if (src_buf + yuv_size > data + size) {
        tjDestroy(handle);
        return;
    }
    
    unsigned char *jpeg_buf = NULL;
    unsigned long jpeg_size = 0;
    
    tjCompressFromYUV(handle, src_buf, width, align, height, subsamp,
                     &jpeg_buf, &jpeg_size, jpegQual, flags);
    
    if (jpeg_buf) {
        tjFree(jpeg_buf);
    }
    tjDestroy(handle);
}

int LLVMFuzzerTestOneInput_9(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    uint8_t selector = data[0] % 6;
    
    switch (selector) {
        case 0:
            fuzz_tj3Alloc(data + 1, size - 1);
            break;
        case 1:
            fuzz_tj3CompressFromYUV8(data + 1, size - 1);
            break;
        case 2:
            fuzz_tjCompress(data + 1, size - 1);
            break;
        case 3:
            fuzz_tj3JPEGBufSize(data + 1, size - 1);
            break;
        case 4:
            fuzz_tjCompress2(data + 1, size - 1);
            break;
        case 5:
            fuzz_tjCompressFromYUV(data + 1, size - 1);
            break;
    }
    
    return 0;
}