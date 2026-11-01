// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSize at turbojpeg.c:933:25 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static void init_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    int width = Data[0];
    int height = Data[1];
    int subsamp = Data[2] % 5; // TJSAMP_444 to TJSAMP_GRAY range roughly
    int quality = Data[3] % 101;

    // Clamp dimensions to reasonable limits to avoid immediate crashes or huge allocations
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        width = 64;
        height = 64;
    }

    // Test 1: TJBUFSIZE
    unsigned long bufSize1 = TJBUFSIZE(width, height);
    if (bufSize1 == (unsigned long)-1) {
        // Error case, continue
    } else {
        unsigned char *dstBuf = malloc(bufSize1);
        if (dstBuf) {
            tjhandle handle = tjInitCompress();
            if (handle) {
                unsigned long jpegSize = 0;
                // Create a dummy source buffer
                unsigned char *srcBuf = malloc(width * height * 3);
                if (srcBuf) {
                    memset(srcBuf, 0, width * height * 3);
                    int pitch = width * 3;
                    int pixelSize = TJPF_RGB;
                    int ret = tjCompress(handle, srcBuf, width, pitch, height, pixelSize,
                                         dstBuf, &jpegSize, subsamp, quality, 0);
                    (void)ret;
                    free(srcBuf);
                }
                tjDestroy(handle);
            }
            free(dstBuf);
        }
    }

    // Test 2: tjBufSize
    unsigned long bufSize2 = tjBufSize(width, height, subsamp);
    if (bufSize2 == (unsigned long)-1) {
        // Error case
    } else {
        unsigned char *jpegBuf = malloc(bufSize2);
        if (jpegBuf) {
            tjhandle handle = tjInitCompress();
            if (handle) {
                unsigned long jpegSize = 0;
                unsigned char *srcBuf = malloc(width * height * 3);
                if (srcBuf) {
                    memset(srcBuf, 0, width * height * 3);
                    int pitch = width * 3;
                    int pixelSize = TJPF_RGB;
                    int ret = tjCompress(handle, srcBuf, width, pitch, height, pixelSize,
                                         jpegBuf, &jpegSize, subsamp, quality, 0);
                    (void)ret;
                    free(srcBuf);
                }
                tjDestroy(handle);
            }
            free(jpegBuf);
        }
    }

    // Test 3: tjBufSizeYUV2
    int align = (Size > 4) ? (Data[4] % 16 + 1) : 1;
    unsigned long yuvSize = tjBufSizeYUV2(width, align, height, subsamp);
    if (yuvSize == (unsigned long)-1) {
        // Error case
    } else {
        unsigned char *yuvBuf = malloc(yuvSize);
        if (yuvBuf) {
            memset(yuvBuf, 0, yuvSize);
            free(yuvBuf);
        }
    }

    // Test 4: tj3YUVPlaneSize
    int componentID = (Size > 5) ? (Data[5] % 3) : 0;
    int stride = (Size > 6) ? Data[6] : 0;
    size_t planeSize = tj3YUVPlaneSize(componentID, width, stride, height, subsamp);
    if (planeSize > 0) {
        unsigned char *planeBuf = malloc(planeSize);
        if (planeBuf) {
            memset(planeBuf, 0, planeSize);
            free(planeBuf);
        }
    }

    // Test 5: tj3CompressFromYUVPlanes8
    tjhandle handle = tjInitCompress();
    if (handle) {
        unsigned char *yPlane = NULL, *uPlane = NULL, *vPlane = NULL;
        unsigned char *jpegBuf = NULL;
        size_t jpegSize = 0;
        int strides[3] = {0, 0, 0};

        // Calculate plane sizes
        size_t ySize = tj3YUVPlaneSize(0, width, 0, height, subsamp);
        size_t uSize = tj3YUVPlaneSize(1, width, 0, height, subsamp);
        size_t vSize = tj3YUVPlaneSize(2, width, 0, height, subsamp);

        if (ySize > 0) yPlane = malloc(ySize);
        if (uSize > 0) uPlane = malloc(uSize);
        if (vSize > 0) vPlane = malloc(vSize);

        if (yPlane) memset(yPlane, 128, ySize);
        if (uPlane) memset(uPlane, 128, uSize);
        if (vPlane) memset(vPlane, 128, vSize);

        const unsigned char *srcPlanes[3];
        srcPlanes[0] = yPlane;
        srcPlanes[1] = uPlane;
        srcPlanes[2] = vPlane;

        // If subsamp is grayscale, we might only need Y, but the API expects array
        // We pass all, but if subsamp implies fewer planes, the library handles it or errors.
        int ret = tj3CompressFromYUVPlanes8(handle, srcPlanes, width, strides, height,
                                            &jpegBuf, &jpegSize);
        (void)ret;

        if (jpegBuf) free(jpegBuf);
        if (yPlane) free(yPlane);
        if (uPlane) free(uPlane);
        if (vPlane) free(vPlane);

        tjDestroy(handle);
    }

    // Write dummy file if data is present
    if (Size > 0) {
        init_dummy_file(Data, Size);
    }

    return 0;
}