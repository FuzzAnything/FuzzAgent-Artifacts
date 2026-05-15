// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
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

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    // Initialize TurboJPEG instance for compression
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) {
        remove_dummy_file();
        return 0;
    }
    
    // Test tj3Compress16 with various parameters
    if (Size > 100) {
        unsigned char *jpegBuf = NULL;
        size_t jpegSize = 0;
        
        // Use part of the data as source buffer
        size_t srcSize = Size > 1000 ? 1000 : Size;
        unsigned short *srcBuf = (unsigned short *)Data;
        
        // Extract parameters from data
        int width = (Data[0] << 8) | Data[1];
        if (width <= 0) width = 16;
        if (width > 256) width = 256;
        
        int height = (Data[2] << 8) | Data[3];
        if (height <= 0) height = 16;
        if (height > 256) height = 256;
        
        int pitch = width * 3; // Assume RGB format
        int pixelFormat = Data[4] % 10; // Various pixel formats
        
        // Set some parameters
        tj3Set(handle, TJPARAM_QUALITY, Data[5] % 101);
        tj3Set(handle, TJPARAM_SUBSAMP, Data[6] % 4);
        
        // Test compression
        tj3Compress16(handle, srcBuf, width, pitch, height, 
                      pixelFormat, &jpegBuf, &jpegSize);
        
        if (jpegBuf) {
            tj3Free(jpegBuf);
        }
    }
    
    // Test tj3LoadImage16
    if (Size > 50) {
        int width, height, pixelFormat;
        pixelFormat = TJPF_UNKNOWN;
        
        unsigned short *image = tj3LoadImage16(handle, "./dummy_file", 
                                               &width, 1, &height, 
                                               &pixelFormat);
        if (image) {
            tj3Free(image);
        }
    }
    
    // Test tj3Get with various parameters
    if (Size > 10) {
        for (int i = 0; i < 5 && i < Size; i++) {
            int param = Data[i] % 20; // Test various parameters
            tj3Get(handle, param);
        }
    }
    
    // Test tjLoadImage
    if (Size > 50) {
        int width, height, pixelFormat;
        unsigned char *image = tjLoadImage("./dummy_file", &width, 1, 
                                           &height, &pixelFormat, 0);
        if (image) {
            free(image);
        }
    }
    
    // Test tjSaveImage
    if (Size > 100) {
        // Create a small buffer for saving
        unsigned char buffer[1000];
        memcpy(buffer, Data, Size > 1000 ? 1000 : Size);
        
        int width = 16;
        int height = 16;
        int pitch = width * 3;
        int pixelFormat = TJPF_RGB;
        
        tjSaveImage("./dummy_file", buffer, width, pitch, height, 
                    pixelFormat, 0);
    }
    
    // Test tjDecodeYUVPlanes
    if (Size > 200) {
        // Reinitialize for decompression
        tj3Destroy(handle);
        handle = tj3Init(TJINIT_DECOMPRESS);
        if (!handle) {
            remove_dummy_file();
            return 0;
        }
        
        // Prepare YUV planes
        const unsigned char *srcPlanes[3];
        int strides[3] = {16, 8, 8};
        unsigned char dstBuf[768]; // 16x16 RGB
        
        // Use data for YUV planes
        size_t offset = 0;
        for (int i = 0; i < 3 && offset < Size; i++) {
            srcPlanes[i] = Data + offset;
            offset += 64;
        }
        
        int subsamp = Data[0] % 4;
        
        tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp,
                          dstBuf, 16, 48, 16, TJPF_RGB, 0);
    }
    
    // Cleanup
    tj3Destroy(handle);
    remove_dummy_file();
    
    return 0;
}