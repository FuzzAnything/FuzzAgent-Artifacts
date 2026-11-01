// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int fuzz_tjPlaneWidth(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;
    int componentID = Data[0];
    int width = (Data[1] << 8) | Data[2];
    int subsamp = Data[0] & 0x0F;
    int result = tjPlaneWidth(componentID, width, subsamp);
    return (result == -1) ? 0 : 1;
}

static int fuzz_tj3YUVPlaneHeight(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;
    int componentID = Data[0];
    int height = (Data[1] << 8) | Data[2];
    int subsamp = Data[0] & 0x0F;
    int result = tj3YUVPlaneHeight(componentID, height, subsamp);
    return (result == 0) ? 0 : 1;
}

static int fuzz_tjPlaneHeight(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;
    int componentID = Data[0];
    int height = (Data[1] << 8) | Data[2];
    int subsamp = Data[0] & 0x0F;
    int result = tjPlaneHeight(componentID, height, subsamp);
    return (result == -1) ? 0 : 1;
}

static int fuzz_tjEncodeYUV2(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int pixelFormat = Data[4] & 0x0F;
    int subsamp = Data[5] & 0x0F;
    int flags = (Data[6] << 8) | Data[7];

    if (width <= 0 || height <= 0) {
        tjDestroy(handle);
        return 0;
    }

    // Validate pixelFormat to prevent out-of-bounds access on tjPixelSize
    if (pixelFormat < 0 || pixelFormat > TJPF_XBGR) {
        tjDestroy(handle);
        return 0;
    }

    int pitch = width * tjPixelSize[pixelFormat];
    size_t srcSize = (size_t)pitch * (size_t)height;
    
    // Prevent integer overflow in allocation size
    if (srcSize == 0 || srcSize > 100 * 1024 * 1024) { 
        tjDestroy(handle);
        return 0;
    }

    unsigned char *srcBuf = malloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return 0;
    }
    memset(srcBuf, 0, srcSize);
    if (Size > 8) {
        size_t copySize = (Size - 8) < srcSize ? (Size - 8) : srcSize;
        memcpy(srcBuf, Data + 8, copySize);
    }

    // tjBufSizeYUV2 requires an alignment parameter (4th arg)
    int align = 4; 
    unsigned long yuvSize = tjBufSizeYUV2(width, align, height, subsamp);
    if (yuvSize == 0) {
        free(srcBuf);
        tjDestroy(handle);
        return 0;
    }
    
    unsigned char *dstBuf = malloc(yuvSize);
    if (!dstBuf) {
        free(srcBuf);
        tjDestroy(handle);
        return 0;
    }

    int result = tjEncodeYUV2(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, subsamp, flags);

    free(srcBuf);
    free(dstBuf);
    tjDestroy(handle);

    return (result == -1) ? 0 : 1;
}

static int fuzz_tjEncodeYUVPlanes(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int pixelFormat = Data[4] & 0x0F;
    int subsamp = Data[5] & 0x0F;
    int flags = (Data[6] << 8) | Data[7];
    int pitch = (Data[8] << 8) | Data[9];

    if (width <= 0 || height <= 0) {
        tjDestroy(handle);
        return 0;
    }

    // Validate pixelFormat
    if (pixelFormat < 0 || pixelFormat > TJPF_XBGR) {
        tjDestroy(handle);
        return 0;
    }

    if (pitch == 0) pitch = width * tjPixelSize[pixelFormat];

    size_t srcSize = (size_t)pitch * (size_t)height;
    if (srcSize == 0 || srcSize > 100 * 1024 * 1024) {
        tjDestroy(handle);
        return 0;
    }

    unsigned char *srcBuf = malloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return 0;
    }
    memset(srcBuf, 0, srcSize);
    if (Size > 10) {
        size_t copySize = (Size - 10) < srcSize ? (Size - 10) : srcSize;
        memcpy(srcBuf, Data + 10, copySize);
    }

    int planeWidths[3];
    int planeHeights[3];
    unsigned char *dstPlanes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};

    for (int i = 0; i < 3; i++) {
        planeWidths[i] = tjPlaneWidth(i, width, subsamp);
        planeHeights[i] = tjPlaneHeight(i, height, subsamp);
        if (planeWidths[i] <= 0 || planeHeights[i] <= 0) {
            for (int j = 0; j < i; j++) free(dstPlanes[j]);
            free(srcBuf);
            tjDestroy(handle);
            return 0;
        }
        size_t planeSize = (size_t)planeWidths[i] * (size_t)planeHeights[i];
        if (planeSize == 0 || planeSize > 100 * 1024 * 1024) {
            for (int j = 0; j < i; j++) free(dstPlanes[j]);
            free(srcBuf);
            tjDestroy(handle);
            return 0;
        }
        dstPlanes[i] = malloc(planeSize);
        if (!dstPlanes[i]) {
            for (int j = 0; j < i; j++) free(dstPlanes[j]);
            free(srcBuf);
            tjDestroy(handle);
            return 0;
        }
        strides[i] = planeWidths[i];
    }

    int result = tjEncodeYUVPlanes(handle, srcBuf, width, pitch, height, pixelFormat, dstPlanes, strides, subsamp, flags);

    for (int i = 0; i < 3; i++) free(dstPlanes[i]);
    free(srcBuf);
    tjDestroy(handle);

    return (result == -1) ? 0 : 1;
}

static int fuzz_tj3EncodeYUVPlanes8(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int pixelFormat = Data[4] & 0x0F;
    int pitch = (Data[5] << 8) | Data[6];
    int subsamp = Data[7] & 0x0F;

    if (width <= 0 || height <= 0) {
        tjDestroy(handle);
        return 0;
    }

    // Validate pixelFormat
    if (pixelFormat < 0 || pixelFormat > TJPF_XBGR) {
        tjDestroy(handle);
        return 0;
    }

    if (pitch == 0) pitch = width * tjPixelSize[pixelFormat];

    size_t srcSize = (size_t)pitch * (size_t)height;
    if (srcSize == 0 || srcSize > 100 * 1024 * 1024) {
        tjDestroy(handle);
        return 0;
    }

    unsigned char *srcBuf = malloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return 0;
    }
    memset(srcBuf, 0, srcSize);
    if (Size > 8) {
        size_t copySize = (Size - 8) < srcSize ? (Size - 8) : srcSize;
        memcpy(srcBuf, Data + 8, copySize);
    }

    int planeWidths[3];
    int planeHeights[3];
    unsigned char *dstPlanes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};

    for (int i = 0; i < 3; i++) {
        planeWidths[i] = tjPlaneWidth(i, width, subsamp);
        planeHeights[i] = tjPlaneHeight(i, height, subsamp);
        if (planeWidths[i] <= 0 || planeHeights[i] <= 0) {
            for (int j = 0; j < i; j++) free(dstPlanes[j]);
            free(srcBuf);
            tjDestroy(handle);
            return 0;
        }
        size_t planeSize = (size_t)planeWidths[i] * (size_t)planeHeights[i];
        if (planeSize == 0 || planeSize > 100 * 1024 * 1024) {
            for (int j = 0; j < i; j++) free(dstPlanes[j]);
            free(srcBuf);
            tjDestroy(handle);
            return 0;
        }
        dstPlanes[i] = malloc(planeSize);
        if (!dstPlanes[i]) {
            for (int j = 0; j < i; j++) free(dstPlanes[j]);
            free(srcBuf);
            tjDestroy(handle);
            return 0;
        }
        strides[i] = planeWidths[i];
    }

    int result = tj3EncodeYUVPlanes8(handle, srcBuf, width, pitch, height, pixelFormat, dstPlanes, strides);

    for (int i = 0; i < 3; i++) free(dstPlanes[i]);
    free(srcBuf);
    tjDestroy(handle);

    return (result == -1) ? 0 : 1;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    int funcChoice = Data[0] % 6;

    switch (funcChoice) {
        case 0:
            return fuzz_tjPlaneWidth(Data, Size);
        case 1:
            return fuzz_tj3YUVPlaneHeight(Data, Size);
        case 2:
            return fuzz_tjPlaneHeight(Data, Size);
        case 3:
            return fuzz_tjEncodeYUV2(Data, Size);
        case 4:
            return fuzz_tjEncodeYUVPlanes(Data, Size);
        case 5:
            return fuzz_tj3EncodeYUVPlanes8(Data, Size);
    }
    return 0;
}