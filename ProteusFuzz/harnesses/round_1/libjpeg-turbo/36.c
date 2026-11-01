// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// TJBUFSIZE at turbojpeg.c:948:25 in turbojpeg.h
// tjCompress at turbojpeg.c:1235:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// TJBUFSIZEYUV at turbojpeg.c:1013:25 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int init_tj_handle(tjhandle *handle) {
    *handle = tjInitCompress();
    if (*handle == NULL) {
        return -1;
    }
    return 0;
}

static void cleanup_tj_handle(tjhandle handle) {
    if (handle != NULL) {
        tjDestroy(handle);
    }
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    int width = Data[0];
    int height = Data[1];
    int subsamp = (Size > 2) ? Data[2] % 5 : 0;
    int componentID = (Size > 3) ? Data[3] % 3 : 0;
    int stride = (Size > 4) ? (int)Data[4] : 0;
    int align = (Size > 5) ? (int)Data[5] % 16 : 1;
    int quality = (Size > 6) ? (int)Data[6] % 100 : 90;
    int flags = (Size > 7) ? (int)Data[7] % 4 : 0;

    // Clamp dimensions to reasonable values to avoid allocation failures
    if (width <= 0 || width > 4096) width = 64;
    if (height <= 0 || height > 4096) height = 64;
    if (stride < 0) stride = 0;

    // Test tjPlaneSizeYUV
    unsigned long planeSize = tjPlaneSizeYUV(componentID, width, stride, height, subsamp);
    if (planeSize == (unsigned long)-1) {
        // Invalid parameters, ignore
    }

    // Test TJBUFSIZEYUV
    unsigned long bufSizeYUV = TJBUFSIZEYUV(width, height, subsamp);
    if (bufSizeYUV == 0) {
        // Invalid parameters, ignore
    }

    // Test tjBufSizeYUV
    unsigned long legacyBufSize = tjBufSizeYUV(width, height, subsamp);
    if (legacyBufSize == 0) {
        // Invalid parameters, ignore
    }

    // Test tjBufSizeYUV2
    unsigned long bufSizeYUV2 = tjBufSizeYUV2(width, align, height, subsamp);
    if (bufSizeYUV2 == 0) {
        // Invalid parameters, ignore
    }

    // Test tj3YUVPlaneSize
    size_t planeSize3 = tj3YUVPlaneSize(componentID, width, stride, height, subsamp);
    if (planeSize3 == 0) {
        // Invalid parameters, ignore
    }

    // Test tjCompress
    tjhandle handle;
    if (init_tj_handle(&handle) != 0) {
        return 0;
    }

    // Allocate source buffer (RGB or YUV depending on flags)
    int pixelSize = (flags & TJ_YUV) ? 1 : 3; // Simplified assumption
    int pitch = width * pixelSize;
    unsigned long srcBufSize = (unsigned long)width * height * pixelSize;
    unsigned char *srcBuf = NULL;
    if (srcBufSize > 0 && srcBufSize < 1024 * 1024 * 10) {
        srcBuf = (unsigned char *)malloc(srcBufSize);
        if (srcBuf != NULL) {
            if (Size > 8) {
                size_t copySize = (Size - 8 < srcBufSize) ? Size - 8 : srcBufSize;
                memcpy(srcBuf, Data + 8, copySize);
            } else {
                memset(srcBuf, 0, srcBufSize);
            }
        }
    }

    // Allocate destination buffer
    unsigned long jpegBufSize = 0;
    if (flags & TJ_YUV) {
        jpegBufSize = bufSizeYUV2;
    } else {
        // TJBUFSIZE only takes width and height
        jpegBufSize = TJBUFSIZE(width, height);
    }

    unsigned char *dstBuf = NULL;
    if (jpegBufSize > 0 && jpegBufSize < 1024 * 1024 * 10) {
        dstBuf = (unsigned char *)malloc(jpegBufSize);
    }

    unsigned long compressedSize = jpegBufSize;
    int result = 0;

    if (srcBuf != NULL && dstBuf != NULL) {
        result = tjCompress(handle, srcBuf, width, pitch, height, pixelSize,
                            dstBuf, &compressedSize, subsamp, quality, flags);
        if (result == -1) {
            // Error occurred, ignore
        }
    }

    if (srcBuf != NULL) {
        free(srcBuf);
    }
    if (dstBuf != NULL) {
        free(dstBuf);
    }

    cleanup_tj_handle(handle);

    return 0;
}