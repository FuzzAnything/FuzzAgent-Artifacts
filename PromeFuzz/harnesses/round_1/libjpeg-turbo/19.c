// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "turbojpeg.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;
    
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    unsigned long jpegSize = Size;
    unsigned char *jpegBuf = (unsigned char *)Data;
    
    // Test tjDecompressHeader
    int ret = tjDecompressHeader(handle, jpegBuf, jpegSize, &width, &height);
    if (ret == 0 && width > 0 && height > 0 && width <= 4096 && height <= 4096) {
        // Test tjDecompressHeader2
        ret = tjDecompressHeader2(handle, jpegBuf, jpegSize, &width, &height, &jpegSubsamp);
        
        // Test tjDecompressHeader3
        if (ret == 0) {
            ret = tjDecompressHeader3(handle, jpegBuf, jpegSize, &width, &height, 
                                      &jpegSubsamp, &jpegColorspace);
        }
        
        // Only proceed with decompression if dimensions are reasonable
        if (width > 0 && height > 0 && width <= 4096 && height <= 4096) {
            // Calculate buffer sizes
            int pitch = width * tjPixelSize[TJPF_RGB];
            unsigned long rgbSize = pitch * height;
            unsigned long yuvSize = tjBufSizeYUV2(width, 4, height, jpegSubsamp);
            
            // Allocate buffers
            unsigned char *dstBuf = NULL;
            unsigned char *yuvBuf = NULL;
            
            if (rgbSize > 0 && rgbSize <= 100 * 1024 * 1024) { // Limit to 100MB
                dstBuf = (unsigned char *)malloc(rgbSize);
            }
            
            if (yuvSize > 0 && yuvSize <= 100 * 1024 * 1024) { // Limit to 100MB
                yuvBuf = (unsigned char *)malloc(yuvSize);
            }
            
            // Test tjDecompressToYUV
            if (yuvBuf) {
                tjDecompressToYUV(handle, jpegBuf, jpegSize, yuvBuf, 0);
            }
            
            // Test tjDecompress2
            if (dstBuf) {
                // Try different pixel formats based on input data
                int pixelFormat = TJPF_RGB;
                if (Size > 1) {
                    pixelFormat = Data[0] % (TJPF_CMYK + 1);
                }
                
                tjDecompress2(handle, jpegBuf, jpegSize, dstBuf, width, pitch, 
                             height, pixelFormat, 0);
            }
            
            // Test tjDecompressToYUV2
            if (yuvBuf) {
                int align = 4;
                if (Size > 2) {
                    align = 1 << (Data[1] % 4); // 1, 2, 4, or 8
                }
                tjDecompressToYUV2(handle, jpegBuf, jpegSize, yuvBuf, width, 
                                  align, height, 0);
            }
            
            // Clean up buffers
            free(dstBuf);
            free(yuvBuf);
        }
    }
    
    // Test with file-based approach
    write_dummy_file(Data, Size);
    FILE *f = fopen("./dummy_file", "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        long file_size = ftell(f);
        fseek(f, 0, SEEK_SET);
        
        if (file_size > 0 && file_size <= 10 * 1024 * 1024) { // Limit to 10MB
            unsigned char *file_buf = (unsigned char *)malloc(file_size);
            if (file_buf) {
                fread(file_buf, 1, file_size, f);
                
                // Test with file data
                tjDecompressHeader(handle, file_buf, file_size, &width, &height);
                
                free(file_buf);
            }
        }
        fclose(f);
    }
    remove_dummy_file();
    
    tjDestroy(handle);
    return 0;
}