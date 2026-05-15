// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static void fill_struct(void* struct_ptr, size_t struct_size, const uint8_t* Data, size_t Size, size_t* offset) {
    if (*offset + struct_size <= Size) {
        memcpy(struct_ptr, Data + *offset, struct_size);
        *offset += struct_size;
    } else if (Size > 0) {
        for (size_t i = 0; i < struct_size; i++) {
            ((uint8_t*)struct_ptr)[i] = Data[(*offset + i) % Size];
        }
        *offset += struct_size;
    } else {
        memset(struct_ptr, 0, struct_size);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    // Initialize structures with fuzzer data
    cmsCIEXYZ xyz1, xyz2, xyz3, xyz4, xyz5, xyz6, xyz7, xyz8;
    cmsCIELab lab1, lab2;
    cmsCIExyY xyY1, xyY2;
    
    // Use cyclic pattern to fill structures if data is insufficient
    size_t offset = 0;
    
    // Fill all structures
    fill_struct(&xyz1, sizeof(xyz1), Data, Size, &offset);
    fill_struct(&xyz2, sizeof(xyz2), Data, Size, &offset);
    fill_struct(&xyz3, sizeof(xyz3), Data, Size, &offset);
    fill_struct(&xyz4, sizeof(xyz4), Data, Size, &offset);
    fill_struct(&xyz5, sizeof(xyz5), Data, Size, &offset);
    fill_struct(&xyz6, sizeof(xyz6), Data, Size, &offset);
    fill_struct(&lab1, sizeof(lab1), Data, Size, &offset);
    fill_struct(&lab2, sizeof(lab2), Data, Size, &offset);
    fill_struct(&xyY1, sizeof(xyY1), Data, Size, &offset);
    fill_struct(&xyY2, sizeof(xyY2), Data, Size, &offset);
    
    // Test cmsXYZ2Lab with various inputs
    cmsXYZ2Lab(&xyz1, &lab1, &xyz2);  // With white point
    cmsXYZ2Lab(NULL, &lab1, &xyz2);   // With NULL white point (defaults to D50)
    
    // Test cmsD50_XYZ
    const cmsCIEXYZ* d50 = cmsD50_XYZ();
    
    // Test cmsLab2XYZ with various inputs
    cmsLab2XYZ(&xyz3, &xyz4, &lab2);  // With white point
    cmsLab2XYZ(NULL, &xyz4, &lab2);   // With NULL white point
    
    // Test cmsxyY2XYZ
    cmsxyY2XYZ(&xyz5, &xyY1);
    
    // Test cmsAdaptToIlluminant
    cmsBool result = cmsAdaptToIlluminant(&xyz6, &xyz1, &xyz2, &xyz3);
    (void)result;  // Use result to avoid unused variable warning
    
    // Test cmsXYZ2xyY
    cmsXYZ2xyY(&xyY2, &xyz4);
    
    // Test with D50 white point from cmsD50_XYZ()
    cmsXYZ2Lab(d50, &lab1, &xyz5);
    cmsLab2XYZ(d50, &xyz7, &lab2);
    
    // Test adaptation using D50
    cmsAdaptToIlluminant(&xyz8, d50, &xyz1, &xyz2);
    
    // Test chain of conversions
    cmsCIEXYZ intermediate1, intermediate2;
    cmsCIELab intermediate_lab;
    cmsCIExyY intermediate_xyY;
    
    fill_struct(&intermediate1, sizeof(intermediate1), Data, Size, &offset);
    fill_struct(&intermediate2, sizeof(intermediate2), Data, Size, &offset);
    fill_struct(&intermediate_lab, sizeof(intermediate_lab), Data, Size, &offset);
    fill_struct(&intermediate_xyY, sizeof(intermediate_xyY), Data, Size, &offset);
    
    // XYZ -> Lab -> XYZ
    cmsXYZ2Lab(&xyz1, &intermediate_lab, &xyz2);
    cmsLab2XYZ(&xyz1, &intermediate1, &intermediate_lab);
    
    // XYZ -> xyY -> XYZ
    cmsXYZ2xyY(&intermediate_xyY, &xyz3);
    cmsxyY2XYZ(&intermediate2, &intermediate_xyY);
    
    // Adaptation chain
    cmsAdaptToIlluminant(&intermediate1, &xyz1, &xyz2, &xyz3);
    cmsAdaptToIlluminant(&intermediate2, &xyz2, &xyz3, &intermediate1);
    
    return 0;
}