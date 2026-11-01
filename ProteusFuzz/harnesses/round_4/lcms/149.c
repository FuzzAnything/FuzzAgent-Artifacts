// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGDBAddPoint at cmssm.c:358:19 in lcms2.h
// cmsGDBCheckPoint at cmssm.c:390:19 in lcms2.h
// cmsGDBCompute at cmssm.c:550:19 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <lcms2.h>

static void fuzz_cmsDesaturateLab(const uint8_t *Data, size_t Size) {
    cmsCIELab Lab = {0, 0, 0};
    double amax, amin, bmax, bmin;

    if (Size >= 4) {
        Lab.L = (double)Data[0] * 2.55; // 0-255 -> 0-650.25
        Lab.a = (double)Data[1] * 2.55 - 128; // -128 to 127 -> -326.4 to 323.85
        Lab.b = (double)Data[2] * 2.55 - 128;
    }

    if (Size >= 8) {
        amax = (double)Data[4] * 2.55;
        amin = (double)Data[5] * 2.55 - 128;
        bmax = (double)Data[6] * 2.55;
        bmin = (double)Data[7] * 2.55 - 128;
    } else {
        amax = 100.0;
        amin = -100.0;
        bmax = 100.0;
        bmin = -100.0;
    }

    cmsDesaturateLab(&Lab, amax, amin, bmax, bmin);
}

static void fuzz_cmsGDB(const uint8_t *Data, size_t Size) {
    cmsContext Context = cmsCreateContext(NULL, NULL);
    if (!Context) return;

    cmsHANDLE hGBD = cmsGBDAlloc(Context);
    if (!hGBD) {
        cmsDeleteContext(Context);
        return;
    }

    cmsCIELab Lab = {0, 0, 0};
    if (Size >= 3) {
        Lab.L = (double)Data[0] * 2.55;
        Lab.a = (double)Data[1] * 2.55 - 128;
        Lab.b = (double)Data[2] * 2.55 - 128;
    }

    cmsGDBAddPoint(hGBD, &Lab);

    cmsBool checkResult = cmsGDBCheckPoint(hGBD, &Lab);
    (void)checkResult;

    cmsUInt32Number flags = 0;
    if (Size >= 7) {
        flags = (cmsUInt32Number)Data[3] | ((cmsUInt32Number)Data[4] << 8) | 
                ((cmsUInt32Number)Data[5] << 16) | ((cmsUInt32Number)Data[6] << 24);
    }

    cmsBool computeResult = cmsGDBCompute(hGBD, flags);
    (void)computeResult;

    cmsGBDFree(hGBD);
    cmsDeleteContext(Context);
}

int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz cmsDesaturateLab
    fuzz_cmsDesaturateLab(Data, Size);

    // Fuzz cmsGDB related functions
    fuzz_cmsGDB(Data, Size);

    return 0;
}