// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <lcms2.h>

static cmsCIELab generate_lab_from_data(const uint8_t *data, size_t size, size_t offset) {
    cmsCIELab lab;
    memset(&lab, 0, sizeof(lab));

    // Need 3 doubles (24 bytes) for L, a, b
    if (size < offset + 24) {
        lab.L = 50.0;
        lab.a = 0.0;
        lab.b = 0.0;
        return lab;
    }

    double l_val, a_val, b_val;
    memcpy(&l_val, data + offset, sizeof(double));
    memcpy(&a_val, data + offset + sizeof(double), sizeof(double));
    memcpy(&b_val, data + offset + 2 * sizeof(double), sizeof(double));

    // Clamp L to [0, 100]
    if (l_val < 0.0) l_val = 0.0;
    if (l_val > 100.0) l_val = 100.0;

    // Clamp a and b to reasonable ranges [-128, 127]
    if (a_val < -128.0) a_val = -128.0;
    if (a_val > 127.0) a_val = 127.0;
    if (b_val < -128.0) b_val = -128.0;
    if (b_val > 127.0) b_val = 127.0;

    lab.L = l_val;
    lab.a = a_val;
    lab.b = b_val;

    return lab;
}

static cmsFloat64Number generate_double_from_data(const uint8_t *data, size_t size, size_t offset) {
    if (size < offset + sizeof(double)) {
        return 1.0;
    }
    cmsFloat64Number val;
    memcpy(&val, data + offset, sizeof(double));
    return val;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Minimum size required: 
    // 2 Lab structs (24 bytes each) = 48 bytes
    // 8 doubles for parameters (8 bytes each) = 64 bytes
    // Total = 112 bytes
    if (Size < 112) {
        return 0;
    }

    // Generate two Lab values
    cmsCIELab lab1 = generate_lab_from_data(Data, Size, 0);
    cmsCIELab lab2 = generate_lab_from_data(Data, Size, 24);

    // Generate weighting factors for CIE2000 and CMC
    cmsFloat64Number Kl = generate_double_from_data(Data, Size, 48);
    cmsFloat64Number Kc = generate_double_from_data(Data, Size, 56);
    cmsFloat64Number Kh = generate_double_from_data(Data, Size, 64);
    cmsFloat64Number l_ratio = generate_double_from_data(Data, Size, 72);
    cmsFloat64Number c_ratio = generate_double_from_data(Data, Size, 80);

    // Test cmsCIE2000DeltaE
    cmsFloat64Number deltaE2000 = cmsCIE2000DeltaE(&lab1, &lab2, Kl, Kc, Kh);
    (void)deltaE2000;

    // Test cmsCMCdeltaE
    cmsFloat64Number deltaECMC = cmsCMCdeltaE(&lab1, &lab2, l_ratio, c_ratio);
    (void)deltaECMC;

    // Test cmsBFDdeltaE
    cmsFloat64Number deltaEBFD = cmsBFDdeltaE(&lab1, &lab2);
    (void)deltaEBFD;

    // Test cmsDeltaE (CIE76)
    cmsFloat64Number deltaE76 = cmsDeltaE(&lab1, &lab2);
    (void)deltaE76;

    // Test cmsCIE94DeltaE
    cmsFloat64Number deltaE94 = cmsCIE94DeltaE(&lab1, &lab2);
    (void)deltaE94;

    // Test cmsDesaturateLab
    cmsCIELab lab3 = lab1; 
    double amax = generate_double_from_data(Data, Size, 88);
    double amin = generate_double_from_data(Data, Size, 96);
    double bmax = generate_double_from_data(Data, Size, 104);
    double bmin = generate_double_from_data(Data, Size, 112);

    cmsBool result = cmsDesaturateLab(&lab3, amax, amin, bmax, bmin);
    (void)result;

    return 0;
}