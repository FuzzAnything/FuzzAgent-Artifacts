// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "turbojpeg.h"

static tjhandle init_handle(void) {
    return tjInitDecompress();
}

static void cleanup_handle(tjhandle handle) {
    if (handle) {
        tjDestroy(handle);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    tjhandle handle = init_handle();
    if (!handle) {
        return 0;
    }

    // Write data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        cleanup_handle(handle);
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Read dummy file back into buffer
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        cleanup_handle(handle);
        return 0;
    }
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    if (file_size <= 0) {
        fclose(fp);
        cleanup_handle(handle);
        return 0;
    }

    unsigned char *jpegBuf = (unsigned char *)malloc(file_size);
    if (!jpegBuf) {
        fclose(fp);
        cleanup_handle(handle);
        return 0;
    }
    
    size_t read_size = fread(jpegBuf, 1, file_size, fp);
    fclose(fp);
    
    if (read_size != (size_t)file_size) {
        free(jpegBuf);
        cleanup_handle(handle);
        return 0;
    }

    unsigned long jpegSize = (unsigned long)file_size;
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    
    // Test tjDecompressHeader
    int result1 = tjDecompressHeader(handle, jpegBuf, jpegSize, &width, &height);
    
    // Test tjDecompressHeader2
    int result2 = tjDecompressHeader2(handle, jpegBuf, jpegSize, &width, &height, &jpegSubsamp);
    
    // Test tjDecompressHeader3
    int result3 = tjDecompressHeader3(handle, jpegBuf, jpegSize, &width, &height, &jpegSubsamp, &jpegColorspace);
    
    // Only proceed with decompression if headers were successfully parsed
    if (result1 == 0 || result2 == 0 || result3 == 0) {
        if (width > 0 && height > 0) {
            // Limit dimensions to prevent excessive memory allocation
            if (width > 4096) width = 4096;
            if (height > 4096) height = 4096;
            
            // Calculate buffer sizes
            unsigned long yuvBufSize = tjBufSizeYUV(width, 4, height);
            unsigned long rgbBufSize = (unsigned long)width * height * 3; // TJPF_RGB
            
            // Allocate output buffers
            unsigned char *dstBufYUV = NULL;
            unsigned char *dstBufRGB = NULL;
            
            if (yuvBufSize > 0 && yuvBufSize < 100 * 1024 * 1024) { // Limit to 100MB
                dstBufYUV = (unsigned char *)malloc(yuvBufSize);
            }
            
            if (rgbBufSize > 0 && rgbBufSize < 100 * 1024 * 1024) { // Limit to 100MB
                dstBufRGB = (unsigned char *)malloc(rgbBufSize);
            }
            
            // Test tjDecompressToYUV
            if (dstBufYUV) {
                tjDecompressToYUV(handle, jpegBuf, jpegSize, dstBufYUV, 0);
            }
            
            // Test tjDecompressToYUV2 with various parameters
            if (dstBufYUV) {
                // Try different align values
                for (int align = 1; align <= 16; align *= 2) {
                    tjDecompressToYUV2(handle, jpegBuf, jpegSize, dstBufYUV, 
                                      width, align, height, 0);
                }
                
                // Try with scaled dimensions
                int scaled_width = width / 2;
                int scaled_height = height / 2;
                if (scaled_width > 0 && scaled_height > 0) {
                    unsigned long scaledYuvSize = tjBufSizeYUV(scaled_width, 4, scaled_height);
                    if (scaledYuvSize > 0 && scaledYuvSize <= yuvBufSize) {
                        tjDecompressToYUV2(handle, jpegBuf, jpegSize, dstBufYUV,
                                          scaled_width, 4, scaled_height, 0);
                    }
                }
            }
            
            // Test tjDecompress2 with various pixel formats
            if (dstBufRGB) {
                // Try different pixel formats
                int pixelFormats[] = {TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX, 
                                     TJPF_XRGB, TJPF_XBGR, TJPF_GRAY};
                int numFormats = sizeof(pixelFormats) / sizeof(pixelFormats[0]);
                
                for (int i = 0; i < numFormats; i++) {
                    int pixelSize = tjPixelSize[pixelFormats[i]];
                    unsigned long formatBufSize = (unsigned long)width * height * pixelSize;
                    
                    if (formatBufSize <= rgbBufSize) {
                        tjDecompress2(handle, jpegBuf, jpegSize, dstBufRGB,
                                     width, width * pixelSize, height, 
                                     pixelFormats[i], 0);
                    }
                }
                
                // Try with scaled dimensions
                int scaled_width = width / 2;
                int scaled_height = height / 2;
                if (scaled_width > 0 && scaled_height > 0) {
                    tjDecompress2(handle, jpegBuf, jpegSize, dstBufRGB,
                                 scaled_width, scaled_width * 3, scaled_height,
                                 TJPF_RGB, 0);
                }
            }
            
            // Clean up output buffers
            if (dstBufYUV) free(dstBufYUV);
            if (dstBufRGB) free(dstBufRGB);
        }
    }
    
    // Clean up
    free(jpegBuf);
    cleanup_handle(handle);
    
    return 0;
}