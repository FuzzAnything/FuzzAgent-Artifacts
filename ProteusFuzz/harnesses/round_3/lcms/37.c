// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcms2.h"

static void parse_lab_from_data(const uint8_t *data, size_t size, cmsCIELab *lab) {
    // Initialize with safe defaults
    lab->L = 0.0;
    lab->a = 0.0;
    lab->b = 0.0;

    if (size < 24) return;

    // Parse L, a, b from the first 24 bytes (8 bytes each as double)
    memcpy(&lab->L, data, sizeof(double));
    memcpy(&lab->a, data + 8, sizeof(double));
    memcpy(&lab->b, data + 16, sizeof(double));
}

static void parse_float64_from_data(const uint8_t *data, size_t size, cmsFloat64Number *val) {
    if (size < 8) {
        *val = 1.0;
        return;
    }
    memcpy(val, data, sizeof(double));
    // Ensure we don't pass NaN or Inf if possible, though fuzzing should test these too
    if (!isfinite(*val)) {
        *val = 1.0;
    }
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    // Minimum size required: 2 Lab structs (24 bytes each) = 48 bytes
    if (Size < 48) {
        return 0;
    }

    cmsCIELab Lab1, Lab2;
    cmsFloat64Number Kl, Kc, Kh, l, c;

    // Parse Lab1
    parse_lab_from_data(Data, Size, &Lab1);
    
    // Parse Lab2 (offset by 24 bytes)
    parse_lab_from_data(Data + 24, Size > 24 ? Size - 24 : 0, &Lab2);

    // Parse weighting factors if available
    cmsFloat64Number default_factor = 1.0;
    
    // Kl
    if (Size > 48) parse_float64_from_data(Data + 48, Size - 48, &Kl);
    else Kl = default_factor;
    
    // Kc
    if (Size > 56) parse_float64_from_data(Data + 56, Size - 56, &Kc);
    else Kc = default_factor;

    // Kh
    if (Size > 64) parse_float64_from_data(Data + 64, Size - 64, &Kh);
    else Kh = default_factor;

    // l (CMC ratio)
    if (Size > 72) parse_float64_from_data(Data + 72, Size - 72, &l);
    else l = 2.0; 

    // c (CMC ratio)
    if (Size > 80) parse_float64_from_data(Data + 80, Size - 80, &c);
    else c = 1.0; 

    // 1. cmsCIE94DeltaE
    cmsFloat64Number delta94 = cmsCIE94DeltaE(&Lab1, &Lab2);
    (void)delta94;

    // 2. cmsCIE2000DeltaE
    cmsFloat64Number delta2000 = cmsCIE2000DeltaE(&Lab1, &Lab2, Kl, Kc, Kh);
    (void)delta2000;

    // 3. cmsCMCdeltaE
    cmsFloat64Number deltaCMC = cmsCMCdeltaE(&Lab1, &Lab2, l, c);
    (void)deltaCMC;

    // 4. cmsDesaturateLab
    cmsCIELab LabCopy = Lab1;
    double amax = 128.0, amin = -128.0, bmax = 128.0, bmin = -128.0;
    
    // We need 4 doubles (32 bytes) starting at offset 88.
    // Offset 88 + 32 = 120. So we need Size >= 120.
    if (Size >= 120) {
        memcpy(&amax, Data + 88, sizeof(double));
        memcpy(&amin, Data + 96, sizeof(double));
        memcpy(&bmax, Data + 104, sizeof(double));
        memcpy(&bmin, Data + 112, sizeof(double));
    }
    
    cmsBool result = cmsDesaturateLab(&LabCopy, amax, amin, bmax, bmin);
    (void)result;

    // 5. cmsDeltaE (CIE76)
    cmsFloat64Number delta76 = cmsDeltaE(&Lab1, &Lab2);
    (void)delta76;

    // 6. cmsBFDdeltaE
    cmsFloat64Number deltaBFD = cmsBFDdeltaE(&Lab1, &Lab2);
    (void)deltaBFD;

    return 0;
}