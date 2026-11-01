// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void write_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test tjGetErrorStr (global)
    char *err_str = tjGetErrorStr();
    if (err_str) {
        size_t len = strlen(err_str);
        if (len > 0) len = len; 
    }

    // Test tj3GetErrorStr with NULL (global)
    char *err_str3_null = tj3GetErrorStr(NULL);
    if (err_str3_null) {
        size_t len = strlen(err_str3_null);
        if (len > 0) len = len;
    }

    // Create a handle to test handle-specific functions
    tjhandle handle = tjInitCompress();
    
    if (handle) {
        // Test tjGetErrorStr2 with valid handle
        char *err_str2 = tjGetErrorStr2(handle);
        if (err_str2) {
            size_t len = strlen(err_str2);
            if (len > 0) len = len;
        }

        // Test tjGetErrorCode with valid handle
        int code = tjGetErrorCode(handle);
        if (code != 0) code = code;

        // Test tj3GetErrorStr with valid handle
        char *err_str3_handle = tj3GetErrorStr(handle);
        if (err_str3_handle) {
            size_t len = strlen(err_str3_handle);
            if (len > 0) len = len;
        }

        // Test tjSaveImage
        int width = (Size > 4) ? (Data[0] % 100) + 1 : 10;
        int height = (Size > 5) ? (Data[1] % 100) + 1 : 10;
        int pixelFormat = (Size > 6) ? (Data[2] % 5) : TJPF_RGB;
        int flags = (Size > 7) ? (Data[3] % 10) : 0;
        
        if (width > 1000) width = 1000;
        if (height > 1000) height = 1000;
        
        int pitch = width * tjPixelSize[pixelFormat];
        size_t buf_size = (size_t)height * pitch;
        
        unsigned char *img_buf = NULL;
        if (Size >= buf_size) {
            img_buf = (unsigned char *)Data;
        } else {
            img_buf = (unsigned char *)malloc(buf_size);
            if (img_buf) {
                memset(img_buf, 0, buf_size);
            }
        }

        if (img_buf) {
            write_dummy_file("./dummy_file.ppm", img_buf, buf_size);
            int ret = tjSaveImage("./dummy_file.ppm", img_buf, width, pitch, height, pixelFormat, flags);
            if (ret == -1) {
                char *err = tjGetErrorStr();
                if (err) { size_t l = strlen(err); if(l>0) l=l; }
            }
        }

        if (img_buf && Size < buf_size) {
            free(img_buf);
        }

        // Test tj3SaveImage16
        // Determine samples per pixel based on pixelFormat to calculate correct buffer size
        int samplesPerPixel = 1;
        if (pixelFormat == TJPF_CMYK) samplesPerPixel = 4;
        else if (pixelFormat == TJPF_RGB || pixelFormat == TJPF_XRGB || pixelFormat == TJPF_BGR || pixelFormat == TJPF_XBGR) samplesPerPixel = 3;
        else if (pixelFormat == TJPF_GRAY) samplesPerPixel = 1;
        else samplesPerPixel = 3; // Default fallback

        // Calculate required size for 16-bit buffer (2 bytes per sample)
        size_t buf_size16 = (size_t)height * width * samplesPerPixel * 2;
        
        // Cap buffer size to prevent allocation failures or huge allocations
        if (buf_size16 > 1024 * 1024 * 10) { // 10MB limit
            buf_size16 = 1024 * 1024 * 10;
        }

        unsigned short *img_buf16 = NULL;
        
        // Check if input data is large enough to cast directly
        if (Size >= buf_size16) {
            img_buf16 = (unsigned short *)Data;
        } else {
            img_buf16 = (unsigned short *)malloc(buf_size16);
            if (img_buf16) {
                memset(img_buf16, 0, buf_size16);
            }
        }

        if (img_buf16) {
            write_dummy_file("./dummy_file.pgm", (uint8_t*)img_buf16, buf_size16);
            int ret16 = tj3SaveImage16(handle, "./dummy_file.pgm", img_buf16, width, 0, height, pixelFormat);
            if (ret16 == -1) {
                char *err = tj3GetErrorStr(handle);
                if (err) { size_t l = strlen(err); if(l>0) l=l; }
            }
        }

        if (img_buf16 && Size < buf_size16) {
            free(img_buf16);
        }

        // Cleanup handle
        tjDestroy(handle);
    } else {
        // If handle creation failed, test error retrieval on NULL or invalid context
        char *err_fail = tjGetErrorStr();
        if (err_fail) { size_t l = strlen(err_fail); if(l>0) l=l; }
        
        char *err_fail2 = tjGetErrorStr2(NULL);
        if (err_fail2) { size_t l = strlen(err_fail2); if(l>0) l=l; }
        
        int code_fail = tjGetErrorCode(NULL);
        if (code_fail != 0) code_fail = code_fail;
    }

    return 0;
}