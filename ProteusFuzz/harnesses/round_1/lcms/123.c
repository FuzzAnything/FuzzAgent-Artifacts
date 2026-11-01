// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <lcms2.h>

/* 
 * Fuzz driver for lcms color difference and conversion functions.
 * Targets: cmsCMCdeltaE, cmsLab2LCh, cmsLCh2Lab, cmsBFDdeltaE, 
 *          cmsDeltaE, cmsCIE94DeltaE
 */

static void fuzz_cmsLab2LCh(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELab)) return;

    cmsCIELab lab;
    cmsCIELCh lch;

    memcpy(&lab, Data, sizeof(cmsCIELab));
    
    /* Ensure L is within reasonable bounds to avoid NaN/Inf in some implementations */
    if (lab.L < 0.0) lab.L = 0.0;
    if (lab.L > 100.0) lab.L = 100.0;

    cmsLab2LCh(&lch, &lab);
}

static void fuzz_cmsLCh2Lab(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELCh)) return;

    cmsCIELCh lch;
    cmsCIELab lab;

    memcpy(&lch, Data, sizeof(cmsCIELCh));

    /* Ensure L is within reasonable bounds */
    if (lch.L < 0.0) lch.L = 0.0;
    if (lch.L > 100.0) lch.L = 100.0;
    if (lch.C < 0.0) lch.C = 0.0;

    cmsLCh2Lab(&lab, &lch);
}

static void fuzz_delta_e_functions(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELab) * 2) return;

    cmsCIELab lab1, lab2;
    cmsFloat64Number l_ratio, c_ratio;
    cmsFloat64Number result;

    memcpy(&lab1, Data, sizeof(cmsCIELab));
    memcpy(&lab2, Data + sizeof(cmsCIELab), sizeof(cmsCIELab));

    /* Normalize L values to avoid potential issues */
    if (lab1.L < 0.0) lab1.L = 0.0;
    if (lab1.L > 100.0) lab1.L = 100.0;
    if (lab2.L < 0.0) lab2.L = 0.0;
    if (lab2.L > 100.0) lab2.L = 100.0;

    /* Generate ratios from fuzz data if available */
    if (Size >= sizeof(cmsCIELab) * 2 + sizeof(cmsFloat64Number) * 2) {
        memcpy(&l_ratio, Data + sizeof(cmsCIELab) * 2, sizeof(cmsFloat64Number));
        memcpy(&c_ratio, Data + sizeof(cmsCIELab) * 2 + sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
        
        /* Ensure ratios are positive */
        if (l_ratio <= 0.0) l_ratio = 1.0;
        if (c_ratio <= 0.0) c_ratio = 1.0;
    } else {
        l_ratio = 2.0;
        c_ratio = 1.0;
    }

    /* Fuzz cmsCMCdeltaE */
    result = cmsCMCdeltaE(&lab1, &lab2, l_ratio, c_ratio);
    (void)result;

    /* Fuzz cmsBFDdeltaE */
    result = cmsBFDdeltaE(&lab1, &lab2);
    (void)result;

    /* Fuzz cmsDeltaE */
    result = cmsDeltaE(&lab1, &lab2);
    (void)result;

    /* Fuzz cmsCIE94DeltaE */
    result = cmsCIE94DeltaE(&lab1, &lab2);
    (void)result;
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* 
     * We split the input data to exercise different functions.
     * The fuzzer will naturally provide various sizes and patterns.
     */
    
    /* Exercise conversion functions */
    fuzz_cmsLab2LCh(Data, Size);
    fuzz_cmsLCh2Lab(Data, Size);
    
    /* Exercise delta E functions */
    fuzz_delta_e_functions(Data, Size);

    return 0;
}