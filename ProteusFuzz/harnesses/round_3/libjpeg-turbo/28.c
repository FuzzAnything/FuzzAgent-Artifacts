// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static void write_dummy_ppm(const char *filename, int width, int height, int pixelFormat, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    int channels = 1;
    if (pixelFormat == TJPF_RGB || pixelFormat == TJPF_CMYK || pixelFormat == TJPF_XRGB || pixelFormat == TJPF_BGR || pixelFormat == TJPF_ABGR || pixelFormat == TJPF_XBGR) {
        channels = 3;
    } else if (pixelFormat == TJPF_RGBA || pixelFormat == TJPF_BGRA || pixelFormat == TJPF_ARGB || pixelFormat == TJPF_ABGR) {
        channels = 4;
    }
    
    // Write PPM header
    fprintf(f, "P6\n%d %d\n255\n", width, height);
    
    // Write dummy data based on available input or generate random
    size_t pixels = width * height * channels;
    if (size >= pixels) {
        fwrite(data, 1, pixels, f);
    } else {
        uint8_t *buf = malloc(pixels);
        if (buf) {
            if (size > 0) {
                memcpy(buf, data, size);
            }
            if (size < pixels) {
                memset(buf + size, 0, pixels - size);
            }
            fwrite(buf, 1, pixels, f);
            free(buf);
        }
    }
    fclose(f);
}

static void write_dummy_pgm16(const char *filename, int width, int height, const uint16_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    fprintf(f, "P5\n%d %d\n65535\n", width, height);
    
    size_t pixels = width * height;
    if (size >= pixels) {
        fwrite(data, sizeof(uint16_t), pixels, f);
    } else {
        uint16_t *buf = malloc(pixels * sizeof(uint16_t));
        if (buf) {
            if (size > 0) {
                memcpy(buf, data, size * sizeof(uint16_t));
            }
            if (size < pixels) {
                memset(buf + size, 0, (pixels - size) * sizeof(uint16_t));
            }
            fwrite(buf, sizeof(uint16_t), pixels, f);
            free(buf);
        }
    }
    fclose(f);
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Extract parameters from input
    int width = Data[0] + 1;
    int height = Data[1] + 1;
    
    // Limit dimensions to prevent huge allocations
    if (width > 1024 || height > 1024) {
        width = 1024;
        height = 1024;
    }
    if (width == 0 || height == 0) {
        width = 1;
        height = 1;
    }

    // Create TurboJPEG handle
    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    // Test tj3LoadImage16 and tj3SaveImage16
    int align = 1;
    int pixelFormat = (Size > 2) ? (Data[2] % 6) : 0; // TJPF_GRAY to TJPF_CMYK
    int widthOut, heightOut, formatOut;
    
    // Write dummy PGM file for 16-bit load test
    write_dummy_pgm16("./dummy_file.pgm", width, height, NULL, 0);
    
    unsigned short *buffer16 = tj3LoadImage16(handle, "./dummy_file.pgm", &widthOut, align, &heightOut, &pixelFormat);
    if (buffer16) {
        // Save the loaded buffer
        tj3SaveImage16(handle, "./dummy_file_out.pgm", buffer16, widthOut, 0, heightOut, pixelFormat);
        tj3Free(buffer16);
    }

    // Test tjLoadImage and tjSaveImage
    int align8 = 1;
    int pf8 = (Size > 3) ? (Data[3] % 6) : 0;
    int flags = (Size > 4) ? (Data[4] % 10) : 0;
    
    // Write dummy PPM file for 8-bit load test
    write_dummy_ppm("./dummy_file.ppm", width, height, pf8, Data, Size);
    
    unsigned char *buffer8 = tjLoadImage("./dummy_file.ppm", &widthOut, align8, &heightOut, &pf8, flags);
    if (buffer8) {
        // Save the loaded buffer
        tjSaveImage("./dummy_file_out.ppm", buffer8, widthOut, 0, heightOut, pf8, flags);
        tjFree(buffer8);
    }

    // Test tj3Alloc and tj3Free directly
    size_t allocSize = (Size > 5) ? (Data[5] * 100 + 100) : 100;
    if (allocSize > 1024 * 1024) allocSize = 1024 * 1024; // Cap allocation
    
    void *buf = tj3Alloc(allocSize);
    if (buf) {
        memset(buf, 0xAA, allocSize);
        tj3Free(buf);
    }
    
    // Test tjFree with NULL
    tjFree(NULL);
    tj3Free(NULL);

    // Cleanup
    tjDestroy(handle);
    
    return 0;
}