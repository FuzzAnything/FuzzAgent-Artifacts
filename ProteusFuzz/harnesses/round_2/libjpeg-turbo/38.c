// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tj3DecompressToYUVPlanes8 at turbojpeg.c:2122:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void *safe_malloc(size_t size) {
    if (size == 0) return NULL;
    void *ptr = malloc(size);
    if (!ptr) exit(1);
    return ptr;
}

static void safe_free(void *ptr) {
    if (ptr) free(ptr);
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;

    int width = 0, height = 0;
    int jpegSubsamp = 0, jpegColorspace = 0;
    int ret = 0;

    // Test tjDecompressHeader3
    ret = tjDecompressHeader3(handle, Data, Size, &width, &height, &jpegSubsamp, &jpegColorspace);
    if (ret == 0 && width > 0 && height > 0) {
        // Allocate destination buffer for tjDecompress
        int pixelSize = width * 3; // RGB
        int pitch = width * 3;
        unsigned char *dstBuf = safe_malloc((size_t)height * pitch);
        if (dstBuf) {
            ret = tjDecompress(handle, Data, Size, dstBuf, width, pitch, height, pixelSize, 0);
            safe_free(dstBuf);
        }

        // Allocate YUV buffer for tjDecompressToYUV
        // tjBufSizeYUV2 signature: (width, align, height, subsamp)
        int yuvSize = tjBufSizeYUV2(width, 1, height, jpegSubsamp);
        if (yuvSize > 0) {
            unsigned char *yuvBuf = safe_malloc((size_t)yuvSize);
            if (yuvBuf) {
                ret = tjDecompressToYUV(handle, Data, Size, yuvBuf, 0);
                safe_free(yuvBuf);
            }
        }

        // Prepare for tjDecompressToYUVPlanes
        unsigned char *planes[3];
        int strides[3] = {0, 0, 0};
        int planeWidth = (width + 1) / 2; // Approximate for 4:2:0
        int planeHeight = (height + 1) / 2;
        
        planes[0] = safe_malloc((size_t)width * height); // Y
        planes[1] = safe_malloc((size_t)planeWidth * planeHeight); // U
        planes[2] = safe_malloc((size_t)planeWidth * planeHeight); // V
        
        if (planes[0] && planes[1] && planes[2]) {
            ret = tjDecompressToYUVPlanes(handle, Data, Size, planes, width, strides, height, 0);
            safe_free(planes[0]);
            safe_free(planes[1]);
            safe_free(planes[2]);
        }
    }

    // Test tjDecompressHeader
    width = 0; height = 0;
    ret = tjDecompressHeader(handle, Data, Size, &width, &height);

    // Test tj3DecompressToYUVPlanes8
    if (Size > 0) {
        unsigned char *planes8[3] = {NULL, NULL, NULL};
        int strides8[3] = {0, 0, 0};
        
        // Re-read header to get dimensions for plane allocation if needed
        if (tjDecompressHeader3(handle, Data, Size, &width, &height, &jpegSubsamp, &jpegColorspace) == 0) {
            int planeW = width;
            int planeH = height;
            
            // Check subsampling using standard constants defined in turbojpeg.h
            // TJCS_YCBCR is typically 1, TJCS_YCbCr is typically 2, but we use the constants if available
            // Since the previous error indicated TJCS_YCBCR was undeclared, we rely on the numeric value 
            // or standard TJ_ constants. TJ_SAMP_420 is usually 1.
            // Let's assume standard subsampling values: 0=4:4:4, 1=4:2:2, 2=4:2:0, 3=4:4:0, 4=Grayscale
            if (jpegSubsamp == 1 || jpegSubsamp == 2 || jpegSubsamp == 3) {
                planeW = (width + 1) / 2;
                planeH = (height + 1) / 2;
            }
            
            planes8[0] = safe_malloc((size_t)width * height);
            planes8[1] = safe_malloc((size_t)planeW * planeH);
            planes8[2] = safe_malloc((size_t)planeW * planeH);

            if (planes8[0] && planes8[1] && planes8[2]) {
                ret = tj3DecompressToYUVPlanes8(handle, Data, Size, planes8, strides8);
                safe_free(planes8[0]);
                safe_free(planes8[1]);
                safe_free(planes8[2]);
            }
        }
    }

    tjDestroy(handle);
    return 0;
}