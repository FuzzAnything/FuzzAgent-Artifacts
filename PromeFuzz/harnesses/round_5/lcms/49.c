// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    // We need at least enough data for two cmsCIELab structures (6 doubles = 48 bytes)
    // plus some extra for parameters
    if (Size < 48) return 0;
    
    // Parse input data to create Lab colors and parameters
    // Use the first 48 bytes for two Lab structures (6 doubles)
    cmsCIELab Lab1, Lab2;
    size_t offset = 0;
    
    // Copy data for Lab1 (3 doubles)
    memcpy(&Lab1.L, Data + offset, sizeof(double));
    offset += sizeof(double);
    memcpy(&Lab1.a, Data + offset, sizeof(double));
    offset += sizeof(double);
    memcpy(&Lab1.b, Data + offset, sizeof(double));
    offset += sizeof(double);
    
    // Copy data for Lab2 (3 doubles)
    memcpy(&Lab2.L, Data + offset, sizeof(double));
    offset += sizeof(double);
    memcpy(&Lab2.a, Data + offset, sizeof(double));
    offset += sizeof(double);
    memcpy(&Lab2.b, Data + offset, sizeof(double));
    offset += sizeof(double);
    
    // Use remaining data for parameters
    // We'll use the next bytes to generate parameter values
    // If we run out of data, use default values
    
    // For functions that need parameters, extract them from remaining data
    double param1 = 1.0, param2 = 1.0, param3 = 1.0, param4 = 1.0;
    
    if (Size >= offset + sizeof(double)) {
        memcpy(&param1, Data + offset, sizeof(double));
        offset += sizeof(double);
    }
    if (Size >= offset + sizeof(double)) {
        memcpy(&param2, Data + offset, sizeof(double));
        offset += sizeof(double);
    }
    if (Size >= offset + sizeof(double)) {
        memcpy(&param3, Data + offset, sizeof(double));
        offset += sizeof(double);
    }
    if (Size >= offset + sizeof(double)) {
        memcpy(&param4, Data + offset, sizeof(double));
        offset += sizeof(double);
    }
    
    // Call all target functions with the parsed data
    // 1. cmsCIE2000DeltaE
    cmsCIE2000DeltaE(&Lab1, &Lab2, param1, param2, param3);
    
    // 2. cmsCIE94DeltaE
    cmsCIE94DeltaE(&Lab1, &Lab2);
    
    // 3. cmsCMCdeltaE
    cmsCMCdeltaE(&Lab1, &Lab2, param1, param2);
    
    // 4. cmsDeltaE
    cmsDeltaE(&Lab1, &Lab2);
    
    // 5. cmsBFDdeltaE
    cmsBFDdeltaE(&Lab1, &Lab2);
    
    // 6. cmsDesaturateLab
    // Create a copy of Lab1 to modify
    cmsCIELab LabCopy = Lab1;
    // Use param1-param4 as bounds: amax, amin, bmax, bmin
    // Ensure amin <= amax and bmin <= bmax for valid input
    double amax = param1;
    double amin = param2;
    double bmax = param3;
    double bmin = param4;
    
    // Swap if min > max
    if (amin > amax) {
        double temp = amin;
        amin = amax;
        amax = temp;
    }
    if (bmin > bmax) {
        double temp = bmin;
        bmin = bmax;
        bmax = temp;
    }
    
    cmsDesaturateLab(&LabCopy, amax, amin, bmax, bmin);
    
    return 0;
}