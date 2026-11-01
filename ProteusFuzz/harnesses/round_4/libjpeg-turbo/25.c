// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <turbojpeg.h>

static void write_dummy_ppm(const char *filename, int width, int height, int pixelFormat) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;

    int samplesPerPixel = 1;
    if (pixelFormat == TJPF_GRAY) {
        samplesPerPixel = 1;
    } else if (pixelFormat == TJPF_CMYK) {
        samplesPerPixel = 4;
    } else {
        samplesPerPixel = 3; // RGB
    }

    fprintf(f, "P6\n%d %d\n255\n", width, height);

    unsigned char *buf = (unsigned char *)malloc(width * height * samplesPerPixel);
    if (!buf) {
        fclose(f);
        return;
    }

    for (int i = 0; i < width * height * samplesPerPixel; i++) {
        buf[i] = (unsigned char)(i % 256);
    }

    fwrite(buf, 1, width * height * samplesPerPixel, f);
    free(buf);
    fclose(f);
}

static void write_dummy_pgm(const char *filename, int width, int height) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;

    fprintf(f, "P5\n%d %d\n255\n", width, height);

    unsigned char *buf = (unsigned char *)malloc(width * height);
    if (!buf) {
        fclose(f);
        return;
    }

    for (int i = 0; i < width * height; i++) {
        buf[i] = (unsigned char)(i % 256);
    }

    fwrite(buf, 1, width * height, f);
    free(buf);
    fclose(f);
}

static void write_dummy_16bit_ppm(const char *filename, int width, int height, int pixelFormat) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;

    int samplesPerPixel = 1;
    if (pixelFormat == TJPF_GRAY) {
        samplesPerPixel = 1;
    } else if (pixelFormat == TJPF_CMYK) {
        samplesPerPixel = 4;
    } else {
        samplesPerPixel = 3;
    }

    fprintf(f, "P6\n%d %d\n65535\n", width, height);

    unsigned short *buf = (unsigned short *)malloc(width * height * samplesPerPixel * sizeof(unsigned short));
    if (!buf) {
        fclose(f);
        return;
    }

    for (int i = 0; i < width * height * samplesPerPixel; i++) {
        buf[i] = (unsigned short)(i % 65536);
    }

    fwrite(buf, sizeof(unsigned short), width * height * samplesPerPixel, f);
    free(buf);
    fclose(f);
}

static void write_dummy_16bit_pgm(const char *filename, int width, int height) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;

    fprintf(f, "P5\n%d %d\n65535\n", width, height);

    unsigned short *buf = (unsigned short *)malloc(width * height * sizeof(unsigned short));
    if (!buf) {
        fclose(f);
        return;
    }

    for (int i = 0; i < width * height; i++) {
        buf[i] = (unsigned short)(i % 65536);
    }

    fwrite(buf, sizeof(unsigned short), width * height, f);
    free(buf);
    fclose(f);
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    int width, height, pixelFormat;
    int align = 1;
    int flags = 0;
    tjhandle handle = NULL;

    // Initialize handle
    handle = tjInitDecompress();
    if (!handle) return 0;

    // Extract parameters from input if possible
    int w = (Size > 0) ? Data[0] : 10;
    int h = (Size > 1) ? Data[1] : 10;
    int pf = (Size > 2) ? (Data[2] % 5) : 0; // 0 to 4
    int align_val = (Size > 3) ? (1 << (Data[3] % 4)) : 1; // Power of 2: 1, 2, 4, 8

    // Ensure minimum dimensions
    if (w < 1) w = 1;
    if (h < 1) h = 1;

    // Test tjLoadImage (8-bit)
    const char *filename8 = "./dummy_file8";
    write_dummy_ppm(filename8, w, h, pf);
    unsigned char *buf8 = tjLoadImage(filename8, &width, align_val, &height, &pixelFormat, flags);
    if (buf8) {
        // Do some work or just free
        tjFree(buf8);
    }

    // Test tjAlloc and tjSaveImage (8-bit)
    unsigned char *allocBuf = tjAlloc(w * h * 3);
    if (allocBuf) {
        for (int i = 0; i < w * h * 3; i++) {
            allocBuf[i] = (unsigned char)(i % 256);
        }
        tjSaveImage("./dummy_save8.ppm", allocBuf, w, 0, h, pf, flags);
        tjFree(allocBuf);
    }

    // Test tj3LoadImage16
    const char *filename16 = "./dummy_file16";
    write_dummy_16bit_ppm(filename16, w, h, pf);
    unsigned short *buf16 = tj3LoadImage16(handle, filename16, &width, align_val, &height, &pixelFormat);
    if (buf16) {
        // Do some work
        tj3Free(buf16);
    }

    // Test tj3SaveImage16
    unsigned short *saveBuf16 = (unsigned short *)malloc(w * h * 3 * sizeof(unsigned short));
    if (saveBuf16) {
        for (int i = 0; i < w * h * 3; i++) {
            saveBuf16[i] = (unsigned short)(i % 65536);
        }
        tj3SaveImage16(handle, "./dummy_save16.ppm", saveBuf16, w, 0, h, pf);
        free(saveBuf16);
    }

    // Cleanup
    tjDestroy(handle);

    // Remove dummy files
    remove(filename8);
    remove(filename16);
    remove("./dummy_save8.ppm");
    remove("./dummy_save16.ppm");

    return 0;
}