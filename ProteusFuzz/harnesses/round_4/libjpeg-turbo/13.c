// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// TJBUFSIZEYUV at turbojpeg.c:1013:25 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <turbojpeg.h>

static void *safe_malloc(size_t size) {
    if (size == 0) return NULL;
    void *ptr = malloc(size);
    if (ptr) memset(ptr, 0, size);
    return ptr;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int subsamp = (Data[2] % 4); // 0: 444, 1: 422, 2: 420, 3: 411
    int quality = (Data[3] % 100) + 1;
    
    // Clamp dimensions to reasonable limits to avoid OOM or crashes
    if (width > 4096 || height > 4096) return 0;
    if (width <= 0 || height <= 0) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    // Test tjBufSize and TJBUFSIZEYUV
    unsigned long jpegBufSize = tjBufSize(width, height, subsamp);
    unsigned long yuvBufSize = TJBUFSIZEYUV(width, height, subsamp);
    unsigned long yuvBufSize2 = tjBufSizeYUV2(width, 1, height, subsamp);

    // Allocate buffers
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    
    // Allocate YUV planes based on subsampling
    unsigned char *planeY = NULL;
    unsigned char *planeU = NULL;
    unsigned char *planeV = NULL;
    const unsigned char *srcPlanes[3] = {NULL, NULL, NULL};

    // Calculate plane sizes using tj3YUVPlaneSize
    size_t sizeY = tj3YUVPlaneSize(0, width, 0, height, subsamp);
    size_t sizeU = tj3YUVPlaneSize(1, width, 0, height, subsamp);
    size_t sizeV = tj3YUVPlaneSize(2, width, 0, height, subsamp);

    if (sizeY > 0) planeY = safe_malloc(sizeY);
    if (sizeU > 0) planeU = safe_malloc(sizeU);
    if (sizeV > 0) planeV = safe_malloc(sizeV);

    if (planeY) srcPlanes[0] = planeY;
    if (planeU) srcPlanes[1] = planeU;
    if (planeV) srcPlanes[2] = planeV;

    // Fill planes with fuzz data safely
    if (planeY && sizeY > 0) {
        size_t copySize = (Size < sizeY) ? Size : sizeY;
        if (copySize > 0) {
            memcpy(planeY, Data, copySize);
        }
    }

    // Test tj3CompressFromYUVPlanes8
    unsigned char *jpegBuf8 = NULL;
    size_t jpegSize8 = 0;
    if (srcPlanes[0]) {
        int strides[3] = {0, 0, 0}; // 0 means auto-calculate
        tj3CompressFromYUVPlanes8(handle, srcPlanes, width, strides, height, &jpegBuf8, &jpegSize8);
        if (jpegBuf8) {
            free(jpegBuf8);
            jpegBuf8 = NULL;
        }
    }

    // Test tjCompress with RGB input (simulated)
    unsigned char *rgbBuf = NULL;
    unsigned long rgbSize = (unsigned long)width * height * 3;
    if (rgbSize > 0 && rgbSize < (1ULL << 30)) { // Prevent overflow
        rgbBuf = safe_malloc(rgbSize);
        if (rgbBuf && Size > 0) {
            size_t copySize = (Size < rgbSize) ? Size : rgbSize;
            memcpy(rgbBuf, Data, copySize);
        }
    }
    
    unsigned char *jpegBufCompress = NULL;
    unsigned long compressedSize = 0;
    
    if (jpegBufSize > 0 && jpegBufSize < (1ULL << 30)) {
        jpegBufCompress = safe_malloc(jpegBufSize);
        if (jpegBufCompress) {
            if (rgbBuf) {
                tjCompress(handle, rgbBuf, width, width * 3, height, 3, jpegBufCompress, &compressedSize, subsamp, quality, 0);
            }
        }
    }

    // Cleanup
    if (planeY) free(planeY);
    if (planeU) free(planeU);
    if (planeV) free(planeV);
    if (jpegBufCompress) free(jpegBufCompress);
    if (rgbBuf) free(rgbBuf);
    if (jpegBuf8) free(jpegBuf8);
    
    tjDestroy(handle);

    return 0;
}