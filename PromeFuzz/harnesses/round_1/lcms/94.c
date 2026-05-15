// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>

static void parse_lab_from_data(const uint8_t *data, size_t size, size_t offset, cmsCIELab *lab) {
    if (offset + 24 <= size) {
        memcpy(&lab->L, data + offset, 8);
        memcpy(&lab->a, data + offset + 8, 8);
        memcpy(&lab->b, data + offset + 16, 8);
    } else {
        lab->L = lab->a = lab->b = 0.0;
    }
}

static void parse_lch_from_data(const uint8_t *data, size_t size, size_t offset, cmsCIELCh *lch) {
    if (offset + 24 <= size) {
        memcpy(&lch->L, data + offset, 8);
        memcpy(&lch->C, data + offset + 8, 8);
        memcpy(&lch->h, data + offset + 16, 8);
    } else {
        lch->L = lch->C = lch->h = 0.0;
    }
}

static cmsFloat64Number parse_double(const uint8_t *data, size_t size, size_t offset) {
    cmsFloat64Number value = 0.0;
    if (offset + 8 <= size) {
        memcpy(&value, data + offset, 8);
    }
    return value;
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size < 24) return 0;

    cmsCIELab lab1, lab2;
    cmsCIELCh lch1, lch2;
    cmsFloat64Number result;

    // Parse first Lab color (24 bytes)
    parse_lab_from_data(Data, Size, 0, &lab1);
    
    // Parse second Lab color (24 bytes)
    parse_lab_from_data(Data, Size, 24, &lab2);
    
    // Parse first LCh color (24 bytes)
    parse_lch_from_data(Data, Size, 48, &lch1);
    
    // Parse second LCh color (24 bytes)
    parse_lch_from_data(Data, Size, 72, &lch2);
    
    // Parse parameters for cmsCIE2000DeltaE (24 bytes)
    cmsFloat64Number Kl = parse_double(Data, Size, 96);
    cmsFloat64Number Kc = parse_double(Data, Size, 104);
    cmsFloat64Number Kh = parse_double(Data, Size, 112);

    // Call target API functions
    result = cmsBFDdeltaE(&lab1, &lab2);
    result = cmsCIE2000DeltaE(&lab1, &lab2, Kl, Kc, Kh);
    result = cmsCIE94DeltaE(&lab1, &lab2);
    
    cmsLCh2Lab(&lab1, &lch1);
    cmsLCh2Lab(&lab2, &lch2);
    
    result = cmsDeltaE(&lab1, &lab2);
    
    cmsLab2LCh(&lch1, &lab1);
    cmsLab2LCh(&lch2, &lab2);

    return 0;
}