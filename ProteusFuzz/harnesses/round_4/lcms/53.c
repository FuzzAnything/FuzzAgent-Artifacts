// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsDeltaE at cmspcs.c:438:28 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <lcms2.h>

static void fuzz_desaturate_lab(const uint8_t *Data, size_t Size) {
    cmsCIELab lab;
    double amax, amin, bmax, bmin;

    if (Size < 24) return;

    memcpy(&lab, Data, sizeof(cmsCIELab));
    memcpy(&amax, Data + sizeof(cmsCIELab), sizeof(double));
    memcpy(&amin, Data + sizeof(cmsCIELab) + sizeof(double), sizeof(double));
    memcpy(&bmax, Data + sizeof(cmsCIELab) + 2 * sizeof(double), sizeof(double));
    memcpy(&bmin, Data + sizeof(cmsCIELab) + 3 * sizeof(double), sizeof(double));

    cmsDesaturateLab(&lab, amax, amin, bmax, bmin);
}

static void fuzz_delta_e(const uint8_t *Data, size_t Size) {
    cmsCIELab lab1, lab2;

    if (Size < 2 * sizeof(cmsCIELab)) return;

    memcpy(&lab1, Data, sizeof(cmsCIELab));
    memcpy(&lab2, Data + sizeof(cmsCIELab), sizeof(cmsCIELab));

    cmsDeltaE(&lab1, &lab2);
}

static void fuzz_cie2000_delta_e(const uint8_t *Data, size_t Size) {
    cmsCIELab lab1, lab2;
    cmsFloat64Number Kl, Kc, Kh;

    if (Size < 2 * sizeof(cmsCIELab) + 3 * sizeof(cmsFloat64Number)) return;

    memcpy(&lab1, Data, sizeof(cmsCIELab));
    memcpy(&lab2, Data + sizeof(cmsCIELab), sizeof(cmsCIELab));
    memcpy(&Kl, Data + 2 * sizeof(cmsCIELab), sizeof(cmsFloat64Number));
    memcpy(&Kc, Data + 2 * sizeof(cmsCIELab) + sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
    memcpy(&Kh, Data + 2 * sizeof(cmsCIELab) + 2 * sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));

    cmsCIE2000DeltaE(&lab1, &lab2, Kl, Kc, Kh);
}

static void fuzz_xyz2lab(const uint8_t *Data, size_t Size) {
    cmsCIEXYZ whitePoint, xyz;
    cmsCIELab lab;

    if (Size < 2 * sizeof(cmsCIEXYZ)) return;

    memcpy(&whitePoint, Data, sizeof(cmsCIEXYZ));
    memcpy(&xyz, Data + sizeof(cmsCIEXYZ), sizeof(cmsCIEXYZ));

    cmsXYZ2Lab(&whitePoint, &lab, &xyz);
}

// Note: cmsGDBAddPoint and cmsGDBCheckPoint require a valid cmsHANDLE from cmsCreateGDB.
// Since cmsCreateGDB is not a public API in standard lcms2.h or was not found in the environment,
// we cannot safely instantiate the handle without knowing the correct internal API or version.
// To ensure compilation and avoid undefined behavior, we skip fuzzing these specific functions
// that depend on the missing constructor/destructor in this specific build environment.
// If cmsCreateGDB were available, the pattern would be:
// cmsHANDLE hGBD = cmsCreateGDB(0);
// ... use hGBD ...
// cmsDeleteGDB(hGBD);

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    switch (Data[0] % 4) {
        case 0:
            fuzz_desaturate_lab(Data + 1, Size - 1);
            break;
        case 1:
            fuzz_delta_e(Data + 1, Size - 1);
            break;
        case 2:
            fuzz_cie2000_delta_e(Data + 1, Size - 1);
            break;
        case 3:
            fuzz_xyz2lab(Data + 1, Size - 1);
            break;
    }

    return 0;
}