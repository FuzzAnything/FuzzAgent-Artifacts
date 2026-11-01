// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
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

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return -1;
    if (size > 0 && fwrite(data, 1, size, f) != size) {
        fclose(f);
        return -1;
    }
    fclose(f);
    return 0;
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;

    // Test tj3GetScalingFactors
    int numFactors = 0;
    tjscalingfactor *factors = tj3GetScalingFactors(&numFactors);
    if (factors && numFactors > 0) {
        // Try setting a random scaling factor if available
        if (Size > 0) {
            int idx = Data[0] % numFactors;
            tj3SetScalingFactor(handle, factors[idx]);
        }
    }

    // Prepare destination buffer size estimation
    // We don't know the exact image dimensions, so we allocate a reasonable buffer
    // or use a small fixed size for testing error paths.
    // In a real scenario, we would call tj3DecompressHeader first to get dimensions.
    // Here we simulate by trying to decompress directly to trigger validation.
    size_t maxBufSize = 1024 * 1024; // 1MB max for safety
    unsigned char *dstBuf = NULL;
    if (Size > 0) {
        // Use input data as a hint for buffer size, but cap it
        size_t hint = Size;
        if (hint > maxBufSize) hint = maxBufSize;
        dstBuf = (unsigned char *)malloc(hint);
    }

    if (dstBuf) {
        // Test tj3DecompressToYUV8 with various alignments
        int align = 1;
        if (Size > 1) {
            // Ensure align is power of 2: 1, 2, 4, 8, 16, 32, 64, 128
            int val = Data[1] % 8;
            align = 1 << val;
        }

        // Attempt decompression
        // If Size is small, it might not be a valid JPEG, which is good for fuzzing
        tj3DecompressToYUV8(handle, Data, Size, dstBuf, align);
        
        free(dstBuf);
    }

    // Test tj3SetCroppingRegion
    // We need to create a dummy region. 
    // Since we didn't call tj3DecompressHeader, this might fail, which is expected.
    tjregion region = {0};
    if (Size >= 8) {
        region.x = Data[2];
        region.y = Data[3];
        region.w = Data[4];
        region.h = Data[5];
        // Ensure non-negative
        if (region.x < 0) region.x = 0;
        if (region.y < 0) region.y = 0;
        if (region.w < 0) region.w = 0;
        if (region.h < 0) region.h = 0;
    }
    tj3SetCroppingRegion(handle, region);

    // Cleanup
    tj3Destroy(handle);
    
    return 0;
}