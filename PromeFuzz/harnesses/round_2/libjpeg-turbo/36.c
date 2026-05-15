// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjEncodeYUV2 at turbojpeg.c:1758:15 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjEncodeYUV at turbojpeg.c:1767:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3EncodeYUVPlanes8 at turbojpeg.c:1508:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define FUZZ_WIDTH 64
#define FUZZ_HEIGHT 64
#define MAX_PLANES 3

static void fuzz_tjEncodeYUV3(tjhandle handle, const uint8_t *data, size_t size) {
    if (size < FUZZ_WIDTH * FUZZ_HEIGHT * 3) return;
    
    const unsigned char *srcBuf = data;
    int width = FUZZ_WIDTH;
    int height = FUZZ_HEIGHT;
    int pitch = width * 3;
    int pixelFormat = TJPF_RGB;
    
    int align = 1 << (data[0] % 4);
    int subsamp = data[1] % 5;
    int flags = data[2] & 0xFF;
    
    unsigned long yuvBufSize = tj3YUVBufSize(width, align, height, subsamp);
    unsigned char *dstBuf = (unsigned char *)malloc(yuvBufSize);
    if (!dstBuf) return;
    
    tjEncodeYUV3(handle, srcBuf, width, pitch, height, pixelFormat,
                 dstBuf, align, subsamp, flags);
    
    free(dstBuf);
}

static void fuzz_tj3EncodeYUVPlanes8(tjhandle handle, const uint8_t *data, size_t size) {
    if (size < FUZZ_WIDTH * FUZZ_HEIGHT * 3) return;
    
    const unsigned char *srcBuf = data;
    int width = FUZZ_WIDTH;
    int height = FUZZ_HEIGHT;
    int pitch = width * 3;
    int pixelFormat = TJPF_RGB;
    
    int subsamp = data[0] % 5;
    tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
    
    unsigned char *dstPlanes[MAX_PLANES];
    int strides[MAX_PLANES] = {0};
    
    for (int i = 0; i < MAX_PLANES; i++) {
        int planeSize = tj3YUVPlaneSize(i, width, 0, height, subsamp);
        if (planeSize > 0) {
            dstPlanes[i] = (unsigned char *)malloc(planeSize);
            if (!dstPlanes[i]) {
                for (int j = 0; j < i; j++) free(dstPlanes[j]);
                return;
            }
        } else {
            dstPlanes[i] = NULL;
        }
    }
    
    tj3EncodeYUVPlanes8(handle, srcBuf, width, pitch, height, pixelFormat,
                        dstPlanes, strides);
    
    for (int i = 0; i < MAX_PLANES; i++) {
        free(dstPlanes[i]);
    }
}

static void fuzz_tj3EncodeYUV8(tjhandle handle, const uint8_t *data, size_t size) {
    if (size < FUZZ_WIDTH * FUZZ_HEIGHT * 3) return;
    
    const unsigned char *srcBuf = data;
    int width = FUZZ_WIDTH;
    int height = FUZZ_HEIGHT;
    int pitch = width * 3;
    int pixelFormat = TJPF_RGB;
    
    int subsamp = data[0] % 5;
    tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
    
    int align = 1 << (data[1] % 4);
    
    unsigned long yuvBufSize = tj3YUVBufSize(width, align, height, subsamp);
    unsigned char *dstBuf = (unsigned char *)malloc(yuvBufSize);
    if (!dstBuf) return;
    
    tj3EncodeYUV8(handle, srcBuf, width, pitch, height, pixelFormat,
                  dstBuf, align);
    
    free(dstBuf);
}

static void fuzz_tjEncodeYUV2(tjhandle handle, const uint8_t *data, size_t size) {
    if (size < FUZZ_WIDTH * FUZZ_HEIGHT * 3) return;
    
    unsigned char *srcBuf = (unsigned char *)data;
    int width = FUZZ_WIDTH;
    int height = FUZZ_HEIGHT;
    int pitch = width * 3;
    int pixelFormat = TJPF_RGB;
    
    int subsamp = data[0] % 5;
    int flags = data[1] & 0xFF;
    
    unsigned long yuvBufSize = tj3YUVBufSize(width, 1, height, subsamp);
    unsigned char *dstBuf = (unsigned char *)malloc(yuvBufSize);
    if (!dstBuf) return;
    
    tjEncodeYUV2(handle, srcBuf, width, pitch, height, pixelFormat,
                 dstBuf, subsamp, flags);
    
    free(dstBuf);
}

static void fuzz_tjDecodeYUV(tjhandle handle, const uint8_t *data, size_t size) {
    if (size < FUZZ_WIDTH * FUZZ_HEIGHT) return;
    
    const unsigned char *srcBuf = data;
    int align = 1 << (data[0] % 4);
    int subsamp = data[1] % 5;
    
    int width = FUZZ_WIDTH;
    int height = FUZZ_HEIGHT;
    int pitch = width * 3;
    int pixelFormat = TJPF_RGB;
    int flags = data[2] & 0xFF;
    
    unsigned char *dstBuf = (unsigned char *)malloc(width * height * 3);
    if (!dstBuf) return;
    
    tjDecodeYUV(handle, srcBuf, align, subsamp, dstBuf,
                width, pitch, height, pixelFormat, flags);
    
    free(dstBuf);
}

static void fuzz_tjEncodeYUV(tjhandle handle, const uint8_t *data, size_t size) {
    if (size < FUZZ_WIDTH * FUZZ_HEIGHT * 3) return;
    
    unsigned char *srcBuf = (unsigned char *)data;
    int width = FUZZ_WIDTH;
    int height = FUZZ_HEIGHT;
    int pitch = width * 3;
    int pixelSize = 3;
    
    int subsamp = data[0] % 5;
    int flags = data[1] & 0xFF;
    
    unsigned long yuvBufSize = tj3YUVBufSize(width, 1, height, subsamp);
    unsigned char *dstBuf = (unsigned char *)malloc(yuvBufSize);
    if (!dstBuf) return;
    
    tjEncodeYUV(handle, srcBuf, width, pitch, height, pixelSize,
                dstBuf, subsamp, flags);
    
    free(dstBuf);
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;
    
    tjhandle comp_handle = tjInitCompress();
    tjhandle decomp_handle = tjInitDecompress();
    
    if (!comp_handle || !decomp_handle) {
        if (comp_handle) tjDestroy(comp_handle);
        if (decomp_handle) tjDestroy(decomp_handle);
        return 0;
    }
    
    int function_selector = Data[0] % 6;
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    switch (function_selector) {
        case 0:
            fuzz_tjEncodeYUV3(comp_handle, fuzz_data, fuzz_size);
            break;
        case 1:
            fuzz_tj3EncodeYUVPlanes8(comp_handle, fuzz_data, fuzz_size);
            break;
        case 2:
            fuzz_tj3EncodeYUV8(comp_handle, fuzz_data, fuzz_size);
            break;
        case 3:
            fuzz_tjEncodeYUV2(comp_handle, fuzz_data, fuzz_size);
            break;
        case 4:
            fuzz_tjDecodeYUV(decomp_handle, fuzz_data, fuzz_size);
            break;
        case 5:
            fuzz_tjEncodeYUV(comp_handle, fuzz_data, fuzz_size);
            break;
    }
    
    tjDestroy(comp_handle);
    tjDestroy(decomp_handle);
    
    return 0;
}