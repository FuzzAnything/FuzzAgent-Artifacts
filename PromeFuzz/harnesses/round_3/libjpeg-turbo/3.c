// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "turbojpeg.h"

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    tjhandle handle = NULL;
    unsigned short *imgBuf = NULL;
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    int width = 0, height = 0, pixelFormat = TJPF_UNKNOWN;
    int ret = 0;
    
    // Initialize TurboJPEG instance for compression
    handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) {
        return 0;
    }
    
    // First set call - use fuzzer data to determine parameter and value
    if (Size > 0) {
        tj3Set(handle, Data[0] % 20, Data[0]);
    }
    
    // Second set call
    if (Size > 1) {
        tj3Set(handle, Data[1] % 20, Data[1]);
    }
    
    // Third set call
    if (Size > 2) {
        tj3Set(handle, Data[2] % 20, Data[2]);
    }
    
    // Fourth set call
    if (Size > 3) {
        tj3Set(handle, Data[3] % 20, Data[3]);
    }
    
    // Fifth set call
    if (Size > 4) {
        tj3Set(handle, Data[4] % 20, Data[4]);
    }
    
    // Write fuzzer data to file for tj3LoadImage16
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        // Write a simple PPM header followed by fuzzer data
        // This creates a minimal valid PPM file that tj3LoadImage16 can attempt to parse
        const char *ppm_header = "P6\n16 16\n65535\n";
        fwrite(ppm_header, 1, strlen(ppm_header), fp);
        if (Size > 0) {
            // Write some pixel data - use fuzzer input for pixel values
            // 16x16 image with 3 channels (RGB) = 768 bytes for 8-bit, but we need 16-bit
            // So we need 16*16*3*2 = 1536 bytes
            size_t pixels_needed = 16 * 16 * 3 * 2;
            size_t to_write = Size < pixels_needed ? Size : pixels_needed;
            fwrite(Data, 1, to_write, fp);
        }
        fclose(fp);
    }
    
    // Load image from file
    imgBuf = tj3LoadImage16(handle, "./dummy_file", &width, 1, &height, &pixelFormat);
    
    // Clean up the temporary file
    remove("./dummy_file");
    
    // If image loading failed, use default values
    if (!imgBuf || width <= 0 || height <= 0) {
        width = 16;
        height = 16;
        pixelFormat = TJPF_RGB;
    }
    
    // Calculate JPEG buffer size
    jpegSize = tj3JPEGBufSize(width, height, TJSAMP_444);
    
    // Get a parameter value
    if (Size > 5) {
        tj3Get(handle, Data[5] % 20);
    }
    
    // Allocate JPEG buffer if we have a valid size
    if (jpegSize > 0) {
        jpegBuf = (unsigned char *)tj3Alloc(jpegSize);
    }
    
    // Sixth set call
    if (Size > 6) {
        tj3Set(handle, Data[6] % 20, Data[6]);
    }
    
    // Seventh set call
    if (Size > 7) {
        tj3Set(handle, Data[7] % 20, Data[7]);
    }
    
    // Eighth set call
    if (Size > 8) {
        tj3Set(handle, Data[8] % 20, Data[8]);
    }
    
    // Compress the image if we have valid buffers
    if (handle && jpegBuf && jpegSize > 0) {
        // Create a source buffer if image loading failed
        unsigned short *srcBuf = imgBuf;
        size_t srcSize = 0;
        
        if (!srcBuf) {
            // Allocate a minimal source buffer
            srcSize = width * height * tjPixelSize[pixelFormat] * sizeof(unsigned short);
            if (srcSize > 0 && srcSize < 1024 * 1024) { // Limit to 1MB
                srcBuf = (unsigned short *)malloc(srcSize);
                if (srcBuf && Size > 0) {
                    // Initialize with fuzzer data
                    size_t to_copy = Size < srcSize ? Size : srcSize;
                    memcpy(srcBuf, Data, to_copy);
                }
            }
        }
        
        if (srcBuf) {
            size_t compressSize = jpegSize;
            tj3Compress16(handle, srcBuf, width, 0, height, pixelFormat, &jpegBuf, &compressSize);
            
            // Free the temporary source buffer if we allocated it
            if (!imgBuf && srcBuf) {
                free(srcBuf);
            }
        }
    }
    
    // Free allocated buffers
    if (imgBuf) {
        tj3Free(imgBuf);
    }
    
    if (jpegBuf) {
        tj3Free(jpegBuf);
    }
    
    // Destroy TurboJPEG instance
    tj3Destroy(handle);
    
    return 0;
}