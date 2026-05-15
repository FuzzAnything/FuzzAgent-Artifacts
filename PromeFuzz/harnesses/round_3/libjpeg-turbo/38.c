// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjPlaneSizeYUV at turbojpeg.c:1048:25 in turbojpeg.h
// TJBUFSIZEYUV at turbojpeg.c:1013:25 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_tjPlaneSizeYUV(const uint8_t* data, size_t size) {
    if (size < 5) return;
    
    int componentID = data[0] % 3;
    int width = (data[1] * 256 + data[2]) % 1024 + 1;
    int stride = (data[3] * 256 + data[4]) % 1024 + 1;
    int height = (data[0] * 256 + data[1]) % 1024 + 1;
    int subsamp = data[2] % 5;
    
    tjPlaneSizeYUV(componentID, width, stride, height, subsamp);
}

static void fuzz_TJBUFSIZEYUV(const uint8_t* data, size_t size) {
    if (size < 3) return;
    
    int width = (data[0] * 256 + data[1]) % 2048 + 1;
    int height = (data[1] * 256 + data[2]) % 2048 + 1;
    int jpegSubsamp = data[0] % 5;
    
    TJBUFSIZEYUV(width, height, jpegSubsamp);
}

static void fuzz_tjBufSizeYUV(const uint8_t* data, size_t size) {
    if (size < 3) return;
    
    int width = (data[0] * 256 + data[1]) % 2048 + 1;
    int height = (data[1] * 256 + data[2]) % 2048 + 1;
    int subsamp = data[0] % 5;
    
    tjBufSizeYUV(width, height, subsamp);
}

static void fuzz_tjBufSizeYUV2(const uint8_t* data, size_t size) {
    if (size < 4) return;
    
    int width = (data[0] * 256 + data[1]) % 2048 + 1;
    int align = (data[2] % 16) + 1;
    int height = (data[1] * 256 + data[3]) % 2048 + 1;
    int subsamp = data[0] % 5;
    
    tjBufSizeYUV2(width, align, height, subsamp);
}

static void fuzz_tjDecompressHeader2(const uint8_t* data, size_t size) {
    FILE* file = fopen("./dummy_file", "wb");
    if (!file) return;
    
    fwrite(data, 1, size, file);
    fclose(file);
    
    file = fopen("./dummy_file", "rb");
    if (!file) return;
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (file_size > 0) {
        unsigned char* jpegBuf = (unsigned char*)malloc(file_size);
        if (jpegBuf) {
            fread(jpegBuf, 1, file_size, file);
            
            tjhandle handle = tjInitDecompress();
            if (handle) {
                int width, height, jpegSubsamp;
                tjDecompressHeader2(handle, jpegBuf, file_size, &width, &height, &jpegSubsamp);
                tjDestroy(handle);
            }
            free(jpegBuf);
        }
    }
    
    fclose(file);
}

static void fuzz_tjDecodeYUVPlanes(const uint8_t* data, size_t size) {
    if (size < 100) return;
    
    int width = (data[0] * 256 + data[1]) % 512 + 1;
    int height = (data[1] * 256 + data[2]) % 512 + 1;
    int subsamp = data[3] % 5;
    int pixelFormat = data[4] % 12;
    int flags = data[5] % 256;
    int pitch = width * 4;
    
    unsigned long yuvSize = tjBufSizeYUV(width, height, subsamp);
    if (yuvSize == (unsigned long)-1 || yuvSize > size - 6) return;
    
    const unsigned char* srcData = data + 6;
    
    const unsigned char* srcPlanes[3];
    int strides[3];
    
    int yWidth = width;
    int yHeight = height;
    int uvWidth = width;
    int uvHeight = height;
    
    switch (subsamp) {
        case TJSAMP_420:
            uvWidth = (width + 1) / 2;
            uvHeight = (height + 1) / 2;
            break;
        case TJSAMP_422:
            uvWidth = (width + 1) / 2;
            uvHeight = height;
            break;
        case TJSAMP_440:
            uvWidth = width;
            uvHeight = (height + 1) / 2;
            break;
        case TJSAMP_444:
            uvWidth = width;
            uvHeight = height;
            break;
        case TJSAMP_GRAY:
            uvWidth = 0;
            uvHeight = 0;
            break;
        default:
            return;
    }
    
    size_t yPlaneSize = yWidth * yHeight;
    size_t uvPlaneSize = uvWidth * uvHeight;
    
    if (6 + yPlaneSize + 2 * uvPlaneSize > size) return;
    
    srcPlanes[0] = srcData;
    strides[0] = yWidth;
    
    if (subsamp != TJSAMP_GRAY) {
        srcPlanes[1] = srcData + yPlaneSize;
        srcPlanes[2] = srcData + yPlaneSize + uvPlaneSize;
        strides[1] = uvWidth;
        strides[2] = uvWidth;
    } else {
        srcPlanes[1] = NULL;
        srcPlanes[2] = NULL;
        strides[1] = 0;
        strides[2] = 0;
    }
    
    unsigned char* dstBuf = (unsigned char*)malloc(width * height * 4);
    if (!dstBuf) return;
    
    tjhandle handle = tjInitDecompress();
    if (handle) {
        tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp, dstBuf, width, pitch, height, pixelFormat, flags);
        tjDestroy(handle);
    }
    
    free(dstBuf);
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0:
            fuzz_tjPlaneSizeYUV(Data, Size);
            break;
        case 1:
            fuzz_TJBUFSIZEYUV(Data, Size);
            break;
        case 2:
            fuzz_tjBufSizeYUV(Data, Size);
            break;
        case 3:
            fuzz_tjBufSizeYUV2(Data, Size);
            break;
        case 4:
            fuzz_tjDecompressHeader2(Data, Size);
            break;
        case 5:
            fuzz_tjDecodeYUVPlanes(Data, Size);
            break;
    }
    
    return 0;
}