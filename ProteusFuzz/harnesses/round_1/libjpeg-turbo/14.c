// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneHeight at turbojpeg.c:1117:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <turbojpeg.h>

static int fuzzer_test_tjDecodeYUVPlanes(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    int width = Data[0];
    int height = Data[1];
    int subsamp = Data[2] % 5; // TJSAMP_444 to TJSAMP_420
    int pixelFormat = Data[3] % 10; // Various formats

    if (width == 0 || height == 0) return 0;
    if (width > 4096 || height > 4096) return 0;

    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    // Calculate plane sizes
    int yH = tjPlaneHeight(0, height, subsamp);
    int uH = tjPlaneHeight(1, height, subsamp);
    int vH = tjPlaneHeight(2, height, subsamp);
    int yW = tjPlaneWidth(0, width, subsamp);
    int uW = tjPlaneWidth(1, width, subsamp);
    int vW = tjPlaneWidth(2, width, subsamp);

    if (yH <= 0 || uH <= 0 || vH <= 0 || yW <= 0 || uW <= 0 || vW <= 0) {
        tjDestroy(handle);
        return 0;
    }

    size_t ySize = (size_t)yW * yH;
    size_t uSize = (size_t)uW * uH;
    size_t vSize = (size_t)vW * vH;
    size_t totalSize = ySize + uSize + vSize;

    if (Size < totalSize) {
        tjDestroy(handle);
        return 0;
    }

    unsigned char *yPlane = (unsigned char *)malloc(ySize);
    unsigned char *uPlane = (unsigned char *)malloc(uSize);
    unsigned char *vPlane = (unsigned char *)malloc(vSize);
    unsigned char *dstBuf = (unsigned char *)malloc((size_t)width * height * 4);

    if (!yPlane || !uPlane || !vPlane || !dstBuf) {
        free(yPlane); free(uPlane); free(vPlane); free(dstBuf);
        tjDestroy(handle);
        return 0;
    }

    memcpy(yPlane, Data, ySize);
    memcpy(uPlane, Data + ySize, uSize);
    memcpy(vPlane, Data + ySize + uSize, vSize);

    const unsigned char *srcPlanes[3] = {yPlane, uPlane, vPlane};
    int strides[3] = {yW, uW, vW};
    int pitch = width * 4;

    tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp, dstBuf, width, pitch, height, pixelFormat, 0);

    free(yPlane); free(uPlane); free(vPlane); free(dstBuf);
    tjDestroy(handle);
    return 0;
}

static int fuzzer_test_tjEncodeYUV3(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    int width = Data[0];
    int height = Data[1];
    int subsamp = Data[2] % 5;
    int pixelFormat = Data[3] % 10;

    if (width == 0 || height == 0) return 0;
    if (width > 4096 || height > 4096) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int yH = tjPlaneHeight(0, height, subsamp);
    int uH = tjPlaneHeight(1, height, subsamp);
    int vH = tjPlaneHeight(2, height, subsamp);
    int yW = tjPlaneWidth(0, width, subsamp);
    int uW = tjPlaneWidth(1, width, subsamp);
    int vW = tjPlaneWidth(2, width, subsamp);

    if (yH <= 0 || uH <= 0 || vH <= 0 || yW <= 0 || uW <= 0 || vW <= 0) {
        tjDestroy(handle);
        return 0;
    }

    size_t ySize = (size_t)yW * yH;
    size_t uSize = (size_t)uW * uH;
    size_t vSize = (size_t)vW * vH;
    size_t totalYUVSize = ySize + uSize + vSize;
    size_t srcSize = (size_t)width * height * 4; // Assume 4 bytes per pixel for simplicity

    if (Size < srcSize) {
        tjDestroy(handle);
        return 0;
    }

    unsigned char *srcBuf = (unsigned char *)malloc(srcSize);
    unsigned char *dstBuf = (unsigned char *)malloc(totalYUVSize);

    if (!srcBuf || !dstBuf) {
        free(srcBuf); free(dstBuf);
        tjDestroy(handle);
        return 0;
    }

    memcpy(srcBuf, Data, srcSize);

    int pitch = width * 4;
    tjEncodeYUV3(handle, srcBuf, width, pitch, height, pixelFormat, dstBuf, 1, subsamp, 0);

    free(srcBuf); free(dstBuf);
    tjDestroy(handle);
    return 0;
}

static int fuzzer_test_plane_functions(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;

    int componentID = Data[0] % 3;
    int dimension = Data[1];
    int subsamp = Data[2] % 5;

    if (dimension == 0) return 0;
    if (dimension > 4096) return 0;

    // Test tjPlaneWidth and tjPlaneHeight
    int w1 = tjPlaneWidth(componentID, dimension, subsamp);
    int h1 = tjPlaneHeight(componentID, dimension, subsamp);

    // Test tj3YUVPlaneWidth and tj3YUVPlaneHeight
    int w2 = tj3YUVPlaneWidth(componentID, dimension, subsamp);
    int h2 = tj3YUVPlaneHeight(componentID, dimension, subsamp);

    // Just use the results to avoid unused variable warnings
    (void)w1; (void)h1; (void)w2; (void)h2;

    return 0;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    int testType = Data[0] % 3;

    switch (testType) {
        case 0:
            fuzzer_test_tjDecodeYUVPlanes(Data, Size);
            break;
        case 1:
            fuzzer_test_tjEncodeYUV3(Data, Size);
            break;
        case 2:
            fuzzer_test_plane_functions(Data, Size);
            break;
    }

    return 0;
}