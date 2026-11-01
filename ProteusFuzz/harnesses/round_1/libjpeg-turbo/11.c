// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
// tjFree at turbojpeg.c:896:16 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjAlloc at turbojpeg.c:883:26 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void cleanup_file(const char *filename) {
    if (filename != NULL) {
        remove(filename);
    }
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 5) {
        return 0;
    }

    const char *dummy_file = "./dummy_file";
    tjhandle handle = NULL;
    unsigned char *buf8 = NULL;
    unsigned short *buf16 = NULL;
    int width, height, pixelFormat;
    int pitch;
    int align;
    int flags;
    int ret;

    // Extract parameters from input data safely
    width = Data[0];
    height = Data[1];
    pixelFormat = Data[2] % 5; // Limit to 0-4
    pitch = Data[3];
    align = (Data[0] & 0x0F) + 1; 
    flags = Data[4] % 100; 

    // Ensure align is power of 2
    if (align > 0) {
        align = 1 << (align % 5); // 1, 2, 4, 8, 16
    } else {
        align = 1;
    }

    // Initialize TurboJPEG handle
    handle = tjInitCompress();
    if (handle == NULL) {
        return 0;
    }

    // Write dummy data to file for loading functions
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        // Write a minimal PPM header and some data
        fprintf(f, "P6\n%d %d\n255\n", width > 0 ? width : 1, height > 0 ? height : 1);
        size_t data_len = Size > 100 ? 100 : Size;
        fwrite(Data, 1, data_len, f);
        fclose(f);
    }

    // Test 1: tjLoadImage
    width = 0; height = 0; pixelFormat = TJPF_UNKNOWN;
    buf8 = tjLoadImage(dummy_file, &width, align, &height, &pixelFormat, flags);
    if (buf8 != NULL) {
        tjFree(buf8);
        buf8 = NULL;
    }

    // Test 2: tj3LoadImage16
    width = 0; height = 0; pixelFormat = TJPF_UNKNOWN;
    buf16 = tj3LoadImage16(handle, dummy_file, &width, align, &height, &pixelFormat);
    if (buf16 != NULL) {
        tj3Free(buf16);
        buf16 = NULL;
    }

    // Test 3: tjSaveImage
    // Only proceed if dimensions are valid and reasonable to avoid overflow
    if (width > 0 && height > 0) {
        int samplesPerPixel = (pixelFormat == TJPF_GRAY) ? 1 : (pixelFormat == TJPF_CMYK ? 4 : 3);
        // Calculate row size in bytes. If pitch is 0, calculate based on width and pixel format.
        // Ensure pitch is consistent with the buffer we allocate.
        int rowBytes = (pitch > 0) ? pitch : (width * samplesPerPixel);
        
        // Prevent integer overflow in total size calculation
        if (width > 0 && height > 0 && rowBytes > 0) {
            // Check for potential overflow before multiplication
            if ((int64_t)width * height > 1000000) {
                // Skip if dimensions are too large
            } else {
                int totalBytes = rowBytes * height;
                if (totalBytes > 0 && totalBytes < 10000000) { 
                    buf8 = tjAlloc(totalBytes);
                    if (buf8 != NULL) {
                        // Fill buffer with fuzz data safely
                        size_t fillLen = (Size < (size_t)totalBytes) ? Size : (size_t)totalBytes;
                        memcpy(buf8, Data, fillLen);
                        
                        // Pass pitch as rowBytes to ensure consistency with allocated buffer
                        ret = tjSaveImage(dummy_file, buf8, width, rowBytes, height, pixelFormat, flags);
                        (void)ret; 
                        
                        tjFree(buf8);
                        buf8 = NULL;
                    }
                }
            }
        }
    }

    // Test 4: tj3SaveImage16
    if (width > 0 && height > 0) {
        int samplesPerPixel16 = (pixelFormat == TJPF_GRAY) ? 1 : (pixelFormat == TJPF_CMYK ? 4 : 3);
        int rowSamples = (pitch > 0) ? pitch : (width * samplesPerPixel16);
        
        if (width > 0 && height > 0 && rowSamples > 0) {
             if ((int64_t)width * height > 500000) {
                // Skip if dimensions are too large
            } else {
                int totalSamples = rowSamples * height;
                if (totalSamples > 0 && totalSamples < 5000000) {
                    buf16 = tj3Alloc(totalSamples * sizeof(unsigned short));
                    if (buf16 != NULL) {
                        // Fill buffer safely
                        size_t fillLen = (Size < (size_t)(totalSamples * 2)) ? Size : (size_t)(totalSamples * 2);
                        memcpy(buf16, Data, fillLen);
                        
                        // Pass pitch as rowSamples to ensure consistency
                        ret = tj3SaveImage16(handle, dummy_file, buf16, width, rowSamples, height, pixelFormat);
                        (void)ret;
                        
                        tj3Free(buf16);
                        buf16 = NULL;
                    }
                }
            }
        }
    }

    // Cleanup
    tjDestroy(handle);
    cleanup_file(dummy_file);

    return 0;
}