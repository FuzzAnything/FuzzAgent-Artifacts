// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int fuzz_tj3YUVPlaneHeight(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;
    int componentID = Data[0] % 3; // 0, 1, or 2
    int height = *(int32_t *)(Data + 1);
    int subsamp = Data[5];

    return tj3YUVPlaneHeight(componentID, height, subsamp);
}

static int fuzz_tjEncodeYUV3(const uint8_t *Data, size_t Size) {
    if (Size < 32) return 0;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = *(int32_t *)(Data);
    int height = *(int32_t *)(Data + 4);
    int pitch = *(int32_t *)(Data + 8);
    int pixelFormat = Data[12] % 6;
    int align = Data[13];
    int subsamp = Data[14] % 5;
    int flags = *(int32_t *)(Data + 16);

    if (width <= 0 || height <= 0) {
        tjDestroy(handle);
        return 0;
    }

    // Allocate source and destination buffers
    size_t srcSize = (size_t)width * height * 4; // Max 4 bytes per pixel
    // tjBufSizeYUV2 signature: (width, align, height, subsamp)
    size_t dstSize = tjBufSizeYUV2(width, align, height, subsamp);
    if (srcSize == 0 || dstSize == 0) {
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

    // Fill source with fuzz data
    memcpy(srcBuf, Data, Size < srcSize ? Size : srcSize);

    int result = tjEncodeYUV3(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, align, subsamp, flags);

    free(srcBuf);
    free(dstBuf);
    tjDestroy(handle);
    return result;
}

static int fuzz_tjEncodeYUVPlanes(const uint8_t *Data, size_t Size) {
    if (Size < 32) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = *(int32_t *)(Data);
    int height = *(int32_t *)(Data + 4);
    int pitch = *(int32_t *)(Data + 8);
    int pixelFormat = Data[12] % 6;
    int subsamp = Data[13] % 5;
    int flags = *(int32_t *)(Data + 16);

    if (width <= 0 || height <= 0) {
        tjDestroy(handle);
        return 0;
    }

    size_t srcSize = (size_t)width * height * 4;
    unsigned char *srcBuf = malloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return 0;
    }
    memcpy(srcBuf, Data, Size < srcSize ? Size : srcSize);

    // Calculate plane sizes
    int planeWidth[3], planeHeight[3];
    int planeSize[3];
    
    planeWidth[0] = width;
    planeHeight[0] = height;
    planeSize[0] = planeWidth[0] * planeHeight[0];

    if (subsamp == TJSAMP_420 || subsamp == TJSAMP_422) {
        planeWidth[1] = width / 2;
        planeWidth[2] = width / 2;
        planeHeight[1] = height / 2;
        planeHeight[2] = height / 2;
    } else if (subsamp == TJSAMP_GRAY) {
        planeWidth[1] = 0; planeHeight[1] = 0;
        planeWidth[2] = 0; planeHeight[2] = 0;
    } else {
        // 444
        planeWidth[1] = width; planeHeight[1] = height;
        planeWidth[2] = width; planeHeight[2] = height;
    }
    
    planeSize[1] = planeWidth[1] * planeHeight[1];
    planeSize[2] = planeWidth[2] * planeHeight[2];

    unsigned char *dstPlanes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};

    if (planeSize[0] > 0) dstPlanes[0] = malloc(planeSize[0]);
    if (planeSize[1] > 0) dstPlanes[1] = malloc(planeSize[1]);
    if (planeSize[2] > 0) dstPlanes[2] = malloc(planeSize[2]);

    if (!dstPlanes[0] || (subsamp != TJSAMP_GRAY && (!dstPlanes[1] || !dstPlanes[2]))) {
        free(srcBuf);
        if (dstPlanes[0]) free(dstPlanes[0]);
        if (dstPlanes[1]) free(dstPlanes[1]);
        if (dstPlanes[2]) free(dstPlanes[2]);
        tjDestroy(handle);
        return 0;
    }

    int result = tjEncodeYUVPlanes(handle, srcBuf, width, pitch, height, pixelFormat, dstPlanes, strides, subsamp, flags);

    free(srcBuf);
    if (dstPlanes[0]) free(dstPlanes[0]);
    if (dstPlanes[1]) free(dstPlanes[1]);
    if (dstPlanes[2]) free(dstPlanes[2]);
    tjDestroy(handle);
    return result;
}

static int fuzz_tjEncodeYUV2(const uint8_t *Data, size_t Size) {
    if (Size < 24) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = *(int32_t *)(Data);
    int height = *(int32_t *)(Data + 4);
    int pitch = *(int32_t *)(Data + 8);
    int pixelFormat = Data[12] % 6;
    int subsamp = Data[13] % 5;
    int flags = *(int32_t *)(Data + 16);

    if (width <= 0 || height <= 0) {
        tjDestroy(handle);
        return 0;
    }

    size_t srcSize = (size_t)width * height * 4;
    // tjBufSizeYUV2 signature: (width, align, height, subsamp). 
    // tjEncodeYUV2 uses fixed alignment of 4 (MCU boundary).
    size_t dstSize = tjBufSizeYUV2(width, 4, height, subsamp);
    if (srcSize == 0 || dstSize == 0) {
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

    memcpy(srcBuf, Data, Size < srcSize ? Size : srcSize);

    int result = tjEncodeYUV2(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, subsamp, flags);

    free(srcBuf);
    free(dstBuf);
    tjDestroy(handle);
    return result;
}

static int fuzz_tjDecodeYUVPlanes(const uint8_t *Data, size_t Size) {
    if (Size < 32) return 0;

    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    int width = *(int32_t *)(Data);
    int height = *(int32_t *)(Data + 4);
    int subsamp = Data[8] % 5;
    int pixelFormat = Data[9] % 6;
    int flags = *(int32_t *)(Data + 12);

    if (width <= 0 || height <= 0) {
        tjDestroy(handle);
        return 0;
    }

    // Setup source planes
    int planeWidth[3], planeHeight[3];
    planeWidth[0] = width; planeHeight[0] = height;
    if (subsamp == TJSAMP_420 || subsamp == TJSAMP_422) {
        planeWidth[1] = width / 2; planeHeight[1] = height / 2;
        planeWidth[2] = width / 2; planeHeight[2] = height / 2;
    } else if (subsamp == TJSAMP_GRAY) {
        planeWidth[1] = 0; planeHeight[1] = 0;
        planeWidth[2] = 0; planeHeight[2] = 0;
    } else {
        planeWidth[1] = width; planeHeight[1] = height;
        planeWidth[2] = width; planeHeight[2] = height;
    }

    size_t planeSize[3];
    planeSize[0] = planeWidth[0] * planeHeight[0];
    planeSize[1] = planeWidth[1] * planeHeight[1];
    planeSize[2] = planeWidth[2] * planeHeight[2];

    unsigned char *srcPlanes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};

    if (planeSize[0] > 0) srcPlanes[0] = malloc(planeSize[0]);
    if (planeSize[1] > 0) srcPlanes[1] = malloc(planeSize[1]);
    if (planeSize[2] > 0) srcPlanes[2] = malloc(planeSize[2]);

    if (!srcPlanes[0] || (subsamp != TJSAMP_GRAY && (!srcPlanes[1] || !srcPlanes[2]))) {
        free(srcPlanes[0]); free(srcPlanes[1]); free(srcPlanes[2]);
        tjDestroy(handle);
        return 0;
    }

    // Fill planes with fuzz data
    size_t totalPlaneSize = planeSize[0] + planeSize[1] + planeSize[2];
    if (Size > totalPlaneSize) Size = totalPlaneSize;
    size_t offset = 0;
    if (srcPlanes[0]) { memcpy(srcPlanes[0], Data + offset, planeSize[0]); offset += planeSize[0]; }
    if (srcPlanes[1]) { memcpy(srcPlanes[1], Data + offset, planeSize[1]); offset += planeSize[1]; }
    if (srcPlanes[2]) { memcpy(srcPlanes[2], Data + offset, planeSize[2]); }

    // Destination buffer
    size_t dstSize = (size_t)width * height * 4;
    unsigned char *dstBuf = malloc(dstSize);
    if (!dstBuf) {
        free(srcPlanes[0]); free(srcPlanes[1]); free(srcPlanes[2]);
        tjDestroy(handle);
        return 0;
    }

    int result = tjDecodeYUVPlanes(handle, (const unsigned char **)srcPlanes, strides, subsamp, dstBuf, width, 0, height, pixelFormat, flags);

    free(srcPlanes[0]); free(srcPlanes[1]); free(srcPlanes[2]);
    free(dstBuf);
    tjDestroy(handle);
    return result;
}

static int fuzz_tj3EncodeYUVPlanes8(const uint8_t *Data, size_t Size) {
    if (Size < 32) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = *(int32_t *)(Data);
    int height = *(int32_t *)(Data + 4);
    int pitch = *(int32_t *)(Data + 8);
    int pixelFormat = Data[12] % 6;
    
    if (width <= 0 || height <= 0) {
        tjDestroy(handle);
        return 0;
    }

    size_t srcSize = (size_t)width * height * 4;
    unsigned char *srcBuf = malloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return 0;
    }
    memcpy(srcBuf, Data, Size < srcSize ? Size : srcSize);

    // We need to guess subsampling to allocate planes. Let's try 420.
    int planeWidth[3] = {width, width/2, width/2};
    int planeHeight[3] = {height, height/2, height/2};
    int planeSize[3];
    planeSize[0] = planeWidth[0] * planeHeight[0];
    planeSize[1] = planeWidth[1] * planeHeight[1];
    planeSize[2] = planeWidth[2] * planeHeight[2];

    unsigned char *dstPlanes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};

    if (planeSize[0] > 0) dstPlanes[0] = malloc(planeSize[0]);
    if (planeSize[1] > 0) dstPlanes[1] = malloc(planeSize[1]);
    if (planeSize[2] > 0) dstPlanes[2] = malloc(planeSize[2]);

    if (!dstPlanes[0] || !dstPlanes[1] || !dstPlanes[2]) {
        free(srcBuf);
        if (dstPlanes[0]) free(dstPlanes[0]);
        if (dstPlanes[1]) free(dstPlanes[1]);
        if (dstPlanes[2]) free(dstPlanes[2]);
        tjDestroy(handle);
        return 0;
    }

    int result = tj3EncodeYUVPlanes8(handle, srcBuf, width, pitch, height, pixelFormat, dstPlanes, strides);

    free(srcBuf);
    if (dstPlanes[0]) free(dstPlanes[0]);
    if (dstPlanes[1]) free(dstPlanes[1]);
    if (dstPlanes[2]) free(dstPlanes[2]);
    tjDestroy(handle);
    return result;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Use the first byte to select the function to fuzz
    int choice = Data[0] % 6;

    switch (choice) {
        case 0:
            return fuzz_tj3YUVPlaneHeight(Data, Size);
        case 1:
            return fuzz_tjEncodeYUV3(Data, Size);
        case 2:
            return fuzz_tjEncodeYUVPlanes(Data, Size);
        case 3:
            return fuzz_tjEncodeYUV2(Data, Size);
        case 4:
            return fuzz_tjDecodeYUVPlanes(Data, Size);
        case 5:
            return fuzz_tj3EncodeYUVPlanes8(Data, Size);
    }
    return 0;
}