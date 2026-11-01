// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void cleanup_handle(tjhandle *handle) {
    if (handle && *handle) {
        tjDestroy(*handle);
        *handle = NULL;
    }
}

static void cleanup_buffer(unsigned char **buffer) {
    if (buffer && *buffer) {
        tjFree(*buffer);
        *buffer = NULL;
    }
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 6) {
        return 0;
    }

    tjhandle handle = tjInitCompress();
    if (!handle) {
        return 0;
    }

    // Extract parameters from fuzz input safely
    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelFormat = Data[2] % 5; // TJPF_GRAY, TJPF_RGB, TJPF_BGR, TJPF_CMYK, TJPF_YUV
    int subsamp = Data[3] % 3;     // TJSAMP_444, TJSAMP_422, TJSAMP_420
    int align = 1 << (Data[4] % 8); // Power of 2 alignment: 1, 2, 4, ..., 128
    int flags = Data[5] % 16;

    // Ensure minimum dimensions to avoid trivial cases
    if (width < 1) width = 1;
    if (height < 1) height = 1;

    // Calculate source buffer size
    int pixelSize = tjPixelSize[pixelFormat];
    if (pixelSize <= 0) {
        cleanup_handle(&handle);
        return 0;
    }
    
    int pitch = width * pixelSize;
    size_t srcSize = (size_t)pitch * height;
    
    // Allocate source buffer
    unsigned char *srcBuf = tjAlloc((int)srcSize);
    if (!srcBuf) {
        cleanup_handle(&handle);
        return 0;
    }

    // Fill source buffer with fuzz data
    size_t copyLen = (Size > srcSize) ? srcSize : Size;
    memcpy(srcBuf, Data, copyLen);
    if (copyLen < srcSize) {
        memset(srcBuf + copyLen, 0, srcSize - copyLen);
    }

    // Calculate destination buffer size for YUV
    int dstSize = tj3YUVBufSize(width, height, align, subsamp);
    if (dstSize <= 0) {
        cleanup_buffer(&srcBuf);
        cleanup_handle(&handle);
        return 0;
    }

    unsigned char *dstBuf = tjAlloc(dstSize);
    if (!dstBuf) {
        cleanup_buffer(&srcBuf);
        cleanup_handle(&handle);
        return 0;
    }

    // Fuzz tj3EncodeYUV8
    tj3EncodeYUV8(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, align);

    // Fuzz tjEncodeYUV3
    tjEncodeYUV3(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, align, subsamp, flags);

    // Fuzz tjEncodeYUV2
    tjEncodeYUV2(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, subsamp, flags);

    // Fuzz tjEncodeYUV (requires pixelSize)
    tjEncodeYUV(handle, srcBuf, width, pitch, height, pixelSize, dstBuf, subsamp, flags);

    // Cleanup
    cleanup_buffer(&dstBuf);
    cleanup_buffer(&srcBuf);
    cleanup_handle(&handle);

    return 0;
}