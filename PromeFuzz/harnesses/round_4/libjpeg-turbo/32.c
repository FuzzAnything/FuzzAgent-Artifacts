// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDecompress at turbojpeg.c:2108:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static tjscalingfactor get_scaling_factor_from_data(const uint8_t *data, size_t size) {
    static const tjscalingfactor factors[] = {
        {1, 1},   // TJUNSCALED
        {1, 2},
        {1, 4},
        {1, 8},
        {3, 8},
        {5, 8},
        {3, 4},
        {7, 8},
        {2, 1}
    };
    
    if (size == 0) return factors[0];
    return factors[data[0] % (sizeof(factors)/sizeof(factors[0]))];
}

static enum TJPF get_pixel_format_from_data(const uint8_t *data, size_t size) {
    static const enum TJPF formats[] = {
        TJPF_RGB,
        TJPF_BGR,
        TJPF_RGBX,
        TJPF_BGRX,
        TJPF_XRGB,
        TJPF_XBGR,
        TJPF_GRAY,
        TJPF_RGBA,
        TJPF_BGRA,
        TJPF_ABGR,
        TJPF_ARGB
    };
    
    if (size < 2) return TJPF_RGB;
    return formats[data[1] % (sizeof(formats)/sizeof(formats[0]))];
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;
    
    int width = 0, height = 0, jpegSubsamp = 0;
    unsigned char *jpegBuf = (unsigned char *)Data;
    unsigned long jpegSize = Size;
    
    // Test tjDecompressHeader
    tjDecompressHeader(handle, jpegBuf, jpegSize, &width, &height);
    
    // Test tjDecompressHeader2
    tjDecompressHeader2(handle, jpegBuf, jpegSize, &width, &height, &jpegSubsamp);
    
    // Test tj3SetScalingFactor
    tjscalingfactor scalingFactor = get_scaling_factor_from_data(Data, Size);
    tj3SetScalingFactor(handle, scalingFactor);
    
    // Calculate scaled dimensions
    int scaledWidth = TJSCALED(width, scalingFactor);
    int scaledHeight = TJSCALED(height, scalingFactor);
    
    if (scaledWidth > 0 && scaledHeight > 0) {
        // Test tjDecompressToYUV2
        size_t yuvBufSize = scaledWidth * scaledHeight * 3 / 2;
        unsigned char *yuvBuf = (unsigned char *)malloc(yuvBufSize);
        if (yuvBuf) {
            tjDecompressToYUV2(handle, jpegBuf, jpegSize, yuvBuf, 
                              scaledWidth, 4, scaledHeight, 0);
            free(yuvBuf);
        }
        
        // Test tjDecompress
        int pixelSize = 3; // RGB
        int pitch = scaledWidth * pixelSize;
        size_t rgbBufSize = pitch * scaledHeight;
        unsigned char *rgbBuf = (unsigned char *)malloc(rgbBufSize);
        if (rgbBuf) {
            tjDecompress(handle, jpegBuf, jpegSize, rgbBuf, 
                        scaledWidth, pitch, scaledHeight, pixelSize, 0);
            free(rgbBuf);
        }
        
        // Test tjDecompress2
        enum TJPF pixelFormat = get_pixel_format_from_data(Data, Size);
        int bytesPerPixel = tjPixelSize[pixelFormat];
        pitch = scaledWidth * bytesPerPixel;
        rgbBufSize = pitch * scaledHeight;
        rgbBuf = (unsigned char *)malloc(rgbBufSize);
        if (rgbBuf) {
            tjDecompress2(handle, jpegBuf, jpegSize, rgbBuf, 
                         scaledWidth, pitch, scaledHeight, pixelFormat, 0);
            free(rgbBuf);
        }
    }
    
    tjDestroy(handle);
    return 0;
}