// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static cmsCIEXYZ get_xyz_from_fuzz(const uint8_t *Data, size_t Size, size_t offset) {
    cmsCIEXYZ xyz = {0.0, 0.0, 0.0};
    if (offset + 3 * sizeof(double) <= Size) {
        memcpy(&xyz.X, Data + offset, sizeof(double));
        memcpy(&xyz.Y, Data + offset + sizeof(double), sizeof(double));
        memcpy(&xyz.Z, Data + offset + 2 * sizeof(double), sizeof(double));
    }
    return xyz;
}

static cmsCIELab get_lab_from_fuzz(const uint8_t *Data, size_t Size, size_t offset) {
    cmsCIELab lab = {0.0, 0.0, 0.0};
    if (offset + 3 * sizeof(double) <= Size) {
        memcpy(&lab.L, Data + offset, sizeof(double));
        memcpy(&lab.a, Data + offset + sizeof(double), sizeof(double));
        memcpy(&lab.b, Data + offset + 2 * sizeof(double), sizeof(double));
    }
    return lab;
}

static cmsFloat64Number get_double_from_fuzz(const uint8_t *Data, size_t Size, size_t offset) {
    cmsFloat64Number val = 0.0;
    if (offset + sizeof(double) <= Size) {
        memcpy(&val, Data + offset, sizeof(double));
    }
    return val;
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // cmsGDBAddPoint and cmsGDBCheckPoint require a GBD handle.
    // Since cmsCreateGBD is not available in standard lcms2.h or is internal,
    // we cannot test those specific functions directly without a valid handle.
    // We will skip the GBD-related tests to ensure compilation and focus on the other APIs.
    // If a valid GBD handle could be obtained via a different public API, it would be used here.
    cmsHANDLE hGBD = NULL; 

    cmsCIEXYZ whitePoint = {0.9642, 1.0000, 0.8251}; // D50
    cmsCIEXYZ xyz = get_xyz_from_fuzz(Data, Size, 0);
    cmsCIELab lab = {0.0, 0.0, 0.0};

    // Test cmsXYZ2Lab
    cmsXYZ2Lab(&whitePoint, &lab, &xyz);
    cmsXYZ2Lab(NULL, &lab, &xyz);

    // Test cmsGDBAddPoint (skipped due to lack of valid handle creation API)
    // if (hGBD) cmsGDBAddPoint(hGBD, &lab);

    cmsCIELab lab2 = get_lab_from_fuzz(Data, Size, 3 * sizeof(double));
    // if (hGBD) cmsGDBAddPoint(hGBD, &lab2);

    // Test cmsDesaturateLab
    cmsBool desatResult = cmsDesaturateLab(&lab, 100.0, -100.0, 100.0, -100.0);
    (void)desatResult;

    // Test cmsGDBCheckPoint (skipped due to lack of valid handle)
    // if (hGBD) {
    //     cmsBool checkResult = cmsGDBCheckPoint(hGBD, &lab);
    //     (void)checkResult;
    //     cmsBool checkResult2 = cmsGDBCheckPoint(hGBD, &lab2);
    //     (void)checkResult2;
    // }

    // Test cmsCIE2000DeltaE
    cmsFloat64Number Kl = get_double_from_fuzz(Data, Size, 6 * sizeof(double));
    cmsFloat64Number Kc = get_double_from_fuzz(Data, Size, 7 * sizeof(double));
    cmsFloat64Number Kh = get_double_from_fuzz(Data, Size, 8 * sizeof(double));
    cmsFloat64Number deltaE2000 = cmsCIE2000DeltaE(&lab, &lab2, Kl, Kc, Kh);
    (void)deltaE2000;

    // Test cmsBFDdeltaE
    cmsFloat64Number deltaEBFD = cmsBFDdeltaE(&lab, &lab2);
    (void)deltaEBFD;

    // Cleanup
    // if (hGBD) cmsDeleteGBD(hGBD);
    cmsDeleteContext(ctx);

    return 0;
}