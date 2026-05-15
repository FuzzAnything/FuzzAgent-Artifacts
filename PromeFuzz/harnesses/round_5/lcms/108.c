// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "lcms2.h"

static void parse_lab_from_data(const uint8_t *data, size_t size, size_t offset, cmsCIELab *lab) {
    if (offset + sizeof(cmsCIELab) <= size) {
        memcpy(lab, data + offset, sizeof(cmsCIELab));
    } else {
        lab->L = 50.0;
        lab->a = 0.0;
        lab->b = 0.0;
    }
}

static void parse_lch_from_data(const uint8_t *data, size_t size, size_t offset, cmsCIELCh *lch) {
    if (offset + sizeof(cmsCIELCh) <= size) {
        memcpy(lch, data + offset, sizeof(cmsCIELCh));
    } else {
        lch->L = 50.0;
        lch->C = 0.0;
        lch->h = 0.0;
    }
}

static double parse_double_from_data(const uint8_t *data, size_t size, size_t offset, double default_val) {
    double val = default_val;
    if (offset + sizeof(double) <= size) {
        memcpy(&val, data + offset, sizeof(double));
    }
    return val;
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsCIELab lab1, lab2;
    cmsCIELCh lch1, lch2;
    double Kl, Kc, Kh;

    size_t offset = 0;
    parse_lab_from_data(Data, Size, offset, &lab1);
    offset += sizeof(cmsCIELab);
    
    parse_lab_from_data(Data, Size, offset, &lab2);
    offset += sizeof(cmsCIELab);
    
    parse_lch_from_data(Data, Size, offset, &lch1);
    offset += sizeof(cmsCIELCh);
    
    parse_lch_from_data(Data, Size, offset, &lch2);
    offset += sizeof(cmsCIELCh);
    
    Kl = parse_double_from_data(Data, Size, offset, 1.0);
    offset += sizeof(double);
    
    Kc = parse_double_from_data(Data, Size, offset, 1.0);
    offset += sizeof(double);
    
    Kh = parse_double_from_data(Data, Size, offset, 1.0);
    offset += sizeof(double);

    double result1 = cmsCIE2000DeltaE(&lab1, &lab2, Kl, Kc, Kh);
    (void)result1;

    double result2 = cmsCIE94DeltaE(&lab1, &lab2);
    (void)result2;

    cmsLab2LCh(&lch1, &lab1);
    cmsLCh2Lab(&lab2, &lch2);

    double result3 = cmsDeltaE(&lab1, &lab2);
    (void)result3;

    double result4 = cmsBFDdeltaE(&lab1, &lab2);
    (void)result4;

    return 0;
}