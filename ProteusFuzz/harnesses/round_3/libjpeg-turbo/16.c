// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 12) {
        return 0;
    }

    // Parse input to determine configuration
    int initType = Data[0];
    int param1 = Data[1];
    int val1 = Data[2];
    int param2 = Data[3];
    int val2 = Data[4];
    int param3 = Data[5];
    int val3 = Data[6];
    int param4 = Data[7];
    int val4 = Data[8];
    int param5 = Data[9];
    int val5 = Data[10] + 1;

    // Create dummy file for tj3LoadImage16
    const char *dummyFile = "./dummy_file";
    FILE *f = fopen(dummyFile, "wb");
    if (f) {
        // Write a minimal valid PGM header and data for 16-bit
        int w = (Data[10] % 10) + 1;
        int h = (Data[11] % 10) + 1;
        int maxval = 65535; // 16-bit
        
        fprintf(f, "P5\n%d %d\n%d\n", w, h, maxval);
        // Write dummy pixel data (2 bytes per pixel)
        for (int i = 0; i < w * h; i++) {
            unsigned char b1 = Data[(12 + i) % Size];
            unsigned char b2 = Data[(13 + i) % Size];
            unsigned short pixel = (b1 << 8) | b2;
            fwrite(&pixel, sizeof(unsigned short), 1, f);
        }
        fclose(f);
    }

    tjhandle handle = tj3Init(initType);
    if (!handle) {
        return 0;
    }

    // tj3Set calls
    tj3Set(handle, param1, val1);
    tj3Set(handle, param2, val2);
    tj3Set(handle, param3, val3);
    tj3Set(handle, param4, val4);
    tj3Set(handle, param5, val5);

    // Load image
    int width = 0;
    int height = 0;
    int pixelFormat = TJPF_UNKNOWN;
    int align = 1; // Power of 2

    unsigned short *srcBuf = tj3LoadImage16(handle, dummyFile, &width, align, &height, &pixelFormat);
    
    if (!srcBuf) {
        tj3Destroy(handle);
        return 0;
    }

    // Calculate buffer size
    int jpegSubsamp = TJSAMP_444;
    size_t jpegSize = tj3JPEGBufSize(width, height, jpegSubsamp);
    
    // Get current settings
    int quality = tj3Get(handle, TJPARAM_QUALITY);
    (void)quality;

    // Allocate buffer
    unsigned char *jpegBuf = NULL;
    jpegBuf = (unsigned char *)tj3Alloc(jpegSize);
    if (!jpegBuf) {
        tj3Free(srcBuf);
        tj3Destroy(handle);
        return 0;
    }
    size_t jpegSizePtr = jpegSize;

    // More tj3Set calls before compression
    tj3Set(handle, param1, val1);
    tj3Set(handle, param2, val2);
    tj3Set(handle, param3, val3);

    // Compress
    int pitch = width * tjPixelSize[pixelFormat];
    int ret = tj3Compress16(handle, srcBuf, width, pitch, height, pixelFormat, &jpegBuf, &jpegSizePtr);
    (void)ret;

    // Cleanup
    tj3Free(jpegBuf);
    tj3Free(srcBuf);
    tj3Destroy(handle);

    return 0;
}