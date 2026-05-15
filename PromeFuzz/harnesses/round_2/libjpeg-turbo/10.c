// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "turbojpeg.h"

static int parse_int(const uint8_t *data, size_t size, size_t *offset, int min_val, int max_val) {
    if (*offset + sizeof(int) > size) {
        return min_val;
    }
    int val = *(int*)(data + *offset);
    *offset += sizeof(int);
    if (val < min_val) val = min_val;
    if (val > max_val) val = max_val;
    return val;
}

static size_t parse_size_t(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(size_t) > size) {
        return 0;
    }
    size_t val = *(size_t*)(data + *offset);
    *offset += sizeof(size_t);
    return val;
}

static void test_tj3Alloc(const uint8_t *data, size_t size) {
    size_t offset = 0;
    size_t alloc_size = parse_size_t(data, size, &offset);
    if (alloc_size > 1024 * 1024) {
        alloc_size = 1024 * 1024;
    }
    void *buffer = tj3Alloc(alloc_size);
    if (buffer != NULL) {
        tj3Free(buffer);
    }
}

static void test_tj3Compress16(const uint8_t *data, size_t size) {
    size_t offset = 0;
    int width = parse_int(data, size, &offset, 1, 1024);
    int height = parse_int(data, size, &offset, 1, 1024);
    int pitch = parse_int(data, size, &offset, 0, 2048);
    int pixelFormat = parse_int(data, size, &offset, TJPF_RGB, TJPF_CMYK);
    
    if (pixelFormat < TJPF_RGB || pixelFormat > TJPF_CMYK) {
        pixelFormat = TJPF_RGB;
    }
    
    int pixelSize = tjPixelSize[pixelFormat];
    size_t src_size = (size_t)width * height * pixelSize;
    if (src_size > size - offset) {
        src_size = size - offset;
    }
    if (src_size == 0) {
        return;
    }
    
    unsigned short *src_buf = (unsigned short *)malloc(src_size);
    if (src_buf == NULL) {
        return;
    }
    memcpy(src_buf, data + offset, src_size);
    
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (handle == NULL) {
        free(src_buf);
        return;
    }
    
    unsigned char *jpeg_buf = NULL;
    size_t jpeg_size = 0;
    
    int result = tj3Compress16(handle, src_buf, width, pitch, height, pixelFormat, &jpeg_buf, &jpeg_size);
    
    if (jpeg_buf != NULL) {
        tj3Free(jpeg_buf);
    }
    tj3Destroy(handle);
    free(src_buf);
}

static void test_tjBufSize(const uint8_t *data, size_t size) {
    size_t offset = 0;
    int width = parse_int(data, size, &offset, 1, 4096);
    int height = parse_int(data, size, &offset, 1, 4096);
    int jpegSubsamp = parse_int(data, size, &offset, TJSAMP_444, TJSAMP_GRAY);
    
    unsigned long buf_size = tjBufSize(width, height, jpegSubsamp);
    (void)buf_size;
}

static void test_TJBUFSIZE(const uint8_t *data, size_t size) {
    size_t offset = 0;
    int width = parse_int(data, size, &offset, 1, 4096);
    int height = parse_int(data, size, &offset, 1, 4096);
    
    unsigned long buf_size = TJBUFSIZE(width, height);
    (void)buf_size;
}

static void test_tj3JPEGBufSize(const uint8_t *data, size_t size) {
    size_t offset = 0;
    int width = parse_int(data, size, &offset, 1, 4096);
    int height = parse_int(data, size, &offset, 1, 4096);
    int jpegSubsamp = parse_int(data, size, &offset, TJSAMP_444, TJSAMP_GRAY);
    
    size_t buf_size = tj3JPEGBufSize(width, height, jpegSubsamp);
    (void)buf_size;
}

static void test_tjCompress2(const uint8_t *data, size_t size) {
    size_t offset = 0;
    int width = parse_int(data, size, &offset, 1, 1024);
    int height = parse_int(data, size, &offset, 1, 1024);
    int pitch = parse_int(data, size, &offset, 0, 2048);
    int pixelFormat = parse_int(data, size, &offset, TJPF_RGB, TJPF_CMYK);
    int jpegSubsamp = parse_int(data, size, &offset, TJSAMP_444, TJSAMP_GRAY);
    int jpegQual = parse_int(data, size, &offset, 1, 100);
    int flags = parse_int(data, size, &offset, 0, TJFLAG_PROGRESSIVE);
    
    if (pixelFormat < TJPF_RGB || pixelFormat > TJPF_CMYK) {
        pixelFormat = TJPF_RGB;
    }
    
    int pixelSize = tjPixelSize[pixelFormat];
    int actual_pitch = (pitch == 0) ? width * pixelSize : pitch;
    
    if (actual_pitch < width * pixelSize) {
        return;
    }
    
    size_t src_size = (size_t)actual_pitch * height;
    if (src_size > size - offset) {
        src_size = size - offset;
    }
    if (src_size == 0) {
        return;
    }
    
    unsigned char *src_buf = (unsigned char *)malloc(src_size);
    if (src_buf == NULL) {
        return;
    }
    
    memcpy(src_buf, data + offset, src_size);
    
    tjhandle handle = tjInitCompress();
    if (handle == NULL) {
        free(src_buf);
        return;
    }
    
    unsigned char *jpeg_buf = NULL;
    unsigned long jpeg_size = 0;
    
    int result = tjCompress2(handle, src_buf, width, actual_pitch, height, pixelFormat, 
                            &jpeg_buf, &jpeg_size, jpegSubsamp, jpegQual, flags);
    
    if (jpeg_buf != NULL) {
        tjFree(jpeg_buf);
    }
    tjDestroy(handle);
    free(src_buf);
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(int)) {
        return 0;
    }
    
    int test_selector = Data[0] % 6;
    
    switch (test_selector) {
        case 0:
            test_tj3Alloc(Data + 1, Size - 1);
            break;
        case 1:
            test_tj3Compress16(Data + 1, Size - 1);
            break;
        case 2:
            test_tjBufSize(Data + 1, Size - 1);
            break;
        case 3:
            test_TJBUFSIZE(Data + 1, Size - 1);
            break;
        case 4:
            test_tj3JPEGBufSize(Data + 1, Size - 1);
            break;
        case 5:
            test_tjCompress2(Data + 1, Size - 1);
            break;
    }
    
    return 0;
}