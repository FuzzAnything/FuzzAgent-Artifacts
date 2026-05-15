// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3DecompressToYUVPlanes8 at turbojpeg.c:2122:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// tjPlaneWidth at turbojpeg.c:1083:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
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

#define MAX_DIM 4096
#define MAX_PLANES 3

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    write_dummy_file(Data, Size);
    
    tjhandle handle = tjInitDecompress();
    if (!handle) {
        cleanup_file();
        return 0;
    }
    
    int width = 0, height = 0, jpegSubsamp = 0;
    int ret = tjDecompressHeader2(handle, (unsigned char *)Data, Size, &width, &height, &jpegSubsamp);
    
    if (ret == 0 && width > 0 && height > 0 && width <= MAX_DIM && height <= MAX_DIM) {
        unsigned long yuvBufSize = tjBufSizeYUV(width, height, jpegSubsamp);
        unsigned long rgbBufSize = width * height * tjPixelSize[TJPF_RGB];
        
        if (yuvBufSize > 0 && yuvBufSize <= 100 * 1024 * 1024) {
            unsigned char *yuvBuf = (unsigned char *)malloc(yuvBufSize);
            if (yuvBuf) {
                tjDecompressToYUV(handle, (unsigned char *)Data, Size, yuvBuf, 0);
                free(yuvBuf);
            }
        }
        
        if (rgbBufSize > 0 && rgbBufSize <= 100 * 1024 * 1024) {
            unsigned char *rgbBuf = (unsigned char *)malloc(rgbBufSize);
            if (rgbBuf) {
                tjDecompress2(handle, (unsigned char *)Data, Size, rgbBuf, width, 0, height, TJPF_RGB, 0);
                free(rgbBuf);
            }
        }
        
        unsigned long yuv2BufSize = tjBufSizeYUV2(width, 4, height, jpegSubsamp);
        if (yuv2BufSize > 0 && yuv2BufSize <= 100 * 1024 * 1024) {
            unsigned char *yuv2Buf = (unsigned char *)malloc(yuv2BufSize);
            if (yuv2Buf) {
                tjDecompressToYUV2(handle, (unsigned char *)Data, Size, yuv2Buf, width, 4, height, 0);
                free(yuv2Buf);
            }
        }
        
        unsigned char *planes[MAX_PLANES] = {0};
        int strides[MAX_PLANES] = {0};
        int planeSizes[MAX_PLANES] = {0};
        
        for (int i = 0; i < MAX_PLANES; i++) {
            planeSizes[i] = tjPlaneSizeYUV(i, width, 0, height, jpegSubsamp);
            if (planeSizes[i] > 0 && planeSizes[i] <= 50 * 1024 * 1024) {
                planes[i] = (unsigned char *)malloc(planeSizes[i]);
                strides[i] = tjPlaneWidth(i, width, jpegSubsamp);
            }
        }
        
        if (planes[0] && planes[1] && planes[2]) {
            tjDecompressToYUVPlanes(handle, (unsigned char *)Data, Size, planes, width, strides, height, 0);
        }
        
        for (int i = 0; i < MAX_PLANES; i++) {
            if (planes[i]) free(planes[i]);
        }
        
        tjhandle handle3 = tj3Init(TJINIT_DECOMPRESS);
        if (handle3) {
            unsigned char *planes3[MAX_PLANES] = {0};
            int strides3[MAX_PLANES] = {0};
            
            for (int i = 0; i < MAX_PLANES; i++) {
                if (planeSizes[i] > 0 && planeSizes[i] <= 50 * 1024 * 1024) {
                    planes3[i] = (unsigned char *)malloc(planeSizes[i]);
                }
            }
            
            if (planes3[0] && planes3[1] && planes3[2]) {
                tj3DecompressToYUVPlanes8(handle3, Data, Size, planes3, strides3);
            }
            
            for (int i = 0; i < MAX_PLANES; i++) {
                if (planes3[i]) free(planes3[i]);
            }
            tj3Destroy(handle3);
        }
    }
    
    tjDestroy(handle);
    cleanup_file();
    return 0;
}