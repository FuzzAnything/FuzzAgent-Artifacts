// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tjEncodeYUV3 at turbojpeg.c:1734:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int is_power_of_two(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 5) return 0;

    int width = (Data[0] % 1024) + 1;
    int height = (Data[1] % 1024) + 1;
    int align = (Data[2] & 0x0F) + 1;
    if (!is_power_of_two(align)) align = 1;
    
    int subsamp = Data[3] % 4; // TJSAMP_444 to TJSAMP_GRAY
    int pixelFormat = (Data[4] % 6) + 1; // TJPF_GRAY to TJPF_CMYK
    
    // Ensure dimensions are reasonable for testing
    if (width > 2048) width = 2048;
    if (height > 2048) height = 2048;

    tjhandle handle = tj3Init(TJINIT_DECOMPRESS | TJINIT_COMPRESS);
    if (!handle) return 0;

    // Calculate buffer sizes
    size_t yuvSize = tj3YUVBufSize(width, align, height, subsamp);
    if (yuvSize == 0) {
        tj3Destroy(handle);
        return 0;
    }

    size_t rgbSize = width * height * tjPixelSize[pixelFormat];
    if (rgbSize == 0) {
        tj3Destroy(handle);
        return 0;
    }

    // Allocate buffers
    unsigned char *rgbBuf = (unsigned char *)malloc(rgbSize);
    unsigned char *yuvBuf = (unsigned char *)malloc(yuvSize);
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;

    if (!rgbBuf || !yuvBuf) {
        free(rgbBuf);
        free(yuvBuf);
        tj3Destroy(handle);
        return 0;
    }

    // Initialize buffers with fuzz data
    memset(rgbBuf, 0, rgbSize);
    memset(yuvBuf, 0, yuvSize);
    
    // Copy fuzz data into buffers safely
    size_t copyLen = (Size < rgbSize) ? Size : rgbSize;
    memcpy(rgbBuf, Data, copyLen);
    
    copyLen = (Size < yuvSize) ? Size : yuvSize;
    memcpy(yuvBuf, Data, copyLen);

    // 1. Test tj3EncodeYUV8: RGB -> YUV
    int pitch = width * tjPixelSize[pixelFormat];
    if (pitch <= 0) pitch = 1;
    tj3EncodeYUV8(handle, rgbBuf, width, pitch, height, pixelFormat, yuvBuf, align);

    // 2. Test tj3DecodeYUV8: YUV -> RGB
    tj3DecodeYUV8(handle, yuvBuf, align, rgbBuf, width, pitch, height, pixelFormat);

    // 3. Test tjEncodeYUV3: RGB -> YUV (legacy wrapper)
    tjEncodeYUV3(handle, rgbBuf, width, pitch, height, pixelFormat, yuvBuf, align, subsamp, 0);

    // 4. Test tj3DecompressToYUV8: JPEG -> YUV
    // We use the fuzz data as the JPEG buffer.
    tj3DecompressToYUV8(handle, Data, Size, yuvBuf, align);

    // 5. Test tj3CompressFromYUV8: YUV -> JPEG
    // Reset jpegBuf pointer for allocation
    jpegBuf = NULL;
    jpegSize = 0;
    tj3CompressFromYUV8(handle, yuvBuf, width, align, height, &jpegBuf, &jpegSize);
    if (jpegBuf) {
        free(jpegBuf);
        jpegBuf = NULL;
    }

    // 6. Test tj3YUVBufSize with various inputs
    int fuzzWidth = (Data[0] % 4096) + 1;
    int fuzzHeight = (Data[1] % 4096) + 1;
    int fuzzAlign = (Data[2] & 0x0F) + 1;
    if (!is_power_of_two(fuzzAlign)) fuzzAlign = 1;
    int fuzzSubsamp = Data[3] % 4;
    tj3YUVBufSize(fuzzWidth, fuzzAlign, fuzzHeight, fuzzSubsamp);

    // Cleanup
    free(rgbBuf);
    free(yuvBuf);
    tj3Destroy(handle);

    return 0;
}