// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;
    
    int width, height, pixelFormat;
    int align = 1;
    int flags = 0;
    int subsamp = TJSAMP_420;
    int pitch;
    
    // Test tj3LoadImage16
    width = 0; height = 0; pixelFormat = TJPF_UNKNOWN;
    align = 1 + (Data[0] % 32);
    if (align <= 0) align = 1;
    
    unsigned short *img16 = tj3LoadImage16(handle, "./dummy_file", 
                                          &width, align, &height, &pixelFormat);
    if (img16) {
        // Test tj3Get
        int param_val = tj3Get(handle, TJPARAM_PRECISION);
        (void)param_val;
        
        // Test tj3Compress16 if we have valid image
        if (width > 0 && height > 0 && pixelFormat != TJPF_UNKNOWN) {
            unsigned char *jpegBuf = NULL;
            size_t jpegSize = 0;
            pitch = width * tjPixelSize[pixelFormat];
            
            tj3Set(handle, TJPARAM_NOREALLOC, 0);
            tj3Compress16(handle, img16, width, pitch, height, 
                         pixelFormat, &jpegBuf, &jpegSize);
            
            if (jpegBuf) {
                free(jpegBuf);
            }
        }
        tj3Free(img16);
    }
    
    // Test tjLoadImage
    width = 0; height = 0; pixelFormat = TJPF_UNKNOWN;
    align = 1 + (Data[0] % 32);
    if (align <= 0) align = 1;
    flags = Data[0] % 256;
    
    unsigned char *img8 = tjLoadImage("./dummy_file", &width, align, 
                                     &height, &pixelFormat, flags);
    if (img8) {
        // Test tjSaveImage if we have valid image
        if (width > 0 && height > 0 && pixelFormat != TJPF_UNKNOWN) {
            pitch = width * tjPixelSize[pixelFormat];
            flags = Data[0] % 256;
            tjSaveImage("./dummy_file", img8, width, pitch, 
                       height, pixelFormat, flags);
        }
        free(img8);
    }
    
    // Test tjDecodeYUVPlanes
    if (Size >= 100) {
        width = 64;
        height = 64;
        pitch = width * 3; // RGB
        pixelFormat = TJPF_RGB;
        flags = Data[0] % 256;
        
        unsigned char *dstBuf = (unsigned char*)malloc(pitch * height);
        if (dstBuf) {
            const unsigned char *srcPlanes[3];
            int strides[3];
            
            // Simple YUV plane setup
            int y_size = width * height;
            int uv_size = (width/2) * (height/2);
            
            if (Size >= (size_t)(y_size + 2*uv_size)) {
                srcPlanes[0] = Data;
                srcPlanes[1] = Data + y_size;
                srcPlanes[2] = Data + y_size + uv_size;
                
                strides[0] = width;
                strides[1] = width/2;
                strides[2] = width/2;
                
                subsamp = Data[0] % 4;
                tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp,
                                 dstBuf, width, pitch, height, 
                                 pixelFormat, flags);
            }
            free(dstBuf);
        }
    }
    
    tj3Destroy(handle);
    return 0;
}