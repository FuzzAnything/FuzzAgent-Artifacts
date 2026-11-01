// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
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
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

static int parse_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) return 0;
    int val = *(const int *)(data + *offset);
    *offset += sizeof(int);
    return val;
}

static int clamp(int val, int min, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;

    size_t offset = 0;
    int initType = parse_int(Data, Size, &offset);
    int width = parse_int(Data, Size, &offset);
    int height = parse_int(Data, Size, &offset);
    int align = parse_int(Data, Size, &offset);
    int subsamp = parse_int(Data, Size, &offset);
    int pixelFormat = parse_int(Data, Size, &offset);

    // Clamp and sanitize inputs
    width = clamp(width, 1, 8192);
    height = clamp(height, 1, 8192);
    if (align <= 0) align = 1;
    // Ensure align is power of 2 for safety
    if ((align & (align - 1)) != 0) align = 1;
    
    // Initialize handle
    tjhandle handle = tj3Init(initType);
    if (!handle) return 0;

    // tj3Set calls (8 times as requested)
    // Using valid parameters from turbojpeg.h
    tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(handle, TJPARAM_QUALITY, clamp(parse_int(Data, Size, &offset), 1, 100));
    tj3Set(handle, TJPARAM_FASTDCT, clamp(parse_int(Data, Size, &offset), 0, 2));
    tj3Set(handle, TJPARAM_PROGRESSIVE, clamp(parse_int(Data, Size, &offset), 0, 1));
    tj3Set(handle, TJPARAM_FASTDCT, clamp(parse_int(Data, Size, &offset), 0, 2));
    tj3Set(handle, TJPARAM_PROGRESSIVE, clamp(parse_int(Data, Size, &offset), 0, 1));
    tj3Set(handle, TJPARAM_SUBSAMP, clamp(parse_int(Data, Size, &offset), 0, 3));
    tj3Set(handle, TJPARAM_QUALITY, clamp(parse_int(Data, Size, &offset), 1, 100));

    // tj3JPEGBufSize
    size_t jpegBufSize = tj3JPEGBufSize(width, height, subsamp);
    if (jpegBufSize == 0) {
        tj3Destroy(handle);
        return 0;
    }

    // tj3Alloc for JPEG buffer
    unsigned char *jpegBuf = (unsigned char *)tj3Alloc(jpegBufSize);
    if (!jpegBuf) {
        tj3Destroy(handle);
        return 0;
    }
    size_t jpegSize = jpegBufSize;

    // tj3YUVBufSize
    size_t yuvBufSize = tj3YUVBufSize(width, align, height, subsamp);
    if (yuvBufSize == 0) {
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }

    // tj3Alloc for YUV buffer
    unsigned char *yuvBuf = (unsigned char *)tj3Alloc(yuvBufSize);
    if (!yuvBuf) {
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }

    // tj3Set (2 more times)
    tj3Set(handle, TJPARAM_JPEGWIDTH, width);
    tj3Set(handle, TJPARAM_JPEGHEIGHT, height);

    // Prepare source buffer for tj3EncodeYUV8
    // We need a source RGB buffer. Size = width * height * 3 (RGB)
    size_t srcSize = (size_t)width * height * 3;
    unsigned char *srcBuf = (unsigned char *)malloc(srcSize);
    if (!srcBuf) {
        tj3Free(yuvBuf);
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    // Fill source buffer with fuzz data or pattern
    if (Size > offset) {
        size_t copyLen = Size - offset;
        if (copyLen > srcSize) copyLen = srcSize;
        memcpy(srcBuf, Data + offset, copyLen);
        if (copyLen < srcSize) {
            memset(srcBuf + copyLen, 0, srcSize - copyLen);
        }
    } else {
        memset(srcBuf, 0, srcSize);
    }

    // tj3EncodeYUV8
    int pitch = width * 3;
    tj3EncodeYUV8(handle, srcBuf, width, pitch, height, pixelFormat, yuvBuf, align);

    // tj3CompressFromYUV8
    tj3CompressFromYUV8(handle, yuvBuf, width, align, height, &jpegBuf, &jpegSize);

    // Cleanup
    tj3Free(jpegBuf);
    tj3Free(yuvBuf);
    free(srcBuf);
    tj3Destroy(handle);

    return 0;
}