// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    // Initialize TurboJPEG instances
    tjhandle compress_handle = tj3Init(TJINIT_COMPRESS);
    tjhandle decompress_handle = tj3Init(TJINIT_DECOMPRESS);
    
    if (!compress_handle || !decompress_handle) {
        if (compress_handle) tj3Destroy(compress_handle);
        if (decompress_handle) tj3Destroy(decompress_handle);
        return 0;
    }
    
    // Set some parameters using fuzzer data
    int width = 16;
    int height = 16;
    int pitch = width * 3; // For RGB
    int pixelFormat = TJPF_RGB;
    int align = 1;
    int flags = 0;
    int subsamp = TJSAMP_420;
    
    // Use fuzzer data to vary parameters
    if (Size > 0) {
        width = (Data[0] % 64) + 1;
        height = (Data[1] % 64) + 1;
        pixelFormat = Data[2] % (TJPF_CMYK + 1);
        align = 1 << (Data[3] % 4);
        flags = Data[4] % 4;
        subsamp = Data[5] % (TJSAMP_GRAY + 1);
    }
    
    pitch = width * tjPixelSize[pixelFormat];
    
    // Test tj3Get
    int quality = tj3Get(compress_handle, TJPARAM_QUALITY);
    int chroma = tj3Get(compress_handle, TJPARAM_SUBSAMP);
    
    // Create source buffer for compression
    size_t srcBufSize = pitch * height;
    unsigned short *srcBuf16 = NULL;
    unsigned char *srcBuf8 = NULL;
    
    if (srcBufSize > 0 && srcBufSize <= Size * 2) {
        srcBuf16 = (unsigned short *)malloc(srcBufSize * sizeof(unsigned short));
        if (srcBuf16) {
            // Use fuzzer data to fill buffer (convert 8-bit to 16-bit)
            for (size_t i = 0; i < srcBufSize && i * 2 < Size; i++) {
                srcBuf16[i] = (Data[i * 2] << 8) | (Data[(i * 2 + 1) % Size]);
            }
        }
        
        srcBuf8 = (unsigned char *)malloc(srcBufSize);
        if (srcBuf8) {
            memcpy(srcBuf8, Data, srcBufSize < Size ? srcBufSize : Size);
        }
    }
    
    // Test tj3Compress16 if we have source buffer
    if (srcBuf16) {
        unsigned char *jpegBuf = NULL;
        size_t jpegSize = 0;
        
        tj3Compress16(compress_handle, srcBuf16, width, pitch, height, 
                     pixelFormat, &jpegBuf, &jpegSize);
        
        if (jpegBuf) {
            tj3Free(jpegBuf);
        }
    }
    
    // Test file-based functions using dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size > 4096 ? 4096 : Size, fp);
        fclose(fp);
        
        // Test tj3LoadImage16
        int loadedWidth, loadedHeight, loadedPixelFormat;
        unsigned short *loadedBuf16 = tj3LoadImage16(decompress_handle, 
                                                    "./dummy_file", 
                                                    &loadedWidth, align, 
                                                    &loadedHeight, 
                                                    &loadedPixelFormat);
        if (loadedBuf16) {
            tj3Free(loadedBuf16);
        }
        
        // Test tjLoadImage
        int loadedWidth8, loadedHeight8, loadedPixelFormat8;
        unsigned char *loadedBuf8 = tjLoadImage("./dummy_file", &loadedWidth8,
                                               align, &loadedHeight8,
                                               &loadedPixelFormat8, flags);
        if (loadedBuf8) {
            free(loadedBuf8);
        }
        
        // Test tjSaveImage if we have source buffer
        if (srcBuf8) {
            tjSaveImage("./dummy_file", srcBuf8, width, pitch, height,
                       pixelFormat, flags);
        }
        
        remove("./dummy_file");
    }
    
    // Test tjDecodeYUVPlanes
    if (srcBuf8) {
        // Create YUV planes from RGB data (simplified conversion)
        int ySize = width * height;
        int uvSize = ((width + 1) / 2) * ((height + 1) / 2);
        
        unsigned char *yPlane = (unsigned char *)malloc(ySize);
        unsigned char *uPlane = (unsigned char *)malloc(uvSize);
        unsigned char *vPlane = (unsigned char *)malloc(uvSize);
        
        if (yPlane && uPlane && vPlane) {
            // Fill with fuzzer data
            memcpy(yPlane, Data, ySize < Size ? ySize : Size);
            memcpy(uPlane, Data, uvSize < Size ? uvSize : Size);
            memcpy(vPlane, Data + (uvSize % Size), uvSize < Size ? uvSize : Size);
            
            const unsigned char *planes[3] = {yPlane, uPlane, vPlane};
            int strides[3] = {width, (width + 1) / 2, (width + 1) / 2};
            
            unsigned char *dstBuf = (unsigned char *)malloc(pitch * height);
            if (dstBuf) {
                tjDecodeYUVPlanes(decompress_handle, planes, strides, subsamp,
                                 dstBuf, width, pitch, height, pixelFormat, flags);
                free(dstBuf);
            }
            
            free(yPlane);
            free(uPlane);
            free(vPlane);
        } else {
            free(yPlane);
            free(uPlane);
            free(vPlane);
        }
    }
    
    // Cleanup
    free(srcBuf16);
    free(srcBuf8);
    tj3Destroy(compress_handle);
    tj3Destroy(decompress_handle);
    
    return 0;
}