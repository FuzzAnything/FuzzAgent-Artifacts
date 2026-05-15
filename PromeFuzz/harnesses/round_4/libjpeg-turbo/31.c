// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "src/turbojpeg.h"

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Initialize decompressor handle
    tjhandle handle = tjInitDecompress();
    if (!handle) {
        return 0;
    }
    
    int width = 0, height = 0;
    unsigned long jpegSize = Size;
    
    // Test tjDecompressHeader
    int header_result = tjDecompressHeader(handle, (unsigned char *)Data, jpegSize, &width, &height);
    
    // Test error handling functions regardless of header result
    int error_code = tjGetErrorCode(handle);
    (void)error_code; // Use result to avoid unused variable warning
    
    char *error_str = tjGetErrorStr();
    (void)error_str;
    
    char *error_str2 = tjGetErrorStr2(handle);
    (void)error_str2;
    
    char *error_str3 = tj3GetErrorStr(handle);
    (void)error_str3;
    
    // If header decompression succeeded and we have valid dimensions, try full decompression
    if (header_result == 0 && width > 0 && height > 0 && width <= 10000 && height <= 10000) {
        // Calculate buffer size for RGB pixels
        unsigned long dstBufSize = width * height * 3; // 3 bytes per pixel for RGB
        unsigned char *dstBuf = NULL;
        
        if (dstBufSize > 0 && dstBufSize < 100 * 1024 * 1024) { // Limit to 100MB
            dstBuf = (unsigned char *)malloc(dstBufSize);
            if (dstBuf) {
                // Test tjDecompress2 with various pixel formats
                int pixel_formats[] = {TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, TJPF_XRGB, TJPF_XBGR};
                int num_formats = sizeof(pixel_formats) / sizeof(pixel_formats[0]);
                
                for (int i = 0; i < num_formats; i++) {
                    int pitch = width * tjPixelSize[pixel_formats[i]];
                    int decompress_result = tjDecompress2(handle, Data, jpegSize, dstBuf, 
                                                          width, pitch, height, pixel_formats[i], 0);
                    (void)decompress_result;
                    
                    // Check error functions again
                    error_code = tjGetErrorCode(handle);
                    (void)error_code;
                    
                    error_str = tjGetErrorStr();
                    (void)error_str;
                    
                    error_str2 = tjGetErrorStr2(handle);
                    (void)error_str2;
                    
                    error_str3 = tj3GetErrorStr(handle);
                    (void)error_str3;
                }
                
                free(dstBuf);
            }
        }
    }
    
    // Test with NULL handle for some error functions
    error_str3 = tj3GetErrorStr(NULL);
    (void)error_str3;
    
    // Clean up
    tjDestroy(handle);
    
    return 0;
}