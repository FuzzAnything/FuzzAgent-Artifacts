// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

static int fuzz_tj3CompressFromYUVPlanes8(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    int width = *(const int*)(Data);
    int height = *(const int*)(Data + 4);
    int subsamp = *(const int*)(Data + 8);
    int quality = *(const int*)(Data + 12);

    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) return 0;
    if (subsamp < TJSAMP_444 || subsamp > TJSAMP_GRAY) subsamp = TJSAMP_420;
    if (quality < 1 || quality > 100) quality = 75;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    unsigned long ySize = tjPlaneSizeYUV(0, width, 0, height, subsamp);
    unsigned long uSize = tjPlaneSizeYUV(1, width, 0, height, subsamp);
    unsigned long vSize = tjPlaneSizeYUV(2, width, 0, height, subsamp);

    if (ySize == 0 || uSize == 0 || vSize == 0) {
        tjDestroy(handle);
        return 0;
    }

    unsigned char *yPlane = malloc(ySize);
    unsigned char *uPlane = malloc(uSize);
    unsigned char *vPlane = malloc(vSize);
    if (!yPlane || !uPlane || !vPlane) {
        free(yPlane); free(uPlane); free(vPlane);
        tjDestroy(handle);
        return 0;
    }

    memset(yPlane, 128, ySize);
    memset(uPlane, 128, uSize);
    memset(vPlane, 128, vSize);

    if (Size > 16) {
        size_t fillLen = Size - 16;
        size_t totalSize = ySize + uSize + vSize;
        size_t copyLen = (fillLen > totalSize) ? totalSize : fillLen;
        memcpy(yPlane, Data + 16, copyLen);
    }

    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    const unsigned char *srcPlanes[3] = {yPlane, uPlane, vPlane};
    int strides[3] = {0, 0, 0};

    tj3CompressFromYUVPlanes8(handle, srcPlanes, width, strides, height, &jpegBuf, &jpegSize);

    free(yPlane);
    free(uPlane);
    free(vPlane);
    free(jpegBuf);
    tjDestroy(handle);
    return 0;
}

static int fuzz_tjCompress(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    int width = *(const int*)(Data);
    int height = *(const int*)(Data + 4);
    int subsamp = *(const int*)(Data + 8);
    int quality = *(const int*)(Data + 12);

    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) return 0;
    if (subsamp < TJSAMP_444 || subsamp > TJSAMP_GRAY) subsamp = TJSAMP_420;
    if (quality < 1 || quality > 100) quality = 75;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    int pixelSize = 3;
    unsigned long bufSize = TJBUFSIZE(width, height);
    if (bufSize == 0) {
        tjDestroy(handle);
        return 0;
    }

    unsigned char *srcBuf = malloc(bufSize);
    unsigned char *dstBuf = malloc(bufSize);
    if (!srcBuf || !dstBuf) {
        free(srcBuf);
        free(dstBuf);
        tjDestroy(handle);
        return 0;
    }

    memset(srcBuf, 128, bufSize);
    if (Size > 16) {
        size_t fillLen = Size - 16;
        size_t copyLen = (fillLen > bufSize) ? bufSize : fillLen;
        memcpy(srcBuf, Data + 16, copyLen);
    }

    unsigned long compressedSize = 0;
    int pitch = width * pixelSize;

    tjCompress(handle, srcBuf, width, pitch, height, pixelSize, dstBuf, &compressedSize, subsamp, quality, 0);

    free(srcBuf);
    free(dstBuf);
    tjDestroy(handle);
    return 0;
}

static int fuzz_tjBufSizeYUV2(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    int width = *(const int*)(Data);
    int height = *(const int*)(Data + 4);
    int align = *(const int*)(Data + 8);
    int subsamp = *(const int*)(Data + 12);

    if (width <= 0 || height <= 0) return 0;
    if (align < 1) align = 1;
    if (subsamp < TJSAMP_444 || subsamp > TJSAMP_GRAY) subsamp = TJSAMP_420;

    unsigned long size = tjBufSizeYUV2(width, align, height, subsamp);
    (void)size; 
    return 0;
}

static int fuzz_tj3YUVPlaneSize(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;

    int componentID = *(const int*)(Data);
    int width = *(const int*)(Data + 4);
    int stride = *(const int*)(Data + 8);
    int height = *(const int*)(Data + 12);
    int subsamp = *(const int*)(Data + 16);

    if (width <= 0 || height <= 0) return 0;
    if (componentID < 0 || componentID > 2) componentID = 0;
    if (subsamp < TJSAMP_444 || subsamp > TJSAMP_GRAY) subsamp = TJSAMP_420;

    size_t size = tj3YUVPlaneSize(componentID, width, stride, height, subsamp);
    (void)size;
    return 0;
}

static int fuzz_tjPlaneSizeYUV(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;

    int componentID = *(const int*)(Data);
    int width = *(const int*)(Data + 4);
    int stride = *(const int*)(Data + 8);
    int height = *(const int*)(Data + 12);
    int subsamp = *(const int*)(Data + 16);

    if (width <= 0 || height <= 0) return 0;
    if (componentID < 0 || componentID > 2) componentID = 0;
    if (subsamp < TJSAMP_444 || subsamp > TJSAMP_GRAY) subsamp = TJSAMP_420;

    unsigned long size = tjPlaneSizeYUV(componentID, width, stride, height, subsamp);
    (void)size;
    return 0;
}

static int fuzz_TJBUFSIZE(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    int width = *(const int*)(Data);
    int height = *(const int*)(Data + 4);

    if (width <= 0 || height <= 0) return 0;

    unsigned long size = TJBUFSIZE(width, height);
    (void)size;
    return 0;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;

    int choice = *(const int*)(Data);
    int mode = choice % 6;

    switch (mode) {
        case 0: fuzz_tj3CompressFromYUVPlanes8(Data, Size); break;
        case 1: fuzz_tjCompress(Data, Size); break;
        case 2: fuzz_tjBufSizeYUV2(Data, Size); break;
        case 3: fuzz_tj3YUVPlaneSize(Data, Size); break;
        case 4: fuzz_tjPlaneSizeYUV(Data, Size); break;
        case 5: fuzz_TJBUFSIZE(Data, Size); break;
    }
    return 0;
}