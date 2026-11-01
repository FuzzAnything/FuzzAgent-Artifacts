// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
// tj3DecompressToYUVPlanes8 at turbojpeg.c:2122:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static int fuzz_header_extraction(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;
    
    int width = 0, height = 0, subsamp = 0, colorspace = 0;
    int ret = 0;
    
    // Test tjDecompressHeader
    ret = tjDecompressHeader(handle, (unsigned char*)Data, Size, &width, &height);
    if (ret == 0) {
        // Test tjDecompressHeader2
        ret = tjDecompressHeader2(handle, (unsigned char*)Data, Size, &width, &height, &subsamp);
        if (ret == 0) {
            // Test tjDecompressHeader3
            ret = tjDecompressHeader3(handle, (unsigned char*)Data, Size, &width, &height, &subsamp, &colorspace);
        }
    }
    
    tjDestroy(handle);
    return 0;
}

static int fuzz_yuv_decompression(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;
    
    int width = 0, height = 0, subsamp = 0;
    int ret = tjDecompressHeader(handle, (unsigned char*)Data, Size, &width, &height);
    if (ret != 0) {
        tjDestroy(handle);
        return 0;
    }
    
    // Allocate YUV buffer for tjDecompressToYUV
    size_t yuvSize = tjBufSize(width, height, TJSAMP_420);
    unsigned char *yuvBuf = (unsigned char*)malloc(yuvSize);
    if (!yuvBuf) {
        tjDestroy(handle);
        return 0;
    }
    
    // Test tjDecompressToYUV
    ret = tjDecompressToYUV(handle, (unsigned char*)Data, Size, yuvBuf, 0);
    
    // Prepare for plane-based decompression
    unsigned char *planes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    
    // Calculate plane sizes using tj3YUVPlaneSize
    // Note: We use a simplified approach as exact scaling factor isn't known without full setup
    int planeWidth = width;
    int planeHeight = height;
    
    // Y plane
    planes[0] = (unsigned char*)malloc(planeWidth * planeHeight);
    // U plane (subsampling affects this)
    planes[1] = (unsigned char*)malloc((planeWidth / 2) * (planeHeight / 2));
    // V plane
    planes[2] = (unsigned char*)malloc((planeWidth / 2) * (planeHeight / 2));
    
    if (planes[0] && planes[1] && planes[2]) {
        // Test tj3DecompressToYUVPlanes8
        ret = tj3DecompressToYUVPlanes8(handle, (const unsigned char*)Data, Size, planes, strides);
        
        // Test tjDecompressToYUVPlanes with explicit dimensions
        ret = tjDecompressToYUVPlanes(handle, (const unsigned char*)Data, Size, planes, width, strides, height, 0);
    }
    
    free(yuvBuf);
    free(planes[0]);
    free(planes[1]);
    free(planes[2]);
    tjDestroy(handle);
    
    return 0;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Run header extraction fuzzing
    fuzz_header_extraction(Data, Size);
    
    // Run YUV decompression fuzzing
    fuzz_yuv_decompression(Data, Size);
    
    return 0;
}