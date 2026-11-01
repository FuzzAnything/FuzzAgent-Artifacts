// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompressFromYUVPlanes at turbojpeg.c:1394:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int fuzz_tjCompressFromYUVPlanes(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int subsamp = Data[2] % 3; // 0, 1, or 2
    int quality = Data[3] % 101;
    int flags = Data[4];

    // Allocate YUV planes
    int planeCount = (subsamp == 0) ? 1 : 3;
    const unsigned char *srcPlanes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    
    for (int i = 0; i < planeCount; i++) {
        int planeWidth = (i == 0) ? width : (width + 1) / 2;
        int planeHeight = (i == 0) ? height : (height + 1) / 2;
        strides[i] = planeWidth;
        // Allocate slightly more than needed to avoid edge cases if library reads padding
        size_t planeSize = (size_t)strides[i] * planeHeight;
        srcPlanes[i] = (unsigned char *)malloc(planeSize + 16);
        if (!srcPlanes[i]) {
            for (int j = 0; j < i; j++) free((void*)srcPlanes[j]);
            tjDestroy(handle);
            return 0;
        }
        memset((void*)srcPlanes[i], 0, planeSize);
    }

    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;

    tjCompressFromYUVPlanes(handle, srcPlanes, width, strides, height, subsamp,
                            &jpegBuf, &jpegSize, quality, flags);

    if (jpegBuf) tjFree(jpegBuf);
    for (int i = 0; i < planeCount; i++) {
        if (srcPlanes[i]) free((void*)srcPlanes[i]);
    }
    tjDestroy(handle);
    return 0;
}

static int fuzz_tj3SetICCProfile(const uint8_t *Data, size_t Size) {
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    // Use Data as ICC profile buffer
    size_t iccSize = Size;
    unsigned char *iccBuf = (unsigned char *)Data;

    // Fuzz with NULL or 0 size occasionally
    if (Size > 0 && Data[0] % 2 == 0) {
        iccBuf = NULL;
        iccSize = 0;
    }

    tj3SetICCProfile(handle, iccBuf, iccSize);
    tjDestroy(handle);
    return 0;
}

static int fuzz_tjCompress2(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelFormat = Data[2] % 4; // 0-3
    int jpegSubsamp = Data[3] % 3;
    int jpegQual = Data[4] % 101;
    int flags = Data[5];

    int pitch = width * 4; // Assuming RGBA
    size_t srcSize = (size_t)pitch * height;
    unsigned char *srcBuf = (unsigned char *)malloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return 0;
    }
    memset(srcBuf, 0, srcSize);

    unsigned char *jpegBuf = NULL;
    unsigned long jpegSize = 0;

    tjCompress2(handle, srcBuf, width, pitch, height, pixelFormat,
                &jpegBuf, &jpegSize, jpegSubsamp, jpegQual, flags);

    if (jpegBuf) tjFree(jpegBuf);
    free(srcBuf);
    tjDestroy(handle);
    return 0;
}

static int fuzz_tj3CompressFromYUVPlanes8(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int subsamp = Data[2] % 3;

    int planeCount = (subsamp == 0) ? 1 : 3;
    const unsigned char *srcPlanes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};

    for (int i = 0; i < planeCount; i++) {
        int planeWidth = (i == 0) ? width : (width + 1) / 2;
        int planeHeight = (i == 0) ? height : (height + 1) / 2;
        strides[i] = planeWidth;
        size_t planeSize = (size_t)strides[i] * planeHeight;
        srcPlanes[i] = (unsigned char *)malloc(planeSize + 16);
        if (!srcPlanes[i]) {
            for (int j = 0; j < i; j++) free((void*)srcPlanes[j]);
            tjDestroy(handle);
            return 0;
        }
        memset((void*)srcPlanes[i], 0, planeSize);
    }

    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;

    tj3CompressFromYUVPlanes8(handle, srcPlanes, width, strides, height,
                              &jpegBuf, &jpegSize);

    if (jpegBuf) tjFree(jpegBuf);
    for (int i = 0; i < planeCount; i++) {
        if (srcPlanes[i]) free((void*)srcPlanes[i]);
    }
    tjDestroy(handle);
    return 0;
}

static int fuzz_tjCompress(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelSize = Data[2] % 4 + 1; // 1-4 bytes per pixel
    int jpegSubsamp = Data[3] % 3;
    int jpegQual = Data[4] % 101;
    int flags = Data[5];

    int pitch = width * pixelSize;
    size_t srcSize = (size_t)pitch * height;
    unsigned char *srcBuf = (unsigned char *)malloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return 0;
    }
    memset(srcBuf, 0, srcSize);

    unsigned char *dstBuf = NULL;
    unsigned long compressedSize = 0;

    // Pre-allocate a large buffer for dstBuf to simulate worst case
    unsigned long maxBufSize = tjBufSize(width, height, jpegSubsamp);
    dstBuf = (unsigned char *)malloc(maxBufSize);
    if (!dstBuf) {
        free(srcBuf);
        tjDestroy(handle);
        return 0;
    }

    tjCompress(handle, srcBuf, width, pitch, height, pixelSize,
               dstBuf, &compressedSize, jpegSubsamp, jpegQual, flags);

    free(srcBuf);
    free(dstBuf);
    tjDestroy(handle);
    return 0;
}

static int fuzz_tj3CompressFromYUV8(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int align = 1 << (Data[2] % 4); // 1, 2, 4, 8
    int subsamp = Data[3] % 3;

    size_t yuvSize = tj3YUVBufSize(width, align, height, subsamp);
    unsigned char *srcBuf = (unsigned char *)malloc(yuvSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return 0;
    }
    memset(srcBuf, 0, yuvSize);

    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;

    tj3CompressFromYUV8(handle, srcBuf, width, align, height,
                        &jpegBuf, &jpegSize);

    if (jpegBuf) tjFree(jpegBuf);
    free(srcBuf);
    tjDestroy(handle);
    return 0;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Use first byte to select which function to fuzz
    int funcIndex = Data[0] % 6;

    switch (funcIndex) {
        case 0: fuzz_tjCompressFromYUVPlanes(Data, Size); break;
        case 1: fuzz_tj3SetICCProfile(Data, Size); break;
        case 2: fuzz_tjCompress2(Data, Size); break;
        case 3: fuzz_tj3CompressFromYUVPlanes8(Data, Size); break;
        case 4: fuzz_tjCompress(Data, Size); break;
        case 5: fuzz_tj3CompressFromYUV8(Data, Size); break;
    }

    return 0;
}