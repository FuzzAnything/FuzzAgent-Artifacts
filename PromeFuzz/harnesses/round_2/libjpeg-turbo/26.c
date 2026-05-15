// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    // Create dummy file for potential use
    write_dummy_file(Data, Size);

    // Initialize decompressor handle
    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return 0;
    }

    // Test tjGetErrorStr (global error string)
    char *error_str = tjGetErrorStr();
    if (error_str) {
        // Access the string to potentially trigger issues
        volatile char c = error_str[0];
        (void)c;
    }

    // Test tjGetErrorStr2 with valid handle
    char *error_str2 = tjGetErrorStr2(handle);
    if (error_str2) {
        volatile char c = error_str2[0];
        (void)c;
    }

    // Test tjGetErrorCode
    int error_code = tjGetErrorCode(handle);
    (void)error_code;

    // Test tj3GetErrorCode (for newer API)
    int error_code3 = tj3GetErrorCode(handle);
    (void)error_code3;

    // Test tj3GetErrorStr with both NULL and valid handle
    char *error_str3_null = tj3GetErrorStr(NULL);
    if (error_str3_null) {
        volatile char c = error_str3_null[0];
        (void)c;
    }

    char *error_str3_handle = tj3GetErrorStr(handle);
    if (error_str3_handle) {
        volatile char c = error_str3_handle[0];
        (void)c;
    }

    // Test tjDecompress2 with various parameters
    // Use first few bytes to parameterize the call
    int width = (Data[0] << 8) | Data[1];
    int height = (Data[2] << 8) | Data[3];
    int pixel_format = Data[4] % 12; // There are 12 pixel formats in turbojpeg.h
    int flags = Data[5] & 0xFF;
    
    // Ensure reasonable dimensions to avoid excessive memory allocation
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    if (width < 1) width = 1;
    if (height < 1) height = 1;
    
    // Calculate pitch (bytes per row)
    int bytes_per_pixel = 3; // Default for RGB
    switch (pixel_format) {
        case TJPF_GRAY:
            bytes_per_pixel = 1;
            break;
        case TJPF_RGB:
        case TJPF_BGR:
            bytes_per_pixel = 3;
            break;
        case TJPF_RGBX:
        case TJPF_BGRX:
        case TJPF_XRGB:
        case TJPF_XBGR:
        case TJPF_RGBA:
        case TJPF_BGRA:
        case TJPF_ABGR:
        case TJPF_ARGB:
            bytes_per_pixel = 4;
            break;
        default:
            bytes_per_pixel = 3;
            pixel_format = TJPF_RGB;
    }
    
    int pitch = width * bytes_per_pixel;
    
    // Allocate destination buffer
    size_t dst_size = (size_t)pitch * (size_t)height;
    if (dst_size > 100 * 1024 * 1024) { // Limit to 100MB
        tjDestroy(handle);
        return 0;
    }
    
    unsigned char *dst_buf = (unsigned char *)malloc(dst_size);
    if (!dst_buf) {
        tjDestroy(handle);
        return 0;
    }
    
    // Skip the first 6 bytes we used for parameters
    const unsigned char *jpeg_buf = Data + 6;
    unsigned long jpeg_size = (unsigned long)(Size - 6);
    if (jpeg_size < 2) {
        jpeg_buf = Data;
        jpeg_size = (unsigned long)Size;
    }
    
    // Call tjDecompress2
    int result = tjDecompress2(handle, jpeg_buf, jpeg_size, dst_buf, 
                               width, pitch, height, pixel_format, flags);
    (void)result;
    
    // Test error functions again after decompression attempt
    error_str = tjGetErrorStr();
    if (error_str) {
        volatile char c = error_str[0];
        (void)c;
    }
    
    error_str2 = tjGetErrorStr2(handle);
    if (error_str2) {
        volatile char c = error_str2[0];
        (void)c;
    }
    
    error_code = tjGetErrorCode(handle);
    (void)error_code;
    
    error_code3 = tj3GetErrorCode(handle);
    (void)error_code3;
    
    error_str3_handle = tj3GetErrorStr(handle);
    if (error_str3_handle) {
        volatile char c = error_str3_handle[0];
        (void)c;
    }
    
    // Clean up
    free(dst_buf);
    tjDestroy(handle);
    
    return 0;
}