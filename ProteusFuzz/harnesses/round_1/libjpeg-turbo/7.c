// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

static int fuzz_tj3Alloc(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    size_t bytes = (Size > 1024 * 1024) ? 1024 * 1024 : Size;
    void *ptr = tj3Alloc(bytes);
    if (ptr) {
        memset(ptr, 0xAB, bytes);
        tj3Free(ptr);
    }
    return 0;
}

static int fuzz_tj3Init(const uint8_t *Data, size_t Size) {
    int initType = (Size > 0) ? Data[0] % 3 : 0;
    tjhandle handle = tj3Init(initType);
    if (handle) {
        // Clean up the handle to prevent leaks
        tj3Destroy(handle);
    }
    return 0;
}

static int fuzz_tj3Set(const uint8_t *Data, size_t Size) {
    int initType = (Size > 0) ? Data[0] % 3 : 0;
    tjhandle handle = tj3Init(initType);
    if (!handle) return 0;

    int param = (Size > 1) ? Data[1] % 100 : 0;
    int value = (Size > 2) ? Data[2] : 0;
    int ret = tj3Set(handle, param, value);
    (void)ret;

    tj3Destroy(handle);
    return 0;
}

static int fuzz_tj3JPEGBufSize(const uint8_t *Data, size_t Size) {
    int width = 0, height = 0, subsamp = 0;
    if (Size >= 4) {
        width = (Data[0] << 8) | Data[1];
        height = (Data[2] << 8) | Data[3];
    }
    if (Size > 4) {
        subsamp = Data[4] % 5;
    }
    size_t size = tj3JPEGBufSize(width, height, subsamp);
    (void)size;
    return 0;
}

static int fuzz_tj3Compress16(const uint8_t *Data, size_t Size) {
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return 0;

    int width = (Size > 0) ? Data[0] : 16;
    int height = (Size > 1) ? Data[1] : 16;
    int pixelFormat = (Size > 2) ? Data[2] % 5 : 0;
    
    // Ensure minimum dimensions to avoid trivial failures
    if (width == 0) width = 16;
    if (height == 0) height = 16;

    int pitch = width * 2; // 16-bit samples
    size_t srcSize = (size_t)height * pitch;
    if (srcSize == 0 || srcSize > 1024 * 1024) srcSize = 1024;

    unsigned short *srcBuf = (unsigned short *)tj3Alloc(srcSize);
    if (!srcBuf) {
        tj3Destroy(handle);
        return 0;
    }

    // Fill with fuzz data
    if (Size > 0) {
        size_t copyLen = (Size < srcSize) ? Size : srcSize;
        memcpy(srcBuf, Data, copyLen);
    }

    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;

    int ret = tj3Compress16(handle, srcBuf, width, pitch, height, pixelFormat, &jpegBuf, &jpegSize);
    (void)ret;

    if (jpegBuf) {
        tj3Free(jpegBuf);
    }
    tj3Free(srcBuf);
    tj3Destroy(handle);
    return 0;
}

static int fuzz_tj3CompressFromYUV8(const uint8_t *Data, size_t Size) {
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return 0;

    int width = (Size > 0) ? Data[0] : 16;
    int height = (Size > 1) ? Data[1] : 16;
    int align = 1;
    if (Size > 2) {
        int a = Data[2];
        if (a > 0 && a <= 32) align = a;
    }
    
    // Ensure dimensions are valid
    if (width == 0) width = 16;
    if (height == 0) height = 16;

    // Calculate YUV buffer size roughly (simplified for fuzzing)
    size_t ySize = (size_t)width * height;
    size_t uvSize = (size_t)(width / 2) * (height / 2) * 2;
    size_t totalSize = ySize + uvSize;
    
    if (totalSize == 0 || totalSize > 1024 * 1024) totalSize = 1024;

    unsigned char *srcBuf = (unsigned char *)tj3Alloc(totalSize);
    if (!srcBuf) {
        tj3Destroy(handle);
        return 0;
    }

    if (Size > 0) {
        size_t copyLen = (Size < totalSize) ? Size : totalSize;
        memcpy(srcBuf, Data, copyLen);
    }

    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;

    int ret = tj3CompressFromYUV8(handle, srcBuf, width, align, height, &jpegBuf, &jpegSize);
    (void)ret;

    if (jpegBuf) {
        tj3Free(jpegBuf);
    }
    tj3Free(srcBuf);
    tj3Destroy(handle);
    return 0;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    int mode = Data[0] % 6;

    switch (mode) {
        case 0:
            fuzz_tj3Alloc(Data, Size);
            break;
        case 1:
            fuzz_tj3Init(Data, Size);
            break;
        case 2:
            fuzz_tj3Set(Data, Size);
            break;
        case 3:
            fuzz_tj3JPEGBufSize(Data, Size);
            break;
        case 4:
            fuzz_tj3Compress16(Data, Size);
            break;
        case 5:
            fuzz_tj3CompressFromYUV8(Data, Size);
            break;
        default:
            break;
    }

    return 0;
}