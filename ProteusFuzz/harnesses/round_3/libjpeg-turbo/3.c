// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3YUVBufSize at turbojpeg.c:971:18 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3DecodeYUV8 at turbojpeg.c:2675:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize handle for decompression
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;

    // 1. tj3SetScalingFactor
    tjscalingfactor sf = {0, 1}; // Default to 1/1 (no scaling)
    // Fuzz scaling factor numerator/denominator within safe bounds
    if (Size >= 2) {
        sf.num = Data[0] % 8 + 1; // 1 to 8
        sf.denom = Data[1] % 8 + 1; // 1 to 8
    }
    tj3SetScalingFactor(handle, sf);

    // 2. tj3Alloc (for JPEG buffer)
    size_t jpegSize = (Size > 0) ? Size : 1;
    unsigned char *jpegBuf = tj3Alloc(jpegSize);
    if (!jpegBuf) {
        tj3Destroy(handle);
        return 0;
    }
    memcpy(jpegBuf, Data, jpegSize);

    // 3. tj3Alloc (for YUV buffer - placeholder, size determined later)
    // We need width/height to calculate YUV size. Let's extract from data or use defaults.
    int width = 64;
    int height = 64;
    int align = 1;
    int subsamp = TJSAMP_444; // Default subsampling

    // Ensure we have enough data before accessing indices 2-7
    if (Size >= 8) {
        width = (Data[2] * 256 + Data[3]) % 4096 + 1;
        height = (Data[4] * 256 + Data[5]) % 4096 + 1;
        align = 1 << (Data[6] % 5); // 1, 2, 4, 8, 16
        subsamp = Data[7] % 4; // TJSAMP_444, 422, 420, 411
    } else if (Size >= 4) {
        // Fallback if we have partial data
        width = (Data[2] * 256 + Data[3]) % 4096 + 1;
    }

    // Calculate YUV buffer size
    size_t yuvSize = tj3YUVBufSize(width, align, height, subsamp);
    if (yuvSize == 0) {
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }

    unsigned char *yuvBuf = tj3Alloc(yuvSize);
    if (!yuvBuf) {
        tj3Free(jpegBuf);
        tj3Destroy(handle);
        return 0;
    }

    // 4. tj3DecompressToYUV8
    // Note: This might fail if jpegBuf is not a valid JPEG, which is expected in fuzzing.
    tj3DecompressToYUV8(handle, jpegBuf, jpegSize, yuvBuf, align);

    // 5. tj3DecodeYUV8
    // Allocate destination buffer for RGB/Grayscale
    int pixelFormat = (Size > 8) ? (Data[8] % 4) : TJPF_RGB; // TJPF_RGB, TJPF_BGR, TJPF_GRAY, TJPF_YUY2
    int pitch = width * tjPixelSize[pixelFormat];
    size_t dstSize = (size_t)width * pitch * height;
    
    unsigned char *dstBuf = tj3Alloc(dstSize);
    if (!dstBuf) {
        tj3Free(jpegBuf);
        tj3Free(yuvBuf);
        tj3Destroy(handle);
        return 0;
    }

    // Decode YUV to RGB
    tj3DecodeYUV8(handle, yuvBuf, align, dstBuf, width, pitch, height, pixelFormat);

    // Cleanup
    tj3Free(dstBuf);
    tj3Free(yuvBuf);
    tj3Free(jpegBuf);
    tj3Destroy(handle);

    return 0;
}