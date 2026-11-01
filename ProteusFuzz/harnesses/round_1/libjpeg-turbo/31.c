// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "turbojpeg.h"

static int create_dummy_ppm(const char *filename, int width, int height, int pixel_format) {
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;

    const char *header = (pixel_format == TJPF_GRAY) ? "P5" : "P6";
    int channels = (pixel_format == TJPF_GRAY) ? 1 : 3;
    
    // Write PPM/PGM header
    fprintf(f, "%s %d %d 65535\n", header, width, height);

    // Write dummy pixel data (16-bit)
    size_t row_size = width * channels;
    unsigned short *row = (unsigned short *)malloc(row_size * sizeof(unsigned short));
    if (!row) {
        fclose(f);
        return -1;
    }

    for (int y = 0; y < height; y++) {
        for (size_t x = 0; x < row_size; x++) {
            row[x] = (unsigned short)((y * width + x) % 65536);
        }
        fwrite(row, sizeof(unsigned short), row_size, f);
    }

    free(row);
    fclose(f);
    return 0;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Extract parameters from input data
    int width = Data[0] + 1;
    int height = Data[1] + 1;
    
    // Ensure dimensions are within reasonable bounds to avoid OOM
    if (width > 4096 || height > 4096) {
        width = 100;
        height = 100;
    }

    // Select pixel format based on data
    int pixel_format = (Size > 2) ? (Data[2] % 5) : TJPF_RGB;
    // Ensure valid pixel format
    if (pixel_format < 0 || pixel_format > 4) pixel_format = TJPF_RGB;

    // Initialize TurboJPEG instance
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS | TJINIT_COMPRESS);
    if (!handle) {
        tjGetErrorStr();
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Create a dummy PPM/PGM file
    if (create_dummy_ppm(filename, width, height, pixel_format) != 0) {
        tj3Free(handle);
        return 0;
    }

    // Fuzz tj3LoadImage16
    int load_width = 0, load_height = 0, load_format = TJPF_UNKNOWN;
    int align = (Size > 3) ? (Data[3] & 0x0F) : 1;
    if (align == 0) align = 1; // Ensure power of 2, minimum 1
    align = 1 << (align & 3); // Force to power of 2 (1, 2, 4, 8)

    unsigned short *buffer = tj3LoadImage16(handle, filename, &load_width, align, &load_height, &load_format);
    if (buffer) {
        // Fuzz tj3SaveImage16 with the loaded buffer
        int pitch = (Size > 4) ? (Data[4] % 100) : 0;
        int save_format = (Size > 5) ? (Data[5] % 5) : TJPF_RGB;
        if (save_format < 0 || save_format > 4) save_format = TJPF_RGB;

        int save_result = tj3SaveImage16(handle, filename, buffer, load_width, pitch, load_height, save_format);
        if (save_result != 0) {
            tj3GetErrorStr(handle);
            tjGetErrorStr2(handle);
        }

        tj3Free(buffer);
    } else {
        // Check error if load failed
        tj3GetErrorStr(handle);
        tjGetErrorStr2(handle);
        tjGetErrorStr();
    }

    // Cleanup
    tj3Free(handle);
    remove(filename);

    return 0;
}