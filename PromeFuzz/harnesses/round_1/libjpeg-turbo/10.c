// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3SetICCProfile at turbojpeg.c:1164:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3CompressFromYUVPlanes8 at turbojpeg.c:1259:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static void cleanup_jpeg_buffer(unsigned char **jpegBuf, size_t *jpegSize) {
    if (*jpegBuf) {
        free(*jpegBuf);
        *jpegBuf = NULL;
    }
    if (jpegSize) {
        *jpegSize = 0;
    }
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 32) return 0;

    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return 0;

    // Extract parameters from fuzz data
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int subsamp = Data[4] % 5; // 0-4 for TJSAMP
    int pixelFormat = Data[5] % 12; // 0-11 for TJPF
    int align = 1 << (Data[6] % 5); // 1,2,4,8,16
    int quality = Data[7];
    int param = Data[8] % 32; // TJPARAM values
    int value = Data[9];

    // Bound dimensions to avoid excessive memory usage
    if (width > 1024) width = 1024;
    if (height > 1024) height = 1024;
    if (width < 1) width = 1;
    if (height < 1) height = 1;

    // Set parameters
    tj3Set(handle, TJPARAM_SUBSAMP, subsamp);
    tj3Set(handle, TJPARAM_QUALITY, quality);
    tj3Set(handle, param, value);

    // Set ICC profile if enough data
    if (Size > 64) {
        size_t iccSize = Data[10] << 8 | Data[11];
        if (iccSize > 0 && iccSize < Size - 64) {
            unsigned char *iccBuf = (unsigned char *)malloc(iccSize);
            if (iccBuf) {
                memcpy(iccBuf, Data + 64, iccSize);
                tj3SetICCProfile(handle, iccBuf, iccSize);
                free(iccBuf);
            }
        }
    }

    // Test tj3YUVBufSize
    size_t yuvBufSize = tj3YUVBufSize(width, align, height, subsamp);
    if (yuvBufSize > 0 && yuvBufSize < 100 * 1024 * 1024) { // Limit to 100MB
        unsigned char *yuvBuf = (unsigned char *)malloc(yuvBufSize);
        unsigned char *rgbBuf = (unsigned char *)malloc(width * height * 4);
        if (yuvBuf && rgbBuf) {
            // Fill with fuzz data
            size_t rgbSize = width * height * 4;
            size_t copySize = Size > rgbSize ? rgbSize : Size;
            memcpy(rgbBuf, Data, copySize);

            // Test tj3EncodeYUV8
            tj3EncodeYUV8(handle, rgbBuf, width, 0, height, pixelFormat, yuvBuf, align);

            // Test tj3CompressFromYUV8
            unsigned char *jpegBuf = NULL;
            size_t jpegSize = 0;
            tj3CompressFromYUV8(handle, yuvBuf, width, align, height, &jpegBuf, &jpegSize);
            cleanup_jpeg_buffer(&jpegBuf, &jpegSize);

            // Test tj3CompressFromYUVPlanes8
            if (subsamp != TJSAMP_GRAY) {
                const unsigned char *srcPlanes[3];
                int strides[3] = {0, 0, 0};
                
                // For simplicity, use same buffer for all planes
                srcPlanes[0] = yuvBuf;
                srcPlanes[1] = yuvBuf;
                srcPlanes[2] = yuvBuf;
                
                tj3CompressFromYUVPlanes8(handle, srcPlanes, width, strides, height, &jpegBuf, &jpegSize);
                cleanup_jpeg_buffer(&jpegBuf, &jpegSize);
            }
        }
        free(yuvBuf);
        free(rgbBuf);
    }

    tj3Destroy(handle);
    return 0;
}