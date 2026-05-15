// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
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
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
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
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    int width, height, pixelFormat;
    int align = 1;
    int flags = 0;
    
    /* Test tjLoadImage */
    width = height = 0;
    pixelFormat = TJPF_UNKNOWN;
    unsigned char *img = tjLoadImage("./dummy_file", &width, align, &height, &pixelFormat, flags);
    if (img) free(img);
    
    /* Test tj3LoadImage16 */
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (handle) {
        width = height = 0;
        pixelFormat = TJPF_UNKNOWN;
        unsigned short *img16 = tj3LoadImage16(handle, "./dummy_file", &width, align, &height, &pixelFormat);
        if (img16) tj3Free(img16);
        tj3Destroy(handle);
    }
    
    /* Test tjSaveImage */
    width = height = 16;
    pixelFormat = TJPF_RGB;
    int pitch = width * tjPixelSize[pixelFormat];
    unsigned char *buffer = (unsigned char *)malloc(pitch * height);
    if (buffer) {
        memset(buffer, 0xAA, pitch * height);
        tjSaveImage("./dummy_file", buffer, width, pitch, height, pixelFormat, flags);
        free(buffer);
    }
    
    /* Test tj3Compress16 */
    handle = tj3Init(TJINIT_COMPRESS);
    if (handle) {
        width = height = 16;
        pixelFormat = TJPF_RGB;
        pitch = width * tjPixelSize[pixelFormat];
        unsigned short *src16 = (unsigned short *)malloc(pitch * height * sizeof(unsigned short));
        if (src16) {
            memset(src16, 0xBB, pitch * height * sizeof(unsigned short));
            unsigned char *jpegBuf = NULL;
            size_t jpegSize = 0;
            tj3Compress16(handle, src16, width, pitch, height, pixelFormat, &jpegBuf, &jpegSize);
            if (jpegBuf) tj3Free(jpegBuf);
            free(src16);
        }
        tj3Destroy(handle);
    }
    
    /* Test tjDecodeYUVPlanes */
    handle = tj3Init(TJINIT_DECOMPRESS);
    if (handle) {
        width = height = 16;
        pixelFormat = TJPF_RGB;
        pitch = width * tjPixelSize[pixelFormat];
        unsigned char *dstBuf = (unsigned char *)malloc(pitch * height);
        if (dstBuf) {
            const unsigned char *srcPlanes[3];
            int strides[3] = {16, 8, 8};
            unsigned char yPlane[256], uPlane[64], vPlane[64];
            memset(yPlane, 0x80, 256);
            memset(uPlane, 0x80, 64);
            memset(vPlane, 0x80, 64);
            srcPlanes[0] = yPlane;
            srcPlanes[1] = uPlane;
            srcPlanes[2] = vPlane;
            tjDecodeYUVPlanes(handle, srcPlanes, strides, TJSAMP_420, dstBuf, width, pitch, height, pixelFormat, flags);
            free(dstBuf);
        }
        tj3Destroy(handle);
    }
    
    /* Test tj3Get */
    handle = tj3Init(TJINIT_COMPRESS);
    if (handle) {
        int param = TJPARAM_QUALITY;
        int value = tj3Get(handle, param);
        (void)value;
        tj3Destroy(handle);
    }
    
    cleanup_dummy_file();
    return 0;
}