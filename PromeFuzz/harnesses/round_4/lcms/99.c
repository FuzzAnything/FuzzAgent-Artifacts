// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGDBAddPoint at cmssm.c:358:19 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGDBCompute at cmssm.c:550:19 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGDBCheckPoint at cmssm.c:390:19 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>

static void fuzz_cmsDesaturateLab(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsCIELab) + 4 * sizeof(double)) return;
    
    cmsCIELab lab;
    double amax, amin, bmax, bmin;
    
    memcpy(&lab, data, sizeof(cmsCIELab));
    data += sizeof(cmsCIELab);
    memcpy(&amax, data, sizeof(double));
    data += sizeof(double);
    memcpy(&amin, data, sizeof(double));
    data += sizeof(double);
    memcpy(&bmax, data, sizeof(double));
    data += sizeof(double);
    memcpy(&bmin, data, sizeof(double));
    
    cmsDesaturateLab(&lab, amax, amin, bmax, bmin);
}

static void fuzz_cmsGDBAddPoint(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsCIELab)) return;
    
    cmsContext context = NULL;
    cmsHANDLE hGBD = cmsGBDAlloc(context);
    if (!hGBD) return;
    
    cmsCIELab lab;
    memcpy(&lab, data, sizeof(cmsCIELab));
    
    cmsGDBAddPoint(hGBD, &lab);
    cmsGBDFree(hGBD);
}

static void fuzz_cmsGBDAlloc(const uint8_t* data, size_t size) {
    (void)data;
    (void)size;
    
    cmsContext context = NULL;
    cmsHANDLE hGBD = cmsGBDAlloc(context);
    if (hGBD) {
        cmsGBDFree(hGBD);
    }
}

static void fuzz_cmsGDBCompute(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsUInt32Number)) return;
    
    cmsContext context = NULL;
    cmsHANDLE hGBD = cmsGBDAlloc(context);
    if (!hGBD) return;
    
    cmsUInt32Number flags;
    memcpy(&flags, data, sizeof(cmsUInt32Number));
    
    cmsGDBCompute(hGBD, flags);
    cmsGBDFree(hGBD);
}

static void fuzz_cmsGBDFree(const uint8_t* data, size_t size) {
    (void)data;
    (void)size;
    
    cmsContext context = NULL;
    cmsHANDLE hGBD = cmsGBDAlloc(context);
    cmsGBDFree(hGBD);
    cmsGBDFree(NULL);
}

static void fuzz_cmsGDBCheckPoint(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsCIELab)) return;
    
    cmsContext context = NULL;
    cmsHANDLE hGBD = cmsGBDAlloc(context);
    if (!hGBD) return;
    
    cmsCIELab lab;
    memcpy(&lab, data, sizeof(cmsCIELab));
    
    cmsGDBCheckPoint(hGBD, &lab);
    cmsGBDFree(hGBD);
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    uint8_t selector = Data[0] % 6;
    Data++;
    Size--;
    
    switch (selector) {
        case 0:
            fuzz_cmsDesaturateLab(Data, Size);
            break;
        case 1:
            fuzz_cmsGDBAddPoint(Data, Size);
            break;
        case 2:
            fuzz_cmsGBDAlloc(Data, Size);
            break;
        case 3:
            fuzz_cmsGDBCompute(Data, Size);
            break;
        case 4:
            fuzz_cmsGBDFree(Data, Size);
            break;
        case 5:
            fuzz_cmsGDBCheckPoint(Data, Size);
            break;
    }
    
    return 0;
}