// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
// cmsFloat2LabEncodedV2 at cmspcs.c:254:16 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGDBAddPoint at cmssm.c:358:19 in lcms2.h
// cmsGDBCheckPoint at cmssm.c:390:19 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
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
#include <stdio.h>

static void init_lab_from_data(cmsCIELab* lab, const uint8_t* data, size_t size, size_t offset) {
    if (offset + sizeof(double) * 3 <= size) {
        const double* dbl_data = (const double*)(data + offset);
        lab->L = dbl_data[0];
        lab->a = dbl_data[1];
        lab->b = dbl_data[2];
    } else {
        lab->L = 50.0;
        lab->a = 0.0;
        lab->b = 0.0;
    }
}

static void init_double_from_data(double* val, const uint8_t* data, size_t size, size_t offset) {
    if (offset + sizeof(double) <= size) {
        const double* dbl_data = (const double*)(data + offset);
        *val = *dbl_data;
    } else {
        *val = 1.0;
    }
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    // Initialize Lab structures from fuzzer data
    cmsCIELab lab1, lab2;
    init_lab_from_data(&lab1, Data, Size, 0);
    init_lab_from_data(&lab2, Data, Size, sizeof(double) * 3);
    
    // Test cmsCIE94DeltaE
    cmsCIE94DeltaE(&lab1, &lab2);
    
    // Test cmsCMCdeltaE with fuzzed l and c parameters
    double l_param, c_param;
    init_double_from_data(&l_param, Data, Size, sizeof(double) * 6);
    init_double_from_data(&c_param, Data, Size, sizeof(double) * 7);
    cmsCMCdeltaE(&lab1, &lab2, l_param, c_param);
    
    // Test cmsFloat2LabEncodedV2
    cmsUInt16Number wLab[3];
    cmsFloat2LabEncodedV2(wLab, &lab1);
    
    // Test GDB functions with a temporary gamut boundary descriptor
    cmsHANDLE hGBD = cmsGBDAlloc(NULL);
    if (hGBD) {
        cmsGDBAddPoint(hGBD, &lab1);
        cmsGDBCheckPoint(hGBD, &lab2);
        cmsGBDFree(hGBD);
    }
    
    // Test cmsDesaturateLab with fuzzed bounds
    double amax, amin, bmax, bmin;
    init_double_from_data(&amax, Data, Size, sizeof(double) * 8);
    init_double_from_data(&amin, Data, Size, sizeof(double) * 9);
    init_double_from_data(&bmax, Data, Size, sizeof(double) * 10);
    init_double_from_data(&bmin, Data, Size, sizeof(double) * 11);
    
    cmsCIELab lab_copy = lab1;
    cmsDesaturateLab(&lab_copy, amax, amin, bmax, bmin);
    
    return 0;
}