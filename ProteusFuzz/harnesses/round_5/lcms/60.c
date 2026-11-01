// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lcms2.h"

static void fuzz_cmsBFDdeltaE(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELab) * 2) return;
    cmsCIELab lab1, lab2;
    memcpy(&lab1, Data, sizeof(cmsCIELab));
    memcpy(&lab2, Data + sizeof(cmsCIELab), sizeof(cmsCIELab));
    
    cmsFloat64Number result = cmsBFDdeltaE(&lab1, &lab2);
    (void)result;
}

static void fuzz_cmsCIE2000DeltaE(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELab) * 2 + sizeof(cmsFloat64Number) * 3) return;
    cmsCIELab lab1, lab2;
    cmsFloat64Number Kl, Kc, Kh;
    
    memcpy(&lab1, Data, sizeof(cmsCIELab));
    memcpy(&lab2, Data + sizeof(cmsCIELab), sizeof(cmsCIELab));
    memcpy(&Kl, Data + sizeof(cmsCIELab) * 2, sizeof(cmsFloat64Number));
    memcpy(&Kc, Data + sizeof(cmsCIELab) * 2 + sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
    memcpy(&Kh, Data + sizeof(cmsCIELab) * 2 + sizeof(cmsFloat64Number) * 2, sizeof(cmsFloat64Number));
    
    cmsFloat64Number result = cmsCIE2000DeltaE(&lab1, &lab2, Kl, Kc, Kh);
    (void)result;
}

static void fuzz_cmsDesaturateLab(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELab) + sizeof(double) * 4) return;
    cmsCIELab lab;
    double amax, amin, bmax, bmin;
    
    memcpy(&lab, Data, sizeof(cmsCIELab));
    memcpy(&amax, Data + sizeof(cmsCIELab), sizeof(double));
    memcpy(&amin, Data + sizeof(cmsCIELab) + sizeof(double), sizeof(double));
    memcpy(&bmax, Data + sizeof(cmsCIELab) + sizeof(double) * 2, sizeof(double));
    memcpy(&bmin, Data + sizeof(cmsCIELab) + sizeof(double) * 3, sizeof(double));
    
    cmsBool result = cmsDesaturateLab(&lab, amax, amin, bmax, bmin);
    (void)result;
}

static void fuzz_cmsCIE94DeltaE(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELab) * 2) return;
    cmsCIELab lab1, lab2;
    memcpy(&lab1, Data, sizeof(cmsCIELab));
    memcpy(&lab2, Data + sizeof(cmsCIELab), sizeof(cmsCIELab));
    
    cmsFloat64Number result = cmsCIE94DeltaE(&lab1, &lab2);
    (void)result;
}

static void fuzz_cmsDeltaE(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELab) * 2) return;
    cmsCIELab lab1, lab2;
    memcpy(&lab1, Data, sizeof(cmsCIELab));
    memcpy(&lab2, Data + sizeof(cmsCIELab), sizeof(cmsCIELab));
    
    cmsFloat64Number result = cmsDeltaE(&lab1, &lab2);
    (void)result;
}

static void fuzz_cmsCMCdeltaE(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELab) * 2 + sizeof(cmsFloat64Number) * 2) return;
    cmsCIELab lab1, lab2;
    cmsFloat64Number l, c;
    
    memcpy(&lab1, Data, sizeof(cmsCIELab));
    memcpy(&lab2, Data + sizeof(cmsCIELab), sizeof(cmsCIELab));
    memcpy(&l, Data + sizeof(cmsCIELab) * 2, sizeof(cmsFloat64Number));
    memcpy(&c, Data + sizeof(cmsCIELab) * 2 + sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
    
    cmsFloat64Number result = cmsCMCdeltaE(&lab1, &lab2, l, c);
    (void)result;
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELab) * 2) return 0;
    
    fuzz_cmsBFDdeltaE(Data, Size);
    fuzz_cmsCIE2000DeltaE(Data, Size);
    fuzz_cmsDesaturateLab(Data, Size);
    fuzz_cmsCIE94DeltaE(Data, Size);
    fuzz_cmsDeltaE(Data, Size);
    fuzz_cmsCMCdeltaE(Data, Size);
    
    return 0;
}