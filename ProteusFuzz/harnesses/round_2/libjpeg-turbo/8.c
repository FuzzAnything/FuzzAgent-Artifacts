// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3CompressFromYUV8 at turbojpeg.c:1429:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int fuzz_compress16(const uint8_t *Data, size_t Size) {
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return 0;

    // Set parameters
    tj3Set(handle, TJPARAM_SUBSAMP, TJSAMP_444);
    tj3Set(handle, TJPARAM_PRECISION, 16);

    if (Size < 16) {
        tj3Destroy(handle);
        return 0;
    }

    // Parse dimensions and format from input
    int width = (Data[0] % 100) + 1;
    int height = (Data[1] % 100) + 1;
    int pixelFormat = Data[2] % 5; // TJPF_RGB, TJPF_GRAY, etc.
    int pitch = (Data[3] % 10) + 1; // Samples per row

    // Calculate required source buffer size
    // For 16-bit, samples are unsigned short (2 bytes)
    size_t srcSize = (size_t)width * height * 2;
    if (pitch > 0) {
        srcSize = (size_t)pitch * height * 2;
    }

    // Ensure we have enough data to simulate source buffer
    if (Size < srcSize + 4) {
        tj3Destroy(handle);
        return 0;
    }

    const unsigned short *srcBuf = (const unsigned short *)(Data + 4);
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;

    int result = tj3Compress16(handle, srcBuf, width, pitch, height, pixelFormat, &jpegBuf, &jpegSize);

    if (jpegBuf) {
        tj3Free(jpegBuf);
    }
    tj3Destroy(handle);
    return 0;
}

static int fuzz_compress_from_yuv8(const uint8_t *Data, size_t Size) {
    tjhandle handle = tj3Init(TJINIT_COMPRESS);
    if (!handle) return 0;

    // Set subsampling as it is required for this function
    tj3Set(handle, TJPARAM_SUBSAMP, TJSAMP_422);

    if (Size < 8) {
        tj3Destroy(handle);
        return 0;
    }

    int width = (Data[0] % 128) + 1;
    int height = (Data[1] % 128) + 1;
    int align = 1 << (Data[2] % 5); // 1, 2, 4, 8, 16

    // Calculate YUV buffer size
    size_t yuvSize = tj3YUVBufSize(width, align, height, TJSAMP_422);
    if (yuvSize == 0) {
        tj3Destroy(handle);
        return 0;
    }

    // Ensure we have enough data for YUV buffer
    if (Size < yuvSize + 4) {
        tj3Destroy(handle);
        return 0;
    }

    const unsigned char *srcBuf = (const unsigned char *)(Data + 4);
    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;

    int result = tj3CompressFromYUV8(handle, srcBuf, width, align, height, &jpegBuf, &jpegSize);

    if (jpegBuf) {
        tj3Free(jpegBuf);
    }
    tj3Destroy(handle);
    return 0;
}

static int fuzz_alloc_free(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to read sizeof(size_t)
    if (Size < sizeof(size_t)) return 0;
    
    size_t allocSize = 0;
    memcpy(&allocSize, Data, sizeof(size_t));
    
    // Cap allocation size to prevent OOM
    if (allocSize > 1024 * 1024) {
        allocSize = 1024 * 1024;
    }

    void *ptr = tj3Alloc(allocSize);
    if (ptr) {
        // Write fuzz data to allocated buffer
        size_t writeSize = (allocSize < Size) ? allocSize : Size;
        memcpy(ptr, Data, writeSize);
        tj3Free(ptr);
    }
    return 0;
}

static int fuzz_init_set(const uint8_t *Data, size_t Size) {
    if (Size < 3) return 0;

    int initType = Data[0] % 2; // 0 or 1 (TJINIT_DECOMPRESS or TJINIT_COMPRESS)
    tjhandle handle = tj3Init(initType);
    if (!handle) return 0;

    // Try setting various parameters
    int param = Data[1] % 10; // Arbitrary param selection
    int value = (Data[2] % 100) - 50; // Arbitrary value

    tj3Set(handle, param, value);
    
    tj3Destroy(handle);
    return 0;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Select fuzzing mode based on first byte
    int mode = Data[0] % 4;

    switch (mode) {
        case 0:
            fuzz_compress16(Data, Size);
            break;
        case 1:
            fuzz_compress_from_yuv8(Data, Size);
            break;
        case 2:
            fuzz_alloc_free(Data, Size);
            break;
        case 3:
            fuzz_init_set(Data, Size);
            break;
    }

    return 0;
}