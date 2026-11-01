// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void init_lab_from_fuzz(cmsCIELab *lab, const uint8_t *data, size_t offset, size_t size) {
    if (offset + 24 <= size) {
        memcpy(&lab->L, data + offset, sizeof(double));
        memcpy(&lab->a, data + offset + 8, sizeof(double));
        memcpy(&lab->b, data + offset + 16, sizeof(double));
    } else {
        lab->L = 0.0;
        lab->a = 0.0;
        lab->b = 0.0;
    }
}

static void init_xyz_from_fuzz(cmsCIEXYZ *xyz, const uint8_t *data, size_t offset, size_t size) {
    if (offset + 24 <= size) {
        memcpy(&xyz->X, data + offset, sizeof(double));
        memcpy(&xyz->Y, data + offset + 8, sizeof(double));
        memcpy(&xyz->Z, data + offset + 16, sizeof(double));
    } else {
        xyz->X = 0.0;
        xyz->Y = 0.0;
        xyz->Z = 0.0;
    }
}

int LLVMFuzzerTestOneInput_192(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsCIELab lab1, lab2, lab_out;
    cmsCIEXYZ xyz, white_point;
    cmsBool result;
    cmsFloat64Number delta_e;

    // Initialize structures from fuzz data
    init_lab_from_fuzz(&lab1, Data, 0, Size);
    init_lab_from_fuzz(&lab2, Data, 24, Size);
    init_xyz_from_fuzz(&xyz, Data, 48, Size);
    init_xyz_from_fuzz(&white_point, Data, 72, Size);

    // 1. Test cmsXYZ2Lab
    // Try with custom white point and NULL
    cmsXYZ2Lab(&white_point, &lab_out, &xyz);
    cmsXYZ2Lab(NULL, &lab_out, &xyz);

    // 3. Test cmsDesaturateLab
    cmsFloat64Number amax = 100.0, amin = -100.0, bmax = 100.0, bmin = -100.0;
    // Ensure we have enough data for 4 doubles (32 bytes) starting at offset 96
    if (Size >= 96 + 32) {
        memcpy(&amax, Data + 96, sizeof(double));
        memcpy(&amin, Data + 104, sizeof(double));
        memcpy(&bmax, Data + 112, sizeof(double));
        memcpy(&bmin, Data + 120, sizeof(double));
    }
    result = cmsDesaturateLab(&lab1, amax, amin, bmax, bmin);
    // Also try with negative L to trigger the FALSE return
    lab1.L = -10.0;
    result = cmsDesaturateLab(&lab1, amax, amin, bmax, bmin);

    // 4. Test cmsCIE2000DeltaE
    cmsFloat64Number Kl = 1.0, Kc = 1.0, Kh = 1.0;
    // Ensure we have enough data for 3 doubles (24 bytes) starting at offset 128
    if (Size >= 128 + 24) {
        memcpy(&Kl, Data + 128, sizeof(double));
        memcpy(&Kc, Data + 136, sizeof(double));
        memcpy(&Kh, Data + 144, sizeof(double));
    }
    delta_e = cmsCIE2000DeltaE(&lab1, &lab2, Kl, Kc, Kh);

    // 5. Test cmsBFDdeltaE
    delta_e = cmsBFDdeltaE(&lab1, &lab2);

    return 0;
}