// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3YUVPlaneWidth at turbojpeg.c:1057:15 in turbojpeg.h
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <turbojpeg.h>

static void cleanup_handle(tjhandle *handle) {
    if (handle && *handle) {
        tjDestroy(*handle);
        *handle = NULL;
    }
}

static void cleanup_buffer(unsigned char **buf) {
    if (buf && *buf) {
        tjFree(*buf);
        *buf = NULL;
    }
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    int componentID = Data[0] % 3;
    int width = Data[1] + 1;
    int height = Data[2] + 1;
    int subsamp = Data[3] % 5;
    int align = (Data[4] % 4) + 1;
    int stride = (Data[5] % 100) + width;
    int quality = (Data[6] % 100) + 1;
    int flags = Data[7] % 4;

    // Test tj3YUVPlaneWidth
    int planeWidth = tj3YUVPlaneWidth(componentID, width, subsamp);
    (void)planeWidth;

    // Test TJBUFSIZE
    unsigned long jpegBufSize = TJBUFSIZE(width, height);
    if (jpegBufSize == (unsigned long)-1) {
        // Handle error case
    }

    // Test tjBufSizeYUV2
    unsigned long yuvBufSize = tjBufSizeYUV2(width, align, height, subsamp);
    if (yuvBufSize == (unsigned long)-1) {
        // Handle error case
    }

    // Test tjPlaneSizeYUV
    unsigned long planeSize = tjPlaneSizeYUV(componentID, width, stride, height, subsamp);
    if (planeSize == (unsigned long)-1) {
        // Handle error case
    }

    // Initialize TurboJPEG handle
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    // Allocate source buffer for YUV data
    unsigned char *srcBuf = NULL;
    unsigned char *dstBuf = NULL;
    size_t jpegSize = 0;

    // Allocate YUV buffer if size is valid
    if (yuvBufSize != (unsigned long)-1 && yuvBufSize > 0) {
        srcBuf = (unsigned char *)tjAlloc(yuvBufSize);
        if (!srcBuf) {
            cleanup_handle(&handle);
            return 0;
        }
        // Fill with fuzz data
        if (Size > 0) {
            size_t copySize = (Size < yuvBufSize) ? Size : yuvBufSize;
            memcpy(srcBuf, Data, copySize);
        }
    }

    // Allocate JPEG buffer if size is valid
    if (jpegBufSize != (unsigned long)-1 && jpegBufSize > 0) {
        dstBuf = (unsigned char *)tjAlloc(jpegBufSize);
        if (!dstBuf) {
            cleanup_buffer(&srcBuf);
            cleanup_handle(&handle);
            return 0;
        }
    }

    // Test tjCompress
    // Only call if we have valid buffers and reasonable dimensions
    if (srcBuf && dstBuf && width > 0 && height > 0) {
        // Ensure stride is at least width to avoid underflow/invalid access
        int safeStride = (stride < width) ? width : stride;
        // Use TJPF_RGB for RGB input (3 bytes per pixel)
        int pixelSize = 3; 
        int result = tjCompress(handle, srcBuf, width, safeStride, height, pixelSize, dstBuf, &jpegSize, subsamp, quality, flags);
        (void)result;
    }

    // Prepare planes for tj3CompressFromYUVPlanes8
    const unsigned char *srcPlanes[3] = {NULL, NULL, NULL};
    int strides[3] = {0, 0, 0};
    unsigned char *jpegBuf = NULL;
    size_t jpegSize2 = 0;

    if (srcBuf && yuvBufSize != (unsigned long)-1 && width > 0 && height > 0) {
        // Calculate plane sizes and strides
        unsigned long sizeY = tjPlaneSizeYUV(0, width, 0, height, subsamp);
        unsigned long sizeU = tjPlaneSizeYUV(1, width, 0, height, subsamp);
        unsigned long sizeV = tjPlaneSizeYUV(2, width, 0, height, subsamp);

        if (sizeY != (unsigned long)-1 && sizeU != (unsigned long)-1 && sizeV != (unsigned long)-1) {
            // Ensure total size fits in allocated buffer
            unsigned long totalSize = sizeY + sizeU + sizeV;
            if (totalSize <= yuvBufSize) {
                srcPlanes[0] = srcBuf;
                srcPlanes[1] = srcBuf + sizeY;
                srcPlanes[2] = srcBuf + sizeY + sizeU;

                int wY = tj3YUVPlaneWidth(0, width, subsamp);
                int wU = tj3YUVPlaneWidth(1, width, subsamp);
                int wV = tj3YUVPlaneWidth(2, width, subsamp);

                strides[0] = (wY > 0) ? wY : width;
                strides[1] = (wU > 0) ? wU : width;
                strides[2] = (wV > 0) ? wV : width;

                // Allocate JPEG buffer for compression
                unsigned long jpegBufSize2 = TJBUFSIZE(width, height);
                if (jpegBufSize2 != (unsigned long)-1) {
                    jpegBuf = (unsigned char *)tjAlloc(jpegBufSize2);
                    if (jpegBuf) {
                        int result = tj3CompressFromYUVPlanes8(handle, srcPlanes, width, strides, height, &jpegBuf, &jpegSize2);
                        (void)result;
                    }
                }
            }
        }
    }

    // Cleanup
    cleanup_buffer(&srcBuf);
    cleanup_buffer(&dstBuf);
    cleanup_buffer(&jpegBuf);
    cleanup_handle(&handle);

    return 0;
}