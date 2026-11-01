// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3DecodeYUVPlanes8 at turbojpeg.c:2508:15 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <turbojpeg.h>

static void cleanup_handle(tjhandle *handle) {
    if (handle && *handle) {
        tj3Destroy(*handle);
        *handle = NULL;
    }
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    tjhandle handle = NULL;
    int width, height, subsamp, pixelFormat;
    unsigned char *srcBuf = NULL;
    unsigned char *dstBuf = NULL;
    unsigned char **planes = NULL;
    int *strides = NULL;
    int ret = 0;

    // Extract parameters from fuzz input
    width = (int)Data[0];
    height = (int)Data[1];
    subsamp = (int)Data[2];
    pixelFormat = (int)Data[3];

    // Clamp dimensions to reasonable values to prevent OOM or stack overflow
    if (width < 1 || width > 4096) width = 64;
    if (height < 1 || height > 4096) height = 64;

    // Validate subsampling
    if (subsamp < TJSAMP_444 || subsamp > TJSAMP_GRAY) subsamp = TJSAMP_420;

    // Validate pixel format
    if (pixelFormat < TJPF_GRAY || pixelFormat > TJPF_XBGR) pixelFormat = TJPF_RGB;

    // Initialize handle
    handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;

    // Test 1: tj3YUVPlaneWidth
    int planeWidth = tj3YUVPlaneWidth(0, width, subsamp);
    (void)planeWidth; // Use return value to suppress unused warning

    // Test 2: tjBufSizeYUV
    unsigned long yuvSize = tjBufSizeYUV(width, height, subsamp);
    if (yuvSize == 0 || yuvSize > 100 * 1024 * 1024) {
        cleanup_handle(&handle);
        return 0;
    }

    // Allocate buffers
    srcBuf = (unsigned char *)malloc(yuvSize);
    if (!srcBuf) {
        cleanup_handle(&handle);
        return 0;
    }

    // Initialize source buffer with fuzz data (clamped to size)
    size_t fillSize = (Size > yuvSize) ? yuvSize : Size;
    if (fillSize > 0) {
        memcpy(srcBuf, Data, fillSize);
        if (yuvSize > fillSize) {
            memset(srcBuf + fillSize, 0, yuvSize - fillSize);
        }
    }

    // Prepare plane pointers for planar functions
    int numPlanes = (subsamp == TJSAMP_GRAY) ? 1 : 3;
    planes = (unsigned char **)malloc(sizeof(unsigned char *) * numPlanes);
    strides = (int *)malloc(sizeof(int) * numPlanes);
    if (!planes || !strides) {
        free(srcBuf);
        cleanup_handle(&handle);
        return 0;
    }

    // Set up plane pointers and strides based on subsampling
    if (subsamp == TJSAMP_GRAY) {
        planes[0] = srcBuf;
        strides[0] = width;
    } else {
        int yStride = width;
        int uStride = (width + 1) / 2;
        int vStride = (width + 1) / 2;
        
        planes[0] = srcBuf;
        strides[0] = yStride;
        
        planes[1] = srcBuf + (width * height);
        strides[1] = uStride;
        
        planes[2] = srcBuf + (width * height) + ((height + 1) / 2 * uStride);
        strides[2] = vStride;
    }

    // Calculate destination buffer size
    int pixelSize = tjPixelSize[pixelFormat];
    unsigned long dstSize = (unsigned long)width * pixelSize * height;
    if (dstSize == 0 || dstSize > 100 * 1024 * 1024) {
        free(planes);
        free(strides);
        free(srcBuf);
        cleanup_handle(&handle);
        return 0;
    }

    dstBuf = (unsigned char *)malloc(dstSize);
    if (!dstBuf) {
        free(planes);
        free(strides);
        free(srcBuf);
        cleanup_handle(&handle);
        return 0;
    }

    // Test 3: tjDecodeYUVPlanes
    ret = tjDecodeYUVPlanes(handle, (const unsigned char **)planes, strides, subsamp, dstBuf, width, width * pixelSize, height, pixelFormat, 0);
    (void)ret;

    // Test 4: tj3DecodeYUVPlanes8
    ret = tj3DecodeYUVPlanes8(handle, (const unsigned char *const *)planes, strides, dstBuf, width, width * pixelSize, height, pixelFormat);
    (void)ret;

    // Test 5: tjDecodeYUV
    int align = 1; // Use 1 to avoid complex padding calculations in fuzzing
    ret = tjDecodeYUV(handle, srcBuf, align, subsamp, dstBuf, width, width * pixelSize, height, pixelFormat, 0);
    (void)ret;

    // Test 6: tjDecompressToYUVPlanes (requires valid JPEG data)
    // Since fuzz input might not be valid JPEG, we try with the raw data
    // but expect potential failure which is fine for fuzzing
    unsigned char *jpegBuf = Data;
    unsigned long jpegSize = Size;
    
    // Create a dummy file for potential file-based operations if needed (not used here but per instructions)
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    ret = tjDecompressToYUVPlanes(handle, jpegBuf, jpegSize, planes, width, strides, height, 0);
    (void)ret;

    // Cleanup
    free(planes);
    free(strides);
    free(srcBuf);
    free(dstBuf);
    cleanup_handle(&handle);

    return 0;
}