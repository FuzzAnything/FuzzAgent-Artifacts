// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3DecodeYUVPlanes8 at turbojpeg.c:2508:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tj3Destroy(handle);
    }
}

static int fuzz_tj3YUVPlaneWidth(const uint8_t *Data, size_t Size) {
    int componentID = 0;
    int width = 0;
    int subsamp = 0;

    if (Size >= 3) {
        componentID = Data[0] % 3; // 0, 1, or 2
        width = (Data[1] << 8) | Data[2];
        if (width > 0 && width < 10000) width = (int)width;
        else width = 0;
        
        subsamp = Data[Size > 3 ? 3 : 0] % 5; // TJSAMP_444, 422, 420, 411, 440
    }

    int result = tj3YUVPlaneWidth(componentID, width, subsamp);
    (void)result;
    return 0;
}

static int fuzz_tjDecodeYUVPlanes(const uint8_t *Data, size_t Size) {
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;

    int width = 16, height = 16;
    int subsamp = TJSAMP_420;
    int pixelFormat = TJPF_RGB;
    int flags = 0;

    if (Size >= 8) {
        width = (Data[0] << 8) | Data[1];
        height = (Data[2] << 8) | Data[3];
        subsamp = Data[4] % 5;
        pixelFormat = Data[5] % 6;
        flags = Data[6] << 8 | Data[7];
    }

    if (width <= 0 || height <= 0) {
        cleanup_handle(handle);
        return 0;
    }

    unsigned long ySize = tjBufSizeYUV(width, height, subsamp);
    unsigned char *yuvBuf = malloc(ySize);
    if (!yuvBuf) {
        cleanup_handle(handle);
        return 0;
    }

    memset(yuvBuf, 0, ySize);

    const unsigned char *srcPlanes[3];
    int strides[3];

    int planeWidth = tj3YUVPlaneWidth(0, width, subsamp);
    int planeHeight = tj3YUVPlaneHeight(0, height, subsamp);
    
    if (planeWidth <= 0 || planeHeight <= 0) {
        free(yuvBuf);
        cleanup_handle(handle);
        return 0;
    }

    srcPlanes[0] = yuvBuf;
    strides[0] = planeWidth;

    if (subsamp != TJSAMP_GRAY) {
        int uWidth = tj3YUVPlaneWidth(1, width, subsamp);
        int uHeight = tj3YUVPlaneHeight(1, height, subsamp);
        if (uWidth > 0 && uHeight > 0) {
            srcPlanes[1] = yuvBuf + (planeWidth * planeHeight);
            strides[1] = uWidth;
            srcPlanes[2] = srcPlanes[1] + (uWidth * uHeight);
            strides[2] = uWidth;
        }
    }

    int dstWidth = width;
    int dstHeight = height;
    int dstPitch = dstWidth * tjPixelSize[pixelFormat];
    unsigned long dstSize = dstPitch * dstHeight;
    unsigned char *dstBuf = malloc(dstSize);
    if (!dstBuf) {
        free(yuvBuf);
        cleanup_handle(handle);
        return 0;
    }

    int ret = tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp, dstBuf, dstWidth, dstPitch, dstHeight, pixelFormat, flags);
    (void)ret;

    free(yuvBuf);
    free(dstBuf);
    cleanup_handle(handle);
    return 0;
}

static int fuzz_tjBufSizeYUV(const uint8_t *Data, size_t Size) {
    int width = 16, height = 16;
    int subsamp = TJSAMP_420;

    if (Size >= 4) {
        width = (Data[0] << 8) | Data[1];
        height = (Data[2] << 8) | Data[3];
        subsamp = Data[Size > 4 ? 4 : 0] % 5;
    }

    if (width <= 0 || height <= 0) return 0;

    unsigned long size = tjBufSizeYUV(width, height, subsamp);
    (void)size;
    return 0;
}

static int fuzz_tj3DecodeYUVPlanes8(const uint8_t *Data, size_t Size) {
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;

    int width = 16, height = 16;
    int subsamp = TJSAMP_420;
    int pixelFormat = TJPF_RGB;

    if (Size >= 6) {
        width = (Data[0] << 8) | Data[1];
        height = (Data[2] << 8) | Data[3];
        subsamp = Data[4] % 5;
        pixelFormat = Data[5] % 6;
    }

    if (width <= 0 || height <= 0) {
        cleanup_handle(handle);
        return 0;
    }

    unsigned long ySize = tjBufSizeYUV(width, height, subsamp);
    unsigned char *yuvBuf = malloc(ySize);
    if (!yuvBuf) {
        cleanup_handle(handle);
        return 0;
    }

    memset(yuvBuf, 0, ySize);

    const unsigned char *srcPlanes[3];
    int strides[3];

    int planeWidth = tj3YUVPlaneWidth(0, width, subsamp);
    int planeHeight = tj3YUVPlaneHeight(0, height, subsamp);
    
    if (planeWidth <= 0 || planeHeight <= 0) {
        free(yuvBuf);
        cleanup_handle(handle);
        return 0;
    }

    srcPlanes[0] = yuvBuf;
    strides[0] = planeWidth;

    if (subsamp != TJSAMP_GRAY) {
        int uWidth = tj3YUVPlaneWidth(1, width, subsamp);
        int uHeight = tj3YUVPlaneHeight(1, height, subsamp);
        if (uWidth > 0 && uHeight > 0) {
            srcPlanes[1] = yuvBuf + (planeWidth * planeHeight);
            strides[1] = uWidth;
            srcPlanes[2] = srcPlanes[1] + (uWidth * uHeight);
            strides[2] = uWidth;
        }
    }

    int dstPitch = width * tjPixelSize[pixelFormat];
    unsigned long dstSize = dstPitch * height;
    unsigned char *dstBuf = malloc(dstSize);
    if (!dstBuf) {
        free(yuvBuf);
        cleanup_handle(handle);
        return 0;
    }

    int ret = tj3DecodeYUVPlanes8(handle, srcPlanes, strides, dstBuf, width, dstPitch, height, pixelFormat);
    (void)ret;

    free(yuvBuf);
    free(dstBuf);
    cleanup_handle(handle);
    return 0;
}

static int fuzz_tjDecodeYUV(const uint8_t *Data, size_t Size) {
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;

    int width = 16, height = 16;
    int subsamp = TJSAMP_420;
    int pixelFormat = TJPF_RGB;
    int align = 1;
    int flags = 0;

    if (Size >= 8) {
        width = (Data[0] << 8) | Data[1];
        height = (Data[2] << 8) | Data[3];
        subsamp = Data[4] % 5;
        pixelFormat = Data[5] % 6;
        align = Data[6] % 4 + 1;
        flags = Data[7];
    }

    if (width <= 0 || height <= 0) {
        cleanup_handle(handle);
        return 0;
    }

    // Calculate size with alignment padding
    unsigned long ySize = tjBufSizeYUV(width, height, subsamp);
    unsigned char *srcBuf = malloc(ySize);
    if (!srcBuf) {
        cleanup_handle(handle);
        return 0;
    }
    memset(srcBuf, 0, ySize);

    int dstPitch = width * tjPixelSize[pixelFormat];
    unsigned long dstSize = dstPitch * height;
    unsigned char *dstBuf = malloc(dstSize);
    if (!dstBuf) {
        free(srcBuf);
        cleanup_handle(handle);
        return 0;
    }

    int ret = tjDecodeYUV(handle, srcBuf, align, subsamp, dstBuf, width, dstPitch, height, pixelFormat, flags);
    (void)ret;

    free(srcBuf);
    free(dstBuf);
    cleanup_handle(handle);
    return 0;
}

static int fuzz_tjDecompressToYUVPlanes(const uint8_t *Data, size_t Size) {
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;

    if (Size < 2) {
        cleanup_handle(handle);
        return 0;
    }

    unsigned long jpegSize = Size;
    const unsigned char *jpegBuf = Data;

    int width = 16, height = 16;
    int flags = 0;

    if (Size >= 6) {
        width = (Data[0] << 8) | Data[1];
        height = (Data[2] << 8) | Data[3];
        flags = (Data[4] << 8) | Data[5];
    }

    unsigned char *dstPlanes[3];
    int strides[3];
    
    // Allocate planes
    // We use a dummy size allocation strategy based on worst case 4:4:4
    unsigned long maxPlaneSize = (unsigned long)width * height * 3;
    unsigned char *planeMem = malloc(maxPlaneSize);
    if (!planeMem) {
        cleanup_handle(handle);
        return 0;
    }

    dstPlanes[0] = planeMem;
    strides[0] = width;
    
    if (width > 0 && height > 0) {
        dstPlanes[1] = planeMem + (width * height);
        strides[1] = width;
        dstPlanes[2] = planeMem + (width * height * 2);
        strides[2] = width;
    }

    int ret = tjDecompressToYUVPlanes(handle, jpegBuf, jpegSize, dstPlanes, width, strides, height, flags);
    (void)ret;

    free(planeMem);
    cleanup_handle(handle);
    return 0;
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    int op = Data[0] % 6;

    switch (op) {
        case 0:
            fuzz_tj3YUVPlaneWidth(Data, Size);
            break;
        case 1:
            fuzz_tjDecodeYUVPlanes(Data, Size);
            break;
        case 2:
            fuzz_tjBufSizeYUV(Data, Size);
            break;
        case 3:
            fuzz_tj3DecodeYUVPlanes8(Data, Size);
            break;
        case 4:
            fuzz_tjDecodeYUV(Data, Size);
            break;
        case 5:
            fuzz_tjDecompressToYUVPlanes(Data, Size);
            break;
        default:
            break;
    }

    return 0;
}