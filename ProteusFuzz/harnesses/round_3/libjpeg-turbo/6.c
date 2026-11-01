// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static int is_power_of_2(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Extract parameters from input data
    int width = (Data[0] & 0x7F) + 1;
    int height = (Data[1] & 0x7F) + 1;
    int align = (Data[2] & 0x03) + 1; // 1, 2, 4, 8
    if (!is_power_of_2(align)) align = 4;
    
    int subsamp = (Data[3] & 0x07); // 0-7
    if (subsamp > 4) subsamp = 2; // Clamp to safe range
    
    int pixelFormat = (Data[3] >> 3) & 0x07;
    if (pixelFormat > 6) pixelFormat = 0;

    // Limit dimensions to prevent OOM
    if (width > 2048) width = 2048;
    if (height > 2048) height = 2048;

    // Calculate buffer sizes
    size_t yuvSize = tj3YUVBufSize(width, align, height, subsamp);
    if (yuvSize == 0) return 0;

    // Allocate buffers
    unsigned char *yuvBuf = (unsigned char *)malloc(yuvSize);
    if (!yuvBuf) return 0;
    
    size_t rgbSize = (size_t)width * height * 3; // Assume RGB
    unsigned char *rgbBuf = (unsigned char *)malloc(rgbSize);
    if (!rgbBuf) { free(yuvBuf); return 0; }

    // Initialize with random data from input
    memset(yuvBuf, 0, yuvSize);
    memset(rgbBuf, 0, rgbSize);
    if (Size > 4) {
        size_t copyLen = Size - 4;
        if (copyLen > yuvSize) copyLen = yuvSize;
        memcpy(yuvBuf, Data + 4, copyLen);
        if (copyLen > rgbSize) copyLen = rgbSize;
        memcpy(rgbBuf, Data + 4, copyLen);
    }

    // Initialize TurboJPEG handles
    tjhandle compHandle = tj3Init(TJINIT_COMPRESS);
    tjhandle decompHandle = tj3Init(TJINIT_DECOMPRESS);

    if (!compHandle || !decompHandle) {
        free(yuvBuf);
        free(rgbBuf);
        return 0;
    }

    // Set subsampling for compression handle
    // Note: tj3SetSubsamp is not in the provided list, but required for YUV ops.
    // We assume standard usage where subsamp is set. 
    // Since we don't have tj3SetSubsamp in the provided list, we rely on defaults or 
    // the fact that some functions might fail gracefully if not set.
    // However, the prompt says "Use after initializing a handle and setting subsampling".
    // Without tj3SetSubsamp in the API list, we cannot explicitly set it.
    // We will proceed and let the functions handle the state (likely error if not set).
    // In a real scenario, we would call tj3SetSubsamp(compHandle, subsamp);

    // 1. tj3EncodeYUV8: RGB -> YUV (Compression path)
    int pitch = width * 3;
    tj3EncodeYUV8(compHandle, rgbBuf, width, pitch, height, pixelFormat, yuvBuf, align);

    // 2. tj3DecodeYUV8: YUV -> RGB (Decompression path)
    unsigned char *rgbBuf2 = (unsigned char *)malloc(rgbSize);
    if (rgbBuf2) {
        tj3DecodeYUV8(decompHandle, yuvBuf, align, rgbBuf2, width, pitch, height, pixelFormat);
        free(rgbBuf2);
    }

    // 3. tj3CompressFromYUV8: YUV -> JPEG
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    tj3CompressFromYUV8(compHandle, yuvBuf, width, align, height, &jpegBuf, &jpegSize);
    if (jpegBuf) {
        // 4. tj3DecompressToYUV8: JPEG -> YUV
        unsigned char *yuvBuf2 = (unsigned char *)malloc(yuvSize);
        if (yuvBuf2) {
            tj3DecompressToYUV8(decompHandle, jpegBuf, jpegSize, yuvBuf2, align);
            free(yuvBuf2);
        }
        tj3Free(jpegBuf);
    }

    // Cleanup
    tj3Destroy(compHandle);
    tj3Destroy(decompHandle);
    free(yuvBuf);
    free(rgbBuf);

    return 0;
}