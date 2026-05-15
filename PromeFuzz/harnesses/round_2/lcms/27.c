// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void parse_lab_from_data(const uint8_t *data, size_t size, size_t offset, cmsCIELab *lab) {
    if (offset + sizeof(cmsCIELab) <= size) {
        memcpy(lab, data + offset, sizeof(cmsCIELab));
    } else {
        lab->L = 50.0;
        lab->a = 0.0;
        lab->b = 0.0;
    }
}

static double parse_double_from_data(const uint8_t *data, size_t size, size_t offset, double default_val) {
    double val;
    if (offset + sizeof(double) <= size) {
        memcpy(&val, data + offset, sizeof(double));
    } else {
        val = default_val;
    }
    return val;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELab) * 2) {
        return 0;
    }

    cmsCIELab lab1, lab2;
    size_t offset = 0;
    
    parse_lab_from_data(Data, Size, offset, &lab1);
    offset += sizeof(cmsCIELab);
    
    parse_lab_from_data(Data, Size, offset, &lab2);
    offset += sizeof(cmsCIELab);

    double Kl = parse_double_from_data(Data, Size, offset, 1.0);
    offset += sizeof(double);
    
    double Kc = parse_double_from_data(Data, Size, offset, 1.0);
    offset += sizeof(double);
    
    double Kh = parse_double_from_data(Data, Size, offset, 1.0);
    offset += sizeof(double);

    double l = parse_double_from_data(Data, Size, offset, 1.0);
    offset += sizeof(double);
    
    double c = parse_double_from_data(Data, Size, offset, 1.0);
    offset += sizeof(double);

    double amax = parse_double_from_data(Data, Size, offset, 127.0);
    offset += sizeof(double);
    
    double amin = parse_double_from_data(Data, Size, offset, -128.0);
    offset += sizeof(double);
    
    double bmax = parse_double_from_data(Data, Size, offset, 127.0);
    offset += sizeof(double);
    
    double bmin = parse_double_from_data(Data, Size, offset, -128.0);
    offset += sizeof(double);

    cmsCIELab lab_copy = lab1;
    cmsDesaturateLab(&lab_copy, amax, amin, bmax, bmin);

    cmsCIE2000DeltaE(&lab1, &lab2, Kl, Kc, Kh);
    cmsBFDdeltaE(&lab1, &lab2);
    cmsCMCdeltaE(&lab1, &lab2, l, c);
    cmsCIE94DeltaE(&lab1, &lab2);
    cmsDeltaE(&lab1, &lab2);

    return 0;
}