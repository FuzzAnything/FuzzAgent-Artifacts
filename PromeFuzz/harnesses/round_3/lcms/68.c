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
#include <stdint.h>
#include <string.h>
#include "lcms2.h"

static void parse_lab_from_data(const uint8_t *data, size_t size, cmsCIELab *lab1, cmsCIELab *lab2) {
    // Initialize with default values
    lab1->L = 50.0;
    lab1->a = 0.0;
    lab1->b = 0.0;
    lab2->L = 50.0;
    lab2->a = 0.0;
    lab2->b = 0.0;
    
    // Parse data to fill Lab structures if enough data is available
    if (size >= 6 * sizeof(double)) {
        const double *doubles = (const double *)data;
        lab1->L = doubles[0];
        lab1->a = doubles[1];
        lab1->b = doubles[2];
        lab2->L = doubles[3];
        lab2->a = doubles[4];
        lab2->b = doubles[5];
    }
}

static double parse_double_param(const uint8_t *data, size_t size, size_t offset, double default_val) {
    if (size >= (offset + 1) * sizeof(double)) {
        const double *doubles = (const double *)data;
        return doubles[offset];
    }
    return default_val;
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    // Parse Lab structures from input data
    cmsCIELab lab1, lab2;
    parse_lab_from_data(Data, Size, &lab1, &lab2);
    
    // Parse additional parameters for functions that need them
    double param_l = parse_double_param(Data, Size, 6, 1.0);
    double param_c = parse_double_param(Data, Size, 7, 1.0);
    double param_amax = parse_double_param(Data, Size, 8, 127.0);
    double param_amin = parse_double_param(Data, Size, 9, -128.0);
    double param_bmax = parse_double_param(Data, Size, 10, 127.0);
    double param_bmin = parse_double_param(Data, Size, 11, -128.0);
    double param_Kl = parse_double_param(Data, Size, 12, 1.0);
    double param_Kc = parse_double_param(Data, Size, 13, 1.0);
    double param_Kh = parse_double_param(Data, Size, 14, 1.0);
    
    // Call target API functions
    cmsFloat64Number deltaE = cmsDeltaE(&lab1, &lab2);
    (void)deltaE;  // Use result to avoid unused variable warning
    
    cmsFloat64Number cmcDeltaE = cmsCMCdeltaE(&lab1, &lab2, param_l, param_c);
    (void)cmcDeltaE;
    
    // For cmsDesaturateLab, we need to work on a copy since it modifies the input
    cmsCIELab lab_copy = lab1;
    cmsBool desaturate_result = cmsDesaturateLab(&lab_copy, param_amax, param_amin, param_bmax, param_bmin);
    (void)desaturate_result;
    
    cmsFloat64Number cie2000DeltaE = cmsCIE2000DeltaE(&lab1, &lab2, param_Kl, param_Kc, param_Kh);
    (void)cie2000DeltaE;
    
    cmsFloat64Number bfdDeltaE = cmsBFDdeltaE(&lab1, &lab2);
    (void)bfdDeltaE;
    
    cmsFloat64Number cie94DeltaE = cmsCIE94DeltaE(&lab1, &lab2);
    (void)cie94DeltaE;
    
    return 0;
}