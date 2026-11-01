// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void cleanup_handle(tjhandle *handle) {
    if (handle && *handle) {
        tj3Destroy(*handle);
        *handle = NULL;
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Need at least 5 bytes to safely read width, height, subsamp, align, and pixelFormat
    if (Size < 5) return 0;

    tjhandle handle = NULL;
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    unsigned char *yuvBuf = NULL;
    unsigned char *rgbBuf = NULL;
    int width, height, subsamp, align, pixelFormat;
    int ret;

    // Extract parameters from input data
    width = Data[0];
    height = Data[1];
    subsamp = Data[2];
    align = Data[3];
    pixelFormat = Data[4] % 6; // TJPF_GRAY to TJPF_XBGR

    // Validate and constrain parameters
    if (width <= 0 || height <= 0) return 0;
    if (width > 8192 || height > 8192) return 0;

    // Ensure align is power of 2 and valid
    if (align <= 0 || (align & (align - 1)) != 0) {
        align = 1;
    } else if (align > 32) {
        align = 16;
    }

    // Ensure subsamp is valid (common values: 4:4:4, 4:2:2, 4:2:0, etc.)
    // Valid subsamp values are typically 0-5, but we clamp to safe range
    if (subsamp < 0 || subsamp > 5) {
        subsamp = 0; // Default to 4:4:4
    }

    // Initialize TurboJPEG handle for decompression
    handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;

    // Test tjPlaneWidth
    int planeWidth = tjPlaneWidth(0, width, subsamp);
    if (planeWidth < 0) {
        cleanup_handle(&handle);
        return 0;
    }

    // Test tjBufSizeYUV (legacy)
    unsigned long bufSizeLegacy = tjBufSizeYUV(width, height, subsamp);

    // Test tj3YUVBufSize
    size_t yuvSize = tj3YUVBufSize(width, align, height, subsamp);
    if (yuvSize == 0) {
        cleanup_handle(&handle);
        return 0;
    }

    // Allocate YUV buffer
    yuvBuf = (unsigned char *)malloc(yuvSize);
    if (!yuvBuf) {
        cleanup_handle(&handle);
        return 0;
    }

    // Fill YUV buffer with fuzz data
    memset(yuvBuf, 0, yuvSize);
    size_t fillSize = (Size > yuvSize) ? yuvSize : Size;
    memcpy(yuvBuf, Data, fillSize);

    // Test tj3DecodeYUV8 (YUV to RGB)
    size_t rgbSize = (size_t)width * height * 4; // Assume 4 bytes per pixel (e.g., RGBA)
    rgbBuf = (unsigned char *)malloc(rgbSize);
    if (!rgbBuf) {
        cleanup_handle(&handle);
        free(yuvBuf);
        return 0;
    }

    ret = tj3DecodeYUV8(handle, yuvBuf, align, rgbBuf, width, 0, height, pixelFormat);
    // Ignore return value for fuzzing, just exercise the function

    // Prepare for compression test
    // Create a new handle for compression
    tjhandle compHandle = tj3Init(TJINIT_COMPRESS);
    if (compHandle) {
        unsigned char *compressedJpeg = NULL;
        size_t compressedSize = 0;

        ret = tj3CompressFromYUV8(compHandle, yuvBuf, width, align, height, &compressedJpeg, &compressedSize);
        
        if (ret == 0 && compressedJpeg && compressedSize > 0) {
            // Now test decompression with the generated JPEG
            jpegBuf = compressedJpeg;
            jpegSize = compressedSize;

            // Re-initialize handle for decompression
            tjhandle decompHandle = tj3Init(TJINIT_DECOMPRESS);
            if (decompHandle) {
                unsigned char *decompYuvBuf = NULL;
                size_t decompYuvSize = tj3YUVBufSize(width, align, height, subsamp);
                if (decompYuvSize > 0) {
                    decompYuvBuf = (unsigned char *)malloc(decompYuvSize);
                    if (decompYuvBuf) {
                        ret = tj3DecompressToYUV8(decompHandle, jpegBuf, jpegSize, decompYuvBuf, align);
                        free(decompYuvBuf);
                    }
                }
                tj3Destroy(decompHandle);
            }
        }

        if (compressedJpeg) {
            free(compressedJpeg);
        }
        tj3Destroy(compHandle);
    }

    // Cleanup
    free(rgbBuf);
    free(yuvBuf);
    cleanup_handle(&handle);

    return 0;
}