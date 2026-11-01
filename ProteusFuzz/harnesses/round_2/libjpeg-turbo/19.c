// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static int fuzzer_run(const uint8_t *Data, size_t Size) {
    tjhandle handle = NULL;
    int width = 0, height = 0;
    int pixelFormat = TJPF_UNKNOWN;
    int align = 1;
    int pitch = 0;
    int flags = 0;
    int ret = 0;
    unsigned short *buf16 = NULL;
    unsigned char *buf8 = NULL;
    const char *dummy_file = "./dummy_file";

    // Initialize handle
    handle = tjInitCompress();
    if (!handle) {
        tjGetErrorStr();
        return 0;
    }

    // Parse input to determine behavior
    if (Size > 0) {
        flags = Data[0] & 0xFF;
        align = (Data[Size > 1 ? 1 : 0] % 4) + 1; // 1, 2, 4
        pixelFormat = (Size > 2 ? Data[2] : 0) % 5; // 0-4
        if (pixelFormat == 4) pixelFormat = TJPF_CMYK;
        else if (pixelFormat == 3) pixelFormat = TJPF_GRAY;
        else if (pixelFormat == 2) pixelFormat = TJPF_XBGR;
        else if (pixelFormat == 1) pixelFormat = TJPF_RGB;
        else pixelFormat = TJPF_BGR;
    }

    // Create dummy file with random data for loading tests
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        // Write a minimal PGM header and some data
        fprintf(f, "P6\n");
        if (Size > 0) {
            int w = (Data[0] % 100) + 1;
            int h = (Size > 1 ? (Data[1] % 100) + 1 : 1);
            fprintf(f, "%d %d\n255\n", w, h);
            // Write some pixel data
            size_t bytes = w * h * 3;
            if (bytes > Size) bytes = Size;
            fwrite(Data, 1, bytes, f);
            // Pad if needed
            for (size_t i = bytes; i < w * h * 3; i++) {
                fputc(0, f);
            }
        } else {
            fprintf(f, "1 1\n255\n");
            fputc(0, f); fputc(0, f); fputc(0, f);
        }
        fclose(f);
    }

    // Test tj3LoadImage16
    buf16 = tj3LoadImage16(handle, dummy_file, &width, align, &height, &pixelFormat);
    if (buf16) {
        // Test tj3SaveImage16
        ret = tj3SaveImage16(handle, dummy_file, buf16, width, pitch, height, pixelFormat);
        if (ret != 0) {
            tj3GetErrorStr(handle);
        }
        tj3Free(buf16);
        buf16 = NULL;
    } else {
        tj3GetErrorStr(handle);
    }

    // Test tjSaveImage with 8-bit buffer
    if (width > 0 && height > 0) {
        int pf8 = (pixelFormat == TJPF_CMYK) ? TJPF_RGB : pixelFormat;
        int size8 = width * height * tjPixelSize[pf8];
        buf8 = (unsigned char *)malloc(size8);
        if (buf8) {
            memset(buf8, 0x55, size8);
            // If we have data, copy some
            if (Size > 0) {
                size_t copy = (Size < size8) ? Size : size8;
                memcpy(buf8, Data, copy);
            }
            ret = tjSaveImage(dummy_file, buf8, width, 0, height, pf8, flags);
            if (ret != 0) {
                tjGetErrorStr();
            }
            free(buf8);
            buf8 = NULL;
        }
    }

    // Test error retrieval functions directly
    char *err1 = tjGetErrorStr();
    char *err2 = tjGetErrorStr2(handle);
    char *err3 = tj3GetErrorStr(handle);
    char *err4 = tj3GetErrorStr(NULL);

    // Cleanup
    if (handle) {
        tjDestroy(handle);
    }
    
    return 0;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    return fuzzer_run(Data, Size);
}