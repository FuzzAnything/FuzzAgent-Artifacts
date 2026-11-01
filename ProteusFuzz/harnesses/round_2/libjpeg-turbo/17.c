// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3DecodeYUVPlanes8 at turbojpeg.c:2508:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "turbojpeg.h"

static int fuzz_tj3YUVPlaneWidth(const uint8_t *Data, size_t Size) {
    int componentID = 0;
    int width = 0;
    int subsamp = 0;

    if (Size >= 1) componentID = Data[0] % 3;
    if (Size >= 2) width = (int)(Data[1] * 256 + (Size > 3 ? Data[2] : 0));
    if (Size >= 4) subsamp = Data[3] % 5;

    // Test with valid and potentially invalid inputs
    int result = tj3YUVPlaneWidth(componentID, width, subsamp);
    (void)result; // Suppress unused variable warning
    return 0;
}

static int fuzz_tjDecodeYUV(const uint8_t *Data, size_t Size) {
    tjhandle handle = NULL;
    const unsigned char *srcBuf = NULL;
    int align = 1;
    int subsamp = TJSAMP_444;
    unsigned char *dstBuf = NULL;
    int width = 0;
    int pitch = 0;
    int height = 0;
    int pixelFormat = TJPF_RGB;
    int flags = 0;

    if (Size < 8) return 0;

    width = (Data[0] << 8) | Data[1];
    height = (Data[2] << 8) | Data[3];
    subsamp = Data[4] % 5;
    pixelFormat = Data[5] % 6;
    align = (Data[6] % 16) + 1;
    flags = Data[7];

    if (width <= 0 || height <= 0) return 0;

    // Calculate buffer sizes
    size_t ySize = tj3YUVPlaneSize(0, width, 0, height, subsamp);
    size_t uSize = tj3YUVPlaneSize(1, width, 0, height, subsamp);
    size_t vSize = tj3YUVPlaneSize(2, width, 0, height, subsamp);
    size_t dstSize = (size_t)width * height * tjPixelSize[pixelFormat];

    if (ySize == 0 || uSize == 0 || vSize == 0 || dstSize == 0) return 0;
    if (ySize > SIZE_MAX / 3 || uSize > SIZE_MAX / 3 || vSize > SIZE_MAX / 3) return 0;

    srcBuf = malloc(ySize + uSize + vSize);
    dstBuf = malloc(dstSize);
    if (!srcBuf || !dstBuf) {
        free(srcBuf);
        free(dstBuf);
        return 0;
    }

    memset(srcBuf, 0, ySize + uSize + vSize);
    memset(dstBuf, 0, dstSize);

    // Initialize handle
    handle = tjInitDecompress();
    if (!handle) {
        free(srcBuf);
        free(dstBuf);
        return 0;
    }

    // Call target function
    int ret = tjDecodeYUV(handle, srcBuf, align, subsamp, dstBuf, width, pitch, height, pixelFormat, flags);
    (void)ret;

    tjDestroy(handle);
    free(srcBuf);
    free(dstBuf);
    return 0;
}

static int fuzz_tj3YUVPlaneSize(const uint8_t *Data, size_t Size) {
    int componentID = 0;
    int width = 0;
    int stride = 0;
    int height = 0;
    int subsamp = 0;

    if (Size >= 1) componentID = Data[0] % 3;
    if (Size >= 2) width = (int)(Data[1] * 256 + (Size > 3 ? Data[2] : 0));
    if (Size >= 4) stride = (int)(Data[3] * 256 + (Size > 5 ? Data[4] : 0));
    if (Size >= 6) height = (int)(Data[5] * 256 + (Size > 7 ? Data[6] : 0));
    if (Size >= 8) subsamp = Data[7] % 5;

    size_t result = tj3YUVPlaneSize(componentID, width, stride, height, subsamp);
    (void)result;
    return 0;
}

static int fuzz_tjDecompressToYUVPlanes(const uint8_t *Data, size_t Size) {
    tjhandle handle = NULL;
    const unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;
    unsigned char *dstPlanes[3] = {NULL, NULL, NULL};
    int width = 0;
    int strides[3] = {0, 0, 0};
    int height = 0;
    int flags = 0;

    if (Size < 4) return 0;

    width = (Data[0] << 8) | Data[1];
    height = (Data[2] << 8) | Data[3];
    flags = (Size > 4) ? Data[4] : 0;

    if (width <= 0 || height <= 0) return 0;

    // Create a dummy JPEG file
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return 0;
    fwrite(Data, 1, Size, f);
    fclose(f);

    // Read back into buffer
    FILE *rf = fopen("./dummy_file", "rb");
    if (!rf) return 0;
    fseek(rf, 0, SEEK_END);
    jpegSize = (unsigned long)ftell(rf);
    fseek(rf, 0, SEEK_SET);
    jpegBuf = malloc(jpegSize);
    if (!jpegBuf) {
        fclose(rf);
        return 0;
    }
    fread(jpegBuf, 1, jpegSize, rf);
    fclose(rf);

    // Initialize handle
    handle = tjInitDecompress();
    if (!handle) {
        free((void*)jpegBuf);
        return 0;
    }

    // Allocate destination planes
    int subsamp = TJSAMP_420; // Default subsampling
    size_t ySize = tj3YUVPlaneSize(0, width, 0, height, subsamp);
    size_t uSize = tj3YUVPlaneSize(1, width, 0, height, subsamp);
    size_t vSize = tj3YUVPlaneSize(2, width, 0, height, subsamp);

    if (ySize && ySize < SIZE_MAX) dstPlanes[0] = malloc(ySize);
    if (uSize && uSize < SIZE_MAX) dstPlanes[1] = malloc(uSize);
    if (vSize && vSize < SIZE_MAX) dstPlanes[2] = malloc(vSize);

    if (!dstPlanes[0] || !dstPlanes[1] || !dstPlanes[2]) {
        free(dstPlanes[0]);
        free(dstPlanes[1]);
        free(dstPlanes[2]);
        free((void*)jpegBuf);
        tjDestroy(handle);
        return 0;
    }

    int ret = tjDecompressToYUVPlanes(handle, jpegBuf, jpegSize, dstPlanes, width, strides, height, flags);
    (void)ret;

    free(dstPlanes[0]);
    free(dstPlanes[1]);
    free(dstPlanes[2]);
    free((void*)jpegBuf);
    tjDestroy(handle);
    return 0;
}

static int fuzz_tj3DecodeYUVPlanes8(const uint8_t *Data, size_t Size) {
    tjhandle handle = NULL;
    const unsigned char *srcPlanes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    unsigned char *dstBuf = NULL;
    int width = 0;
    int pitch = 0;
    int height = 0;
    int pixelFormat = TJPF_RGB;

    if (Size < 4) return 0;

    width = (Data[0] << 8) | Data[1];
    height = (Data[2] << 8) | Data[3];
    pixelFormat = (Size > 4) ? Data[4] % 6 : TJPF_RGB;

    if (width <= 0 || height <= 0) return 0;

    int subsamp = TJSAMP_420;
    size_t ySize = tj3YUVPlaneSize(0, width, 0, height, subsamp);
    size_t uSize = tj3YUVPlaneSize(1, width, 0, height, subsamp);
    size_t vSize = tj3YUVPlaneSize(2, width, 0, height, subsamp);
    size_t dstSize = (size_t)width * height * tjPixelSize[pixelFormat];

    if (ySize == 0 || uSize == 0 || vSize == 0 || dstSize == 0) return 0;
    if (ySize > SIZE_MAX / 3 || uSize > SIZE_MAX / 3 || vSize > SIZE_MAX / 3) return 0;

    srcPlanes[0] = malloc(ySize);
    srcPlanes[1] = malloc(uSize);
    srcPlanes[2] = malloc(vSize);
    dstBuf = malloc(dstSize);

    if (!srcPlanes[0] || !srcPlanes[1] || !srcPlanes[2] || !dstBuf) {
        free(srcPlanes[0]);
        free(srcPlanes[1]);
        free(srcPlanes[2]);
        free(dstBuf);
        return 0;
    }

    memset(srcPlanes[0], 0, ySize);
    memset(srcPlanes[1], 0, uSize);
    memset(srcPlanes[2], 0, vSize);
    memset(dstBuf, 0, dstSize);

    handle = tjInitDecompress();
    if (!handle) {
        free(srcPlanes[0]);
        free(srcPlanes[1]);
        free(srcPlanes[2]);
        free(dstBuf);
        return 0;
    }

    int ret = tj3DecodeYUVPlanes8(handle, srcPlanes, strides, dstBuf, width, pitch, height, pixelFormat);
    (void)ret;

    free(srcPlanes[0]);
    free(srcPlanes[1]);
    free(srcPlanes[2]);
    free(dstBuf);
    tjDestroy(handle);
    return 0;
}

static int fuzz_tjDecodeYUVPlanes(const uint8_t *Data, size_t Size) {
    tjhandle handle = NULL;
    const unsigned char *srcPlanes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    int subsamp = TJSAMP_420;
    unsigned char *dstBuf = NULL;
    int width = 0;
    int pitch = 0;
    int height = 0;
    int pixelFormat = TJPF_RGB;
    int flags = 0;

    if (Size < 4) return 0;

    width = (Data[0] << 8) | Data[1];
    height = (Data[2] << 8) | Data[3];
    subsamp = (Size > 4) ? Data[4] % 5 : TJSAMP_420;
    pixelFormat = (Size > 5) ? Data[5] % 6 : TJPF_RGB;
    flags = (Size > 6) ? Data[6] : 0;

    if (width <= 0 || height <= 0) return 0;

    size_t ySize = tj3YUVPlaneSize(0, width, 0, height, subsamp);
    size_t uSize = tj3YUVPlaneSize(1, width, 0, height, subsamp);
    size_t vSize = tj3YUVPlaneSize(2, width, 0, height, subsamp);
    size_t dstSize = (size_t)width * height * tjPixelSize[pixelFormat];

    if (ySize == 0 || uSize == 0 || vSize == 0 || dstSize == 0) return 0;
    if (ySize > SIZE_MAX / 3 || uSize > SIZE_MAX / 3 || vSize > SIZE_MAX / 3) return 0;

    srcPlanes[0] = malloc(ySize);
    srcPlanes[1] = malloc(uSize);
    srcPlanes[2] = malloc(vSize);
    dstBuf = malloc(dstSize);

    if (!srcPlanes[0] || !srcPlanes[1] || !srcPlanes[2] || !dstBuf) {
        free(srcPlanes[0]);
        free(srcPlanes[1]);
        free(srcPlanes[2]);
        free(dstBuf);
        return 0;
    }

    memset(srcPlanes[0], 0, ySize);
    memset(srcPlanes[1], 0, uSize);
    memset(srcPlanes[2], 0, vSize);
    memset(dstBuf, 0, dstSize);

    handle = tjInitDecompress();
    if (!handle) {
        free(srcPlanes[0]);
        free(srcPlanes[1]);
        free(srcPlanes[2]);
        free(dstBuf);
        return 0;
    }

    int ret = tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp, dstBuf, width, pitch, height, pixelFormat, flags);
    (void)ret;

    free(srcPlanes[0]);
    free(srcPlanes[1]);
    free(srcPlanes[2]);
    free(dstBuf);
    tjDestroy(handle);
    return 0;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Select a function to fuzz based on the first byte
    int choice = Data[0] % 6;

    switch (choice) {
        case 0:
            fuzz_tj3YUVPlaneWidth(Data, Size);
            break;
        case 1:
            fuzz_tjDecodeYUV(Data, Size);
            break;
        case 2:
            fuzz_tj3YUVPlaneSize(Data, Size);
            break;
        case 3:
            fuzz_tjDecompressToYUVPlanes(Data, Size);
            break;
        case 4:
            fuzz_tj3DecodeYUVPlanes8(Data, Size);
            break;
        case 5:
            fuzz_tjDecodeYUVPlanes(Data, Size);
            break;
    }

    return 0;
}