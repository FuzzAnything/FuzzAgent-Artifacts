// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static int fuzz_tjEncodeYUV8(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    if (Size < 16) {
        tjDestroy(handle);
        return 0;
    }

    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelFormat = Data[2] % 4; // TJPF_RGB, TJPF_BGR, TJPF_GRAY, TJPF_CMYK
    int align = 1 << (Data[3] % 8); // Power of 2 alignment: 1, 2, 4, ..., 128
    int subsamp = Data[4] % 4; // TJSAMP_444, TJSAMP_422, TJSAMP_420, TJSAMP_GRAY

    // Ensure dimensions are reasonable to avoid huge allocations
    if (width > 4096 || height > 4096) {
        tjDestroy(handle);
        return 0;
    }

    int pixelSize = tjPixelSize[pixelFormat];
    int pitch = width * pixelSize;
    size_t srcSize = (size_t)height * pitch;

    // Prepare source buffer
    unsigned char *srcBuf = NULL;
    if (Size > srcSize) {
        srcBuf = (unsigned char *)Data;
    } else {
        // Allocate and fill with pattern if input is too small
        srcBuf = (unsigned char *)malloc(srcSize);
        if (!srcBuf) {
            tjDestroy(handle);
            return 0;
        }
        memset(srcBuf, 0xAB, srcSize);
    }

    // Calculate destination buffer size
    int dstSize = tj3YUVBufSize(width, align, height, subsamp);
    if (dstSize <= 0) {
        if (srcBuf != (unsigned char *)Data) free(srcBuf);
        tjDestroy(handle);
        return 0;
    }

    unsigned char *dstBuf = tjAlloc(dstSize);
    if (!dstBuf) {
        if (srcBuf != (unsigned char *)Data) free(srcBuf);
        tjDestroy(handle);
        return 0;
    }

    int result = tj3EncodeYUV8(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, align);

    tjFree(dstBuf);
    if (srcBuf != (unsigned char *)Data) free(srcBuf);
    tjDestroy(handle);

    return result;
}

static int fuzz_tjEncodeYUV2(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    if (Size < 12) {
        tjDestroy(handle);
        return 0;
    }

    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelFormat = Data[2] % 4;
    int subsamp = Data[3] % 4;
    int flags = Data[4] % 8;

    if (width > 4096 || height > 4096) {
        tjDestroy(handle);
        return 0;
    }

    int pixelSize = tjPixelSize[pixelFormat];
    int pitch = width * pixelSize;
    size_t srcSize = (size_t)height * pitch;

    unsigned char *srcBuf = NULL;
    if (Size > srcSize) {
        srcBuf = (unsigned char *)Data;
    } else {
        srcBuf = (unsigned char *)malloc(srcSize);
        if (!srcBuf) {
            tjDestroy(handle);
            return 0;
        }
        memset(srcBuf, 0xCD, srcSize);
    }

    int dstSize = tjBufSize(width, height, subsamp);
    if (dstSize <= 0) {
        if (srcBuf != (unsigned char *)Data) free(srcBuf);
        tjDestroy(handle);
        return 0;
    }

    unsigned char *dstBuf = tjAlloc(dstSize);
    if (!dstBuf) {
        if (srcBuf != (unsigned char *)Data) free(srcBuf);
        tjDestroy(handle);
        return 0;
    }

    int result = tjEncodeYUV2(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, subsamp, flags);

    tjFree(dstBuf);
    if (srcBuf != (unsigned char *)Data) free(srcBuf);
    tjDestroy(handle);

    return result;
}

static int fuzz_tjEncodeYUV3(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    if (Size < 12) {
        tjDestroy(handle);
        return 0;
    }

    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelFormat = Data[2] % 4;
    int align = 1 << (Data[3] % 8);
    int subsamp = Data[4] % 4;
    int flags = Data[5] % 8;

    if (width > 4096 || height > 4096) {
        tjDestroy(handle);
        return 0;
    }

    int pixelSize = tjPixelSize[pixelFormat];
    int pitch = width * pixelSize;
    size_t srcSize = (size_t)height * pitch;

    unsigned char *srcBuf = NULL;
    if (Size > srcSize) {
        srcBuf = (unsigned char *)Data;
    } else {
        srcBuf = (unsigned char *)malloc(srcSize);
        if (!srcBuf) {
            tjDestroy(handle);
            return 0;
        }
        memset(srcBuf, 0xEF, srcSize);
    }

    int dstSize = tj3YUVBufSize(width, align, height, subsamp);
    if (dstSize <= 0) {
        if (srcBuf != (unsigned char *)Data) free(srcBuf);
        tjDestroy(handle);
        return 0;
    }

    unsigned char *dstBuf = tjAlloc(dstSize);
    if (!dstBuf) {
        if (srcBuf != (unsigned char *)Data) free(srcBuf);
        tjDestroy(handle);
        return 0;
    }

    int result = tjEncodeYUV3(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, align, subsamp, flags);

    tjFree(dstBuf);
    if (srcBuf != (unsigned char *)Data) free(srcBuf);
    tjDestroy(handle);

    return result;
}

static int fuzz_tjEncodeYUV(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    if (Size < 10) {
        tjDestroy(handle);
        return 0;
    }

    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelSize = Data[2] % 5; // 1 to 4 bytes per pixel
    int subsamp = Data[3] % 4;
    int flags = Data[4] % 8;

    if (width > 4096 || height > 4096) {
        tjDestroy(handle);
        return 0;
    }

    int pitch = width * pixelSize;
    size_t srcSize = (size_t)height * pitch;

    unsigned char *srcBuf = NULL;
    if (Size > srcSize) {
        srcBuf = (unsigned char *)Data;
    } else {
        srcBuf = (unsigned char *)malloc(srcSize);
        if (!srcBuf) {
            tjDestroy(handle);
            return 0;
        }
        memset(srcBuf, 0x12, srcSize);
    }

    int dstSize = tjBufSize(width, height, subsamp);
    if (dstSize <= 0) {
        if (srcBuf != (unsigned char *)Data) free(srcBuf);
        tjDestroy(handle);
        return 0;
    }

    unsigned char *dstBuf = tjAlloc(dstSize);
    if (!dstBuf) {
        if (srcBuf != (unsigned char *)Data) free(srcBuf);
        tjDestroy(handle);
        return 0;
    }

    int result = tjEncodeYUV(handle, srcBuf, width, pitch, height, pixelSize, dstBuf, subsamp, flags);

    tjFree(dstBuf);
    if (srcBuf != (unsigned char *)Data) free(srcBuf);
    tjDestroy(handle);

    return result;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    int choice = Data[0] % 4;

    switch (choice) {
        case 0:
            fuzz_tjEncodeYUV8(Data, Size);
            break;
        case 1:
            fuzz_tjEncodeYUV2(Data, Size);
            break;
        case 2:
            fuzz_tjEncodeYUV3(Data, Size);
            break;
        case 3:
            fuzz_tjEncodeYUV(Data, Size);
            break;
    }

    return 0;
}