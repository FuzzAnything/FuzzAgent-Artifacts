// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void write_dummy_ppm(const char *filename, int width, int height, int bits_per_sample) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    int max_val = (bits_per_sample == 16) ? 65535 : 255;
    fprintf(f, "P6\n%d %d\n%d\n", width, height, max_val);
    
    size_t bytes_per_pixel = (bits_per_sample == 16) ? 2 : 1;
    size_t row_size = width * 3 * bytes_per_pixel;
    unsigned char *row = malloc(row_size);
    if (!row) { fclose(f); return; }
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width * 3; x++) {
            if (bits_per_sample == 16) {
                unsigned short val = (unsigned short)((x + y) % (max_val + 1));
                row[x * 2] = val & 0xFF;
                row[x * 2 + 1] = (val >> 8) & 0xFF;
            } else {
                row[x] = (unsigned char)((x + y) % (max_val + 1));
            }
        }
        fwrite(row, 1, row_size, f);
    }
    
    free(row);
    fclose(f);
}

static void write_dummy_pgm(const char *filename, int width, int height, int bits_per_sample) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    int max_val = (bits_per_sample == 16) ? 65535 : 255;
    fprintf(f, "P5\n%d %d\n%d\n", width, height, max_val);
    
    size_t bytes_per_pixel = (bits_per_sample == 16) ? 2 : 1;
    size_t row_size = width * bytes_per_pixel;
    unsigned char *row = malloc(row_size);
    if (!row) { fclose(f); return; }
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (bits_per_sample == 16) {
                unsigned short val = (unsigned short)((x + y) % (max_val + 1));
                row[x * 2] = val & 0xFF;
                row[x * 2 + 1] = (val >> 8) & 0xFF;
            } else {
                row[x] = (unsigned char)((x + y) % (max_val + 1));
            }
        }
        fwrite(row, 1, row_size, f);
    }
    
    free(row);
    fclose(f);
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    int width = (Data[0] % 100) + 1;
    int height = (Data[1] % 100) + 1;
    int pixelFormat = Data[2] % 6;
    int align = 1 << (Data[3] % 5);
    
    // Create dummy files for testing
    write_dummy_ppm("./dummy_file.ppm", width, height, 8);
    write_dummy_pgm("./dummy_file.pgm", width, height, 8);
    write_dummy_ppm("./dummy_file16.ppm", width, height, 16);
    write_dummy_pgm("./dummy_file16.pgm", width, height, 16);
    
    tjhandle handle = tjInitDecompress();
    
    // Test tjGetErrorStr (global error)
    char *err = tjGetErrorStr();
    (void)err;
    
    // Test tj3GetErrorStr with NULL
    err = tj3GetErrorStr(NULL);
    (void)err;
    
    // Test tjGetErrorStr2 with NULL
    err = tjGetErrorStr2(NULL);
    (void)err;
    
    // Test tjSaveImage
    unsigned char *buffer = malloc(width * height * 4);
    if (buffer) {
        memset(buffer, 0, width * height * 4);
        tjSaveImage("./dummy_out.ppm", buffer, width, width * 4, height, pixelFormat, 0);
        free(buffer);
    }
    
    // Test tj3LoadImage16
    int w, h, pf;
    w = h = pf = 0;
    unsigned short *buf16 = tj3LoadImage16(handle, "./dummy_file16.ppm", &w, align, &h, &pf);
    
    if (buf16) {
        // Test tj3SaveImage16 BEFORE freeing the buffer
        tj3SaveImage16(handle, "./dummy_out16.ppm", buf16, w, 0, h, pf);
        
        // Now free the buffer
        tj3Free(buf16);
        buf16 = NULL; // Ensure we don't use it again
    }
    
    // Test error retrieval after potential failures with valid handle
    if (handle) {
        err = tj3GetErrorStr(handle);
        (void)err;
        
        err = tjGetErrorStr2(handle);
        (void)err;
    }
    
    // Cleanup
    if (handle) {
        tjDestroy(handle);
    }
    
    return 0;
}