// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static int fuzz_decompress_to_yuv8(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (handle == NULL) return 0;

    // Try to decompress the input data as a JPEG
    // We need a destination buffer. Since we don't know the size yet,
    // we can try to decompress header first to get dimensions, but for fuzzing
    // simplicity, we can try a reasonable max size or let the API fail gracefully.
    // However, tj3DecompressToYUV8 requires a valid dstBuf.
    // Let's try to get scaling factors and use TJSCALED if possible, but we need header info.
    // For fuzzing, we can try a fixed large buffer or try to read header first.
    
    // Attempt to read header to get dimensions for buffer allocation
    // Note: tj3DecompressHeader is not in the provided list, but it's needed for robustness.
    // Since it's not provided, we will attempt to allocate a large buffer or skip header reading.
    // The API might fail if buffer is too small, which is fine for fuzzing.
    
    // Let's try a dynamic approach: 
    // 1. Try to decompress header (if we had the function, but we don't).
    // 2. Fallback: Allocate a large buffer (e.g., 10MB) and hope it's enough or API handles it.
    //    But the API requires the exact size calculated by tj3YUVBufSize.
    //    Without header, we can't calculate exact size.
    
    // Alternative: The fuzzing input might be a valid JPEG. If not, the function will return -1.
    // We can try to allocate a buffer of Size * 4 (worst case for YUV444) or a fixed max.
    // Let's use a fixed max buffer size for safety in fuzzing.
    size_t maxBufSize = 10 * 1024 * 1024; // 10MB
    unsigned char *dstBuf = NULL;
    
    if (Size < 10) { // Too small to be a valid JPEG
        tj3Destroy(handle);
        return 0;
    }

    dstBuf = (unsigned char *)malloc(maxBufSize);
    if (dstBuf == NULL) {
        tj3Destroy(handle);
        return 0;
    }

    // Try different alignment values (powers of 2)
    int aligns[] = {1, 2, 4, 8, 16, 32, 64, 128};
    for (int i = 0; i < 8; i++) {
        int ret = tj3DecompressToYUV8(handle, Data, Size, dstBuf, aligns[i]);
        // Ignore return value for fuzzing, just want to trigger code paths
        (void)ret;
    }

    free(dstBuf);
    tj3Destroy(handle);
    return 0;
}

static int fuzz_scaling_and_cropping(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (handle == NULL) return 0;

    // Get scaling factors
    int numFactors = 0;
    tjscalingfactor *factors = tj3GetScalingFactors(&numFactors);
    if (factors == NULL) {
        tj3Destroy(handle);
        return 0;
    }

    // Try to set scaling factors
    for (int i = 0; i < numFactors; i++) {
        tj3SetScalingFactor(handle, factors[i]);
    }

    // Try to set cropping region
    // We need a valid region. Since we don't have header info, we try some dummy values.
    // The API will validate and return error if invalid, which is good for fuzzing.
    tjregion regions[] = {
        {0, 0, 0, 0}, // TJUNCROPPED
        {0, 0, 10, 10},
        {10, 10, 100, 100},
        {-1, 0, 10, 10}, // Invalid
        {0, -1, 10, 10}, // Invalid
    };
    int numRegions = sizeof(regions) / sizeof(regions[0]);

    for (int i = 0; i < numRegions; i++) {
        tj3SetCroppingRegion(handle, regions[i]);
    }

    tj3Destroy(handle);
    return 0;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz decompression to YUV
    fuzz_decompress_to_yuv8(Data, Size);

    // Fuzz scaling and cropping
    fuzz_scaling_and_cropping(Data, Size);

    return 0;
}