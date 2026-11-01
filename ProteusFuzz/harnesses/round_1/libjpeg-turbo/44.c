// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int fuzz_tjEncodeYUV3(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    if (Size < 16) {
        tjDestroy(handle);
        return 0;
    }

    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelFormat = Data[2] % 4; 
    int subsamp = Data[3] % 3;     
    int align = Data[4] & 0x07;    
    if (align == 0) align = 1;
    int flags = Data[5];

    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        tjDestroy(handle);
        return 0;
    }

    int pitch = width * tjPixelSize[pixelFormat];
    size_t srcSize = (size_t)pitch * height;
    size_t dstSize = tj3YUVBufSize(width, align, height, subsamp);

    if (srcSize == 0 || dstSize == 0 || dstSize < height) {
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

    memset(srcBuf, 0, srcSize);
    if (Size > 0) {
        size_t copyLen = (Size < srcSize) ? Size : srcSize;
        memcpy(srcBuf, Data, copyLen);
    }

    tjEncodeYUV3(handle, srcBuf, width, pitch, height, pixelFormat,
                 dstBuf, align, subsamp, flags);

    free(srcBuf);
    free(dstBuf);
    tjDestroy(handle);
    return 0;
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
    int subsamp = Data[3] % 3;
    int flags = Data[4];

    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        tjDestroy(handle);
        return 0;
    }

    int pitch = width * tjPixelSize[pixelFormat];
    size_t srcSize = (size_t)pitch * height;
    size_t dstSize = tj3YUVBufSize(width, 4, height, subsamp);

    if (srcSize == 0 || dstSize == 0 || dstSize < height) {
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

    memset(srcBuf, 0, srcSize);
    if (Size > 0) {
        size_t copyLen = (Size < srcSize) ? Size : srcSize;
        memcpy(srcBuf, Data, copyLen);
    }

    tjEncodeYUV2(handle, srcBuf, width, pitch, height, pixelFormat,
                 dstBuf, subsamp, flags);

    free(srcBuf);
    free(dstBuf);
    tjDestroy(handle);
    return 0;
}

static int fuzz_tjEncodeYUVPlanes(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    if (Size < 12) {
        tjDestroy(handle);
        return 0;
    }

    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelFormat = Data[2] % 4;
    int subsamp = Data[3] % 3;
    int flags = Data[4];

    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        tjDestroy(handle);
        return 0;
    }

    int pitch = width * tjPixelSize[pixelFormat];
    size_t srcSize = (size_t)pitch * height;
    
    int yStride = width;
    int uStride = width;
    int vStride = width;
    
    if (subsamp == TJSAMP_444) {
        yStride = width;
        uStride = width;
        vStride = width;
    } else if (subsamp == TJSAMP_422) {
        yStride = width;
        uStride = (width + 1) / 2;
        vStride = (width + 1) / 2;
    } else if (subsamp == TJSAMP_420) {
        yStride = width;
        uStride = (width + 1) / 2;
        vStride = (width + 1) / 2;
    } else {
        yStride = width;
        uStride = width;
        vStride = width;
    }

    if (yStride < 1) yStride = 1;
    if (uStride < 1) uStride = 1;
    if (vStride < 1) vStride = 1;

    int yHeight = height;
    int uHeight = height;
    int vHeight = height;
    
    if (subsamp == TJSAMP_420) {
        uHeight = (height + 1) / 2;
        vHeight = (height + 1) / 2;
    }
    if (uHeight < 1) uHeight = 1;
    if (vHeight < 1) vHeight = 1;

    // tj3YUVPlaneSize signature: (componentID, width, stride, height, subsamp)
    // Component IDs: 0=Y, 1=U, 2=V
    size_t ySize = tj3YUVPlaneSize(0, width, yStride, height, subsamp);
    size_t uSize = tj3YUVPlaneSize(1, width, uStride, height, subsamp);
    size_t vSize = tj3YUVPlaneSize(2, width, vStride, height, subsamp);

    if (srcSize == 0 || ySize == 0) {
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

    memset(srcBuf, 0, srcSize);
    if (Size > 0) {
        size_t copyLen = (Size < srcSize) ? Size : srcSize;
        memcpy(srcBuf, Data, copyLen);
    }

    unsigned char *dstPlanes[3] = {yPlane, uPlane, vPlane};
    int strides[3] = {yStride, uStride, vStride};

    tjEncodeYUVPlanes(handle, srcBuf, width, pitch, height, pixelFormat,
                      dstPlanes, strides, subsamp, flags);

    free(srcBuf);
    free(yPlane);
    free(uPlane);
    free(vPlane);
    tjDestroy(handle);
    return 0;
}

static int fuzz_tj3EncodeYUVPlanes8(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    if (Size < 10) {
        tjDestroy(handle);
        return 0;
    }

    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelFormat = Data[2] % 4;
    int subsamp = Data[3] % 3;

    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        tjDestroy(handle);
        return 0;
    }

    int pitch = width * tjPixelSize[pixelFormat];
    size_t srcSize = (size_t)pitch * height;

    int yStride = width;
    int uStride = width;
    int vStride = width;
    if (subsamp == 1) { // 4:2:2
        uStride = (width + 1) / 2;
        vStride = (width + 1) / 2;
    } else if (subsamp == 2) { // 4:2:0
        uStride = (width + 1) / 2;
        vStride = (width + 1) / 2;
    }
    
    if (uStride < 1) uStride = 1;
    if (vStride < 1) vStride = 1;

    int yHeight = height;
    int uHeight = height;
    int vHeight = height;
    if (subsamp == 2) { // 4:2:0
        uHeight = (height + 1) / 2;
        vHeight = (height + 1) / 2;
    }
    if (uHeight < 1) uHeight = 1;
    if (vHeight < 1) vHeight = 1;

    // tj3YUVPlaneSize signature: (componentID, width, stride, height, subsamp)
    size_t ySize = tj3YUVPlaneSize(0, width, yStride, height, subsamp);
    size_t uSize = tj3YUVPlaneSize(1, width, uStride, height, subsamp);
    size_t vSize = tj3YUVPlaneSize(2, width, vStride, height, subsamp);

    if (srcSize == 0 || ySize == 0) {
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

    memset(srcBuf, 0, srcSize);
    if (Size > 0) {
        size_t copyLen = (Size < srcSize) ? Size : srcSize;
        memcpy(srcBuf, Data, copyLen);
    }

    unsigned char *dstPlanes[3] = {yPlane, uPlane, vPlane};
    int strides[3] = {yStride, uStride, vStride};

    tj3EncodeYUVPlanes8(handle, srcBuf, width, pitch, height, pixelFormat,
                        dstPlanes, strides);

    free(srcBuf);
    free(yPlane);
    free(uPlane);
    free(vPlane);
    tjDestroy(handle);
    return 0;
}

static int fuzz_tj3EncodeYUV8(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    if (Size < 10) {
        tjDestroy(handle);
        return 0;
    }

    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelFormat = Data[2] % 4;
    int subsamp = Data[3] % 3;
    int align = Data[4] & 0x07;
    if (align == 0) align = 1;

    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        tjDestroy(handle);
        return 0;
    }

    int pitch = width * tjPixelSize[pixelFormat];
    size_t srcSize = (size_t)pitch * height;
    size_t dstSize = tj3YUVBufSize(width, align, height, subsamp);

    if (srcSize == 0 || dstSize == 0 || dstSize < height) {
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

    memset(srcBuf, 0, srcSize);
    if (Size > 0) {
        size_t copyLen = (Size < srcSize) ? Size : srcSize;
        memcpy(srcBuf, Data, copyLen);
    }

    tj3EncodeYUV8(handle, srcBuf, width, pitch, height, pixelFormat,
                  dstBuf, align);

    free(srcBuf);
    free(dstBuf);
    tjDestroy(handle);
    return 0;
}

static int fuzz_tjEncodeYUV(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    if (Size < 12) {
        tjDestroy(handle);
        return 0;
    }

    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelSize = Data[2] % 5; 
    int subsamp = Data[3] % 3;
    int flags = Data[4];

    if (pixelSize == 0) pixelSize = 3; 

    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        tjDestroy(handle);
        return 0;
    }

    int pitch = width * pixelSize;
    size_t srcSize = (size_t)pitch * height;
    size_t dstSize = tj3YUVBufSize(width, 4, height, subsamp);

    if (srcSize == 0 || dstSize == 0 || dstSize < height) {
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

    memset(srcBuf, 0, srcSize);
    if (Size > 0) {
        size_t copyLen = (Size < srcSize) ? Size : srcSize;
        memcpy(srcBuf, Data, copyLen);
    }

    tjEncodeYUV(handle, srcBuf, width, pitch, height, pixelSize,
                dstBuf, subsamp, flags);

    free(srcBuf);
    free(dstBuf);
    tjDestroy(handle);
    return 0;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    int choice = Data[0] % 6;

    switch (choice) {
        case 0:
            fuzz_tjEncodeYUV3(Data, Size);
            break;
        case 1:
            fuzz_tjEncodeYUV2(Data, Size);
            break;
        case 2:
            fuzz_tjEncodeYUVPlanes(Data, Size);
            break;
        case 3:
            fuzz_tj3EncodeYUVPlanes8(Data, Size);
            break;
        case 4:
            fuzz_tj3EncodeYUV8(Data, Size);
            break;
        case 5:
            fuzz_tjEncodeYUV(Data, Size);
            break;
    }

    return 0;
}