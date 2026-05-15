// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGDBAddPoint at cmssm.c:358:19 in lcms2.h
// cmsGDBCheckPoint at cmssm.c:390:19 in lcms2.h
// cmsGDBCheckPoint at cmssm.c:390:19 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCMCdeltaE at cmspcs.c:548:28 in lcms2.h
// cmsFloat2LabEncodedV2 at cmspcs.c:254:16 in lcms2.h
// cmsFloat2LabEncodedV2 at cmspcs.c:254:16 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
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

static double readDouble(const uint8_t **data, size_t *size) {
    if (*size < sizeof(double)) return 0.0;
    double val;
    memcpy(&val, *data, sizeof(double));
    *data += sizeof(double);
    *size -= sizeof(double);
    return val;
}

static void initLabFromData(cmsCIELab *lab, const uint8_t **data, size_t *size) {
    lab->L = readDouble(data, size);
    lab->a = readDouble(data, size);
    lab->b = readDouble(data, size);
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size < 3 * sizeof(double)) return 0;

    const uint8_t *ptr = Data;
    size_t remaining = Size;

    cmsCIELab lab1, lab2;
    initLabFromData(&lab1, &ptr, &remaining);
    
    if (remaining >= 3 * sizeof(double)) {
        initLabFromData(&lab2, &ptr, &remaining);
    } else {
        lab2 = lab1;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHANDLE hGBD = cmsGBDAlloc(ctx);
    if (hGBD) {
        cmsGDBAddPoint(hGBD, &lab1);
        cmsGDBCheckPoint(hGBD, &lab1);
        cmsGDBCheckPoint(hGBD, &lab2);
        cmsGBDFree(hGBD);
    }
    cmsDeleteContext(ctx);

    double l = 1.0, c = 1.0;
    if (remaining >= sizeof(double)) {
        l = readDouble(&ptr, &remaining);
    }
    if (remaining >= sizeof(double)) {
        c = readDouble(&ptr, &remaining);
    }
    cmsCMCdeltaE(&lab1, &lab2, l, c);

    cmsUInt16Number wLab[3];
    cmsFloat2LabEncodedV2(wLab, &lab1);
    cmsFloat2LabEncodedV2(wLab, &lab2);

    cmsCIE94DeltaE(&lab1, &lab2);

    double amax = 100.0, amin = -100.0, bmax = 100.0, bmin = -100.0;
    if (remaining >= 4 * sizeof(double)) {
        amax = readDouble(&ptr, &remaining);
        amin = readDouble(&ptr, &remaining);
        bmax = readDouble(&ptr, &remaining);
        bmin = readDouble(&ptr, &remaining);
    }
    cmsCIELab lab_copy = lab1;
    cmsDesaturateLab(&lab_copy, amax, amin, bmax, bmin);

    return 0;
}