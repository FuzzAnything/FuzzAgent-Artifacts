// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tjGetScalingFactors at turbojpeg.c:1974:28 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test tj3GetScalingFactors and tjGetScalingFactors (no handle required)
    int numFactors = 0;
    tjscalingfactor *factors = tj3GetScalingFactors(&numFactors);
    if (factors) {
        for (int i = 0; i < numFactors; i++) {
            (void)factors[i];
        }
    }
    
    int numFactorsOld = 0;
    tjscalingfactor *factorsOld = tjGetScalingFactors(&numFactorsOld);
    if (factorsOld) {
        for (int i = 0; i < numFactorsOld; i++) {
            (void)factorsOld[i];
        }
    }

    // Test tj3Init with different init types
    tjhandle handleCompress = tj3Init(TJINIT_COMPRESS);
    tjhandle handleDecompress = tj3Init(TJINIT_DECOMPRESS);
    
    // Test tj3SetScalingFactor on decompress handle
    if (handleDecompress && factors && numFactors > 0) {
        tj3SetScalingFactor(handleDecompress, factors[0]);
        tjscalingfactor unscaled = {1, 1};
        tj3SetScalingFactor(handleDecompress, unscaled);
    }

    // Test tj3SetCroppingRegion
    if (handleDecompress) {
        tjregion cropRegion = {0, 0, 0, 0}; 
        tj3SetCroppingRegion(handleDecompress, cropRegion);
        
        tjregion badCrop = {10, 10, 100, 100};
        tj3SetCroppingRegion(handleDecompress, badCrop);
    }

    // Test tj3DecompressToYUV8
    if (handleDecompress && Size > 0) {
        const unsigned char *jpegBuf = Data;
        size_t jpegSize = Size;
        
        size_t dstSize = 1024 * 1024; 
        if (Size > dstSize) dstSize = Size;
        
        unsigned char *dstBuf = (unsigned char *)malloc(dstSize);
        if (dstBuf) {
            int align = 4; 
            tj3DecompressToYUV8(handleDecompress, jpegBuf, jpegSize, dstBuf, align);
            
            tj3DecompressToYUV8(handleDecompress, jpegBuf, jpegSize, dstBuf, 1);
            
            free(dstBuf);
        }
    }

    // Cleanup handles to prevent memory leaks
    if (handleCompress) {
        tj3Destroy(handleCompress);
    }
    if (handleDecompress) {
        tj3Destroy(handleDecompress);
    }

    return 0;
}