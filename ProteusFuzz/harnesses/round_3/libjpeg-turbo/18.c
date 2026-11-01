// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void write_dummy_ppm_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    // Write a minimal PPM header (P6 for RGB, 1x1 pixel)
    fprintf(f, "P6\n1 1\n255\n");
    
    // Write up to 3 bytes of pixel data (RGB) from the fuzzed input
    size_t bytes_to_write = (size < 3) ? size : 3;
    fwrite(data, 1, bytes_to_write, f);
    
    fclose(f);
}

static void write_dummy_pgm_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    // Write a minimal PGM header (P5 for Grayscale, 1x1 pixel)
    fprintf(f, "P5\n1 1\n255\n");
    
    size_t bytes_to_write = (size < 1) ? size : 1;
    fwrite(data, 1, bytes_to_write, f);
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tjhandle handle = tjInitCompress();
    if (!handle) return 0;

    const char *filename = "./dummy_file";
    
    // Write a dummy file based on input
    if (Size > 0 && Data[0] % 2 == 0) {
        write_dummy_ppm_file(filename, Data, Size);
    } else {
        write_dummy_pgm_file(filename, Data, Size);
    }

    // Test tj3LoadImage16
    int width = 0, height = 0, pixelFormat = TJPF_UNKNOWN;
    int align = 1; // Power of 2
    
    // Try different pixel formats for loading (removed TJPF_YUY2 as it is not valid for this context in older headers or specific builds)
    int formats[] = {TJPF_UNKNOWN, TJPF_GRAY, TJPF_RGB, TJPF_CMYK, -1};
    int num_formats = sizeof(formats) / sizeof(formats[0]);
    
    for (int i = 0; i < num_formats; i++) {
        pixelFormat = formats[i];
        unsigned short *buffer = tj3LoadImage16(handle, filename, &width, align, &height, &pixelFormat);
        
        if (buffer) {
            // If successful, try to save it back using tj3SaveImage16
            int pitch = 0;
            if (width > 0 && pixelFormat == TJPF_GRAY) pitch = width;
            else if (width > 0 && (pixelFormat == TJPF_RGB || pixelFormat == TJPF_CMYK)) pitch = width * 3;
            else pitch = 0;

            const char *save_filename = "./dummy_save_file";
            int result = tj3SaveImage16(handle, save_filename, buffer, width, pitch, height, pixelFormat);
            
            if (result != 0) {
                char *err_str = tj3GetErrorStr(handle);
                (void)err_str; 
                int err_code = tjGetErrorCode(handle);
                (void)err_code;
                char *err_str2 = tjGetErrorStr2(handle);
                (void)err_str2;
            }
            
            tj3Free(buffer);
        } else {
            char *err_str = tj3GetErrorStr(handle);
            (void)err_str;
            int err_code = tjGetErrorCode(handle);
            (void)err_code;
            char *err_str2 = tjGetErrorStr2(handle);
            (void)err_str2;
        }
    }

    // Test with NULL handle for global error functions
    char *global_err = tj3GetErrorStr(NULL);
    (void)global_err;

    tjDestroy(handle);
    
    remove(filename);
    remove("./dummy_save_file");

    return 0;
}