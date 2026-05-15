// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3JPEGBufSize at turbojpeg.c:903:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3EncodeYUV8 at turbojpeg.c:1688:15 in turbojpeg.h
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
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;  // Need enough data for parameters
    
    // Initialize with fuzzed initType (clamped to valid range)
    int initType = Data[0] % 3;  // TJINIT_COMPRESS=0, TJINIT_DECOMPRESS=1, TJINIT_TRANSFORM=2
    tjhandle handle = tj3Init(initType);
    if (!handle) return 0;
    
    // Set parameters using fuzzed data
    // First 8 tj3Set calls
    for (int i = 0; i < 8; i++) {
        if (i * 3 + 2 >= Size) break;
        int param = Data[i * 3 + 1] % 20;  // TJPARAM values are 0-19
        int value = Data[i * 3 + 2];
        tj3Set(handle, param, value);
    }
    
    // JPEG buffer size calculation
    int width = (Data[25] << 8) | Data[26];
    int height = (Data[27] << 8) | Data[28];
    int jpegSubsamp = Data[29] % 6;  // TJSAMP values 0-5
    width = width % 1024 + 1;  // Limit to reasonable size
    height = height % 1024 + 1;
    
    size_t jpegBufSize = tj3JPEGBufSize(width, height, jpegSubsamp);
    if (jpegBufSize == 0) {
        tj3Destroy(handle);
        return 0;
    }
    
    // Allocate JPEG buffer
    unsigned char *jpegBuf = (unsigned char *)tj3Alloc(jpegBufSize);
    if (!jpegBuf) {
        tj3Destroy(handle);
        return 0;
    }
    
    // YUV buffer size calculation
    int align = 1 << (Data[30] % 4);  // Power of 2: 1, 2, 4, 8
    int subsamp = Data[31] % 6;  // TJSAMP values 0-5
    size_t yuvBufSize = tj3YUVBufSize(width, align, height, subsamp);
    if (yuvBufSize == 0) {
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    // Two more tj3Set calls
    if (Size >= 34) {
        tj3Set(handle, Data[32] % 20, Data[33]);
    }
    if (Size >= 36) {
        tj3Set(handle, Data[34] % 20, Data[35]);
    }
    
    // Allocate YUV buffer
    unsigned char *yuvBuf = (unsigned char *)malloc(yuvBufSize);
    if (!yuvBuf) {
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }
    
    // Fill YUV buffer with fuzzed data (or use dummy file)
    size_t copySize = Size - 36 > yuvBufSize ? yuvBufSize : Size - 36;
    if (copySize > 0 && Size > 36) {
        memcpy(yuvBuf, Data + 36, copySize);
    } else {
        // Use dummy file if not enough data
        write_dummy_file(Data, Size);
        FILE *f = fopen("./dummy_file", "rb");
        if (f) {
            fread(yuvBuf, 1, yuvBufSize, f);
            fclose(f);
        }
    }
    
    // Encode YUV
    int pitch = width * (Data[37] % 4 + 1);  // Pixel size 1-4
    int pixelFormat = Data[38] % 13;  // TJPF values 0-12
    tj3EncodeYUV8(handle, yuvBuf, width, pitch, height, pixelFormat, yuvBuf, align);
    
    // Compress from YUV
    size_t jpegSize = jpegBufSize;
    tj3CompressFromYUV8(handle, yuvBuf, width, align, height, &jpegBuf, &jpegSize);
    
    // Cleanup
    free(yuvBuf);
    tj3Free(jpegBuf);
    tj3Destroy(handle);
    
    return 0;
}