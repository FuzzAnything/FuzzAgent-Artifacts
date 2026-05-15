// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DUMMY_FILE "./dummy_file"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen(DUMMY_FILE, "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove(DUMMY_FILE);
}

int LLVMFuzzerTestOneInput_33(const uint8_t *data, size_t size) {
    // Write input data to dummy file for potential file-based operations
    write_dummy_file(data, size);
    
    // Initialize handles
    tjhandle handle1 = tjInitDecompress();
    tjhandle handle2 = tjInitDecompress();
    
    // Test tjGetErrorStr (global error string)
    char *error_str1 = tjGetErrorStr();
    (void)error_str1; // Use variable to avoid unused warning
    
    // Test tjDecompress2 with various parameters
    if (handle1 && size > 0) {
        // Try different pixel formats
        int pixel_formats[] = {TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, TJPF_GRAY};
        int flags[] = {0, TJFLAG_FASTDCT, TJFLAG_ACCURATEDCT};
        
        for (int pf_idx = 0; pf_idx < 5; pf_idx++) {
            for (int flag_idx = 0; flag_idx < 3; flag_idx++) {
                // Allocate destination buffer (small fixed size for safety)
                int width = 64;
                int height = 64;
                int pitch = width * tjPixelSize[pixel_formats[pf_idx]];
                unsigned char *dst_buf = (unsigned char *)malloc(pitch * height);
                
                if (dst_buf) {
                    // Try decompression
                    int result = tjDecompress2(handle1, data, 
                                              size > 0xFFFFFFFF ? 0xFFFFFFFF : (unsigned long)size,
                                              dst_buf, width, pitch, height, 
                                              pixel_formats[pf_idx], flags[flag_idx]);
                    
                    // Test error functions after decompression
                    if (result == -1) {
                        // Test old API error functions
                        char *error_str2 = tjGetErrorStr2(handle1);
                        int error_code = tjGetErrorCode(handle1);
                        (void)error_str2;
                        (void)error_code;
                        
                        // Test new API error functions
                        int error_code3 = tj3GetErrorCode(handle1);
                        char *error_str3 = tj3GetErrorStr(handle1);
                        (void)error_code3;
                        (void)error_str3;
                    }
                    
                    free(dst_buf);
                }
            }
        }
    }
    
    // Test error functions with NULL handle
    if (handle2) {
        // Force an error by passing invalid parameters
        unsigned char dummy_buf[10];
        int result = tjDecompress2(handle2, NULL, 0, dummy_buf, 0, 0, 0, TJPF_RGB, 0);
        (void)result; // Result will be -1
        
        // Test error retrieval
        char *error_str4 = tjGetErrorStr2(handle2);
        int error_code2 = tjGetErrorCode(handle2);
        (void)error_str4;
        (void)error_code2;
        
        // Test new API with NULL handle
        char *error_str5 = tj3GetErrorStr(NULL);
        (void)error_str5;
    }
    
    // Test tj3GetErrorCode with various handles
    if (handle1) {
        int error_code4 = tj3GetErrorCode(handle1);
        (void)error_code4;
    }
    
    if (handle2) {
        int error_code5 = tj3GetErrorCode(handle2);
        (void)error_code5;
    }
    
    // Test tj3GetErrorStr with NULL handle (global error)
    char *error_str6 = tj3GetErrorStr(NULL);
    (void)error_str6;
    
    // Cleanup
    if (handle1) {
        tjDestroy(handle1);
    }
    if (handle2) {
        tjDestroy(handle2);
    }
    
    cleanup_dummy_file();
    
    return 0;
}