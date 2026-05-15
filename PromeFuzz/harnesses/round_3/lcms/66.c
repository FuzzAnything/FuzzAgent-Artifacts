// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>

static void parse_lab_structs(const uint8_t *data, size_t size, cmsCIELab *lab1, cmsCIELab *lab2) {
    // Ensure we have enough data for two Lab structures (each with 3 doubles)
    size_t required_size = 2 * 3 * sizeof(double);
    if (size < required_size) {
        // If not enough data, pad with zeros
        memset(lab1, 0, sizeof(cmsCIELab));
        memset(lab2, 0, sizeof(cmsCIELab));
        if (size >= sizeof(double)) {
            memcpy(&lab1->L, data, size > sizeof(cmsCIELab) ? sizeof(cmsCIELab) : size);
        }
        return;
    }
    
    // Parse first Lab structure
    memcpy(&lab1->L, data, sizeof(double));
    memcpy(&lab1->a, data + sizeof(double), sizeof(double));
    memcpy(&lab1->b, data + 2 * sizeof(double), sizeof(double));
    
    // Parse second Lab structure
    memcpy(&lab2->L, data + 3 * sizeof(double), sizeof(double));
    memcpy(&lab2->a, data + 4 * sizeof(double), sizeof(double));
    memcpy(&lab2->b, data + 5 * sizeof(double), sizeof(double));
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    cmsCIELab lab1, lab2;
    
    // Parse Lab structures from input data
    parse_lab_structs(Data, Size, &lab1, &lab2);
    
    // 1. Test cmsDeltaE
    (void)cmsDeltaE(&lab1, &lab2);
    
    // 2. Test cmsCMCdeltaE - extract l and c parameters from remaining data
    cmsFloat64Number l = 1.0, c = 1.0;  // Default values
    if (Size >= 6 * sizeof(double) + 2 * sizeof(cmsFloat64Number)) {
        memcpy(&l, Data + 6 * sizeof(double), sizeof(cmsFloat64Number));
        memcpy(&c, Data + 6 * sizeof(double) + sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
    }
    (void)cmsCMCdeltaE(&lab1, &lab2, l, c);
    
    // 3. Test cmsDesaturateLab - extract bounds from data
    double amax = 100.0, amin = -100.0, bmax = 100.0, bmin = -100.0;  // Default bounds
    size_t offset = 6 * sizeof(double);
    if (Size >= offset + 4 * sizeof(double)) {
        memcpy(&amax, Data + offset, sizeof(double));
        memcpy(&amin, Data + offset + sizeof(double), sizeof(double));
        memcpy(&bmax, Data + offset + 2 * sizeof(double), sizeof(double));
        memcpy(&bmin, Data + offset + 3 * sizeof(double), sizeof(double));
    }
    
    // Create a copy for desaturation since it modifies the input
    cmsCIELab lab_desat = lab1;
    (void)cmsDesaturateLab(&lab_desat, amax, amin, bmax, bmin);
    
    // 4. Test cmsCIE2000DeltaE - extract Kl, Kc, Kh parameters
    cmsFloat64Number Kl = 1.0, Kc = 1.0, Kh = 1.0;  // Default values
    offset = 6 * sizeof(double);
    if (Size >= offset + 3 * sizeof(cmsFloat64Number)) {
        memcpy(&Kl, Data + offset, sizeof(cmsFloat64Number));
        memcpy(&Kc, Data + offset + sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
        memcpy(&Kh, Data + offset + 2 * sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
    }
    (void)cmsCIE2000DeltaE(&lab1, &lab2, Kl, Kc, Kh);
    
    // 5. Test cmsBFDdeltaE
    (void)cmsBFDdeltaE(&lab1, &lab2);
    
    // 6. Test cmsCIE94DeltaE
    (void)cmsCIE94DeltaE(&lab1, &lab2);
    
    return 0;
}