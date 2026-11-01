// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tjDecodeYUVPlanes at turbojpeg.c:2649:15 in turbojpeg.h
// tjDecodeYUV at turbojpeg.c:2721:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void write_dummy_jpeg(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

// Helper to calculate required buffer size for YUV planes given alignment
static size_t calculate_yuv_size(int width, int height, int align, int subsamp) {
    int pw0 = width;
    int ph0 = height;
    
    // Calculate strides with alignment padding
    int stride0 = (pw0 + align - 1) & ~(align - 1);
    
    size_t size = (size_t)stride0 * ph0; // Y plane size with padding
    
    if (subsamp == TJSAMP_GRAY) {
        return size;
    }

    int pw1, ph1;
    switch (subsamp) {
        case TJSAMP_420:
        case TJSAMP_440:
            pw1 = (width + 1) / 2;
            ph1 = (height + 1) / 2;
            break;
        case TJSAMP_422:
            pw1 = (width + 1) / 2;
            ph1 = height;
            break;
        case TJSAMP_444:
            pw1 = width;
            ph1 = height;
            break;
        default:
            pw1 = (width + 1) / 2;
            ph1 = (height + 1) / 2;
            break;
    }

    int stride1 = (pw1 + align - 1) & ~(align - 1);
    size += (size_t)stride1 * ph1; // U plane
    size += (size_t)stride1 * ph1; // V plane (assuming same stride for U and V in 4:2:0/4:2:2/4:4:4)

    return size;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;

    write_dummy_jpeg(Data, Size);

    int width = (Data[0] % 256) + 1;
    int height = (Data[1] % 256) + 1;
    
    if (width > 2048) width = 2048;
    if (height > 2048) height = 2048;

    int align = 1;
    if (Size > 1) {
        int val = Data[0] % 16;
        if (val < 2) align = 1;
        else if (val < 4) align = 2;
        else if (val < 8) align = 4;
        else if (val < 12) align = 8;
        else align = 16;
    }

    int pixelFormat = TJPF_RGB;
    if (Size > 2) {
        int fmt = Data[2] % 6;
        switch (fmt) {
            case 0: pixelFormat = TJPF_RGB; break;
            case 1: pixelFormat = TJPF_BGR; break;
            case 2: pixelFormat = TJPF_GRAY; break;
            case 3: pixelFormat = TJPF_CMYK; break;
            case 4: pixelFormat = TJPF_XRGB; break;
            case 5: pixelFormat = TJPF_XBGR; break;
        }
    }

    int subsamp = TJSAMP_420;
    if (Size > 3) {
        int s = Data[3] % 5;
        switch (s) {
            case 0: subsamp = TJSAMP_444; break;
            case 1: subsamp = TJSAMP_422; break;
            case 2: subsamp = TJSAMP_420; break;
            case 3: subsamp = TJSAMP_GRAY; break;
            case 4: subsamp = TJSAMP_440; break;
        }
    }

    // Calculate buffer sizes dynamically based on alignment and subsampling
    size_t yuv_size = calculate_yuv_size(width, height, align, subsamp);
    size_t rgb_size = (size_t)width * height * 4 + 1024;

    unsigned char *yuvBuf = (unsigned char *)malloc(yuv_size);
    unsigned char *rgbBuf = (unsigned char *)malloc(rgb_size);
    
    if (!yuvBuf || !rgbBuf) {
        free(yuvBuf);
        free(rgbBuf);
        tjDestroy(handle);
        return 0;
    }

    memset(yuvBuf, 0, yuv_size);
    memset(rgbBuf, 0, rgb_size);

    // Fuzz tj3DecompressToYUV8
    tj3DecompressToYUV8(handle, Data, Size, yuvBuf, align);

    // Fuzz tj3DecodeYUV8
    int pitch = width * 4; 
    tj3DecodeYUV8(handle, yuvBuf, align, rgbBuf, width, pitch, height, pixelFormat);

    // Fuzz tjDecodeYUVPlanes
    const unsigned char *srcPlanes[3];
    int strides[3];
    
    // Recalculate strides and pointers exactly as the library does internally to ensure safety
    int pw0 = width;
    int ph0 = height;
    int stride0 = (pw0 + align - 1) & ~(align - 1);
    
    srcPlanes[0] = yuvBuf;
    strides[0] = stride0;

    if (subsamp != TJSAMP_GRAY) {
        int pw1, ph1;
        switch (subsamp) {
            case TJSAMP_420:
            case TJSAMP_440:
                pw1 = (width + 1) / 2;
                ph1 = (height + 1) / 2;
                break;
            case TJSAMP_422:
                pw1 = (width + 1) / 2;
                ph1 = height;
                break;
            case TJSAMP_444:
                pw1 = width;
                ph1 = height;
                break;
            default:
                pw1 = (width + 1) / 2;
                ph1 = (height + 1) / 2;
                break;
        }
        int stride1 = (pw1 + align - 1) & ~(align - 1);
        strides[1] = stride1;
        strides[2] = stride1;
        
        srcPlanes[1] = yuvBuf + (size_t)stride0 * ph0;
        srcPlanes[2] = srcPlanes[1] + (size_t)stride1 * ph1;
    } else {
        srcPlanes[1] = NULL;
        srcPlanes[2] = NULL;
        strides[1] = 0;
        strides[2] = 0;
    }

    tjDecodeYUVPlanes(handle, srcPlanes, strides, subsamp, rgbBuf, width, pitch, height, pixelFormat, 0);

    // Fuzz tjDecodeYUV
    tjDecodeYUV(handle, yuvBuf, align, subsamp, rgbBuf, width, pitch, height, pixelFormat, 0);

    free(yuvBuf);
    free(rgbBuf);
    tjDestroy(handle);

    return 0;
}