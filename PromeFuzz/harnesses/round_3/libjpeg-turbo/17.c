// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3GetErrorCode at turbojpeg.c:643:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    tjhandle handle = NULL;
    
    // Test 1: Initialize decompressor and test error functions without failure
    handle = tjInitDecompress();
    if (handle) {
        // Call error functions on valid handle without prior failure
        int err_code = tjGetErrorCode(handle);
        const char *err_str = tjGetErrorStr();
        const char *err_str2 = tjGetErrorStr2(handle);
        const char *err_str3 = tj3GetErrorStr(handle);
        int err_code3 = tj3GetErrorCode(handle);
        
        // Use the results to avoid optimization
        (void)err_code;
        (void)err_str;
        (void)err_str2;
        (void)err_str3;
        (void)err_code3;
        
        // Test 2: Attempt decompression with fuzzer data
        unsigned char *jpegBuf = (unsigned char *)Data;
        unsigned long jpegSize = Size;
        int width = 0, height = 0;
        int subsamp = 0, colorspace = 0;
        
        // Try to get header info first
        if (tjDecompressHeader3(handle, jpegBuf, jpegSize, &width, &height, &subsamp, &colorspace) == 0) {
            // If header is valid, attempt decompression
            if (width > 0 && height > 0) {
                int pitch = width * 3; // RGB
                unsigned char *dstBuf = (unsigned char *)malloc(pitch * height);
                if (dstBuf) {
                    int pixelFormat = TJPF_RGB;
                    int flags = TJFLAG_FASTDCT;
                    
                    int result = tjDecompress2(handle, jpegBuf, jpegSize, dstBuf, width, pitch, height, pixelFormat, flags);
                    
                    // Test error functions after decompression attempt
                    err_code = tjGetErrorCode(handle);
                    err_str = tjGetErrorStr();
                    err_str2 = tjGetErrorStr2(handle);
                    err_str3 = tj3GetErrorStr(handle);
                    err_code3 = tj3GetErrorCode(handle);
                    
                    (void)result;
                    (void)err_code;
                    (void)err_str;
                    (void)err_str2;
                    (void)err_str3;
                    (void)err_code3;
                    
                    free(dstBuf);
                }
            }
        } else {
            // Decompression header failed - test error functions
            err_code = tjGetErrorCode(handle);
            err_str = tjGetErrorStr();
            err_str2 = tjGetErrorStr2(handle);
            err_str3 = tj3GetErrorStr(handle);
            err_code3 = tj3GetErrorCode(handle);
            
            (void)err_code;
            (void)err_str;
            (void)err_str2;
            (void)err_str3;
            (void)err_code3;
        }
        
        tjDestroy(handle);
    }
    
    // Test 3: Test global error functions without a handle
    const char *global_err_str = tjGetErrorStr();
    const char *global_err_str3 = tj3GetErrorStr(NULL);
    
    (void)global_err_str;
    (void)global_err_str3;
    
    // Test 4: Test with invalid handle
    tjhandle invalid_handle = (tjhandle)((uintptr_t)0xdeadbeef);
    int invalid_err_code = tjGetErrorCode(invalid_handle);
    const char *invalid_err_str2 = tjGetErrorStr2(invalid_handle);
    const char *invalid_err_str3 = tj3GetErrorStr(invalid_handle);
    int invalid_err_code3 = tj3GetErrorCode(invalid_handle);
    
    (void)invalid_err_code;
    (void)invalid_err_str2;
    (void)invalid_err_str3;
    (void)invalid_err_code3;
    
    // Test 5: Test tjDecompress2 with various invalid parameters
    handle = tjInitDecompress();
    if (handle) {
        unsigned char *dstBuf = (unsigned char *)malloc(100 * 100 * 3);
        if (dstBuf) {
            // Test with various combinations of parameters
            for (int i = 0; i < 8; i++) {
                int test_width = (i & 1) ? 100 : -100;
                int test_height = (i & 2) ? 100 : -100;
                int test_pixelFormat = (i & 4) ? TJPF_RGB : 999;
                
                int result = tjDecompress2(handle, Data, Size, dstBuf, 
                                          test_width, test_width * 3, test_height,
                                          test_pixelFormat, TJFLAG_FASTDCT);
                
                // Call error functions
                int test_err_code = tjGetErrorCode(handle);
                const char *test_err_str = tjGetErrorStr();
                const char *test_err_str2 = tjGetErrorStr2(handle);
                const char *test_err_str3 = tj3GetErrorStr(handle);
                int test_err_code3 = tj3GetErrorCode(handle);
                
                (void)result;
                (void)test_err_code;
                (void)test_err_str;
                (void)test_err_str2;
                (void)test_err_str3;
                (void)test_err_code3;
            }
            free(dstBuf);
        }
        tjDestroy(handle);
    }
    
    remove_dummy_file();
    return 0;
}