// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void write_dummy_ppm_file(const char *filename, unsigned char *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    fprintf(f, "P6\n");
    fprintf(f, "1 1\n");
    fprintf(f, "255\n");
    if (size > 0) {
        fwrite(data, 1, size, f);
    }
    fclose(f);
}

static void write_dummy_pgm_file(const char *filename, unsigned short *data, size_t count) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    fprintf(f, "P5\n");
    fprintf(f, "1 1\n");
    fprintf(f, "65535\n");
    if (count > 0) {
        fwrite(data, sizeof(unsigned short), count, f);
    }
    fclose(f);
}

static int get_pixel_size(int pixelFormat) {
    switch (pixelFormat) {
        case TJPF_GRAY: return 1;
        case TJPF_RGB: return 3;
        case TJPF_CMYK: return 4;
        default: return 3; // Default to RGB size for safety
    }
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    int width, height, pixelFormat;
    unsigned char *buf8 = NULL;
    unsigned short *buf16 = NULL;
    tjhandle handle = NULL;
    int align = 1;
    int flags = 0;
    int pitch = 0;

    const char *dummy_ppm = "./dummy_file_ppm";
    const char *dummy_pgm = "./dummy_file_pgm";

    // Write dummy PPM file if we have enough data
    if (Size >= 3) {
        write_dummy_ppm_file(dummy_ppm, (unsigned char *)Data, Size);
    }

    // Write dummy PGM file if we have enough data
    if (Size >= 2) {
        unsigned short *temp16 = (unsigned short *)malloc(Size * sizeof(unsigned short));
        if (temp16) {
            for (size_t i = 0; i < Size; i++) {
                temp16[i] = (unsigned short)Data[i] * 257;
            }
            write_dummy_pgm_file(dummy_pgm, temp16, Size);
            free(temp16);
        }
    }

    // Test tjAlloc
    if (Size > 0) {
        int alloc_size = Data[0] * 100;
        if (alloc_size > 0 && alloc_size < 1024 * 1024) {
            unsigned char *alloc_buf = tjAlloc(alloc_size);
            if (alloc_buf) {
                memset(alloc_buf, 0x42, alloc_size);
                tjFree(alloc_buf);
            }
        }
    }

    // Test tjLoadImage
    width = 0; height = 0; pixelFormat = TJPF_UNKNOWN;
    buf8 = tjLoadImage(dummy_ppm, &width, align, &height, &pixelFormat, flags);
    if (buf8) {
        tjFree(buf8);
    }

    // Test tjSaveImage with dummy data
    if (Size >= 3) {
        int w = 1, h = 1;
        int pf = (Data[0] % 3); 
        if (pf == 0) pf = TJPF_GRAY;
        else if (pf == 1) pf = TJPF_RGB;
        else pf = TJPF_CMYK;
        
        int ps = get_pixel_size(pf);
        unsigned char *save_buf = tjAlloc(w * h * ps);
        if (save_buf) {
            memset(save_buf, 0xFF, w * h * ps);
            tjSaveImage(dummy_ppm, save_buf, w, 0, h, pf, flags);
            tjFree(save_buf);
        }
    }

    // Initialize TurboJPEG handle for 16-bit functions
    handle = tjInitCompress();
    if (handle) {
        // Test tj3LoadImage16
        width = 0; height = 0; pixelFormat = TJPF_UNKNOWN;
        buf16 = tj3LoadImage16(handle, dummy_pgm, &width, align, &height, &pixelFormat);
        if (buf16) {
            tj3Free(buf16);
        }

        // Test tj3SaveImage16 with dummy data
        if (Size >= 2) {
            int w = 1, h = 1;
            int pf = (Data[0] % 3);
            if (pf == 0) pf = TJPF_GRAY;
            else if (pf == 1) pf = TJPF_RGB;
            else pf = TJPF_CMYK;
            
            int ps = get_pixel_size(pf);
            size_t total_samples = (size_t)w * h * ps;
            unsigned short *save_buf16 = (unsigned short *)tj3Alloc(total_samples * sizeof(unsigned short));
            
            if (save_buf16) {
                memset(save_buf16, 0xFF, total_samples * sizeof(unsigned short));
                tj3SaveImage16(handle, dummy_pgm, save_buf16, w, 0, h, pf);
                tj3Free(save_buf16);
            }
        }

        tjDestroy(handle);
    }

    return 0;
}