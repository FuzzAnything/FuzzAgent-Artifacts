// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjCompress2 at turbojpeg.c:1204:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tjCompressFromYUV at turbojpeg.c:1476:15 in turbojpeg.h
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

static void cleanup_buffer(unsigned char **buf) {
    if (buf && *buf) {
        free(*buf);
        *buf = NULL;
    }
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    tjhandle handle = NULL;
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    int width, height, subsamp, quality, flags;
    int ret = 0;

    // Extract parameters from input data with bounds checking
    width = (Data[0] % 100) + 1;
    height = (Data[1] % 100) + 1;
    subsamp = (Size > 2) ? (Data[2] % 4) : 0; // TJSAMP_444, 422, 420, 422H
    quality = (Size > 3) ? ((Data[3] % 90) + 10) : 50;
    flags = (Size > 4) ? (Data[4] % 4) : 0;

    // Ensure dimensions are reasonable to avoid immediate allocation failures
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    // Initialize TurboJPEG instance for compression
    handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return 0;

    // Set parameters using tj3Set
    if (tj3Set(handle, TJPARAM_QUALITY, quality) < 0) {
        cleanup_handle(&handle);
        return 0;
    }

    if (tj3Set(handle, TJPARAM_SUBSAMP, subsamp) < 0) {
        cleanup_handle(&handle);
        return 0;
    }

    // Calculate required JPEG buffer size
    size_t maxJpegSize = tj3JPEGBufSize(width, height, subsamp);
    if (maxJpegSize == 0) {
        cleanup_handle(&handle);
        return 0;
    }

    // Allocate source buffer for YUV or RGB data
    size_t yuvSize = tj3YUVBufSize(width, 1, height, subsamp);
    unsigned char *srcBuf = NULL;
    if (yuvSize > 0 && yuvSize < (Size * 10)) {
        srcBuf = (unsigned char *)malloc(yuvSize);
        if (srcBuf) {
            // Fill with fuzzed data
            if (Size > 0) {
                size_t fillSize = (Size < yuvSize) ? Size : yuvSize;
                memcpy(srcBuf, Data, fillSize);
                // Pad rest with zeros or pattern
                if (fillSize < yuvSize) {
                    memset(srcBuf + fillSize, 0, yuvSize - fillSize);
                }
            }
        }
    }

    // Test tj3CompressFromYUV8
    if (srcBuf) {
        jpegSize = 0;
        ret = tj3CompressFromYUV8(handle, srcBuf, width, 1, height, &jpegBuf, &jpegSize);
        cleanup_buffer(&jpegBuf);
    }

    // Test tjCompressFromYUV (legacy)
    if (srcBuf) {
        jpegSize = 0;
        ret = tjCompressFromYUV(handle, srcBuf, width, 1, height, subsamp, &jpegBuf, &jpegSize, quality, flags);
        cleanup_buffer(&jpegBuf);
    }

    // Test tjCompress2 (RGB input)
    if (srcBuf) {
        // Reuse srcBuf as RGB buffer (format doesn't strictly matter for fuzzing logic)
        int pixelFormat = (Size > 5) ? (Data[5] % 6) : TJPF_RGB;
        int pitch = width * 3; // Assume RGB
        jpegSize = 0;
        ret = tjCompress2(handle, srcBuf, width, pitch, height, pixelFormat, &jpegBuf, &jpegSize, subsamp, quality, flags);
        cleanup_buffer(&jpegBuf);
    }

    // Cleanup
    free(srcBuf);
    cleanup_handle(&handle);

    return 0;
}