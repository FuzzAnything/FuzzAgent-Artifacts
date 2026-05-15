// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
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

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    // Initialize TurboJPEG instance for compression
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) {
        remove_dummy_file();
        return 0;
    }
    
    // Test tj3Set with various parameters
    // Use first byte to determine which parameter to set
    uint8_t param_selector = Data[0];
    int param_value = (Size > 1) ? Data[1] : 50;
    
    // Try setting different parameters based on input
    switch (param_selector % 4) {
        case 0:
            tj3Set(handle, TJPARAM_QUALITY, param_value % 101);
            break;
        case 1:
            tj3Set(handle, TJPARAM_SUBSAMP, param_value % 4);
            break;
        case 2:
            tj3Set(handle, TJPARAM_PRECISION, 13 + (param_value % 4));
            break;
        case 3:
            tj3Set(handle, TJPARAM_NOREALLOC, param_value & 1);
            break;
    }
    
    // Test tj3LoadImage16
    int width = 0, height = 0;
    int pixelFormat = TJPF_UNKNOWN;
    
    // Use second byte for align parameter (must be power of 2)
    int align = 1;
    if (Size > 2) {
        uint8_t align_byte = Data[2];
        if (align_byte & 1) align = 1;
        else if (align_byte & 2) align = 2;
        else if (align_byte & 4) align = 4;
        else if (align_byte & 8) align = 8;
        else align = 16;
    }
    
    unsigned short *image_buffer = tj3LoadImage16(handle, "./dummy_file", 
                                                  &width, align, &height, 
                                                  &pixelFormat);
    
    // Test tj3Alloc with varying sizes
    size_t alloc_size = 1024;
    if (Size > 3) {
        alloc_size = (Data[3] + 1) * 64; // Ensure non-zero
    }
    void *alloc_buffer = tj3Alloc(alloc_size);
    
    // If image was loaded successfully, try compression
    if (image_buffer && width > 0 && height > 0) {
        unsigned char *jpegBuf = NULL;
        size_t jpegSize = 0;
        
        // Determine pitch based on pixel format
        int pitch = 0;
        if (pixelFormat >= 0 && pixelFormat <= 11) {
            pitch = width * tjPixelSize[pixelFormat];
        }
        
        // Try different buffer allocation strategies
        if (Size > 4 && (Data[4] & 1)) {
            // Pre-allocate buffer
            jpegSize = tj3JPEGBufSize(width, height, TJSAMP_444);
            if (jpegSize > 0) {
                jpegBuf = (unsigned char *)tj3Alloc(jpegSize);
            }
        }
        
        int compress_result = tj3Compress16(handle, image_buffer, 
                                           width, pitch, height, pixelFormat,
                                           &jpegBuf, &jpegSize);
        
        // Free JPEG buffer if it was allocated
        if (jpegBuf) {
            tj3Free(jpegBuf);
        }
        
        // Free the image buffer
        tj3Free(image_buffer);
    }
    
    // Free allocated buffer
    if (alloc_buffer) {
        tj3Free(alloc_buffer);
    }
    
    // Clean up
    tj3Destroy(handle);
    remove_dummy_file();
    
    return 0;
}