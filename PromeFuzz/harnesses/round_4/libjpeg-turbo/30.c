// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
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

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    // Write input to dummy file for potential file-based operations
    write_dummy_file(Data, Size);
    
    // Initialize TurboJPEG decompressor instance
    tjhandle handle = tjInitDecompress();
    if (!handle) {
        // Test error functions even when handle creation fails
        (void)tjGetErrorStr();
        (void)tj3GetErrorStr(NULL);
        remove_dummy_file();
        return 0;
    }
    
    // Test tjDecompressHeader with various inputs
    int width = 0, height = 0;
    unsigned long jpegSize = Size;
    
    // First call with original data
    int header_result = tjDecompressHeader(handle, (unsigned char *)Data, jpegSize, &width, &height);
    
    // Test error functions after header decompression
    (void)tjGetErrorCode(handle);
    (void)tjGetErrorStr();
    (void)tj3GetErrorStr(handle);
    (void)tjGetErrorStr2(handle);
    
    // Try different variations if header succeeded
    if (header_result == 0 && width > 0 && height > 0) {
        // Allocate destination buffer for decompression
        int pixelFormat = TJPF_RGB;
        int pitch = width * tjPixelSize[pixelFormat];
        unsigned long dstBufSize = pitch * height;
        unsigned char *dstBuf = NULL;
        
        if (dstBufSize > 0 && dstBufSize < 100 * 1024 * 1024) { // Limit to 100MB
            dstBuf = (unsigned char *)malloc(dstBufSize);
            if (dstBuf) {
                // Test tjDecompress2 with different flags
                int flags[] = {0, TJFLAG_FASTDCT, TJFLAG_ACCURATEDCT, TJFLAG_PROGRESSIVE};
                for (size_t i = 0; i < sizeof(flags)/sizeof(flags[0]); i++) {
                    int decompress_result = tjDecompress2(
                        handle, Data, jpegSize, dstBuf,
                        width, pitch, height, pixelFormat, flags[i]
                    );
                    
                    // Test error functions after decompression
                    if (decompress_result != 0) {
                        (void)tjGetErrorCode(handle);
                        (void)tjGetErrorStr();
                        (void)tj3GetErrorStr(handle);
                        (void)tjGetErrorStr2(handle);
                    }
                }
                
                free(dstBuf);
            }
        }
        
        // Test with different pixel formats if dimensions are reasonable
        if (width < 4096 && height < 4096) {
            int formats[] = {TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_GRAY};
            for (size_t i = 0; i < sizeof(formats)/sizeof(formats[0]); i++) {
                pitch = width * tjPixelSize[formats[i]];
                dstBufSize = pitch * height;
                
                if (dstBufSize > 0 && dstBufSize < 10 * 1024 * 1024) { // Limit to 10MB
                    dstBuf = (unsigned char *)malloc(dstBufSize);
                    if (dstBuf) {
                        (void)tjDecompress2(
                            handle, Data, jpegSize, dstBuf,
                            width, pitch, height, formats[i], 0
                        );
                        free(dstBuf);
                    }
                }
            }
        }
    }
    
    // Test error functions with NULL handle for tj3GetErrorStr
    (void)tj3GetErrorStr(NULL);
    
    // Test error functions with NULL handle for tjGetErrorStr2
    (void)tjGetErrorStr2(NULL);
    
    // Clean up
    tjDestroy(handle);
    remove_dummy_file();
    
    return 0;
}