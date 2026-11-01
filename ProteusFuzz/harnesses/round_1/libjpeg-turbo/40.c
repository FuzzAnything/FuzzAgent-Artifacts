// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;

    // Test tj3GetScalingFactors
    int numScalingFactors = 0;
    tjscalingfactor *sfList = tj3GetScalingFactors(&numScalingFactors);
    if (sfList && numScalingFactors > 0) {
        // Try setting a random scaling factor from the list
        int idx = Data[0] % numScalingFactors;
        if (tj3SetScalingFactor(handle, sfList[idx]) != 0) {
            // Error is acceptable in fuzzing
        }
    }

    // Prepare dummy file if needed (though we use buffer directly here)
    write_dummy_file(Data, Size);

    // Attempt to decompress header to get image parameters
    // We need to pass the data as jpegBuf
    // Note: tj3DecompressHeader is not in the target list, but required for cropping.
    // However, the prompt implies we should test the listed APIs.
    // tj3SetCroppingRegion requires header to be read.
    // Since tj3DecompressHeader is not explicitly listed as a target to fuzz but is a prerequisite,
    // we will attempt to call it if available in the header, or skip cropping if it fails.
    // The prompt says "using the information provided below", but standard usage requires header read.
    // We will assume standard library availability for helper functions needed to make the targets callable.
    // If tj3DecompressHeader is not available in the specific build, this might fail, which is fine for fuzzing.
    
    // Try to read header to enable cropping tests
    if (tj3DecompressHeader(handle, (const unsigned char*)Data, Size) == 0) {
        // Header read successful, try cropping
        tjregion cropRegion = {0, 0, 0, 0}; // TJUNCROPPED equivalent or invalid
        
        // Fuzz crop region values
        if (Size >= 8) {
            cropRegion.x = Data[1];
            cropRegion.y = Data[2];
            cropRegion.w = Data[3];
            cropRegion.h = Data[4];
        }
        
        // Try setting cropping region
        tj3SetCroppingRegion(handle, cropRegion);
        
        // Try setting to uncropped explicitly
        tjregion uncropped = {0, 0, 0, 0};
        tj3SetCroppingRegion(handle, uncropped);
    }

    // Allocate destination buffer for YUV
    // We need width and height to calculate size. 
    // Since we might not have successfully read the header or params, we use a safe max or heuristic.
    // However, tj3YUVBufSize requires width, height, subsamp, align.
    // We can try to get params if header was read, but for fuzzing, let's try a fixed size or dynamic based on input.
    // A safer approach for fuzzing without full param extraction is to allocate a reasonable buffer.
    // But tj3DecompressToYUV8 will fail if buffer is too small, which is fine.
    
    // Let's try to get params if possible, otherwise guess.
    // Since we can't easily extract params without tj3GetParam which isn't listed, 
    // we will allocate a buffer based on input size or a fixed large size.
    // The API requires dstBuf size to be calculated via tj3YUVBufSize.
    // If we don't know width/height, we can't call tj3YUVBufSize correctly.
    // Strategy: Try to call tj3YUVBufSize with dummy values or skip if params unknown.
    // Actually, if tj3DecompressHeader succeeded, we can try to get params.
    // But tj3GetParam is not in the target list. 
    // Let's assume we can't get exact size, so we allocate a buffer of Size + padding.
    // Or better, try to call tj3YUVBufSize with 0 width/height? No, that's invalid.
    
    // Alternative: Just allocate a buffer of 'Size' bytes (or Size*2) and pass it.
    // The function will likely return -1 if size is wrong, which is valid fuzzing behavior.
    // However, to be more effective, let's try to use a standard size if we can't determine it.
    // Let's allocate a buffer of 1MB max or Size*2.
    
    size_t maxBufSize = 1024 * 1024; 
    if (Size > 0 && Size < maxBufSize) maxBufSize = Size * 2;
    
    unsigned char *dstBuf = malloc(maxBufSize);
    if (!dstBuf) {
        tj3Destroy(handle);
        return 0;
    }

    // Try various alignments (powers of 2)
    int alignments[] = {1, 2, 4, 8, 16, 32, 64};
    int alignCount = sizeof(alignments) / sizeof(alignments[0]);
    
    for (int i = 0; i < alignCount; i++) {
        int align = alignments[i];
        // Try decompression
        tj3DecompressToYUV8(handle, (const unsigned char*)Data, Size, dstBuf, align);
    }
    
    // Also try with invalid alignment (not power of 2) to test validation
    tj3DecompressToYUV8(handle, (const unsigned char*)Data, Size, dstBuf, 3);
    tj3DecompressToYUV8(handle, (const unsigned char*)Data, Size, dstBuf, 0);

    free(dstBuf);
    tj3Destroy(handle);
    return 0;
}