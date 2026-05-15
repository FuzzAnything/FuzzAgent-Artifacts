// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_AdaptToIlluminant(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsCIEXYZ) * 3) return;
    
    cmsCIEXYZ Result;
    cmsCIEXYZ* SourceWhitePt = (cmsCIEXYZ*)data;
    cmsCIEXYZ* Illuminant = (cmsCIEXYZ*)(data + sizeof(cmsCIEXYZ));
    cmsCIEXYZ* Value = (cmsCIEXYZ*)(data + sizeof(cmsCIEXYZ) * 2);
    
    cmsAdaptToIlluminant(&Result, SourceWhitePt, Illuminant, Value);
}

static void fuzz_XYZ2xyY(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsCIEXYZ)) return;
    
    cmsCIExyY Dest;
    cmsCIEXYZ* Source = (cmsCIEXYZ*)data;
    
    cmsXYZ2xyY(&Dest, Source);
}

static void fuzz_XYZ2Lab(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsCIEXYZ)) return;
    
    cmsCIELab Lab;
    cmsCIEXYZ* xyz = (cmsCIEXYZ*)data;
    
    // Test with NULL WhitePoint (defaults to D50)
    cmsXYZ2Lab(NULL, &Lab, xyz);
    
    // Test with explicit D50 white point
    const cmsCIEXYZ* WhitePoint = cmsD50_XYZ();
    cmsXYZ2Lab(WhitePoint, &Lab, xyz);
    
    // Test with custom white point if enough data
    if (size >= sizeof(cmsCIEXYZ) * 2) {
        cmsCIEXYZ* customWhitePoint = (cmsCIEXYZ*)(data + sizeof(cmsCIEXYZ));
        cmsXYZ2Lab(customWhitePoint, &Lab, xyz);
    }
}

static void fuzz_Lab2XYZ(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsCIELab)) return;
    
    cmsCIEXYZ xyz;
    cmsCIELab* Lab = (cmsCIELab*)data;
    
    // Test with NULL WhitePoint (defaults to D50)
    cmsLab2XYZ(NULL, &xyz, Lab);
    
    // Test with explicit D50 white point
    const cmsCIEXYZ* WhitePoint = cmsD50_XYZ();
    cmsLab2XYZ(WhitePoint, &xyz, Lab);
    
    // Test with custom white point if enough data
    if (size >= sizeof(cmsCIELab) + sizeof(cmsCIEXYZ)) {
        cmsCIEXYZ* customWhitePoint = (cmsCIEXYZ*)(data + sizeof(cmsCIELab));
        cmsLab2XYZ(customWhitePoint, &xyz, Lab);
    }
}

static void fuzz_xyY2XYZ(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsCIExyY)) return;
    
    cmsCIEXYZ Dest;
    cmsCIExyY* Source = (cmsCIExyY*)data;
    
    cmsxyY2XYZ(&Dest, Source);
}

static void fuzz_chain_conversions(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsCIEXYZ)) return;
    
    cmsCIEXYZ xyz1, xyz2;
    cmsCIExyY xyY;
    cmsCIELab Lab;
    
    cmsCIEXYZ* sourceXYZ = (cmsCIEXYZ*)data;
    
    // Chain: XYZ -> xyY -> XYZ
    cmsXYZ2xyY(&xyY, sourceXYZ);
    cmsxyY2XYZ(&xyz1, &xyY);
    
    // Chain: XYZ -> Lab -> XYZ
    cmsXYZ2Lab(NULL, &Lab, sourceXYZ);
    cmsLab2XYZ(NULL, &xyz2, &Lab);
    
    // Chain with adaptation if enough data
    if (size >= sizeof(cmsCIEXYZ) * 3) {
        cmsCIEXYZ Result;
        cmsCIEXYZ* SourceWhitePt = (cmsCIEXYZ*)data;
        cmsCIEXYZ* Illuminant = (cmsCIEXYZ*)(data + sizeof(cmsCIEXYZ));
        cmsCIEXYZ* Value = (cmsCIEXYZ*)(data + sizeof(cmsCIEXYZ) * 2);
        
        if (cmsAdaptToIlluminant(&Result, SourceWhitePt, Illuminant, Value)) {
            // Convert adapted result to Lab and back
            cmsXYZ2Lab(NULL, &Lab, &Result);
            cmsLab2XYZ(NULL, &xyz1, &Lab);
        }
    }
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Seed random number generator with input data for diverse testing
    unsigned int seed = 0;
    for (size_t i = 0; i < Size && i < sizeof(seed); i++) {
        seed = (seed << 8) | Data[i];
    }
    srand(seed);
    
    // Call each fuzzing function with the input data
    fuzz_AdaptToIlluminant(Data, Size);
    fuzz_XYZ2xyY(Data, Size);
    fuzz_XYZ2Lab(Data, Size);
    fuzz_Lab2XYZ(Data, Size);
    fuzz_xyY2XYZ(Data, Size);
    fuzz_chain_conversions(Data, Size);
    
    return 0;
}