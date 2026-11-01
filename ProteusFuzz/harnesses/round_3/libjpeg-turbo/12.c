// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3DecodeYUVPlanes8 at turbojpeg.c:2508:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static void cleanup_handle(tjhandle *handle) {
    if (handle && *handle) {
        tj3Destroy(*handle);
        *handle = NULL;
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    int width = Data[0];
    int height = Data[1];
    int subsamp = Data[2] % 5; // TJSAMP_444 to TJSAMP_4200 range approx
    int componentID = Data[3] % 3; // 0, 1, 2

    // Clamp dimensions to reasonable limits to avoid allocation failures or crashes
    if (width == 0) width = 1;
    if (height == 0) height = 1;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    // Test tj3YUVPlaneSize
    int stride = 0;
    if (Size > 4) stride = Data[4];
    size_t planeSize = tj3YUVPlaneSize(componentID, width, stride, height, subsamp);

    // Test tj3YUVPlaneWidth
    int planeWidth = tj3YUVPlaneWidth(componentID, width, subsamp);

    // Prepare buffers for decoding functions
    // We need a handle for tjDecodeYUV, tjDecodeYUVPlanes, tj3DecodeYUVPlanes8, tjDecompressToYUVPlanes
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;

    // Allocate source YUV planes
    unsigned char *yPlane = NULL;
    unsigned char *uPlane = NULL;
    unsigned char *vPlane = NULL;
    unsigned char **srcPlanes = NULL;
    int strides[3] = {0, 0, 0};

    // Calculate sizes based on subsampling
    size_t ySize = tj3YUVPlaneSize(0, width, 0, height, subsamp);
    size_t uSize = tj3YUVPlaneSize(1, width, 0, height, subsamp);
    size_t vSize = tj3YUVPlaneSize(2, width, 0, height, subsamp);

    if (ySize > 0) yPlane = malloc(ySize);
    if (uSize > 0) uPlane = malloc(uSize);
    if (vSize > 0) vPlane = malloc(vSize);

    if (yPlane || uPlane || vPlane) {
        // Fill with fuzz data
        if (yPlane && ySize > 0) {
            size_t copyLen = (Size > ySize) ? ySize : Size;
            memcpy(yPlane, Data, copyLen);
            if (ySize > copyLen) memset(yPlane + copyLen, 0, ySize - copyLen);
        }
        if (uPlane && uSize > 0) {
            size_t copyLen = (Size > uSize) ? uSize : Size;
            memcpy(uPlane, Data, copyLen);
            if (uSize > copyLen) memset(uPlane + copyLen, 0, uSize - copyLen);
        }
        if (vPlane && vSize > 0) {
            size_t copyLen = (Size > vSize) ? vSize : Size;
            memcpy(vPlane, Data, copyLen);
            if (vSize > copyLen) memset(vPlane + copyLen, 0, vSize - copyLen);
        }

        srcPlanes = malloc(3 * sizeof(unsigned char *));
        if (srcPlanes) {
            srcPlanes[0] = yPlane;
            srcPlanes[1] = uPlane;
            srcPlanes[2] = vPlane;

            // Set strides based on plane widths
            strides[0] = tj3YUVPlaneWidth(0, width, subsamp);
            strides[1] = tj3YUVPlaneWidth(1, width, subsamp);
            strides[2] = tj3YUVPlaneWidth(2, width, subsamp);

            // Allocate destination buffer for RGB
            int pixelFormat = (Size > 5) ? (Data[5] % 6) : 0; // TJPF_RGB, etc.
            int pixelSize = tjPixelSize[pixelFormat];
            int pitch = width * pixelSize;
            size_t dstSize = (size_t)height * pitch;
            
            if (dstSize > 0 && dstSize < 100 * 1024 * 1024) { // Limit allocation
                unsigned char *dstBuf = malloc(dstSize);
                if (dstBuf) {
                    // Test tjDecodeYUV (requires packed YUV, but we have planes. 
                    // We can try to pass yPlane as packed if subsamp is 444, otherwise it might fail gracefully)
                    // To be safe, we construct a packed buffer or just test with planes.
                    // tjDecodeYUV expects a single buffer. Let's try to use yPlane if it's large enough or just test error paths.
                    // For robustness, we'll create a dummy packed buffer from Y plane if possible, or just pass yPlane.
                    // Note: tjDecodeYUV expects interleaved YUV. Passing planar data will likely fail or produce garbage, which is fine for fuzzing.
                    int flags = (Size > 6) ? (Data[6] % 4) : 0;
                    tjDecodeYUV(handle, yPlane, 0, subsamp, dstBuf, width, pitch, height, pixelFormat, flags);

                    // Test tjDecodeYUVPlanes
                    tjDecodeYUVPlanes(handle, (const unsigned char **)srcPlanes, strides, subsamp, 
                                      dstBuf, width, pitch, height, pixelFormat, flags);

                    // Test tj3DecodeYUVPlanes8
                    tj3DecodeYUVPlanes8(handle, (const unsigned char *const *)srcPlanes, strides, 
                                        dstBuf, width, pitch, height, pixelFormat);

                    free(dstBuf);
                }
            }

            // Test tjDecompressToYUVPlanes
            // We need a JPEG buffer. We can try to use the input data as a JPEG buffer.
            // If it's not valid JPEG, the function should return -1.
            unsigned char *dstPlanes[3] = {NULL, NULL, NULL};
            int dstStrides[3] = {0, 0, 0};
            
            // Allocate destination planes for decompression
            if (ySize > 0) dstPlanes[0] = malloc(ySize);
            if (uSize > 0) dstPlanes[1] = malloc(uSize);
            if (vSize > 0) dstPlanes[2] = malloc(vSize);

            if (dstPlanes[0] || dstPlanes[1] || dstPlanes[2]) {
                int flags = (Size > 7) ? (Data[7] % 4) : 0;
                tjDecompressToYUVPlanes(handle, Data, Size, dstPlanes, width, dstStrides, height, flags);
                
                if (dstPlanes[0]) free(dstPlanes[0]);
                if (dstPlanes[1]) free(dstPlanes[1]);
                if (dstPlanes[2]) free(dstPlanes[2]);
            }
        }
        if (srcPlanes) free(srcPlanes);
    }

    if (yPlane) free(yPlane);
    if (uPlane) free(uPlane);
    if (vPlane) free(vPlane);

    cleanup_handle(&handle);
    return 0;
}