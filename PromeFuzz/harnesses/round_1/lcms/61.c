// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsFloat2XYZEncoded at cmspcs.c:374:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// cmsFloat2XYZEncoded at cmspcs.c:374:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>

static void initialize_structures(const uint8_t *data, size_t size, 
                                  cmsCIEXYZ *xyz, cmsCIELab *lab, 
                                  cmsCIExyY *xyY, cmsUInt16Number *encoded) {
    // Initialize with fuzzer data, using size to prevent overreads
    size_t offset = 0;
    
    // Fill cmsCIEXYZ (3 doubles = 24 bytes)
    if (offset + sizeof(cmsCIEXYZ) <= size) {
        memcpy(xyz, data + offset, sizeof(cmsCIEXYZ));
        offset += sizeof(cmsCIEXYZ);
    }
    
    // Fill cmsCIELab (3 doubles = 24 bytes)
    if (offset + sizeof(cmsCIELab) <= size) {
        memcpy(lab, data + offset, sizeof(cmsCIELab));
        offset += sizeof(cmsCIELab);
    }
    
    // Fill cmsCIExyY (3 doubles = 24 bytes)
    if (offset + sizeof(cmsCIExyY) <= size) {
        memcpy(xyY, data + offset, sizeof(cmsCIExyY));
        offset += sizeof(cmsCIExyY);
    }
    
    // Fill encoded XYZ (3 unsigned shorts = 6 bytes)
    if (offset + 6 <= size) {
        memcpy(encoded, data + offset, 6);
    }
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 78) { // Minimum size for all structures (24+24+24+6)
        return 0;
    }
    
    // Initialize all needed structures
    cmsCIEXYZ xyz1, xyz2, xyz3, whitePoint;
    cmsCIELab lab1, lab2;
    cmsCIExyY xyY1, xyY2;
    cmsUInt16Number encoded[3];
    
    // Initialize with fuzzer data
    initialize_structures(Data, Size, &xyz1, &lab1, &xyY1, encoded);
    
    // Use the same data for white point
    memcpy(&whitePoint, Data, sizeof(cmsCIEXYZ));
    
    // 1. Test cmsXYZEncoded2Float
    cmsXYZEncoded2Float(&xyz2, encoded);
    
    // 2. Test cmsXYZ2Lab with white point
    cmsXYZ2Lab(&whitePoint, &lab2, &xyz1);
    
    // 3. Test cmsXYZ2Lab with NULL white point (defaults to D50)
    cmsXYZ2Lab(NULL, &lab2, &xyz1);
    
    // 4. Test cmsFloat2XYZEncoded
    cmsFloat2XYZEncoded(encoded, &xyz1);
    
    // 5. Test cmsLab2XYZ with white point
    cmsLab2XYZ(&whitePoint, &xyz3, &lab1);
    
    // 6. Test cmsLab2XYZ with NULL white point
    cmsLab2XYZ(NULL, &xyz3, &lab1);
    
    // 7. Test cmsxyY2XYZ
    cmsxyY2XYZ(&xyz2, &xyY1);
    
    // 8. Test cmsXYZ2xyY
    cmsXYZ2xyY(&xyY2, &xyz1);
    
    // Chain conversions to explore more states
    cmsCIEXYZ temp_xyz;
    cmsCIELab temp_lab;
    cmsCIExyY temp_xyY;
    
    // XYZ -> Lab -> XYZ
    cmsXYZ2Lab(&whitePoint, &temp_lab, &xyz1);
    cmsLab2XYZ(&whitePoint, &temp_xyz, &temp_lab);
    
    // XYZ -> xyY -> XYZ
    cmsXYZ2xyY(&temp_xyY, &xyz1);
    cmsxyY2XYZ(&temp_xyz, &temp_xyY);
    
    // Encoded -> Float -> Encoded
    cmsUInt16Number temp_encoded[3];
    cmsXYZEncoded2Float(&temp_xyz, encoded);
    cmsFloat2XYZEncoded(temp_encoded, &temp_xyz);
    
    return 0;
}