// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int get_bytes_per_pixel(int pixelFormat) {
    switch(pixelFormat) {
        case TJPF_GRAY: return 1;
        case TJPF_RGB:
        case TJPF_BGR: return 3;
        case TJPF_RGBX:
        case TJPF_BGRX:
        case TJPF_XBGR:
        case TJPF_XRGB:
        case TJPF_RGBA:
        case TJPF_BGRA:
        case TJPF_ABGR:
        case TJPF_ARGB:
        case TJPF_CMYK: return 4;
        default: return 3;
    }
}

static void fuzz_tjCompress2(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return;
    
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    
    int width = (data[0] * 4) + 1;
    int height = (data[1] * 4) + 1;
    int pitch = width * 3;
    int pixelFormat = data[2] % 12;
    int jpegSubsamp = data[3] % 6;
    int jpegQual = (data[0] % 100) + 1;
    int flags = 0;
    
    const uint8_t *srcData = data + 4;
    size_t srcSize = size - 4;
    size_t requiredSize = pitch * height * get_bytes_per_pixel(pixelFormat);
    
    if (srcSize < requiredSize) {
        tjDestroy(handle);
        return;
    }
    
    tjCompress2(handle, srcData, width, pitch, height, pixelFormat,
                &jpegBuf, &jpegSize, jpegSubsamp, jpegQual, flags);
    
    if (jpegBuf) tjFree(jpegBuf);
    tjDestroy(handle);
}

static void fuzz_tjCompressFromYUV(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return;
    
    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    
    int width = (data[0] * 4) + 1;
    int height = (data[1] * 4) + 1;
    int align = 1;
    int subsamp = data[2] % 6;
    int jpegQual = (data[3] % 100) + 1;
    int flags = 0;
    
    const uint8_t *srcData = data + 4;
    size_t srcSize = size - 4;
    
    // Calculate required YUV buffer size based on width, height, subsampling
    size_t requiredSize = 0;
    switch(subsamp) {
        case TJSAMP_444:
            requiredSize = width * height * 3;  // Y + U + V planes
            break;
        case TJSAMP_422:
            requiredSize = width * height * 2;  // Y + U/V planes
            break;
        case TJSAMP_420:
            requiredSize = width * height * 3 / 2;  // Y + U/V planes
            break;
        case TJSAMP_GRAY:
            requiredSize = width * height;  // Y plane only
            break;
        case TJSAMP_440:
            requiredSize = width * height * 2;  // Y + U/V planes
            break;
        case TJSAMP_411:
            requiredSize = width * height * 3 / 2;  // Y + U/V planes
            break;
        default:
            requiredSize = width * height * 3 / 2;  // Default to 4:2:0
    }
    
    if (srcSize < requiredSize) {
        tjDestroy(handle);
        return;
    }
    
    tjCompressFromYUV(handle, srcData, width, align, height, subsamp,
                      &jpegBuf, &jpegSize, jpegQual, flags);
    
    if (jpegBuf) tjFree(jpegBuf);
    tjDestroy(handle);
}

static void fuzz_tjCompress(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return;
    
    int width = (data[0] * 4) + 1;
    int height = (data[1] * 4) + 1;
    int pitch = width * 3;
    int pixelSize = 3;
    int jpegSubsamp = data[2] % 6;
    int jpegQual = (data[3] % 100) + 1;
    int flags = 0;
    
    const uint8_t *srcData = data + 4;
    size_t srcSize = size - 4;
    size_t requiredSize = pitch * height * pixelSize;
    
    if (srcSize < requiredSize) {
        tjDestroy(handle);
        return;
    }
    
    unsigned long compressedSize = 0;
    unsigned char *dstBuf = (unsigned char *)malloc(width * height * pixelSize * 2);
    if (!dstBuf) {
        tjDestroy(handle);
        return;
    }
    
    tjCompress(handle, (unsigned char *)srcData, width, pitch, height, pixelSize,
               dstBuf, &compressedSize, jpegSubsamp, jpegQual, flags);
    
    free(dstBuf);
    tjDestroy(handle);
}

static void fuzz_tjDecodeYUVPlanes(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    tjhandle handle = tjInitDecompress();
    if (!handle) return;
    
    int width = (data[0] * 4) + 1;
    int height = (data[1] * 4) + 1;
    int pixelFormat = data[3] % 12;
    int bytes_per_pixel = get_bytes_per_pixel(pixelFormat);
    int pitch = width * bytes_per_pixel;
    int subsamp = data[2] % 6;
    int flags = 0;
    
    const uint8_t *remainingData = data + 4;
    size_t remainingSize = size - 4;
    
    // Calculate required YUV buffer size based on width, height, subsampling
    size_t requiredYUVSize = 0;
    switch(subsamp) {
        case TJSAMP_444:
            requiredYUVSize = width * height * 3;
            break;
        case TJSAMP_422:
            requiredYUVSize = width * height * 2;
            break;
        case TJSAMP_420:
            requiredYUVSize = width * height * 3 / 2;
            break;
        case TJSAMP_GRAY:
            requiredYUVSize = width * height;
            break;
        case TJSAMP_440:
            requiredYUVSize = width * height * 2;
            break;
        case TJSAMP_411:
            requiredYUVSize = width * height * 3 / 2;
            break;
        default:
            requiredYUVSize = width * height * 3 / 2;
    }
    
    if (remainingSize < requiredYUVSize) {
        tjDestroy(handle);
        return;
    }
    
    const unsigned char *srcPlanes[3];
    int strides[3] = {0};
    
    // Set up plane pointers based on subsampling
    switch(subsamp) {
        case TJSAMP_444:
            srcPlanes[0] = remainingData;
            srcPlanes[1] = remainingData + width * height;
            srcPlanes[2] = remainingData + width * height * 2;
            strides[0] = width;
            strides[1] = width;
            strides[2] = width;
            break;
        case TJSAMP_422:
            srcPlanes[0] = remainingData;
            srcPlanes[1] = remainingData + width * height;
            srcPlanes[2] = remainingData + width * height + (width * height / 2);
            strides[0] = width;
            strides[1] = width / 2;
            strides[2] = width / 2;
            break;
        case TJSAMP_420:
            srcPlanes[0] = remainingData;
            srcPlanes[1] = remainingData + width * height;
            srcPlanes[2] = remainingData + width * height + (width * height / 4);
            strides[0] = width;
            strides[1] = width / 2;
            strides[2] = width / 2;
            break;
        case TJSAMP_GRAY:
            srcPlanes[0] = remainingData;
            srcPlanes[1] = NULL;
            srcPlanes[2] = NULL;
            strides[0] = width;
            strides[1] = 0;
            strides[2] = 0;
            break;
        case TJSAMP_440:
            srcPlanes[0] = remainingData;
            srcPlanes[1] = remainingData + width * height;
            srcPlanes[2] = remainingData + width * height + (width * height / 2);
            strides[0] = width;
            strides[1] = width;
            strides[2] = width;
            break;
        case TJSAMP_411:
            srcPlanes[0] = remainingData;
            srcPlanes[1] = remainingData + width * height;
            srcPlanes[2] = remainingData + width * height + (width * height / 4);
            strides[0] = width;
            strides[1] = width / 4;
            strides[2] = width / 4;
            break;
        default:
            tjDestroy(handle);
            return;
    }
    
    unsigned char *dstBuf = (unsigned char *)malloc(pitch * height);
    if (!dstBuf) {
        tjDestroy(handle);
        return;
    }
    
    tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp,
                      dstBuf, width, pitch, height, pixelFormat, flags);
    
    free(dstBuf);
    tjDestroy(handle);
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    uint8_t selector = Data[0];
    const uint8_t *remainingData = Data + 1;
    size_t remainingSize = Size - 1;
    
    switch (selector % 4) {
        case 0:
            fuzz_tjCompress2(remainingData, remainingSize);
            break;
        case 1:
            fuzz_tjCompressFromYUV(remainingData, remainingSize);
            break;
        case 2:
            fuzz_tjCompress(remainingData, remainingSize);
            break;
        case 3:
            fuzz_tjDecodeYUVPlanes(remainingData, remainingSize);
            break;
    }
    
    return 0;
}