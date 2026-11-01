// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static int fuzz_tjDecodeYUVPlanes(const uint8_t *data, size_t size) {
    if (size < 100) return 0;
    
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;
    
    // Create dummy YUV planes
    unsigned char *yPlane = malloc(1024 * 1024);
    unsigned char *uPlane = malloc(1024 * 1024);
    unsigned char *vPlane = malloc(1024 * 1024);
    if (!yPlane || !uPlane || !vPlane) {
        tjDestroy(handle);
        free(yPlane);
        free(uPlane);
        free(vPlane);
        return 0;
    }
    
    memset(yPlane, 0, 1024 * 1024);
    memset(uPlane, 0, 1024 * 1024);
    memset(vPlane, 0, 1024 * 1024);
    
    // Fill with fuzz data
    size_t ySize = (size < 1024 * 1024) ? size : 1024 * 1024;
    memcpy(yPlane, data, ySize);
    size_t uvSize = (size < 512 * 512) ? size : 512 * 512;
    memcpy(uPlane, data, uvSize);
    memcpy(vPlane, data, uvSize);
    
    const unsigned char *srcPlanes[3] = {yPlane, uPlane, vPlane};
    int strides[3] = {1024, 512, 512};
    
    unsigned char *dstBuf = malloc(1024 * 1024 * 4);
    if (!dstBuf) {
        tjDestroy(handle);
        free(yPlane);
        free(uPlane);
        free(vPlane);
        return 0;
    }
    
    // Try different subsampling modes
    int subsamp = (data[0] % 4);
    int width = 100 + (data[1] % 100);
    int height = 100 + (data[2] % 100);
    int pixelFormat = (data[3] % 5); // 0-4
    int flags = (data[4] % 10);
    
    tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp, dstBuf, width, width * 4, height, pixelFormat, flags);
    
    tjDestroy(handle);
    free(yPlane);
    free(uPlane);
    free(vPlane);
    free(dstBuf);
    return 0;
}

static int fuzz_tjDecompressToYUV2(const uint8_t *data, size_t size) {
    if (size < 10) return 0;
    
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;
    
    unsigned char *dstBuf = malloc(1024 * 1024 * 3);
    if (!dstBuf) {
        tjDestroy(handle);
        return 0;
    }
    
    int width = (data[0] % 100) + 1;
    int height = (data[1] % 100) + 1;
    int align = (data[2] % 8) + 1;
    int flags = (data[3] % 10);
    
    tjDecompressToYUV2(handle, data, size, dstBuf, width, align, height, flags);
    
    tjDestroy(handle);
    free(dstBuf);
    return 0;
}

static int fuzz_tjDecompress2(const uint8_t *data, size_t size) {
    if (size < 10) return 0;
    
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;
    
    unsigned char *dstBuf = malloc(1024 * 1024 * 4);
    if (!dstBuf) {
        tjDestroy(handle);
        return 0;
    }
    
    int width = (data[0] % 100) + 1;
    int height = (data[1] % 100) + 1;
    int pixelFormat = (data[2] % 5);
    int flags = (data[3] % 10);
    
    tjDecompress2(handle, data, size, dstBuf, width, width * 4, height, pixelFormat, flags);
    
    tjDestroy(handle);
    free(dstBuf);
    return 0;
}

static int fuzz_tjDecompress(const uint8_t *data, size_t size) {
    if (size < 10) return 0;
    
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;
    
    unsigned char *dstBuf = malloc(1024 * 1024 * 4);
    if (!dstBuf) {
        tjDestroy(handle);
        return 0;
    }
    
    int width = (data[0] % 100) + 1;
    int height = (data[1] % 100) + 1;
    int pixelSize = (data[2] % 5);
    int flags = (data[3] % 10);
    
    tjDecompress(handle, (unsigned char *)data, size, dstBuf, width, width * 4, height, pixelSize, flags);
    
    tjDestroy(handle);
    free(dstBuf);
    return 0;
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Fuzz different functions based on input
    switch (Data[0] % 6) {
        case 0:
            fuzz_tjDecodeYUVPlanes(Data, Size);
            break;
        case 1:
            fuzz_tjDecompressToYUV2(Data, Size);
            break;
        case 2:
            fuzz_tjDecompress2(Data, Size);
            break;
        case 3:
            fuzz_tjDecompress(Data, Size);
            break;
        case 4: {
            // Test tjGetErrorCode
            tjhandle handle = tjInitDecompress();
            if (handle) {
                // Trigger an error first
                unsigned char *dummy = malloc(100);
                if (dummy) {
                    tjDecompress2(handle, dummy, 0, dummy, 0, 0, 0, 0, 0);
                    tjGetErrorCode(handle);
                    free(dummy);
                }
                tjDestroy(handle);
            }
            break;
        }
        case 5: {
            // Test tj3GetErrorCode
            tjhandle handle = tjInitDecompress();
            if (handle) {
                // Trigger an error first
                unsigned char *dummy = malloc(100);
                if (dummy) {
                    tjDecompress2(handle, dummy, 0, dummy, 0, 0, 0, 0, 0);
                    tj3GetErrorCode(handle);
                    free(dummy);
                }
                tjDestroy(handle);
            }
            break;
        }
    }
    
    return 0;
}