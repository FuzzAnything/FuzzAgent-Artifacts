// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsCIE94DeltaE at cmspcs.c:451:28 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcms2.h"

static void init_structs_from_fuzz(const uint8_t *Data, size_t Size, cmsCIEXYZ *xyz, cmsCIELab *lab1, cmsCIELab *lab2, cmsCIELab *lab3, cmsCIEXYZ *whitePoint) {
    // Initialize with safe defaults
    memset(xyz, 0, sizeof(cmsCIEXYZ));
    memset(lab1, 0, sizeof(cmsCIELab));
    memset(lab2, 0, sizeof(cmsCIELab));
    memset(lab3, 0, sizeof(cmsCIELab));
    memset(whitePoint, 0, sizeof(cmsCIEXYZ));

    if (Size >= 3 * sizeof(double)) {
        memcpy(&xyz->X, Data, sizeof(double));
        memcpy(&xyz->Y, Data + sizeof(double), sizeof(double));
        memcpy(&xyz->Z, Data + 2 * sizeof(double), sizeof(double));
    }

    if (Size >= 6 * sizeof(double)) {
        memcpy(&lab1->L, Data + 3 * sizeof(double), sizeof(double));
        memcpy(&lab1->a, Data + 4 * sizeof(double), sizeof(double));
        memcpy(&lab1->b, Data + 5 * sizeof(double), sizeof(double));
    }

    if (Size >= 9 * sizeof(double)) {
        memcpy(&lab2->L, Data + 6 * sizeof(double), sizeof(double));
        memcpy(&lab2->a, Data + 7 * sizeof(double), sizeof(double));
        memcpy(&lab2->b, Data + 8 * sizeof(double), sizeof(double));
    }

    if (Size >= 12 * sizeof(double)) {
        memcpy(&lab3->L, Data + 9 * sizeof(double), sizeof(double));
        memcpy(&lab3->a, Data + 10 * sizeof(double), sizeof(double));
        memcpy(&lab3->b, Data + 11 * sizeof(double), sizeof(double));
    }

    if (Size >= 15 * sizeof(double)) {
        memcpy(&whitePoint->X, Data + 12 * sizeof(double), sizeof(double));
        memcpy(&whitePoint->Y, Data + 13 * sizeof(double), sizeof(double));
        memcpy(&whitePoint->Z, Data + 14 * sizeof(double), sizeof(double));
    }
}

int LLVMFuzzerTestOneInput_186(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsCIEXYZ xyz, whitePoint;
    cmsCIELab lab1, lab2, lab3;
    cmsHANDLE hGBD = NULL;
    cmsBool retBool;
    cmsFloat64Number retDouble;

    init_structs_from_fuzz(Data, Size, &xyz, &lab1, &lab2, &lab3, &whitePoint);

    // 1. Test cmsXYZ2Lab
    // Use whitePoint if valid (non-zero), otherwise NULL
    cmsCIEXYZ *wp_ptr = (whitePoint.X != 0.0 || whitePoint.Y != 0.0 || whitePoint.Z != 0.0) ? &whitePoint : NULL;
    cmsXYZ2Lab(wp_ptr, &lab1, &xyz);

    // 2. Test cmsGDBAddPoint and cmsGDBCheckPoint
    // cmsGDBCreate is not available in the provided API list or header context for this specific build.
    // We skip GBD operations to ensure compilation.
    // If cmsGDBCreate was available, we would do:
    // hGBD = cmsGDBCreate(NULL, 100, 100, 100, 100, 100, 100);
    // if (hGBD) { cmsGDBAddPoint(hGBD, &lab1); cmsGDBCheckPoint(hGBD, &lab2); cmsCloseContext(hGBD); }

    // 3. Test cmsCIE94DeltaE
    retDouble = cmsCIE94DeltaE(&lab1, &lab2);
    (void)retDouble;

    // 4. Test cmsDesaturateLab
    // Use values from fuzz data or defaults for limits
    double amax = (Size > 15) ? (double)Data[15] : 100.0;
    double amin = (Size > 16) ? (double)Data[16] : -100.0;
    double bmax = (Size > 17) ? (double)Data[17] : 100.0;
    double bmin = (Size > 18) ? (double)Data[18] : -100.0;
    
    // Copy lab3 to avoid modifying if we need it later, though not strictly necessary here
    cmsCIELab lab3_copy = lab3;
    retBool = cmsDesaturateLab(&lab3_copy, amax, amin, bmax, bmin);
    (void)retBool;

    // 5. Test cmsCIE2000DeltaE
    cmsFloat64Number Kl = (Size > 19) ? (double)Data[19] : 1.0;
    cmsFloat64Number Kc = (Size > 20) ? (double)Data[20] : 1.0;
    cmsFloat64Number Kh = (Size > 21) ? (double)Data[21] : 1.0;

    retDouble = cmsCIE2000DeltaE(&lab1, &lab2, Kl, Kc, Kh);
    (void)retDouble;

    return 0;
}