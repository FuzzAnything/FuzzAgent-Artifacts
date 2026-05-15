// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_error_handling(tjhandle handle) {
    // Call various error functions to exercise them
    const char* err_str = NULL;
    int err_code = 0;
    
    // Old API error functions
    err_str = tjGetErrorStr();
    err_code = tjGetErrorCode(handle);
    err_str = tjGetErrorStr2(handle);
    
    // New API error functions
    err_code = tj3GetErrorCode(handle);
    err_str = tj3GetErrorStr(handle);
    err_str = tj3GetErrorStr(NULL);  // Test with NULL handle
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    // Create a dummy JPEG buffer from fuzzer input
    unsigned char* jpegBuf = NULL;
    unsigned long jpegSize = 0;
    
    if (Size > 0) {
        jpegBuf = (unsigned char*)malloc(Size);
        if (!jpegBuf) return 0;
        memcpy(jpegBuf, Data, Size);
        jpegSize = (unsigned long)Size;
    } else {
        // If no data, use minimal dummy data
        static const unsigned char dummy_jpeg[] = {0xFF, 0xD8, 0xFF, 0xD9};
        jpegBuf = (unsigned char*)malloc(sizeof(dummy_jpeg));
        if (!jpegBuf) return 0;
        memcpy(jpegBuf, dummy_jpeg, sizeof(dummy_jpeg));
        jpegSize = sizeof(dummy_jpeg);
    }
    
    // Initialize decompressor handles
    tjhandle handle_old = tjInitDecompress();
    tjhandle handle_new = tj3Init(TJINIT_DECOMPRESS);
    
    if (handle_old || handle_new) {
        // Try to get JPEG dimensions first
        int width = 0, height = 0;
        int subsamp = 0;
        
        if (handle_old) {
            if (tjDecompressHeader2(handle_old, jpegBuf, jpegSize, &width, &height, &subsamp) == 0) {
                // Valid dimensions obtained, allocate destination buffer
                int pitch = width * 3;  // RGB format
                unsigned char* dstBuf = (unsigned char*)malloc(height * pitch);
                
                if (dstBuf) {
                    // Try decompression with various parameters
                    for (int pixelFormat = TJPF_RGB; pixelFormat <= TJPF_CMYK; pixelFormat++) {
                        for (int flags = 0; flags <= TJFLAG_BOTTOMUP; flags++) {
                            // Call target function
                            int result = tjDecompress2(handle_old, jpegBuf, jpegSize,
                                                      dstBuf, width, pitch, height,
                                                      pixelFormat, flags);
                            
                            // Check error state regardless of result
                            dummy_error_handling(handle_old);
                            
                            // Also test with new API handle if available
                            if (handle_new) {
                                dummy_error_handling(handle_new);
                            }
                        }
                    }
                    free(dstBuf);
                }
            } else {
                // Header decompression failed, still try decompression with arbitrary dimensions
                width = 64;
                height = 64;
                int pitch = width * 3;
                unsigned char* dstBuf = (unsigned char*)malloc(height * pitch);
                
                if (dstBuf) {
                    tjDecompress2(handle_old, jpegBuf, jpegSize,
                                 dstBuf, width, pitch, height,
                                 TJPF_RGB, 0);
                    dummy_error_handling(handle_old);
                    free(dstBuf);
                }
            }
        }
        
        // Also test with new API handle using tj3Decompress8 (similar to tjDecompress2)
        if (handle_new) {
            // Try to get header with new API
            if (tj3DecompressHeader(handle_new, jpegBuf, jpegSize) == 0) {
                width = tj3Get(handle_new, TJPARAM_JPEGWIDTH);
                height = tj3Get(handle_new, TJPARAM_JPEGHEIGHT);
                
                int pitch = width * 3;
                unsigned char* dstBuf = (unsigned char*)malloc(height * pitch);
                
                if (dstBuf) {
                    tj3Decompress8(handle_new, jpegBuf, jpegSize,
                                  dstBuf, pitch, TJPF_RGB);
                    dummy_error_handling(handle_new);
                    free(dstBuf);
                }
            }
        }
    }
    
    // Cleanup
    if (handle_old) tjDestroy(handle_old);
    if (handle_new) tj3Destroy(handle_new);
    free(jpegBuf);
    
    return 0;
}