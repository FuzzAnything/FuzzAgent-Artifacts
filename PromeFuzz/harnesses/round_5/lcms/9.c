// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
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

static void parse_input(const uint8_t *data, size_t size, 
                       cmsCIEXYZ *xyz1, cmsCIEXYZ *xyz2, cmsCIEXYZ *xyz3,
                       cmsCIELab *lab, cmsCIExyY *xyY) {
    size_t offset = 0;
    
    // Initialize all structures to zero
    memset(xyz1, 0, sizeof(cmsCIEXYZ));
    memset(xyz2, 0, sizeof(cmsCIEXYZ));
    memset(xyz3, 0, sizeof(cmsCIEXYZ));
    memset(lab, 0, sizeof(cmsCIELab));
    memset(xyY, 0, sizeof(cmsCIExyY));
    
    // Parse XYZ1 (24 bytes for 3 doubles)
    if (offset + sizeof(double) <= size) memcpy(&xyz1->X, data + offset, sizeof(double));
    offset += sizeof(double);
    if (offset + sizeof(double) <= size) memcpy(&xyz1->Y, data + offset, sizeof(double));
    offset += sizeof(double);
    if (offset + sizeof(double) <= size) memcpy(&xyz1->Z, data + offset, sizeof(double));
    offset += sizeof(double);
    
    // Parse XYZ2
    if (offset + sizeof(double) <= size) memcpy(&xyz2->X, data + offset, sizeof(double));
    offset += sizeof(double);
    if (offset + sizeof(double) <= size) memcpy(&xyz2->Y, data + offset, sizeof(double));
    offset += sizeof(double);
    if (offset + sizeof(double) <= size) memcpy(&xyz2->Z, data + offset, sizeof(double));
    offset += sizeof(double);
    
    // Parse XYZ3
    if (offset + sizeof(double) <= size) memcpy(&xyz3->X, data + offset, sizeof(double));
    offset += sizeof(double);
    if (offset + sizeof(double) <= size) memcpy(&xyz3->Y, data + offset, sizeof(double));
    offset += sizeof(double);
    if (offset + sizeof(double) <= size) memcpy(&xyz3->Z, data + offset, sizeof(double));
    offset += sizeof(double);
    
    // Parse Lab
    if (offset + sizeof(double) <= size) memcpy(&lab->L, data + offset, sizeof(double));
    offset += sizeof(double);
    if (offset + sizeof(double) <= size) memcpy(&lab->a, data + offset, sizeof(double));
    offset += sizeof(double);
    if (offset + sizeof(double) <= size) memcpy(&lab->b, data + offset, sizeof(double));
    offset += sizeof(double);
    
    // Parse xyY
    if (offset + sizeof(double) <= size) memcpy(&xyY->x, data + offset, sizeof(double));
    offset += sizeof(double);
    if (offset + sizeof(double) <= size) memcpy(&xyY->y, data + offset, sizeof(double));
    offset += sizeof(double);
    if (offset + sizeof(double) <= size) memcpy(&xyY->Y, data + offset, sizeof(double));
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize structures
    cmsCIEXYZ xyz1, xyz2, xyz3, result_xyz;
    cmsCIELab lab, result_lab;
    cmsCIExyY xyY, result_xyY;
    
    // Parse input data into structures
    parse_input(Data, Size, &xyz1, &xyz2, &xyz3, &lab, &xyY);
    
    // Get D50 white point
    const cmsCIEXYZ* d50 = cmsD50_XYZ();
    
    // Use first byte to determine variations
    uint8_t variation = Data[0];
    
    // Test cmsXYZ2Lab with different white points
    if (variation & 0x01) {
        // With D50 white point
        cmsXYZ2Lab(d50, &result_lab, &xyz1);
    } else {
        // With NULL white point (defaults to D50)
        cmsXYZ2Lab(NULL, &result_lab, &xyz1);
    }
    
    // Test cmsXYZ2xyY
    cmsXYZ2xyY(&result_xyY, &xyz2);
    
    // Test cmsAdaptToIlluminant
    cmsBool adaptation_result = cmsAdaptToIlluminant(&result_xyz, &xyz1, &xyz2, &xyz3);
    
    // Test cmsLab2XYZ with different white points
    if (variation & 0x02) {
        // With D50 white point
        cmsLab2XYZ(d50, &result_xyz, &lab);
    } else {
        // With NULL white point
        cmsLab2XYZ(NULL, &result_xyz, &lab);
    }
    
    // Test cmsxyY2XYZ
    cmsxyY2XYZ(&result_xyz, &xyY);
    
    // Additional test: chain conversions
    if (Size > 60) {
        cmsCIEXYZ intermediate_xyz;
        cmsCIELab intermediate_lab;
        cmsCIExyY intermediate_xyY;
        
        // XYZ -> Lab -> XYZ
        cmsXYZ2Lab(d50, &intermediate_lab, &xyz1);
        cmsLab2XYZ(d50, &intermediate_xyz, &intermediate_lab);
        
        // XYZ -> xyY -> XYZ
        cmsXYZ2xyY(&intermediate_xyY, &xyz2);
        cmsxyY2XYZ(&intermediate_xyz, &intermediate_xyY);
        
        // Lab -> XYZ -> Lab
        cmsLab2XYZ(d50, &intermediate_xyz, &lab);
        cmsXYZ2Lab(d50, &intermediate_lab, &intermediate_xyz);
    }
    
    return 0;
}