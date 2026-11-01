// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void fuzz_tjEncodeYUV3(const uint8_t *data, size_t size) {
    tjhandle handle = tjInitCompress();
    if (!handle) return;

    if (size < 20) {
        tjDestroy(handle);
        return;
    }

    int width = data[0] + 1;
    int height = data[1] + 1;
    int pixelFormat = data[2] % 4; 
    int subsamp = data[3] % 4;     
    int align = 1 << (data[4] % 4); 

    // Validate dimensions to prevent overflow and invalid states
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        tjDestroy(handle);
        return;
    }

    int pixelSize = tjPixelSize[pixelFormat];
    if (pixelSize <= 0) {
        tjDestroy(handle);
        return;
    }

    int pitch = width * pixelSize;
    size_t srcSize = (size_t)width * (size_t)height * pixelSize;
    
    // Ensure srcSize fits in available data
    if (srcSize > size - 20) {
        srcSize = size - 20;
        // Recalculate height to match available data to avoid buffer overflow
        if (srcSize > 0 && width > 0 && pixelSize > 0) {
            height = srcSize / ((size_t)width * pixelSize);
            if (height == 0) {
                tjDestroy(handle);
                return;
            }
            srcSize = (size_t)width * (size_t)height * pixelSize;
        } else {
            tjDestroy(handle);
            return;
        }
    }

    unsigned char *srcBuf = (unsigned char *)tjAlloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return;
    }
    memcpy(srcBuf, data + 20, srcSize);

    // Calculate destination buffer size
    size_t dstSize = tj3YUVBufSize(width, align, height, subsamp);
    if (dstSize == 0) {
        tjFree(srcBuf);
        tjDestroy(handle);
        return;
    }

    unsigned char *dstBuf = (unsigned char *)tjAlloc(dstSize);
    if (!dstBuf) {
        tjFree(srcBuf);
        tjDestroy(handle);
        return;
    }

    int flags = 0;
    if (data[5] & 0x01) flags |= TJFLAG_FASTDCT;
    if (data[5] & 0x02) flags |= TJFLAG_FASTUPSAMPLE;

    tjEncodeYUV3(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, align, subsamp, flags);

    tjFree(srcBuf);
    tjFree(dstBuf);
    tjDestroy(handle);
}

static void fuzz_tjEncodeYUV(const uint8_t *data, size_t size) {
    tjhandle handle = tjInitCompress();
    if (!handle) return;

    if (size < 15) {
        tjDestroy(handle);
        return;
    }

    int width = data[0] + 1;
    int height = data[1] + 1;
    int pixelSize = data[2] % 4 + 1; 
    int subsamp = data[3] % 4;
    
    // Validate dimensions
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        tjDestroy(handle);
        return;
    }

    int pitch = width * pixelSize;

    size_t srcSize = (size_t)width * (size_t)height * pixelSize;
    if (srcSize > size - 15) {
        srcSize = size - 15;
        // Adjust height to fit available data
        if (srcSize > 0 && width > 0 && pixelSize > 0) {
            height = srcSize / ((size_t)width * pixelSize);
            if (height == 0) {
                tjDestroy(handle);
                return;
            }
            srcSize = (size_t)width * (size_t)height * pixelSize;
        } else {
            tjDestroy(handle);
            return;
        }
    }

    unsigned char *srcBuf = (unsigned char *)tjAlloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return;
    }
    memcpy(srcBuf, data + 15, srcSize);

    // Use tjBufSize for legacy API, but ensure subsamp is valid
    size_t dstSize = tjBufSize(width, height, subsamp);
    if (dstSize == 0) {
        tjFree(srcBuf);
        tjDestroy(handle);
        return;
    }

    unsigned char *dstBuf = (unsigned char *)tjAlloc(dstSize);
    if (!dstBuf) {
        tjFree(srcBuf);
        tjDestroy(handle);
        return;
    }

    int flags = 0;
    if (data[4] & 0x01) flags |= TJFLAG_FASTDCT;

    tjEncodeYUV(handle, srcBuf, width, pitch, height, pixelSize, dstBuf, subsamp, flags);

    tjFree(srcBuf);
    tjFree(dstBuf);
    tjDestroy(handle);
}

static void fuzz_tj3EncodeYUV8(const uint8_t *data, size_t size) {
    tjhandle handle = tjInitCompress();
    if (!handle) return;

    if (size < 15) {
        tjDestroy(handle);
        return;
    }

    int width = data[0] + 1;
    int height = data[1] + 1;
    int pixelFormat = data[2] % 4;
    int align = 1 << (data[3] % 4);
    int subsamp = data[4] % 4; 

    // Validate dimensions
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        tjDestroy(handle);
        return;
    }

    int pixelSize = tjPixelSize[pixelFormat];
    if (pixelSize <= 0) {
        tjDestroy(handle);
        return;
    }

    int pitch = width * pixelSize;
    size_t srcSize = (size_t)width * (size_t)height * pixelSize;
    
    if (srcSize > size - 15) {
        srcSize = size - 15;
        if (srcSize > 0 && width > 0 && pixelSize > 0) {
            height = srcSize / ((size_t)width * pixelSize);
            if (height == 0) {
                tjDestroy(handle);
                return;
            }
            srcSize = (size_t)width * (size_t)height * pixelSize;
        } else {
            tjDestroy(handle);
            return;
        }
    }

    unsigned char *srcBuf = (unsigned char *)tjAlloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return;
    }
    memcpy(srcBuf, data + 15, srcSize);

    size_t dstSize = tj3YUVBufSize(width, align, height, subsamp);
    if (dstSize == 0) {
        tjFree(srcBuf);
        tjDestroy(handle);
        return;
    }

    unsigned char *dstBuf = (unsigned char *)tjAlloc(dstSize);
    if (!dstBuf) {
        tjFree(srcBuf);
        tjDestroy(handle);
        return;
    }

    tj3EncodeYUV8(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, align);

    tjFree(srcBuf);
    tjFree(dstBuf);
    tjDestroy(handle);
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    int mode = Data[0] % 3;

    switch (mode) {
        case 0:
            fuzz_tjEncodeYUV3(Data, Size);
            break;
        case 1:
            fuzz_tjEncodeYUV(Data, Size);
            break;
        case 2:
            fuzz_tj3EncodeYUV8(Data, Size);
            break;
    }

    return 0;
}