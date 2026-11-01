// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGDBAddPoint at cmssm.c:358:19 in lcms2.h
// cmsGDBCompute at cmssm.c:550:19 in lcms2.h
// cmsGDBCheckPoint at cmssm.c:390:19 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static void cleanup_gbd(cmsHANDLE hGBD) {
    if (hGBD != NULL) {
        cmsGBDFree(hGBD);
    }
}

int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    cmsHANDLE hGBD = cmsGBDAlloc(ContextID);
    if (hGBD == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    cmsCIELab Lab = {0.0, 0.0, 0.0};
    if (Size >= 24) {
        memcpy(&Lab.L, Data, sizeof(double));
        memcpy(&Lab.a, Data + 8, sizeof(double));
        memcpy(&Lab.b, Data + 16, sizeof(double));
    } else if (Size >= 8) {
        Lab.L = *(double*)Data;
        if (Size >= 16) {
            Lab.a = *(double*)(Data + 8);
        }
        if (Size >= 24) {
            Lab.b = *(double*)(Data + 16);
        }
    }

    cmsBool res1 = cmsGDBAddPoint(hGBD, &Lab);
    (void)res1;

    cmsBool res2 = cmsGDBCompute(hGBD, 0);
    (void)res2;

    cmsBool res3 = cmsGDBCheckPoint(hGBD, &Lab);
    (void)res3;

    cmsCIELab LabCopy = Lab;
    double amax = 128.0, amin = -128.0, bmax = 128.0, bmin = -128.0;
    if (Size >= 48) {
        memcpy(&amax, Data + 24, sizeof(double));
        memcpy(&amin, Data + 32, sizeof(double));
        memcpy(&bmax, Data + 40, sizeof(double));
        memcpy(&bmin, Data + 48, sizeof(double));
    }
    cmsBool res4 = cmsDesaturateLab(&LabCopy, amax, amin, bmax, bmin);
    (void)res4;

    cmsGBDFree(hGBD);
    cmsDeleteContext(ContextID);

    return 0;
}