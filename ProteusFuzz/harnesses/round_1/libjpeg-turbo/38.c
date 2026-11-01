// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3DecompressToYUVPlanes8 at turbojpeg.c:2122:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static void cleanup_planes(unsigned char **planes, int count) {
    for (int i = 0; i < count; i++) {
        if (planes[i]) {
            free(planes[i]);
            planes[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0; // Minimum sanity check

    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    int width = 0, height = 0, subsamp = 0;
    int ret = 0;

    // 1. Test tjDecompressHeader
    ret = tjDecompressHeader(handle, (unsigned char *)Data, Size, &width, &height);
    if (ret == 0 && width > 0 && height > 0) {
        // 2. Test tjDecompressHeader2
        ret = tjDecompressHeader2(handle, (unsigned char *)Data, Size, &width, &height, &subsamp);
    }

    if (ret != 0 || width <= 0 || height <= 0) {
        tjDestroy(handle);
        return 0;
    }

    // Prepare buffers for decompression
    int pixelSize = 3; // RGB
    int pitch = width * pixelSize;
    size_t dstBufSize = pitch * height;
    unsigned char *dstBuf = (unsigned char *)malloc(dstBufSize);
    if (!dstBuf) {
        tjDestroy(handle);
        return 0;
    }

    // 3. Test tjDecompress
    ret = tjDecompress(handle, (unsigned char *)Data, Size, dstBuf, width, pitch, height, pixelSize, 0);
    if (ret == 0) {
        // Test with TJ_YUV flag if supported
        ret = tjDecompress(handle, (unsigned char *)Data, Size, dstBuf, width, pitch, height, pixelSize, TJ_YUV);
    }

    // 4. Test tjDecompressToYUV
    // For 4:2:0, size is width * height * 1.5
    size_t yuvSize = width * height * 3 / 2;
    unsigned char *yuvBuf = (unsigned char *)malloc(yuvSize);
    if (yuvBuf) {
        ret = tjDecompressToYUV(handle, (unsigned char *)Data, Size, yuvBuf, 0);
        free(yuvBuf);
    }

    // 5. Test tjDecompressToYUVPlanes
    // Allocate planes: Y, U, V
    unsigned char *planes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    
    // Calculate plane sizes based on subsampling (defaulting to 4:2:0 if unknown)
    int hSubsamp = (subsamp == TJSAMP_444) ? 1 : 2;
    int vSubsamp = (subsamp == TJSAMP_444) ? 1 : 2;
    
    int planeWidth = width;
    int planeHeight = height;
    int uWidth = width / hSubsamp;
    int uHeight = height / vSubsamp;

    planes[0] = (unsigned char *)malloc(planeWidth * planeHeight); // Y
    planes[1] = (unsigned char *)malloc(uWidth * uHeight);         // U
    planes[2] = (unsigned char *)malloc(uWidth * uHeight);         // V

    if (planes[0] && planes[1] && planes[2]) {
        ret = tjDecompressToYUVPlanes(handle, (unsigned char *)Data, Size, planes, width, strides, height, 0);
        cleanup_planes(planes, 3);
    }

    // 6. Test tj3DecompressToYUVPlanes8
    // Re-allocate for tj3 API which might need specific strides or sizes
    // Using tj3YUVPlaneSize is ideal but requires knowing exact scaling. 
    // We assume 1:1 scaling for fuzzing simplicity.
    planes[0] = (unsigned char *)malloc(planeWidth * planeHeight);
    planes[1] = (unsigned char *)malloc(uWidth * uHeight);
    planes[2] = (unsigned char *)malloc(uWidth * uHeight);
    
    if (planes[0] && planes[1] && planes[2]) {
        // Set strides to 0 for auto-calculation
        strides[0] = 0; strides[1] = 0; strides[2] = 0;
        ret = tj3DecompressToYUVPlanes8(handle, (unsigned char *)Data, Size, planes, strides);
        cleanup_planes(planes, 3);
    }

    free(dstBuf);
    tjDestroy(handle);
    return 0;
}