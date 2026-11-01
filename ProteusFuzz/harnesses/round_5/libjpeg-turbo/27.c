// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void create_dummy_ppm(const char *filename, int width, int height) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;

    fprintf(f, "P6\n");
    fprintf(f, "%d %d\n", width, height);
    fprintf(f, "255\n");

    int total_pixels = width * height;
    unsigned char *buf = malloc(total_pixels * 3);
    if (!buf) {
        fclose(f);
        return;
    }

    for (int i = 0; i < total_pixels * 3; i++) {
        buf[i] = (unsigned char)(i % 256);
    }

    fwrite(buf, 1, total_pixels * 3, f);
    free(buf);
    fclose(f);
}

static void create_dummy_pgm(const char *filename, int width, int height) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;

    fprintf(f, "P5\n");
    fprintf(f, "%d %d\n", width, height);
    fprintf(f, "65535\n");

    int total_pixels = width * height;
    unsigned short *buf = malloc(total_pixels * sizeof(unsigned short));
    if (!buf) {
        fclose(f);
        return;
    }

    for (int i = 0; i < total_pixels; i++) {
        buf[i] = (unsigned short)(i % 65536);
    }

    fwrite(buf, sizeof(unsigned short), total_pixels, f);
    free(buf);
    fclose(f);
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    int width = Data[0] % 256 + 1;
    int height = Data[1] % 256 + 1;
    int pixelFormatIdx = (Data[0] + Data[1]) % 6; 
    int align = 1 << ((Data[0] % 4) + 1); 
    int flags = Data[0] % 4;

    char *errStr1 = tjGetErrorStr();
    (void)errStr1;

    tjhandle handle = tjInitCompress();
    if (!handle) {
        char *err = tj3GetErrorStr(NULL);
        (void)err;
        return 0;
    }

    char *errStr2 = tj3GetErrorStr(handle);
    (void)errStr2;

    char *errStr3 = tjGetErrorStr2(handle);
    (void)errStr3;

    const char *ppmFile = "./dummy_file.ppm";
    unsigned char *rgbBuffer = NULL;
    
    int safePixelFormat = 0;
    int actualBytesPerPixel = 0;
    
    switch (pixelFormatIdx) {
        case 0: safePixelFormat = TJPF_GRAY; actualBytesPerPixel = 1; break;
        case 1: safePixelFormat = TJPF_RGB; actualBytesPerPixel = 3; break;
        case 2: safePixelFormat = TJPF_CMYK; actualBytesPerPixel = 4; break;
        // YUV formats (YU12, UYVY, YV12) are not supported by tjSaveImage in the same way 
        // or might require specific handling not suitable for this simple fuzz driver 
        // given the error context. We fallback to RGB/GRAY/CMYK for tjSaveImage.
        // However, to test diverse states, we can try to pass them and let the API fail gracefully.
        // But since the enums are missing in the error log, we must use valid ones.
        // Let's map remaining cases to valid formats to ensure compilation.
        case 3: safePixelFormat = TJPF_RGB; actualBytesPerPixel = 3; break;
        case 4: safePixelFormat = TJPF_GRAY; actualBytesPerPixel = 1; break;
        case 5: safePixelFormat = TJPF_CMYK; actualBytesPerPixel = 4; break;
        default: safePixelFormat = TJPF_RGB; actualBytesPerPixel = 3; break;
    }

    int pitch = 0;
    size_t bufferSize = 0;
    
    if (safePixelFormat == TJPF_GRAY) {
        bufferSize = (size_t)width * height * 1;
    } else if (safePixelFormat == TJPF_RGB || safePixelFormat == TJPF_CMYK) {
        bufferSize = (size_t)width * height * actualBytesPerPixel;
    } else {
        bufferSize = (size_t)width * height * 3;
    }

    if (width > 10000 || height > 10000 || bufferSize == 0) {
        tjDestroy(handle);
        return 0;
    }

    rgbBuffer = malloc(bufferSize);
    if (!rgbBuffer) {
        tjDestroy(handle);
        return 0;
    }

    memset(rgbBuffer, 0, bufferSize);

    int ret = tjSaveImage(ppmFile, rgbBuffer, width, pitch, height, safePixelFormat, flags);
    if (ret != 0) {
        char *err = tjGetErrorStr();
        (void)err;
    }
    free(rgbBuffer);

    const char *pgmFile = "./dummy_file.pgm";
    int w = width, h = height;
    int pf = safePixelFormat;

    create_dummy_pgm(pgmFile, width, height);

    unsigned short *buf16 = tj3LoadImage16(handle, pgmFile, &w, align, &h, &pf);
    if (buf16) {
        int ret16 = tj3SaveImage16(handle, "./dummy_file_out.ppm", buf16, w, 0, h, pf);
        if (ret16 != 0) {
            char *err = tj3GetErrorStr(handle);
            (void)err;
        }
        tj3Free(buf16);
    } else {
        char *err = tj3GetErrorStr(handle);
        (void)err;
    }

    char *errStr4 = tjGetErrorStr2(NULL);
    (void)errStr4;

    tjDestroy(handle);

    return 0;
}