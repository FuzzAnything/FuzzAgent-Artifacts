// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void write_dummy_ppm(const char *filename, int width, int height, int pixelFormat) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    int channels = 1;
    if (pixelFormat == TJPF_RGB || pixelFormat == TJPF_CMYK) channels = 3;
    else if (pixelFormat == TJPF_BGR || pixelFormat == TJPF_XBGR || pixelFormat == TJPF_RGBX || pixelFormat == TJPF_XRGB) channels = 4;
    
    // Write a minimal PPM/PGM header
    if (channels == 1) {
        fprintf(f, "P5\n%d %d\n255\n", width, height);
    } else {
        fprintf(f, "P6\n%d %d\n255\n", width, height);
    }
    
    // Write dummy pixel data
    size_t pixel_size = width * height * channels;
    uint8_t *buf = malloc(pixel_size);
    if (buf) {
        memset(buf, 0, pixel_size);
        fwrite(buf, 1, pixel_size, f);
        free(buf);
    }
    
    fclose(f);
}

static void write_dummy_ppm16(const char *filename, int width, int height, int pixelFormat) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    int channels = 1;
    if (pixelFormat == TJPF_RGB || pixelFormat == TJPF_CMYK) channels = 3;
    else if (pixelFormat == TJPF_BGR || pixelFormat == TJPF_XBGR || pixelFormat == TJPF_RGBX || pixelFormat == TJPF_XRGB) channels = 4;
    
    // Write a minimal PPM/PGM header for 16-bit
    if (channels == 1) {
        fprintf(f, "P5\n%d %d\n65535\n", width, height);
    } else {
        fprintf(f, "P6\n%d %d\n65535\n", width, height);
    }
    
    // Write dummy pixel data (16-bit)
    size_t pixel_size = width * height * channels * 2;
    unsigned short *buf = malloc(pixel_size);
    if (buf) {
        memset(buf, 0, pixel_size);
        fwrite(buf, 1, pixel_size, f);
        free(buf);
    }
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 7) return 0;
    
    // Extract parameters from input data
    int width = Data[0] | (Data[1] << 8);
    int height = Data[2] | (Data[3] << 8);
    
    // Sanity checks to prevent huge allocations or invalid dimensions
    if (width <= 0 || height <= 0 || width > 1000 || height > 1000) {
        width = 100;
        height = 100;
    }
    
    // Initialize TurboJPEG instance
    tjhandle handle = tjInitCompress();
    if (!handle) {
        tjInitDecompress(); // Try to initialize decompress context
        return 0;
    }
    
    // Test tjSaveImage
    {
        int pixelFormat = (Data[4] % 6); // 0-5: TJPF_GRAY, TJPF_RGB, TJPF_BGR, TJPF_CMYK, TJPF_XBGR, TJPF_RGBX
        int flags = (Data[5] % 2) ? TJFLAG_FASTDCT : 0;
        
        // Calculate pitch carefully
        int bytes_per_pixel = 1;
        if (pixelFormat == TJPF_RGB || pixelFormat == TJPF_BGR || pixelFormat == TJPF_CMYK) bytes_per_pixel = 3;
        else if (pixelFormat == TJPF_XBGR || pixelFormat == TJPF_RGBX || pixelFormat == TJPF_XRGB) bytes_per_pixel = 4;
        
        int pitch = (Data[6] % 10) ? width * bytes_per_pixel : 0;
        
        // Create dummy buffer
        size_t buf_size = (size_t)width * height * bytes_per_pixel;
        unsigned char *buffer = malloc(buf_size);
        if (buffer) {
            memset(buffer, 0, buf_size);
            // Copy fuzz data safely
            size_t copy_len = (Size - 7 < buf_size) ? (Size - 7) : buf_size;
            if (Size > 7 && copy_len > 0) {
                memcpy(buffer, Data + 7, copy_len);
            }
            
            tjSaveImage("./dummy_file", buffer, width, pitch, height, pixelFormat, flags);
            free(buffer);
        }
    }
    
    // Test tj3SaveImage16
    {
        int pixelFormat = (Data[4] % 6);
        
        int bytes_per_pixel = 1;
        if (pixelFormat == TJPF_RGB || pixelFormat == TJPF_BGR || pixelFormat == TJPF_CMYK) bytes_per_pixel = 3;
        else if (pixelFormat == TJPF_XBGR || pixelFormat == TJPF_RGBX || pixelFormat == TJPF_XRGB) bytes_per_pixel = 4;
        
        int pitch = (Data[6] % 10) ? width * bytes_per_pixel : 0;
        
        // Create dummy 16-bit buffer
        size_t buf_size = (size_t)width * height * bytes_per_pixel;
        unsigned short *buffer = malloc(buf_size * sizeof(unsigned short));
        if (buffer) {
            memset(buffer, 0, buf_size * sizeof(unsigned short));
            // Copy fuzz data safely into 16-bit buffer
            size_t copy_len = (Size - 7 < buf_size) ? (Size - 7) : buf_size;
            if (Size > 7 && copy_len > 0) {
                for (size_t i = 0; i < copy_len; i++) {
                    buffer[i] = (unsigned short)(Data[7 + i]);
                }
            }
            
            tj3SaveImage16(handle, "./dummy_file16", buffer, width, pitch, height, pixelFormat);
            free(buffer);
        }
    }
    
    // Test tj3LoadImage16
    {
        int align = 1 << (Data[4] % 5); // 1, 2, 4, 8, 16
        int pixelFormat = (Data[5] % 6);
        int w, h, pf;
        w = h = pf = 0;
        
        // Create a valid 16-bit PPM/PGM file
        write_dummy_ppm16("./dummy_load16", width, height, pixelFormat);
        
        unsigned short *loaded = tj3LoadImage16(handle, "./dummy_load16", &w, align, &h, &pf);
        if (loaded) {
            tj3Free(loaded);
        }
    }
    
    // Test error handling functions
    {
        // Force an error by using invalid parameters
        tjSaveImage("./dummy_file", NULL, -1, 0, 0, 0, 0);
        const char *err1 = tjGetErrorStr();
        (void)err1; // Suppress unused warning
        
        tj3SaveImage16(handle, "./dummy_file", NULL, -1, 0, 0, 0);
        const char *err2 = tj3GetErrorStr(handle);
        (void)err2;
        
        const char *err3 = tjGetErrorStr2(handle);
        (void)err3;
    }
    
    // Cleanup
    if (handle) {
        tjDestroy(handle);
    }
    
    return 0;
}