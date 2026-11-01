// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static int fuzz_tj3Get(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    int initType = Data[0];
    int param = Data[1];
    
    tjhandle handle = tj3Init(initType);
    if (!handle) return 0;
    
    int result = tj3Get(handle, param);
    
    tj3Destroy(handle);
    return result;
}

static int fuzz_tj3Set(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;
    
    int initType = Data[0];
    int param = Data[1];
    int value = Data[2];
    
    tjhandle handle = tj3Init(initType);
    if (!handle) return 0;
    
    int result = tj3Set(handle, param, value);
    
    tj3Destroy(handle);
    return result;
}

static int fuzz_tj3Decompress16(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    int initType = Data[0];
    int pixelFormat = Data[1];
    int width = Data[2] + 1;
    int height = Data[3] + 1;
    int pitch = Data[4] + 1;
    
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;
    
    // Use the input data as the JPEG buffer
    size_t jpegSize = Size - 5;
    const unsigned char *jpegBuf = Data + 5;
    
    // Allocate destination buffer
    size_t dstSize = width * height * 3 * sizeof(unsigned short);
    unsigned short *dstBuf = (unsigned short *)malloc(dstSize);
    if (!dstBuf) {
        tj3Destroy(handle);
        return 0;
    }
    
    int result = tj3Decompress16(handle, jpegBuf, jpegSize, dstBuf, pitch, pixelFormat);
    
    free(dstBuf);
    tj3Destroy(handle);
    return result;
}

static int fuzz_tj3LoadImage16(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;
    
    int initType = Data[0];
    int pixelFormat = Data[1];
    int align = Data[2];
    
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;
    
    int width = 0, height = 0;
    unsigned short *dstBuf = tj3LoadImage16(handle, "./dummy_file", &width, align, &height, &pixelFormat);
    
    if (dstBuf) {
        tj3Free(dstBuf);
    }
    
    tj3Destroy(handle);
    return 0;
}

static int fuzz_tj3Compress16(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    int initType = Data[0];
    int pixelFormat = Data[1];
    int width = Data[2] + 1;
    int height = Data[3] + 1;
    int pitch = Data[4] + 1;
    
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return 0;
    
    // Create source buffer from input data
    size_t srcSize = width * height * 3 * sizeof(unsigned short);
    unsigned short *srcBuf = (unsigned short *)malloc(srcSize);
    if (!srcBuf) {
        tj3Destroy(handle);
        return 0;
    }
    
    // Fill source buffer with input data
    size_t dataToCopy = (Size - 5) < srcSize ? (Size - 5) : srcSize;
    memcpy(srcBuf, Data + 5, dataToCopy);
    
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    
    int result = tj3Compress16(handle, srcBuf, width, pitch, height, pixelFormat, &jpegBuf, &jpegSize);
    
    if (jpegBuf) {
        tj3Free(jpegBuf);
    }
    
    free(srcBuf);
    tj3Destroy(handle);
    return result;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    int op = Data[0] % 6;
    
    switch (op) {
        case 0:
            return fuzz_tj3Get(Data, Size);
        case 1:
            return fuzz_tj3Set(Data, Size);
        case 2:
            return fuzz_tj3Decompress16(Data, Size);
        case 3:
            return fuzz_tj3LoadImage16(Data, Size);
        case 4:
            return fuzz_tj3Compress16(Data, Size);
        case 5:
            // Test tj3Init directly
            int initType = Data[0] % 2;
            tjhandle handle = tj3Init(initType);
            if (handle) {
                tj3Destroy(handle);
            }
            return 0;
    }
    
    return 0;
}