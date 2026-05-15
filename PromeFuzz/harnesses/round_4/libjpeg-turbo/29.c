// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjLoadImage at turbojpeg.c:3104:26 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"
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

static int get_pixel_format(uint8_t val) {
    static const int formats[] = {
        TJPF_RGB, TJPF_BGR, TJPF_RGBX, TJPF_BGRX,
        TJPF_XRGB, TJPF_XBGR, TJPF_GRAY, TJPF_RGBA,
        TJPF_BGRA, TJPF_ABGR, TJPF_ARGB, TJPF_CMYK,
        TJPF_UNKNOWN
    };
    return formats[val % (sizeof(formats)/sizeof(formats[0]))];
}

static int get_subsamp(uint8_t val) {
    static const int subsamps[] = {
        TJSAMP_444, TJSAMP_422, TJSAMP_420,
        TJSAMP_GRAY, TJSAMP_440, TJSAMP_411
    };
    return subsamps[val % (sizeof(subsamps)/sizeof(subsamps[0]))];
}

static int get_param(uint8_t val) {
    static const int params[] = {
        TJPARAM_STOPONWARNING, TJPARAM_BOTTOMUP,
        TJPARAM_FASTUPSAMPLE, TJPARAM_FASTDCT,
        TJPARAM_OPTIMIZE, TJPARAM_PROGRESSIVE,
        TJPARAM_SCANLIMIT, TJPARAM_ARITHMETIC,
        TJPARAM_LOSSLESS, TJPARAM_LOSSLESSPSV,
        TJPARAM_LOSSLESSPT, TJPARAM_RESTARTROWS,
        TJPARAM_RESTARTBLOCKS, TJPARAM_PRECISION,
        TJPARAM_COLORSPACE, TJPARAM_SUBSAMP,
        TJPARAM_JPEGWIDTH, TJPARAM_JPEGHEIGHT
    };
    return params[val % (sizeof(params)/sizeof(params[0]))];
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    write_dummy_file(Data, Size);
    
    // Test tjLoadImage
    {
        int width = 0, height = 0, pixelFormat = TJPF_UNKNOWN;
        uint8_t *buffer = tjLoadImage("./dummy_file", &width, 1, &height, &pixelFormat, 0);
        if (buffer) {
            free(buffer);
        }
    }
    
    // Test tj3LoadImage16
    {
        tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
        if (handle) {
            int width = 0, height = 0, pixelFormat = get_pixel_format(Data[0]);
            unsigned short *buffer = tj3LoadImage16(handle, "./dummy_file", &width, 1, &height, &pixelFormat);
            if (buffer) {
                tj3Free(buffer);
            }
            tj3Destroy(handle);
        }
    }
    
    // Test tjSaveImage
    {
        int width = 16, height = 16;
        int pixelFormat = get_pixel_format(Data[1]);
        int pitch = width * tjPixelSize[pixelFormat];
        unsigned char *buffer = (unsigned char*)malloc(pitch * height);
        if (buffer) {
            memset(buffer, 0x80, pitch * height);
            tjSaveImage("./dummy_file", buffer, width, pitch, height, pixelFormat, 0);
            free(buffer);
        }
    }
    
    // Test tj3Compress16
    {
        tjhandle handle = tj3Init(TJINIT_COMPRESS);
        if (handle) {
            int width = 16, height = 16;
            int pixelFormat = get_pixel_format(Data[2]);
            int pitch = width * tjPixelSize[pixelFormat];
            unsigned short *srcBuf = (unsigned short*)malloc(pitch * height * sizeof(unsigned short));
            if (srcBuf) {
                memset(srcBuf, 0x8000, pitch * height * sizeof(unsigned short));
                unsigned char *jpegBuf = NULL;
                size_t jpegSize = 0;
                tj3Compress16(handle, srcBuf, width, pitch, height, pixelFormat, &jpegBuf, &jpegSize);
                if (jpegBuf) {
                    tj3Free(jpegBuf);
                }
                free(srcBuf);
            }
            tj3Destroy(handle);
        }
    }
    
    // Test tjDecodeYUVPlanes
    {
        tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
        if (handle) {
            int width = 16, height = 16;
            int pixelFormat = get_pixel_format(Data[3]);
            int pitch = width * tjPixelSize[pixelFormat];
            int subsamp = get_subsamp(Data[0]);
            
            // Allocate YUV planes
            const unsigned char *srcPlanes[3];
            int strides[3];
            unsigned char *yPlane = NULL, *uPlane = NULL, *vPlane = NULL;
            
            // Calculate plane sizes based on subsampling
            int ySize = width * height;
            int uvWidth = (subsamp == TJSAMP_444) ? width : (width + 1) / 2;
            int uvHeight = (subsamp == TJSAMP_420 || subsamp == TJSAMP_422) ? 
                          ((height + 1) / 2) : height;
            
            yPlane = (unsigned char*)malloc(ySize);
            uPlane = (unsigned char*)malloc(uvWidth * uvHeight);
            vPlane = (unsigned char*)malloc(uvWidth * uvHeight);
            
            if (yPlane && uPlane && vPlane) {
                memset(yPlane, 0x80, ySize);
                memset(uPlane, 0x80, uvWidth * uvHeight);
                memset(vPlane, 0x80, uvWidth * uvHeight);
                
                srcPlanes[0] = yPlane;
                srcPlanes[1] = uPlane;
                srcPlanes[2] = vPlane;
                
                strides[0] = width;
                strides[1] = uvWidth;
                strides[2] = uvWidth;
                
                unsigned char *dstBuf = (unsigned char*)malloc(pitch * height);
                if (dstBuf) {
                    tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp,
                                     dstBuf, width, pitch, height, pixelFormat, 0);
                    free(dstBuf);
                }
            }
            
            free(yPlane);
            free(uPlane);
            free(vPlane);
            tj3Destroy(handle);
        }
    }
    
    // Test tj3Get
    {
        tjhandle handle = tj3Init(TJINIT_COMPRESS);
        if (handle) {
            int param = get_param(Data[0]);
            int value = tj3Get(handle, param);
            (void)value; // Use value to avoid unused variable warning
            tj3Destroy(handle);
        }
    }
    
    return 0;
}