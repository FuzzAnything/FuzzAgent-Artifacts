// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int fuzz_tjEncodeYUV3(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = (Data[0] % 1000) + 1;
    int height = (Data[1] % 1000) + 1;
    int pixelFormat = Data[2] % 6; 
    int subsamp = Data[3] % 4;     
    int align = 1 << (Data[4] % 5); 
    int flags = Data[5] % 8;

    int pitch = width * tjPixelSize[pixelFormat];
    size_t srcSize = (size_t)height * pitch;
    size_t dstSize = tj3YUVBufSize(width, align, height, subsamp);

    if (Size < srcSize + dstSize + 10) {
        tjDestroy(handle);
        return 0;
    }

    // Allocate separate buffers to avoid overwriting const input
    unsigned char *srcBuf = malloc(srcSize);
    unsigned char *dstBuf = malloc(dstSize);
    
    if (!srcBuf || !dstBuf) {
        free(srcBuf);
        free(dstBuf);
        tjDestroy(handle);
        return 0;
    }

    // Copy input data to source buffer
    memcpy(srcBuf, Data + 6, srcSize);

    int ret = tjEncodeYUV3(handle, srcBuf, width, pitch, height, pixelFormat,
                           dstBuf, align, subsamp, flags);
    
    free(srcBuf);
    free(dstBuf);
    tjDestroy(handle);
    return ret;
}

static int fuzz_tjEncodeYUV2(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = (Data[0] % 1000) + 1;
    int height = (Data[1] % 1000) + 1;
    int pixelFormat = Data[2] % 6;
    int subsamp = Data[3] % 4;
    int flags = Data[4] % 8;

    int pitch = width * tjPixelSize[pixelFormat];
    size_t srcSize = (size_t)height * pitch;
    size_t dstSize = tj3YUVBufSize(width, 4, height, subsamp); 

    if (Size < srcSize + dstSize + 10) {
        tjDestroy(handle);
        return 0;
    }

    unsigned char *srcBuf = malloc(srcSize);
    unsigned char *dstBuf = malloc(dstSize);

    if (!srcBuf || !dstBuf) {
        free(srcBuf);
        free(dstBuf);
        tjDestroy(handle);
        return 0;
    }

    memcpy(srcBuf, Data + 5, srcSize);

    int ret = tjEncodeYUV2(handle, srcBuf, width, pitch, height, pixelFormat,
                           dstBuf, subsamp, flags);

    free(srcBuf);
    free(dstBuf);
    tjDestroy(handle);
    return ret;
}

static int fuzz_tjEncodeYUVPlanes(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = (Data[0] % 1000) + 1;
    int height = (Data[1] % 1000) + 1;
    int pixelFormat = Data[2] % 6;
    int subsamp = Data[3] % 4;
    int flags = Data[4] % 8;

    int pitch = width * tjPixelSize[pixelFormat];
    size_t srcSize = (size_t)height * pitch;

    size_t ySize = tj3YUVPlaneSize(0, width, 0, height, subsamp);
    size_t uSize = tj3YUVPlaneSize(1, width, 0, height, subsamp);
    size_t vSize = tj3YUVPlaneSize(2, width, 0, height, subsamp);
    size_t totalDstSize = ySize + uSize + vSize;

    if (Size < srcSize + totalDstSize + 20) {
        tjDestroy(handle);
        return 0;
    }

    unsigned char *srcBuf = malloc(srcSize);
    unsigned char *yPlane = malloc(ySize);
    unsigned char *uPlane = malloc(uSize);
    unsigned char *vPlane = malloc(vSize);

    if (!srcBuf || !yPlane || !uPlane || !vPlane) {
        free(srcBuf);
        free(yPlane);
        free(uPlane);
        free(vPlane);
        tjDestroy(handle);
        return 0;
    }

    memcpy(srcBuf, Data + 5, srcSize);

    unsigned char *dstPlanes[3] = {yPlane, uPlane, vPlane};
    int strides[3] = {0, 0, 0}; 

    int ret = tjEncodeYUVPlanes(handle, srcBuf, width, pitch, height,
                                pixelFormat, dstPlanes, strides, subsamp, flags);

    free(srcBuf);
    free(yPlane);
    free(uPlane);
    free(vPlane);
    tjDestroy(handle);
    return ret;
}

static int fuzz_tj3EncodeYUVPlanes8(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = (Data[0] % 1000) + 1;
    int height = (Data[1] % 1000) + 1;
    int pixelFormat = Data[2] % 6;
    int subsamp = 0; 

    int pitch = width * tjPixelSize[pixelFormat];
    size_t srcSize = (size_t)height * pitch;

    size_t ySize = tj3YUVPlaneSize(0, width, 0, height, subsamp);
    size_t uSize = tj3YUVPlaneSize(1, width, 0, height, subsamp);
    size_t vSize = tj3YUVPlaneSize(2, width, 0, height, subsamp);
    size_t totalDstSize = ySize + uSize + vSize;

    if (Size < srcSize + totalDstSize + 10) {
        tjDestroy(handle);
        return 0;
    }

    unsigned char *srcBuf = malloc(srcSize);
    unsigned char *yPlane = malloc(ySize);
    unsigned char *uPlane = malloc(uSize);
    unsigned char *vPlane = malloc(vSize);

    if (!srcBuf || !yPlane || !uPlane || !vPlane) {
        free(srcBuf);
        free(yPlane);
        free(uPlane);
        free(vPlane);
        tjDestroy(handle);
        return 0;
    }

    memcpy(srcBuf, Data + 3, srcSize);

    unsigned char *dstPlanes[3] = {yPlane, uPlane, vPlane};
    int strides[3] = {0, 0, 0};

    int ret = tj3EncodeYUVPlanes8(handle, srcBuf, width, pitch, height,
                                  pixelFormat, dstPlanes, strides);

    free(srcBuf);
    free(yPlane);
    free(uPlane);
    free(vPlane);
    tjDestroy(handle);
    return ret;
}

static int fuzz_tj3EncodeYUV8(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = (Data[0] % 1000) + 1;
    int height = (Data[1] % 1000) + 1;
    int pixelFormat = Data[2] % 6;
    int align = 1 << (Data[3] % 5); 
    int subsamp = 0; 

    int pitch = width * tjPixelSize[pixelFormat];
    size_t srcSize = (size_t)height * pitch;
    size_t dstSize = tj3YUVBufSize(width, align, height, subsamp);

    if (Size < srcSize + dstSize + 10) {
        tjDestroy(handle);
        return 0;
    }

    unsigned char *srcBuf = malloc(srcSize);
    unsigned char *dstBuf = malloc(dstSize);

    if (!srcBuf || !dstBuf) {
        free(srcBuf);
        free(dstBuf);
        tjDestroy(handle);
        return 0;
    }

    memcpy(srcBuf, Data + 4, srcSize);

    int ret = tj3EncodeYUV8(handle, srcBuf, width, pitch, height, pixelFormat,
                            dstBuf, align);

    free(srcBuf);
    free(dstBuf);
    tjDestroy(handle);
    return ret;
}

static int fuzz_tjEncodeYUV(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = (Data[0] % 1000) + 1;
    int height = (Data[1] % 1000) + 1;
    int pixelSize = (Data[2] % 5) + 1; 
    int subsamp = Data[3] % 4;
    int flags = Data[4] % 8;

    int pitch = width * pixelSize;
    size_t srcSize = (size_t)height * pitch;
    size_t dstSize = tj3YUVBufSize(width, 4, height, subsamp);

    if (Size < srcSize + dstSize + 10) {
        tjDestroy(handle);
        return 0;
    }

    unsigned char *srcBuf = malloc(srcSize);
    unsigned char *dstBuf = malloc(dstSize);

    if (!srcBuf || !dstBuf) {
        free(srcBuf);
        free(dstBuf);
        tjDestroy(handle);
        return 0;
    }

    memcpy(srcBuf, Data + 5, srcSize);

    int ret = tjEncodeYUV(handle, srcBuf, width, pitch, height, pixelSize,
                          dstBuf, subsamp, flags);

    free(srcBuf);
    free(dstBuf);
    tjDestroy(handle);
    return ret;
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    int funcChoice = Data[0] % 6;

    switch (funcChoice) {
        case 0:
            return fuzz_tjEncodeYUV3(Data, Size);
        case 1:
            return fuzz_tjEncodeYUV2(Data, Size);
        case 2:
            return fuzz_tjEncodeYUVPlanes(Data, Size);
        case 3:
            return fuzz_tj3EncodeYUVPlanes8(Data, Size);
        case 4:
            return fuzz_tj3EncodeYUV8(Data, Size);
        case 5:
            return fuzz_tjEncodeYUV(Data, Size);
        default:
            return 0;
    }
}