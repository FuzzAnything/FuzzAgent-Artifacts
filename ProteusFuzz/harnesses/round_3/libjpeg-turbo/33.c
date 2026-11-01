// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tjGetScalingFactors at turbojpeg.c:1974:28 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    tjhandle handle = NULL;
    int numScalingFactors = 0;
    tjscalingfactor *scalingFactors = NULL;
    int ret = 0;

    // Test tj3Init with different init types
    // TJINIT_DECOMPRESS is needed for the target functions
    handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        return 0;
    }

    // Test tj3GetScalingFactors (v3)
    scalingFactors = tj3GetScalingFactors(&numScalingFactors);
    if (scalingFactors && numScalingFactors > 0) {
        // Try to set scaling factor with fuzzed index
        // Ensure we have enough data before accessing Data[0]
        if (Size >= 1) {
            int idx = (Data[0] % numScalingFactors);
            ret = tj3SetScalingFactor(handle, scalingFactors[idx]);
        }
    }

    // Test tjGetScalingFactors (legacy, if available)
    int legacyNum = 0;
    tjscalingfactor *legacyFactors = tjGetScalingFactors(&legacyNum);
    if (legacyFactors && legacyNum > 0) {
        // Ensure we have enough data before accessing Data[1]
        if (Size >= 2) {
            int legacyIdx = (Data[1] % legacyNum);
            // Just use the index to explore state, no need to call set again necessarily
            (void)legacyIdx; 
        }
    }

    // Prepare dummy JPEG data for decompression
    const unsigned char *jpegBuf = Data;
    size_t jpegSize = Size;

    // Allocate destination buffer for YUV
    // Cap the size to avoid huge allocations
    size_t maxBufSize = 1024 * 1024; // 1MB
    unsigned char *dstBuf = NULL;
    
    // Heuristic for buffer size: use input size * 2 or maxBufSize
    size_t allocSize = (Size > 0 && Size < maxBufSize) ? Size * 2 : maxBufSize;
    dstBuf = (unsigned char *)malloc(allocSize);
    
    if (dstBuf) {
        // Test tj3SetCroppingRegion
        // Construct a region from fuzz data, ensuring we have enough bytes
        tjregion region = {0};
        if (Size >= 6) {
            region.x = (Data[2] % 100);
            region.y = (Data[3] % 100);
            region.w = (Data[4] % 100);
            region.h = (Data[5] % 100);
            tj3SetCroppingRegion(handle, region);
        }
        
        // Try setting TJUNCROPPED (all zeros)
        tjregion uncropped = {0, 0, 0, 0};
        tj3SetCroppingRegion(handle, uncropped);

        // Test tj3DecompressToYUV8
        // align must be power of 2. Use Data[6] if available, else default
        int align = 1;
        if (Size >= 7) {
            align = 1 << (Data[6] % 4); // 1, 2, 4, or 8
        }
        ret = tj3DecompressToYUV8(handle, jpegBuf, jpegSize, dstBuf, align);
        
        free(dstBuf);
    }

    // Cleanup
    if (handle) {
        tj3Destroy(handle);
    }

    return 0;
}