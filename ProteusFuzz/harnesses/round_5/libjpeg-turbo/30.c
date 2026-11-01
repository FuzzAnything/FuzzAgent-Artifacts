// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tjGetErrorCode at turbojpeg.c:652:15 in turbojpeg.h
// tjSaveImage at turbojpeg.c:3125:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void cleanup_handle(tjhandle handle) {
    if (handle != NULL) {
        tjDestroy(handle);
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Test global error string retrieval (tjGetErrorStr)
    char *errStr = tjGetErrorStr();
    if (errStr != NULL) {
        (void)errStr[0];
    }

    // Create a handle to test handle-specific functions
    tjhandle handle = tjInitCompress();
    if (handle == NULL) {
        char *e = tjGetErrorStr();
        if (e != NULL) (void)e[0];
        return 0;
    }

    // Test tj3GetErrorStr with valid handle
    char *errStr3 = tj3GetErrorStr(handle);
    if (errStr3 != NULL) (void)errStr3[0];

    // Test tj3GetErrorStr with NULL (global)
    char *errStr3Null = tj3GetErrorStr(NULL);
    if (errStr3Null != NULL) (void)errStr3Null[0];

    // Test tjGetErrorStr2 with valid handle
    char *errStr2 = tjGetErrorStr2(handle);
    if (errStr2 != NULL) (void)errStr2[0];

    // Test tjGetErrorCode with valid handle
    int errorCode = tjGetErrorCode(handle);
    (void)errorCode;

    // Prepare data for tjSaveImage and tj3SaveImage16
    int width = 1, height = 1;
    int pixelFormat = TJPF_RGB;
    int flags = 0;

    if (Size >= 4) {
        width = Data[0];
        height = Data[1];
        pixelFormat = Data[2] % 6; // TJPF values are typically 0-5
        flags = Data[3];
    }

    // Clamp dimensions to avoid huge allocations or invalid states
    if (width <= 0 || width > 1024) width = 1;
    if (height <= 0 || height > 1024) height = 1;

    // Test tjSaveImage
    int pixelSize = tjPixelSize[pixelFormat];
    size_t bufSize8 = (size_t)width * height * pixelSize;
    unsigned char *buf8 = NULL;
    if (bufSize8 > 0 && bufSize8 < 1024 * 1024) {
        buf8 = (unsigned char *)malloc(bufSize8);
        if (buf8) {
            memset(buf8, 0, bufSize8);
            size_t copySize = Size;
            if (copySize > bufSize8) copySize = bufSize8;
            memcpy(buf8, Data, copySize);
            
            int ret = tjSaveImage("./dummy_file", buf8, width, 0, height, pixelFormat, flags);
            (void)ret;
            
            char *e = tjGetErrorStr();
            if (e != NULL) (void)e[0];
            
            free(buf8);
        }
    }

    // Test tj3SaveImage16
    int samplesPerPixel = 1;
    if (pixelFormat == TJPF_RGB || pixelFormat == TJPF_CMYK) samplesPerPixel = 3;
    else if (pixelFormat == TJPF_GRAY) samplesPerPixel = 1;
    // Removed TJPF_YUVPLANAR check as it is not a standard packed pixel format constant in this context
    
    size_t bufSize16 = (size_t)width * height * samplesPerPixel;
    unsigned short *buf16 = NULL;
    if (bufSize16 > 0 && bufSize16 < 1024 * 1024) {
        buf16 = (unsigned short *)malloc(bufSize16 * sizeof(unsigned short));
        if (buf16) {
            memset(buf16, 0, bufSize16 * sizeof(unsigned short));
            size_t dataBytes = Size;
            if (dataBytes > bufSize16 * sizeof(unsigned short)) dataBytes = bufSize16 * sizeof(unsigned short);
            memcpy(buf16, Data, dataBytes);
            
            int ret16 = tj3SaveImage16(handle, "./dummy_file", buf16, width, 0, height, pixelFormat);
            (void)ret16;
            
            char *e16 = tj3GetErrorStr(handle);
            if (e16 != NULL) (void)e16[0];
            
            free(buf16);
        }
    }

    cleanup_handle(handle);

    return 0;
}