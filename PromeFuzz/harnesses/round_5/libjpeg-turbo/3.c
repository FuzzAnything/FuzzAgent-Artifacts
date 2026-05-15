// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
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
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int readIntFromData(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) {
        *size = 0;
        return 0;
    }
    int value;
    memcpy(&value, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    return value;
}

static void writeDummyFile(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    const uint8_t *originalData = Data;
    size_t originalSize = Size;
    
    // Read parameters from fuzzed data
    int initType = readIntFromData(&Data, &Size);
    int setParam1 = readIntFromData(&Data, &Size);
    int setValue1 = readIntFromData(&Data, &Size);
    int setParam2 = readIntFromData(&Data, &Size);
    int setValue2 = readIntFromData(&Data, &Size);
    int setParam3 = readIntFromData(&Data, &Size);
    int setValue3 = readIntFromData(&Data, &Size);
    int setParam4 = readIntFromData(&Data, &Size);
    int setValue4 = readIntFromData(&Data, &Size);
    int setParam5 = readIntFromData(&Data, &Size);
    int setValue5 = readIntFromData(&Data, &Size);
    
    // Write remaining data to dummy file for image loading
    writeDummyFile(Data, Size);
    
    // Create TurboJPEG instance
    tjhandle handle = tj3Init(initType);
    if (!handle) {
        return 0;
    }
    
    // Set parameters
    tj3Set(handle, setParam1, setValue1);
    tj3Set(handle, setParam2, setValue2);
    tj3Set(handle, setParam3, setValue3);
    tj3Set(handle, setParam4, setValue4);
    tj3Set(handle, setParam5, setValue5);
    
    // Load image from dummy file
    int width = 0, height = 0, pixelFormat = 0;
    unsigned short *imageBuffer = tj3LoadImage16(handle, "./dummy_file", &width, 1, &height, &pixelFormat);
    
    // Calculate JPEG buffer size if image loaded successfully
    size_t jpegBufSize = 0;
    if (imageBuffer && width > 0 && height > 0) {
        int jpegSubsamp = readIntFromData(&originalData, &originalSize);
        jpegBufSize = tj3JPEGBufSize(width, height, jpegSubsamp);
        
        // Get a parameter
        int getParam = readIntFromData(&originalData, &originalSize);
        tj3Get(handle, getParam);
        
        // Allocate JPEG buffer
        unsigned char *jpegBuffer = NULL;
        if (jpegBufSize > 0) {
            jpegBuffer = tj3Alloc(jpegBufSize);
        }
        
        // Set more parameters
        int setParam6 = readIntFromData(&originalData, &originalSize);
        int setValue6 = readIntFromData(&originalData, &originalSize);
        int setParam7 = readIntFromData(&originalData, &originalSize);
        int setValue7 = readIntFromData(&originalData, &originalSize);
        int setParam8 = readIntFromData(&originalData, &originalSize);
        int setValue8 = readIntFromData(&originalData, &originalSize);
        
        tj3Set(handle, setParam6, setValue6);
        tj3Set(handle, setParam7, setValue7);
        tj3Set(handle, setParam8, setValue8);
        
        // Compress image if we have buffers
        if (jpegBuffer && imageBuffer) {
            size_t actualJpegSize = jpegBufSize;
            int pitch = width * (pixelFormat == TJPF_GRAY ? 1 : 
                               pixelFormat == TJPF_RGB ? 3 : 
                               pixelFormat == TJPF_BGR ? 3 : 
                               pixelFormat == TJPF_RGBX ? 4 : 
                               pixelFormat == TJPF_BGRX ? 4 : 
                               pixelFormat == TJPF_XRGB ? 4 : 
                               pixelFormat == TJPF_XBGR ? 4 : 
                               pixelFormat == TJPF_CMYK ? 4 : 3);
            
            tj3Compress16(handle, imageBuffer, width, pitch, height, 
                         pixelFormat, &jpegBuffer, &actualJpegSize);
        }
        
        // Free JPEG buffer if allocated
        if (jpegBuffer) {
            tj3Free(jpegBuffer);
        }
    }
    
    // Free image buffer if loaded
    if (imageBuffer) {
        tj3Free(imageBuffer);
    }
    
    // Destroy TurboJPEG instance
    tj3Destroy(handle);
    
    return 0;
}