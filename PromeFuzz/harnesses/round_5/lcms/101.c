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
#include <string.h>

static void parse_lab(const uint8_t *data, size_t size, size_t *offset, cmsCIELab *lab) {
    if (*offset + sizeof(double) * 3 <= size) {
        memcpy(&lab->L, data + *offset, sizeof(double));
        *offset += sizeof(double);
        memcpy(&lab->a, data + *offset, sizeof(double));
        *offset += sizeof(double);
        memcpy(&lab->b, data + *offset, sizeof(double));
        *offset += sizeof(double);
    } else {
        lab->L = lab->a = lab->b = 0.0;
    }
}

static double parse_double(const uint8_t *data, size_t size, size_t *offset) {
    double val = 0.0;
    if (*offset + sizeof(double) <= size) {
        memcpy(&val, data + *offset, sizeof(double));
        *offset += sizeof(double);
    }
    return val;
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double) * 3) return 0;
    
    size_t offset = 0;
    
    // Parse Lab colors for various functions
    cmsCIELab lab1, lab2;
    parse_lab(Data, Size, &offset, &lab1);
    parse_lab(Data, Size, &offset, &lab2);
    
    // 1. Test cmsCIE94DeltaE
    cmsCIE94DeltaE(&lab1, &lab2);
    
    // 2. Test cmsCMCdeltaE with parameters from fuzzer input
    double l = parse_double(Data, Size, &offset);
    double c = parse_double(Data, Size, &offset);
    cmsCMCdeltaE(&lab1, &lab2, l, c);
    
    // 3. Test cmsFloat2LabEncodedV2
    cmsUInt16Number wLab[3];
    cmsFloat2LabEncodedV2(wLab, &lab1);
    
    // 4. & 5. Test GDB functions with a temporary descriptor
    cmsHANDLE hGBD = cmsGBDAlloc(NULL);
    if (hGBD) {
        cmsGDBAddPoint(hGBD, &lab1);
        cmsGDBCheckPoint(hGBD, &lab2);
        cmsGBDFree(hGBD);
    }
    
    // 6. Test cmsDesaturateLab with bounds from fuzzer input
    double amax = parse_double(Data, Size, &offset);
    double amin = parse_double(Data, Size, &offset);
    double bmax = parse_double(Data, Size, &offset);
    double bmin = parse_double(Data, Size, &offset);
    cmsDesaturateLab(&lab1, amax, amin, bmax, bmin);
    
    return 0;
}