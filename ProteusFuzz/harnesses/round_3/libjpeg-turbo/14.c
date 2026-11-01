// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int test_tj3YUVPlaneSize(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;
    int componentID = Data[0] % 3;
    int width = Data[1] * Data[2];
    int stride = Data[3] * Data[4];
    int height = Data[5] * Data[6];
    int subsamp = Data[7] % 4;

    // Test with positive values
    size_t size1 = tj3YUVPlaneSize(componentID, width, stride, height, subsamp);
    (void)size1;

    // Test with negative values to trigger error handling
    size_t size2 = tj3YUVPlaneSize(componentID, -width, stride, height, subsamp);
    (void)size2;

    size_t size3 = tj3YUVPlaneSize(componentID, width, stride, -height, subsamp);
    (void)size3;

    return 0;
}

static int test_tjBufSizeYUV2(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;
    int width = Data[0] * Data[1];
    int align = Data[2] % 16;
    int height = Data[3] * Data[4];
    int subsamp = Data[5] % 4;

    unsigned long size = tjBufSizeYUV2(width, align, height, subsamp);
    (void)size;

    // Test with negative values
    unsigned long size2 = tjBufSizeYUV2(-width, align, height, subsamp);
    (void)size2;

    return 0;
}

static int test_TJBUFSIZE(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    int width = Data[0] * Data[1];
    int height = Data[2] * Data[3];

    unsigned long size = TJBUFSIZE(width, height);
    (void)size;

    // Test with negative values
    unsigned long size2 = TJBUFSIZE(-width, height);
    (void)size2;

    return 0;
}

static int test_tjPlaneSizeYUV(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;
    int componentID = Data[0] % 3;
    int width = Data[1] * Data[2];
    int stride = Data[3] * Data[4];
    int height = Data[5] * Data[6];
    int subsamp = Data[7] % 4;

    unsigned long size = tjPlaneSizeYUV(componentID, width, stride, height, subsamp);
    (void)size;

    // Test with negative values
    unsigned long size2 = tjPlaneSizeYUV(componentID, -width, stride, height, subsamp);
    (void)size2;

    return 0;
}

static int test_tjCompress(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = Data[0] * Data[1];
    int height = Data[2] * Data[3];
    if (width <= 0 || height <= 0) {
        tjDestroy(handle);
        return 0;
    }

    // Limit dimensions to prevent huge allocations
    if (width > 4096 || height > 4096) {
        tjDestroy(handle);
        return 0;
    }

    int pitch = width * 3; // Assume RGB
    int pixelSize = 3;
    int jpegSubsamp = Data[4] % 4;
    int jpegQual = Data[5] % 100 + 1;
    int flags = Data[6];

    // Allocate source buffer
    size_t srcSize = (size_t)width * height * pixelSize;
    if (srcSize == 0) {
        tjDestroy(handle);
        return 0;
    }
    
    unsigned char *srcBuf = malloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return 0;
    }
    
    // Fill with fuzz data safely
    size_t copySize = (Size < srcSize) ? Size : srcSize;
    memcpy(srcBuf, Data, copySize);
    if (copySize < srcSize) {
        memset(srcBuf + copySize, 0, srcSize - copySize);
    }

    // Allocate destination buffer
    unsigned long jpegSize = 0;
    unsigned char *dstBuf = NULL;
    unsigned long *compressedSize = &jpegSize;

    int ret = tjCompress(handle, srcBuf, width, pitch, height, pixelSize,
                         dstBuf, compressedSize, jpegSubsamp, jpegQual, flags);
    (void)ret;

    if (dstBuf) {
        free(dstBuf);
    }
    free(srcBuf);
    tjDestroy(handle);

    return 0;
}

static int test_tj3CompressFromYUVPlanes8(const uint8_t *Data, size_t Size) {
    if (Size < 24) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int width = Data[0] * Data[1];
    int height = Data[2] * Data[3];
    if (width <= 0 || height <= 0) {
        tjDestroy(handle);
        return 0;
    }

    // Limit dimensions to prevent huge allocations
    if (width > 4096 || height > 4096) {
        tjDestroy(handle);
        return 0;
    }

    int subsamp = Data[4] % 4;
    int componentCount = (subsamp == TJSAMP_GRAY) ? 1 : 3;

    // Calculate plane sizes
    unsigned long planeSizes[3];
    for (int i = 0; i < componentCount; i++) {
        planeSizes[i] = tjPlaneSizeYUV(i, width, 0, height, subsamp);
        if (planeSizes[i] == (unsigned long)-1) {
            tjDestroy(handle);
            return 0;
        }
    }

    // Allocate planes
    unsigned char *planes[3] = {0};
    for (int i = 0; i < componentCount; i++) {
        planes[i] = malloc(planeSizes[i]);
        if (!planes[i]) {
            for (int j = 0; j < i; j++) free(planes[j]);
            tjDestroy(handle);
            return 0;
        }
        // Fill with fuzz data safely
        size_t fillSize = (Size < planeSizes[i]) ? Size : planeSizes[i];
        memcpy(planes[i], Data, fillSize);
        if (fillSize < planeSizes[i]) {
            memset(planes[i] + fillSize, 0, planeSizes[i] - fillSize);
        }
    }

    // Strides
    int strides[3] = {0};
    for (int i = 0; i < componentCount; i++) {
        strides[i] = 0; // Use default plane width
    }

    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;

    int ret = tj3CompressFromYUVPlanes8(handle, (const unsigned char *const *)planes,
                                        width, strides, height, &jpegBuf, &jpegSize);
    (void)ret;

    if (jpegBuf) {
        free(jpegBuf);
    }

    for (int i = 0; i < componentCount; i++) {
        free(planes[i]);
    }
    tjDestroy(handle);

    return 0;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    test_tj3YUVPlaneSize(Data, Size);
    test_tjBufSizeYUV2(Data, Size);
    test_TJBUFSIZE(Data, Size);
    test_tjPlaneSizeYUV(Data, Size);
    test_tjCompress(Data, Size);
    test_tj3CompressFromYUVPlanes8(Data, Size);
    return 0;
}