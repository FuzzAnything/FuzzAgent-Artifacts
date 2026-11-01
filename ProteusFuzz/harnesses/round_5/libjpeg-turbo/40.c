// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
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

static int fuzzer_test_tj3YUVPlaneWidth(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;
    int componentID = *(const int *)(Data);
    int width = *(const int *)(Data + 4);
    int subsamp = *(const int *)(Data + 8);
    (void)tj3YUVPlaneWidth(componentID, width, subsamp);
    return 0;
}

static int fuzzer_test_TJBUFSIZE(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    int width = *(const int *)(Data);
    int height = *(const int *)(Data + 4);
    (void)TJBUFSIZE(width, height);
    return 0;
}

static int fuzzer_test_tjBufSizeYUV2(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    int width = *(const int *)(Data);
    int align = *(const int *)(Data + 4);
    int height = *(const int *)(Data + 8);
    int subsamp = *(const int *)(Data + 12);
    (void)tjBufSizeYUV2(width, align, height, subsamp);
    return 0;
}

static int fuzzer_test_tjPlaneSizeYUV(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;
    int componentID = *(const int *)(Data);
    int width = *(const int *)(Data + 4);
    int stride = *(const int *)(Data + 8);
    int height = *(const int *)(Data + 12);
    int subsamp = *(const int *)(Data + 16);
    (void)tjPlaneSizeYUV(componentID, width, stride, height, subsamp);
    return 0;
}

static int fuzzer_test_tj3CompressFromYUVPlanes8(const uint8_t *Data, size_t Size) {
    if (Size < 32) return 0;
    
    int width = *(const int *)(Data);
    int height = *(const int *)(Data + 4);
    int subsamp = *(const int *)(Data + 8);
    
    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) return 0;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;
    
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    
    unsigned char *planes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    
    unsigned long ySize = tjPlaneSizeYUV(0, width, 0, height, subsamp);
    unsigned long uSize = tjPlaneSizeYUV(1, width, 0, height, subsamp);
    unsigned long vSize = tjPlaneSizeYUV(2, width, 0, height, subsamp);
    
    if (ySize != (unsigned long)-1) {
        planes[0] = (unsigned char *)malloc(ySize);
        if (planes[0]) {
            if (Size > 32) {
                size_t copyLen = (Size - 32) < ySize ? (Size - 32) : ySize;
                memcpy(planes[0], Data + 32, copyLen);
                if (copyLen < ySize) memset(planes[0] + copyLen, 0, ySize - copyLen);
            } else {
                memset(planes[0], 0, ySize);
            }
        }
    }
    
    if (subsamp != TJSAMP_GRAY) {
        if (uSize != (unsigned long)-1) {
            planes[1] = (unsigned char *)malloc(uSize);
            if (planes[1]) memset(planes[1], 128, uSize);
        }
        if (vSize != (unsigned long)-1) {
            planes[2] = (unsigned char *)malloc(vSize);
            if (planes[2]) memset(planes[2], 128, vSize);
        }
    }
    
    if (planes[0]) {
        tj3CompressFromYUVPlanes8(handle, (const unsigned char *const *)planes, 
                                   width, strides, height, &jpegBuf, &jpegSize);
    }
    
    if (jpegBuf) free(jpegBuf);
    if (planes[0]) free(planes[0]);
    if (planes[1]) free(planes[1]);
    if (planes[2]) free(planes[2]);
    
    tjDestroy(handle);
    return 0;
}

static int fuzzer_test_tjCompress(const uint8_t *Data, size_t Size) {
    if (Size < 24) return 0;
    
    int width = *(const int *)(Data);
    int height = *(const int *)(Data + 4);
    int pixelSize = *(const int *)(Data + 8);
    int subsamp = *(const int *)(Data + 12);
    int quality = *(const int *)(Data + 16);
    int flags = *(const int *)(Data + 20);
    
    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) return 0;
    if (pixelSize <= 0) pixelSize = 3;
    
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;
    
    size_t srcSize = width * height * pixelSize;
    unsigned char *srcBuf = (unsigned char *)malloc(srcSize);
    if (!srcBuf) {
        tjDestroy(handle);
        return 0;
    }
    
    if (Size > 24) {
        size_t copyLen = (Size - 24) < srcSize ? (Size - 24) : srcSize;
        memcpy(srcBuf, Data + 24, copyLen);
        if (copyLen < srcSize) memset(srcBuf + copyLen, 0, srcSize - copyLen);
    } else {
        memset(srcBuf, 0, srcSize);
    }
    
    unsigned long jpegSize = 0;
    unsigned char *jpegBuf = NULL;
    
    int ret = tjCompress(handle, srcBuf, width, width * pixelSize, height, pixelSize,
                         jpegBuf, &jpegSize, subsamp, quality, flags);
    
    if (ret == 0 && jpegBuf) {
        free(jpegBuf);
    }
    
    free(srcBuf);
    tjDestroy(handle);
    return 0;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    int choice = Data[0] % 6;
    
    switch (choice) {
        case 0:
            fuzzer_test_tj3YUVPlaneWidth(Data, Size);
            break;
        case 1:
            fuzzer_test_TJBUFSIZE(Data, Size);
            break;
        case 2:
            fuzzer_test_tjBufSizeYUV2(Data, Size);
            break;
        case 3:
            fuzzer_test_tjPlaneSizeYUV(Data, Size);
            break;
        case 4:
            fuzzer_test_tj3CompressFromYUVPlanes8(Data, Size);
            break;
        case 5:
            fuzzer_test_tjCompress(Data, Size);
            break;
        default:
            break;
    }
    
    return 0;
}