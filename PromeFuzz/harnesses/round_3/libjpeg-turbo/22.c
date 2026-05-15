// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tj3DecompressToYUVPlanes8 at turbojpeg.c:2122:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    tjhandle handle = tjInitDecompress();
    tjhandle handle3 = tj3Init(TJINIT_DECOMPRESS);
    if (!handle || !handle3) {
        if (handle) tjDestroy(handle);
        if (handle3) tj3Destroy(handle3);
        return 0;
    }
    
    int width, height, jpegSubsamp;
    unsigned char *jpegBuf = (unsigned char *)Data;
    unsigned long jpegSize = Size;
    
    tjDecompressHeader2(handle, jpegBuf, jpegSize, &width, &height, &jpegSubsamp);
    
    if (width > 0 && height > 0) {
        width = width > 4096 ? 4096 : width;
        height = height > 4096 ? 4096 : height;
        
        unsigned long yuvBufSize = tjBufSizeYUV(width, height, jpegSubsamp);
        unsigned char *yuvBuf = NULL;
        unsigned char *rgbBuf = NULL;
        unsigned char *planes[3] = {NULL, NULL, NULL};
        int strides[3] = {0, 0, 0};
        
        if (yuvBufSize > 0 && yuvBufSize < 100 * 1024 * 1024) {
            yuvBuf = (unsigned char *)malloc(yuvBufSize);
            if (yuvBuf) {
                tjDecompressToYUV(handle, jpegBuf, jpegSize, yuvBuf, 0);
                tjDecompressToYUV2(handle, jpegBuf, jpegSize, yuvBuf, width, 1, height, 0);
                free(yuvBuf);
            }
        }
        
        int rgbSize = width * height * 3;
        if (rgbSize > 0 && rgbSize < 100 * 1024 * 1024) {
            rgbBuf = (unsigned char *)malloc(rgbSize);
            if (rgbBuf) {
                tjDecompress2(handle, jpegBuf, jpegSize, rgbBuf, width, 0, height, TJPF_RGB, 0);
                free(rgbBuf);
            }
        }
        
        int ySize = width * height;
        int uvWidth = (width + 1) / 2;
        int uvHeight = (height + 1) / 2;
        int uSize = uvWidth * uvHeight;
        int vSize = uvWidth * uvHeight;
        
        if (ySize > 0 && ySize < 50 * 1024 * 1024 &&
            uSize > 0 && uSize < 50 * 1024 * 1024 &&
            vSize > 0 && vSize < 50 * 1024 * 1024) {
            
            planes[0] = (unsigned char *)malloc(ySize);
            planes[1] = (unsigned char *)malloc(uSize);
            planes[2] = (unsigned char *)malloc(vSize);
            
            if (planes[0] && planes[1] && planes[2]) {
                tjDecompressToYUVPlanes(handle, jpegBuf, jpegSize, planes, width, strides, height, 0);
                tj3DecompressToYUVPlanes8(handle3, jpegBuf, jpegSize, planes, strides);
            }
            
            free(planes[0]);
            free(planes[1]);
            free(planes[2]);
        }
    }
    
    tjDestroy(handle);
    tj3Destroy(handle3);
    return 0;
}