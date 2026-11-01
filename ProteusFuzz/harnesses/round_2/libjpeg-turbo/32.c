// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void write_dummy_ppm(const char *filename, int width, int height, const uint8_t *data, size_t dataSize) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    // Calculate how many pixels we can actually write based on available data
    // PPM needs 3 bytes per pixel (RGB)
    size_t maxPixels = dataSize / 3;
    size_t totalPixels = (size_t)width * height;
    
    if (totalPixels > maxPixels) {
        // Adjust width/height to fit available data
        // Try to keep aspect ratio roughly
        if (width > 0) {
            height = (int)(maxPixels / width);
            if (height <= 0) height = 1;
        } else {
            width = 1;
            height = 1;
        }
    }
    
    fprintf(f, "P6\n%d %d\n255\n", width, height);
    size_t bytesToWrite = (size_t)width * height * 3;
    if (bytesToWrite > dataSize) bytesToWrite = dataSize;
    
    fwrite(data, 1, bytesToWrite, f);
    fclose(f);
}

static void write_dummy_pgm16(const char *filename, int width, int height, const uint8_t *data, size_t dataSize) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    // PGM16 needs 2 bytes per pixel
    size_t maxPixels = dataSize / 2;
    size_t totalPixels = (size_t)width * height;
    
    if (totalPixels > maxPixels) {
        if (width > 0) {
            height = (int)(maxPixels / width);
            if (height <= 0) height = 1;
        } else {
            width = 1;
            height = 1;
        }
    }
    
    fprintf(f, "P5\n%d %d\n65535\n", width, height);
    size_t bytesToWrite = (size_t)width * height * 2;
    if (bytesToWrite > dataSize) bytesToWrite = dataSize;
    
    // Write as raw bytes, the loader will interpret them as 16-bit
    fwrite(data, 1, bytesToWrite, f);
    fclose(f);
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    int width = Data[0];
    int height = Data[1];
    int pixelFormat = Data[2] % 5; // 0 to 4
    int align = (Data[3] % 4) + 1; // 1, 2, 4, 8
    if (align > 8) align = 8;
    if (align & (align - 1)) align = 1; // Ensure power of 2

    if (width <= 0) width = 1;
    if (height <= 0) height = 1;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    // Test tjAlloc and tjFree
    unsigned char *allocBuf = tjAlloc(width * height * 4);
    if (allocBuf) {
        memset(allocBuf, 0, width * height * 4);
        tjFree(allocBuf);
    }

    // Test tjLoadImage and tjSaveImage
    if (Size > 4) {
        size_t availableData = Size - 4;
        write_dummy_ppm("./dummy_file.ppm", width, height, Data + 4, availableData);
        
        int w, h, pf;
        unsigned char *buf = tjLoadImage("./dummy_file.ppm", &w, align, &h, &pf, 0);
        if (buf) {
            // Use the dimensions returned by the loader
            tjSaveImage("./dummy_file_out.ppm", buf, w, w * 3, h, pf, 0);
            tjFree(buf);
        }
    }

    // Test tj3LoadImage16 and tj3SaveImage16
    if (Size > 4) {
        size_t availableData = Size - 4;
        write_dummy_pgm16("./dummy_file.pgm", width, height, Data + 4, availableData);
        
        int w16, h16, pf16;
        unsigned short *buf16 = tj3LoadImage16(handle, "./dummy_file.pgm", &w16, align, &h16, &pf16);
        if (buf16) {
            tj3SaveImage16(handle, "./dummy_file_out.pgm", buf16, w16, 0, h16, pf16);
            tj3Free(buf16);
        }
    }

    tjDestroy(handle);
    return 0;
}