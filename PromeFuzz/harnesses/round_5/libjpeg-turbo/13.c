// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3YUVPlaneSize at turbojpeg.c:1020:18 in turbojpeg.h
// tj3DecodeYUVPlanes8 at turbojpeg.c:2508:15 in turbojpeg.h
// tjDecompressToYUVPlanes at turbojpeg.c:2288:15 in turbojpeg.h
// tj3DecompressToYUVPlanes8 at turbojpeg.c:2122:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static tjhandle create_decompress_handle() {
    return tjInitDecompress();
}

static tjhandle create_decompress_handle3() {
    return tj3Init(TJINIT_DECOMPRESS);
}

static void cleanup_handles(tjhandle handle, tjhandle handle3) {
    if (handle) tjDestroy(handle);
    if (handle3) tj3Destroy(handle3);
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    static const char dummy_file[] = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    tjhandle handle = create_decompress_handle();
    tjhandle handle3 = create_decompress_handle3();
    
    if (!handle || !handle3) {
        cleanup_handles(handle, handle3);
        return 0;
    }
    
    int width = 16;
    int height = 16;
    int pitch = width * 3;
    int align = 4;
    int subsamp = TJSAMP_420;
    int pixelFormat = TJPF_RGB;
    int flags = 0;
    
    unsigned char *dstBuf = (unsigned char *)malloc(pitch * height);
    if (!dstBuf) {
        cleanup_handles(handle, handle3);
        return 0;
    }
    
    unsigned long jpegSize = Size > 1024 ? 1024 : Size;
    const unsigned char *jpegBuf = Data;
    
    unsigned char *yuvBuf = (unsigned char *)malloc(tjBufSizeYUV2(width, align, height, subsamp));
    if (yuvBuf) {
        memcpy(yuvBuf, Data, Size > tjBufSizeYUV2(width, align, height, subsamp) ? 
               tjBufSizeYUV2(width, align, height, subsamp) : Size);
        
        tjDecodeYUV(handle, yuvBuf, align, subsamp, dstBuf, width, pitch, height, pixelFormat, flags);
        
        tj3DecodeYUV8(handle3, yuvBuf, align, dstBuf, width, pitch, height, pixelFormat);
        
        free(yuvBuf);
    }
    
    unsigned char *planes[3];
    int strides[3];
    
    for (int i = 0; i < 3; i++) {
        size_t planeSize = tj3YUVPlaneSize(i, width, 0, height, subsamp);
        planes[i] = (unsigned char *)malloc(planeSize);
        if (planes[i]) {
            memset(planes[i], 0, planeSize);
            strides[i] = 0;
        }
    }
    
    if (planes[0] && planes[1] && planes[2]) {
        const unsigned char *srcPlanes[3] = {planes[0], planes[1], planes[2]};
        
        tj3DecodeYUVPlanes8(handle3, srcPlanes, strides, dstBuf, width, pitch, height, pixelFormat);
        
        tjDecompressToYUVPlanes(handle, jpegBuf, jpegSize, planes, width, strides, height, flags);
        
        tj3DecompressToYUVPlanes8(handle3, jpegBuf, jpegSize, planes, strides);
    }
    
    for (int i = 0; i < 3; i++) {
        free(planes[i]);
    }
    
    free(dstBuf);
    cleanup_handles(handle, handle3);
    
    return 0;
}