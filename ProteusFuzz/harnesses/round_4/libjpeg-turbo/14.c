// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjEncodeYUVPlanes at turbojpeg.c:1663:15 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tj3YUVPlaneHeight at turbojpeg.c:1091:15 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
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

static int test_tjEncodeYUV2(tjhandle handle, const uint8_t *data, size_t size) {
    if (!handle || !data || size < 4) return 0;
    
    int width = *(int*)(data);
    int height = *(int*)(data + 4);
    int pixelFormat = *(int*)(data + 8);
    int subsamp = *(int*)(data + 12);
    int flags = *(int*)(data + 16);
    
    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) return 0;
    
    size_t srcSize = (size_t)width * (size_t)height * 4;
    if (size < 20 + srcSize) return 0;
    
    unsigned char *srcBuf = (unsigned char*)(data + 20);
    
    // Calculate destination size
    int ySize = width * height;
    int uvSize = (width / 2) * (height / 2) * 2;
    size_t dstSize = ySize + uvSize;
    
    unsigned char *dstBuf = (unsigned char*)malloc(dstSize);
    if (!dstBuf) return 0;
    
    int result = tjEncodeYUV2(handle, srcBuf, width, width * 4, height, pixelFormat, dstBuf, subsamp, flags);
    
    free(dstBuf);
    return result;
}

static int test_tj3YUVPlaneHeight(const uint8_t *data, size_t size) {
    if (!data || size < 12) return 0;
    
    int componentID = *(int*)(data);
    int height = *(int*)(data + 4);
    int subsamp = *(int*)(data + 8);
    
    if (componentID < 0 || componentID > 2) return 0;
    if (height <= 0 || height > 10000) return 0;
    
    return tj3YUVPlaneHeight(componentID, height, subsamp);
}

static int test_tjEncodeYUV(tjhandle handle, const uint8_t *data, size_t size) {
    if (!handle || !data || size < 20) return 0;
    
    int width = *(int*)(data);
    int height = *(int*)(data + 4);
    int pixelSize = *(int*)(data + 8);
    int subsamp = *(int*)(data + 12);
    int flags = *(int*)(data + 16);
    
    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) return 0;
    if (pixelSize <= 0 || pixelSize > 4) return 0;
    
    size_t srcSize = (size_t)width * (size_t)height * (size_t)pixelSize;
    if (size < 20 + srcSize) return 0;
    
    unsigned char *srcBuf = (unsigned char*)(data + 20);
    
    // Calculate destination size
    int ySize = width * height;
    int uvSize = (width / 2) * (height / 2) * 2;
    size_t dstSize = ySize + uvSize;
    
    unsigned char *dstBuf = (unsigned char*)malloc(dstSize);
    if (!dstBuf) return 0;
    
    int result = tjEncodeYUV(handle, srcBuf, width, width * pixelSize, height, pixelSize, dstBuf, subsamp, flags);
    
    free(dstBuf);
    return result;
}

static int test_tjEncodeYUVPlanes(tjhandle handle, const uint8_t *data, size_t size) {
    if (!handle || !data || size < 24) return 0;
    
    int width = *(int*)(data);
    int height = *(int*)(data + 4);
    int pixelFormat = *(int*)(data + 8);
    int subsamp = *(int*)(data + 12);
    int flags = *(int*)(data + 16);
    
    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) return 0;
    
    size_t srcSize = (size_t)width * (size_t)height * 4;
    if (size < 24 + srcSize) return 0;
    
    unsigned char *srcBuf = (unsigned char*)(data + 24);
    
    // Calculate plane sizes
    int ySize = width * height;
    int uvSize = (width / 2) * (height / 2);
    
    unsigned char *yPlane = (unsigned char*)malloc(ySize);
    unsigned char *uPlane = (unsigned char*)malloc(uvSize);
    unsigned char *vPlane = (unsigned char*)malloc(uvSize);
    
    if (!yPlane || !uPlane || !vPlane) {
        free(yPlane);
        free(uPlane);
        free(vPlane);
        return 0;
    }
    
    unsigned char *dstPlanes[3] = {yPlane, uPlane, vPlane};
    int strides[3] = {width, width/2, width/2};
    
    int result = tjEncodeYUVPlanes(handle, srcBuf, width, width * 4, height, pixelFormat, dstPlanes, strides, subsamp, flags);
    
    free(yPlane);
    free(uPlane);
    free(vPlane);
    
    return result;
}

static int test_tjEncodeYUV3(tjhandle handle, const uint8_t *data, size_t size) {
    if (!handle || !data || size < 24) return 0;
    
    int width = *(int*)(data);
    int height = *(int*)(data + 4);
    int pixelFormat = *(int*)(data + 8);
    int align = *(int*)(data + 12);
    int subsamp = *(int*)(data + 16);
    int flags = *(int*)(data + 20);
    
    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) return 0;
    if (align <= 0) align = 1;
    
    size_t srcSize = (size_t)width * (size_t)height * 4;
    if (size < 24 + srcSize) return 0;
    
    unsigned char *srcBuf = (unsigned char*)(data + 24);
    
    // Calculate destination size
    int ySize = width * height;
    int uvSize = (width / 2) * (height / 2) * 2;
    size_t dstSize = ySize + uvSize;
    
    unsigned char *dstBuf = (unsigned char*)malloc(dstSize);
    if (!dstBuf) return 0;
    
    int result = tjEncodeYUV3(handle, srcBuf, width, width * 4, height, pixelFormat, dstBuf, align, subsamp, flags);
    
    free(dstBuf);
    return result;
}

static int test_tj3YUVPlaneWidth(const uint8_t *data, size_t size) {
    if (!data || size < 12) return 0;
    
    int componentID = *(int*)(data);
    int width = *(int*)(data + 4);
    int subsamp = *(int*)(data + 8);
    
    if (componentID < 0 || componentID > 2) return 0;
    if (width <= 0 || width > 10000) return 0;
    
    return tj3YUVPlaneWidth(componentID, width, subsamp);
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;
    
    // Test tj3YUVPlaneHeight
    test_tj3YUVPlaneHeight(Data, Size);
    
    // Test tj3YUVPlaneWidth
    test_tj3YUVPlaneWidth(Data, Size);
    
    // Test tjEncodeYUV2
    test_tjEncodeYUV2(handle, Data, Size);
    
    // Test tjEncodeYUV
    test_tjEncodeYUV(handle, Data, Size);
    
    // Test tjEncodeYUVPlanes
    test_tjEncodeYUVPlanes(handle, Data, Size);
    
    // Test tjEncodeYUV3
    test_tjEncodeYUV3(handle, Data, Size);
    
    tjDestroy(handle);
    
    return 0;
}