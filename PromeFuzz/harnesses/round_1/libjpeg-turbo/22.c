// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static tjhandle init_decompress_handle(void) {
    return tjInitDecompress();
}

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return -1;
    }
    fwrite(data, 1, size, fp);
    fclose(fp);
    return 0;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Write input data to dummy file for potential file-based operations
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }

    // Initialize decompression handles
    tjhandle handle1 = init_decompress_handle();
    tjhandle handle2 = init_decompress_handle();
    if (!handle1 || !handle2) {
        cleanup_handle(handle1);
        cleanup_handle(handle2);
        return 0;
    }

    // Use first byte to control fuzzing behavior
    uint8_t control = Data[0];
    const uint8_t *jpeg_data = Data + 1;
    size_t jpeg_size = Size - 1;

    // Test tjDecompressHeader2
    int width = 0, height = 0, jpegSubsamp = 0;
    if (jpeg_size > 0) {
        tjDecompressHeader2(handle1, (unsigned char *)jpeg_data, jpeg_size,
                           &width, &height, &jpegSubsamp);
    }

    // Test tjDecompressHeader (legacy version)
    int width2 = 0, height2 = 0;
    if (jpeg_size > 0) {
        tjDecompressHeader(handle2, (unsigned char *)jpeg_data, jpeg_size,
                          &width2, &height2);
    }

    // Test tj3SetScalingFactor if we have a valid handle
    if (control & 0x01) {
        // Create a simple scaling factor structure
        tjscalingfactor scalingFactor;
        scalingFactor.num = 1;
        scalingFactor.denom = 2;  // 1/2 scaling
        
        // Note: tj3SetScalingFactor requires a tj3 handle, but we're using
        // the older API handles. We'll still call it for coverage.
        // In practice, you'd need to use tj3Init() for this.
        // We'll call it with our existing handle for fuzzing purposes.
        tj3SetScalingFactor(handle1, scalingFactor);
    }

    // Test tjDecompressToYUV2 if we have valid dimensions
    if (width > 0 && height > 0 && jpeg_size > 0) {
        // Limit dimensions to prevent excessive memory allocation
        if (width > 4096) width = 4096;
        if (height > 4096) height = 4096;
        
        // Calculate buffer size for YUV planar format
        unsigned long yuv_size = width * height * 3 / 2;
        if (yuv_size > 0 && yuv_size < 100 * 1024 * 1024) {  // Limit to 100MB
            unsigned char *dst_buf = (unsigned char *)malloc(yuv_size);
            if (dst_buf) {
                int align = 1;  // Byte alignment
                int flags = 0;
                
                tjDecompressToYUV2(handle1, (unsigned char *)jpeg_data, jpeg_size,
                                  dst_buf, width, align, height, flags);
                free(dst_buf);
            }
        }
    }

    // Test tjDecompress if we have valid dimensions
    if (width2 > 0 && height2 > 0 && jpeg_size > 0) {
        // Limit dimensions to prevent excessive memory allocation
        if (width2 > 4096) width2 = 4096;
        if (height2 > 4096) height2 = 4096;
        
        // Calculate buffer size for RGB format
        unsigned long rgb_size = width2 * height2 * 3;  // 3 bytes per pixel for RGB
        if (rgb_size > 0 && rgb_size < 100 * 1024 * 1024) {  // Limit to 100MB
            unsigned char *dst_buf = (unsigned char *)malloc(rgb_size);
            if (dst_buf) {
                int pitch = width2 * 3;  // RGB pitch
                int pixelSize = 3;       // RGB
                int flags = 0;
                
                tjDecompress(handle2, (unsigned char *)jpeg_data, jpeg_size,
                            dst_buf, width2, pitch, height2, pixelSize, flags);
                free(dst_buf);
            }
        }
    }

    // Clean up
    cleanup_handle(handle1);
    cleanup_handle(handle2);

    return 0;
}