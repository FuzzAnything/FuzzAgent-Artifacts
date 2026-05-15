// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static void parse_input(const uint8_t *data, size_t size, 
                       cmsCIELab *lab1, cmsCIELab *lab2, 
                       cmsCIELCh *lch1, cmsCIELCh *lch2,
                       double *l_weight, double *c_weight) {
    // Initialize with default values
    if (lab1) {
        lab1->L = 50.0;
        lab1->a = 0.0;
        lab1->b = 0.0;
    }
    if (lab2) {
        lab2->L = 50.0;
        lab2->a = 0.0;
        lab2->b = 0.0;
    }
    if (lch1) {
        lch1->L = 50.0;
        lch1->C = 0.0;
        lch1->h = 0.0;
    }
    if (lch2) {
        lch2->L = 50.0;
        lch2->C = 0.0;
        lch2->h = 0.0;
    }
    if (l_weight) *l_weight = 1.0;
    if (c_weight) *c_weight = 1.0;
    
    // Parse available data
    size_t offset = 0;
    size_t double_size = sizeof(double);
    
    if (lab1 && offset + 3*double_size <= size) {
        memcpy(&lab1->L, data + offset, double_size);
        offset += double_size;
        memcpy(&lab1->a, data + offset, double_size);
        offset += double_size;
        memcpy(&lab1->b, data + offset, double_size);
        offset += double_size;
    }
    
    if (lab2 && offset + 3*double_size <= size) {
        memcpy(&lab2->L, data + offset, double_size);
        offset += double_size;
        memcpy(&lab2->a, data + offset, double_size);
        offset += double_size;
        memcpy(&lab2->b, data + offset, double_size);
        offset += double_size;
    }
    
    if (lch1 && offset + 3*double_size <= size) {
        memcpy(&lch1->L, data + offset, double_size);
        offset += double_size;
        memcpy(&lch1->C, data + offset, double_size);
        offset += double_size;
        memcpy(&lch1->h, data + offset, double_size);
        offset += double_size;
    }
    
    if (lch2 && offset + 3*double_size <= size) {
        memcpy(&lch2->L, data + offset, double_size);
        offset += double_size;
        memcpy(&lch2->C, data + offset, double_size);
        offset += double_size;
        memcpy(&lch2->h, data + offset, double_size);
        offset += double_size;
    }
    
    if (l_weight && offset + double_size <= size) {
        memcpy(l_weight, data + offset, double_size);
        offset += double_size;
    }
    
    if (c_weight && offset + double_size <= size) {
        memcpy(c_weight, data + offset, double_size);
    }
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double)) {
        return 0;
    }
    
    // Parse input data into structures
    cmsCIELab lab1, lab2;
    cmsCIELCh lch1, lch2;
    double l_weight = 1.0, c_weight = 1.0;
    
    parse_input(Data, Size, &lab1, &lab2, &lch1, &lch2, &l_weight, &c_weight);
    
    // Test cmsDeltaE
    double deltaE = cmsDeltaE(&lab1, &lab2);
    (void)deltaE; // Use result to avoid unused variable warning
    
    // Test cmsLCh2Lab
    cmsCIELab lab_from_lch;
    cmsLCh2Lab(&lab_from_lch, &lch1);
    
    // Test cmsCMCdeltaE
    double cmc_deltaE = cmsCMCdeltaE(&lab1, &lab2, l_weight, c_weight);
    (void)cmc_deltaE;
    
    // Test cmsLab2LCh
    cmsCIELCh lch_from_lab;
    cmsLab2LCh(&lch_from_lab, &lab1);
    
    // Test cmsBFDdeltaE
    double bfd_deltaE = cmsBFDdeltaE(&lab1, &lab2);
    (void)bfd_deltaE;
    
    // Test cmsCIE94DeltaE
    double cie94_deltaE = cmsCIE94DeltaE(&lab1, &lab2);
    (void)cie94_deltaE;
    
    // Additional test: chain conversions
    cmsCIELCh temp_lch;
    cmsCIELab temp_lab;
    
    // Lab -> LCh -> Lab
    cmsLab2LCh(&temp_lch, &lab1);
    cmsLCh2Lab(&temp_lab, &temp_lch);
    
    // LCh -> Lab -> LCh
    cmsLCh2Lab(&temp_lab, &lch1);
    cmsLab2LCh(&temp_lch, &temp_lab);
    
    // Test with extreme values
    cmsCIELab extreme_lab = {100.0, 128.0, 128.0};
    cmsCIELCh extreme_lch = {100.0, 128.0, 360.0};
    
    cmsDeltaE(&extreme_lab, &lab1);
    cmsLCh2Lab(&temp_lab, &extreme_lch);
    cmsLab2LCh(&temp_lch, &extreme_lab);
    
    // Test with negative values
    cmsCIELab negative_lab = {-10.0, -128.0, -128.0};
    cmsCIELCh negative_lch = {-10.0, -50.0, -180.0};
    
    cmsDeltaE(&negative_lab, &lab1);
    cmsLCh2Lab(&temp_lab, &negative_lch);
    cmsLab2LCh(&temp_lch, &negative_lab);
    
    // Test with NaN and Inf values if present in input
    // (handled by the parse_input function which uses memcpy)
    
    return 0;
}