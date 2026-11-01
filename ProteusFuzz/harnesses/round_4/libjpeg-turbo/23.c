// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int is_power_of_2(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    uint32_t width = (Data[0] | (Data[1] << 8)) & 0x3FF;
    uint32_t height = (Data[2] | (Data[3] << 8)) & 0x3FF;
    int subsamp = (Data[4] & 0x07);
    int pixelFormat = (Data[5] & 0x0F);
    int align = (Data[6] & 0x0F);
    int initType = (Data[7] & 0x01) ? TJINIT_COMPRESS : TJINIT_DECOMPRESS;
    int quality = (Data[8] & 0x3F) + 1;

    if (width == 0) width = 16;
    if (height == 0) height = 16;
    if (!is_power_of_2(align) || align == 0) align = 1;
    if (align > 64) align = 64;

    if (pixelFormat > 10) pixelFormat = 0;

    tjhandle handle = tj3Init(initType);
    if (!handle) return 0;

    // Map subsamp input to valid TJPARAM_SUBSAMP values
    // 0: 4:4:4, 1: 4:2:2, 2: 4:2:0, 3: 4:1:1, 4: Grayscale
    int subsampValue = 0;
    switch (subsamp) {
        case 0: subsampValue = 0; break; // 4:4:4
        case 1: subsampValue = 1; break; // 4:2:2
        case 2: subsampValue = 2; break; // 4:2:0
        case 3: subsampValue = 3; break; // 4:1:1
        case 4: subsampValue = 4; break; // Grayscale
        default: subsampValue = 2; break; // Default to 4:2:0
    }

    if (initType == TJINIT_COMPRESS) {
        if (tj3Set(handle, TJPARAM_SUBSAMP, subsampValue) != 0) {
            tj3Destroy(handle);
            return 0;
        }
        if (tj3Set(handle, TJPARAM_QUALITY, quality) != 0) {
            // Quality might fail on decompression handles, ignore for compress
        }
    } else {
        // Decompression handle
        if (tj3Set(handle, TJPARAM_SUBSAMP, subsampValue) != 0) {
            tj3Destroy(handle);
            return 0;
        }
    }

    // tj3YUVBufSize does not take a handle argument
    size_t yuvSize = tj3YUVBufSize(width, align, height, subsampValue);
    if (yuvSize == 0) {
        tj3Destroy(handle);
        return 0;
    }

    unsigned char *yuvBuf = (unsigned char *)malloc(yuvSize);
    unsigned char *rgbBuf = (unsigned char *)malloc(width * height * 4);
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;

    if (!yuvBuf || !rgbBuf) {
        free(yuvBuf);
        free(rgbBuf);
        tj3Destroy(handle);
        return 0;
    }

    memset(yuvBuf, 0, yuvSize);
    memset(rgbBuf, 0, width * height * 4);

    // Fill buffers with fuzz data
    size_t fillSize = (Size > yuvSize) ? yuvSize : Size;
    memcpy(yuvBuf, Data, fillSize);
    fillSize = (Size > width * height * 4) ? width * height * 4 : Size;
    memcpy(rgbBuf, Data, fillSize);

    // Test tj3EncodeYUV8 (Compress handle)
    if (initType == TJINIT_COMPRESS) {
        tj3EncodeYUV8(handle, rgbBuf, width, 0, height, pixelFormat, yuvBuf, align);
        
        // Test tj3CompressFromYUV8
        unsigned char *compJpegBuf = NULL;
        size_t compJpegSize = 0;
        tj3CompressFromYUV8(handle, yuvBuf, width, align, height, &compJpegBuf, &compJpegSize);
        if (compJpegBuf) {
            // Write to dummy file if we have a jpeg
            if (compJpegSize > 0) {
                write_dummy_file(compJpegBuf, compJpegSize);
            }
            tj3Free(compJpegBuf);
        }
    }

    // Test tj3DecompressToYUV8 (Decompress handle)
    if (initType == TJINIT_DECOMPRESS) {
        // Attempt to decompress the input data as a JPEG to test robustness/error handling
        unsigned char *decompYuvBuf = (unsigned char *)malloc(yuvSize);
        if (decompYuvBuf) {
            tj3DecompressToYUV8(handle, Data, Size, decompYuvBuf, align);
            free(decompYuvBuf);
        }
    }

    // Test tj3DecodeYUV8 (Decompress handle)
    if (initType == TJINIT_DECOMPRESS) {
        int pitch = width * tjPixelSize[pixelFormat];
        tj3DecodeYUV8(handle, yuvBuf, align, rgbBuf, width, pitch, height, pixelFormat);
    }

    // Test tj3Set with various params to trigger error paths
    tj3Set(handle, TJPARAM_SUBSAMP, -1);
    tj3Set(handle, TJPARAM_QUALITY, -1);
    tj3Set(handle, TJPARAM_JPEGWIDTH, -1);
    tj3Set(handle, TJPARAM_JPEGHEIGHT, -1);

    free(yuvBuf);
    free(rgbBuf);
    tj3Destroy(handle);

    return 0;
}