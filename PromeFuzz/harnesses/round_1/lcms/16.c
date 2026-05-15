// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void parse_input(const uint8_t *data, size_t size, 
                       cmsCIELab *lab1, cmsCIELab *lab2,
                       double *extra_params, size_t max_params) {
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
    
    // Parse data to fill structures if we have enough
    if (size >= sizeof(double) * 6) {
        const double *d = (const double*)data;
        if (lab1) {
            lab1->L = d[0];
            lab1->a = d[1];
            lab1->b = d[2];
        }
        if (lab2) {
            lab2->L = d[3];
            lab2->a = d[4];
            lab2->b = d[5];
        }
        
        // Parse extra parameters if requested
        if (extra_params && max_params > 0) {
            size_t params_to_read = (size / sizeof(double)) - 6;
            if (params_to_read > max_params) params_to_read = max_params;
            
            for (size_t i = 0; i < params_to_read; i++) {
                extra_params[i] = d[6 + i];
            }
        }
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double) * 2) {
        return 0;
    }
    
    cmsCIELab lab1, lab2;
    double extra_params[5] = {1.0, 1.0, 1.0, 1.0, 1.0};
    
    // Parse input data
    parse_input(Data, Size, &lab1, &lab2, extra_params, 5);
    
    // Test cmsBFDdeltaE
    cmsBFDdeltaE(&lab1, &lab2);
    
    // Test cmsCMCdeltaE
    cmsCMCdeltaE(&lab1, &lab2, extra_params[0], extra_params[1]);
    
    // Test cmsCIE2000DeltaE
    cmsCIE2000DeltaE(&lab1, &lab2, extra_params[0], extra_params[1], extra_params[2]);
    
    // Test cmsCIE94DeltaE
    cmsCIE94DeltaE(&lab1, &lab2);
    
    // Test cmsDeltaE
    cmsDeltaE(&lab1, &lab2);
    
    // Test cmsDesaturateLab
    cmsCIELab lab_desat = lab1;
    cmsDesaturateLab(&lab_desat, extra_params[3], extra_params[4], 
                     extra_params[0], extra_params[1]);
    
    return 0;
}