// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static tjhandle create_decompress_handle(void) {
    return tjInitDecompress();
}

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

static tjscalingfactor get_random_scaling_factor(const uint8_t *data, size_t size) {
    tjscalingfactor factor = {1, 1};
    if (size < 2) return factor;
    
    // Use first two bytes to generate scaling factor
    int num = (data[0] % 8) + 1;
    int denom = (data[1] % 8) + 1;
    
    // Ensure valid scaling factor (denominator >= numerator)
    if (denom < num) {
        int temp = num;
        num = denom;
        denom = temp;
    }
    
    factor.num = num;
    factor.denom = denom;
    return factor;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    tjhandle handle = create_decompress_handle();
    if (!handle) return 0;
    
    // Use first part of data as JPEG buffer
    unsigned char *jpegBuf = (unsigned char *)Data;
    unsigned long jpegSize = Size;
    
    // Variables for header extraction
    int width = 0, height = 0, jpegSubsamp = 0;
    
    // Test tjDecompressHeader
    if (tjDecompressHeader(handle, jpegBuf, jpegSize, &width, &height) == 0) {
        // Valid dimensions obtained, test scaling factor
        tjscalingfactor scalingFactor = get_random_scaling_factor(Data, Size);
        tj3SetScalingFactor(handle, scalingFactor);
        
        // Test tjDecompressHeader2
        tjDecompressHeader2(handle, jpegBuf, jpegSize, &width, &height, &jpegSubsamp);
        
        // If we have valid dimensions, test decompression functions
        if (width > 0 && height > 0 && width <= 4096 && height <= 4096) {
            // Calculate scaled dimensions
            int scaledWidth = TJSCALED(width, scalingFactor);
            int scaledHeight = TJSCALED(height, scalingFactor);
            
            // Ensure reasonable dimensions
            if (scaledWidth > 0 && scaledHeight > 0 && scaledWidth <= 4096 && scaledHeight <= 4096) {
                // Allocate buffers for decompression
                int pitch = scaledWidth * 3; // RGB pitch
                size_t rgbBufSize = pitch * scaledHeight;
                size_t yuvBufSize = tjBufSizeYUV2(scaledWidth, 4, scaledHeight, TJSAMP_420);
                
                unsigned char *rgbBuf = NULL;
                unsigned char *yuvBuf = NULL;
                
                if (rgbBufSize > 0 && rgbBufSize < 100 * 1024 * 1024) { // Limit to 100MB
                    rgbBuf = (unsigned char *)malloc(rgbBufSize);
                }
                
                if (yuvBufSize > 0 && yuvBufSize < 100 * 1024 * 1024) { // Limit to 100MB
                    yuvBuf = (unsigned char *)malloc(yuvBufSize);
                }
                
                // Test tjDecompressToYUV2
                if (yuvBuf) {
                    tjDecompressToYUV2(handle, jpegBuf, jpegSize, yuvBuf, 
                                      scaledWidth, 4, scaledHeight, 0);
                }
                
                // Test tjDecompress
                if (rgbBuf) {
                    tjDecompress(handle, jpegBuf, jpegSize, rgbBuf, 
                                scaledWidth, pitch, scaledHeight, 3, 0);
                }
                
                // Test tjDecompress2
                if (rgbBuf) {
                    tjDecompress2(handle, jpegBuf, jpegSize, rgbBuf, 
                                 scaledWidth, pitch, scaledHeight, TJPF_RGB, 0);
                }
                
                // Cleanup buffers
                if (rgbBuf) free(rgbBuf);
                if (yuvBuf) free(yuvBuf);
            }
        }
    }
    
    // Test with different flags and parameters
    int width2 = 0, height2 = 0;
    if (tjDecompressHeader(handle, jpegBuf, jpegSize, &width2, &height2) == 0) {
        if (width2 > 0 && height2 > 0 && width2 <= 1024 && height2 <= 1024) {
            int pitch = width2 * 3;
            size_t bufSize = pitch * height2;
            
            if (bufSize > 0 && bufSize < 10 * 1024 * 1024) {
                unsigned char *buf = (unsigned char *)malloc(bufSize);
                if (buf) {
                    // Test with different flags
                    tjDecompress(handle, jpegBuf, jpegSize, buf, 
                                width2, pitch, height2, 3, TJFLAG_FASTDCT);
                    tjDecompress2(handle, jpegBuf, jpegSize, buf, 
                                 width2, pitch, height2, TJPF_BGR, TJFLAG_ACCURATEDCT);
                    free(buf);
                }
            }
        }
    }
    
    cleanup_handle(handle);
    return 0;
}