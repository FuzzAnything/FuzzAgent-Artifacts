// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
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

static void write_dummy_ppm(const char *filename, int width, int height, int bits_per_sample) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    // PGM format for grayscale, PPM for RGB
    // Using P6 (binary PPM) or P5 (binary PGM)
    if (bits_per_sample > 8) {
        // For 16-bit, we need to write a valid PGM/PPM header that libjpeg-turbo might accept
        // However, standard PPM/PGM max value is 65535 for 16-bit
        fprintf(f, "P5\n%d %d\n65535\n", width, height);
    } else {
        fprintf(f, "P5\n%d %d\n255\n", width, height);
    }
    
    // Write dummy data
    size_t bytes_per_pixel = (bits_per_sample > 8) ? 2 : 1;
    size_t total_bytes = width * height * bytes_per_pixel;
    unsigned char *dummy = (unsigned char *)malloc(total_bytes);
    if (dummy) {
        memset(dummy, 0, total_bytes);
        fwrite(dummy, 1, total_bytes, f);
        free(dummy);
    }
    fclose(f);
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Extract parameters from input
    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int pixelFormat = Data[2] % 5; // 0 to 4
    int flags = Data[3];
    
    // Limit dimensions to reasonable values to avoid allocation issues
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    if (width <= 0) width = 1;
    if (height <= 0) height = 1;

    // Create dummy file for file-based operations
    write_dummy_ppm("./dummy_file", width, height, 16);

    // Test tjSaveImage (8-bit)
    size_t pitch8 = width * 3; // Assume RGB for safety
    size_t bufferSize8 = pitch8 * height;
    unsigned char *buffer8 = (unsigned char *)malloc(bufferSize8);
    if (buffer8) {
        memset(buffer8, 0, bufferSize8);
        // Fill with some data based on input
        for (size_t i = 0; i < bufferSize8 && i < Size; i++) {
            buffer8[i] = Data[i % Size];
        }
        
        int ret = tjSaveImage("./dummy_file", buffer8, width, pitch8, height, pixelFormat, flags);
        if (ret != 0) {
            const char *err = tjGetErrorStr();
            (void)err; // Use error string to suppress unused warning
        }
        free(buffer8);
    }

    // Test tj3SaveImage16 (16-bit)
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (handle) {
        size_t pitch16 = width * 3; // Assume RGB
        size_t bufferSize16 = pitch16 * height * 2; // 16-bit = 2 bytes
        unsigned short *buffer16 = (unsigned short *)malloc(bufferSize16);
        if (buffer16) {
            memset(buffer16, 0, bufferSize16);
            for (size_t i = 0; i < bufferSize16 && i < Size * 2; i += 2) {
                if (i + 1 < Size * 2) {
                    buffer16[i/2] = (unsigned short)(Data[i % Size] | (Data[(i+1) % Size] << 8));
                } else {
                    buffer16[i/2] = (unsigned short)(Data[i % Size]);
                }
            }
            
            int ret = tj3SaveImage16(handle, "./dummy_file", buffer16, width, pitch16, height, pixelFormat);
            if (ret != 0) {
                const char *err = tj3GetErrorStr(handle);
                (void)err;
            }
            free(buffer16);
        }
        
        // Test tj3LoadImage16
        int w, h, pf;
        w = h = pf = 0;
        unsigned short *loaded = tj3LoadImage16(handle, "./dummy_file", &w, 1, &h, &pf);
        if (loaded) {
            tj3Free(loaded);
        } else {
            const char *err = tj3GetErrorStr(handle);
            (void)err;
        }
        
        tj3Destroy(handle);
    }

    // Test error retrieval functions
    const char *err1 = tjGetErrorStr();
    const char *err2 = tjGetErrorStr2(NULL);
    (void)err1;
    (void)err2;

    return 0;
}