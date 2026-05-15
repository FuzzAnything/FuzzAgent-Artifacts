// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsFloat2XYZEncoded at cmspcs.c:374:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 54) return 0; // Need at least 54 bytes for all operations (6 + 24 + 24 + 24)

    // Extract data for various structures
    cmsUInt16Number encodedXYZ[3];
    if (Size >= 6) {
        memcpy(encodedXYZ, Data, 6);
    }
    
    cmsCIEXYZ xyzFloat;
    if (Size >= 30) { // 6 + 24
        memcpy(&xyzFloat, Data + 6, sizeof(cmsCIEXYZ));
    }
    
    cmsCIExyY xyY;
    if (Size >= 54) { // 6 + 24 + 24
        memcpy(&xyY, Data + 30, sizeof(cmsCIExyY));
    }
    
    cmsCIELab lab;
    if (Size >= 78) { // 6 + 24 + 24 + 24
        memcpy(&lab, Data + 54, sizeof(cmsCIELab));
    }
    
    // Test cmsXYZEncoded2Float
    if (Size >= 6) {
        cmsCIEXYZ resultXYZ;
        cmsXYZEncoded2Float(&resultXYZ, encodedXYZ);
    }
    
    // Test cmsXYZ2xyY
    if (Size >= 30) {
        cmsCIExyY resultxyY;
        cmsXYZ2xyY(&resultxyY, &xyzFloat);
    }
    
    // Test cmsXYZ2Lab with NULL white point
    if (Size >= 30) {
        cmsCIELab resultLab1;
        cmsXYZ2Lab(NULL, &resultLab1, &xyzFloat);
    }
    
    // Test cmsXYZ2Lab with provided white point
    if (Size >= 30) {
        cmsCIELab resultLab2;
        cmsXYZ2Lab(&xyzFloat, &resultLab2, &xyzFloat);
    }
    
    // Test cmsFloat2XYZEncoded
    if (Size >= 30) {
        cmsUInt16Number resultEncoded[3];
        cmsFloat2XYZEncoded(resultEncoded, &xyzFloat);
    }
    
    // Test cmsLab2XYZ with NULL white point
    if (Size >= 78) {
        cmsCIEXYZ resultXYZ1;
        cmsLab2XYZ(NULL, &resultXYZ1, &lab);
    }
    
    // Test cmsLab2XYZ with provided white point
    if (Size >= 78) {
        cmsCIEXYZ resultXYZ2;
        cmsLab2XYZ(&xyzFloat, &resultXYZ2, &lab);
    }
    
    // Test cmsxyY2XYZ
    if (Size >= 54) {
        cmsCIEXYZ resultXYZ3;
        cmsxyY2XYZ(&resultXYZ3, &xyY);
    }

    return 0;
}