// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsBFDdeltaE at cmspcs.c:497:28 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsCIE2000DeltaE at cmspcs.c:589:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <lcms2.h>

static void fuzz_lab_from_data(const uint8_t *data, size_t size, cmsCIELab *lab) {
    if (size < 24) {
        lab->L = 50.0;
        lab->a = 0.0;
        lab->b = 0.0;
        return;
    }

    double L_val = *(const double*)(data);
    double a_val = *(const double*)(data + 8);
    double b_val = *(const double*)(data + 16);

    if (L_val < 0.0) L_val = 0.0;
    if (L_val > 100.0) L_val = 100.0;

    lab->L = L_val;
    lab->a = a_val;
    lab->b = b_val;
}

static void fuzz_xyz_from_data(const uint8_t *data, size_t size, cmsCIEXYZ *xyz) {
    if (size < 24) {
        xyz->X = 0.95047;
        xyz->Y = 1.00000;
        xyz->Z = 1.08883;
        return;
    }

    double X_val = *(const double*)(data);
    double Y_val = *(const double*)(data + 8);
    double Z_val = *(const double*)(data + 16);

    xyz->X = X_val;
    xyz->Y = Y_val;
    xyz->Z = Z_val;
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsCIELab lab1, lab2, lab3;
    cmsCIEXYZ xyz;
    cmsFloat64Number kl, kc, kh;
    cmsFloat64Number delta_e;
    cmsBool result;
    cmsCIEXYZ white_point = {0.95047, 1.00000, 1.08883};

    // Initialize defaults
    lab1.L = 50.0; lab1.a = 0.0; lab1.b = 0.0;
    lab2.L = 50.0; lab2.a = 0.0; lab2.b = 0.0;
    lab3.L = 50.0; lab3.a = 0.0; lab3.b = 0.0;
    xyz.X = 0.95047; xyz.Y = 1.0; xyz.Z = 1.08883;
    kl = 1.0; kc = 1.0; kh = 1.0;

    // Parse Lab1
    if (Size >= 24) {
        fuzz_lab_from_data(Data, Size, &lab1);
    }
    // Parse Lab2
    if (Size >= 48) {
        fuzz_lab_from_data(Data + 24, Size - 24, &lab2);
    }
    // Parse Lab3
    if (Size >= 72) {
        fuzz_lab_from_data(Data + 48, Size - 48, &lab3);
    }
    // Parse XYZ
    if (Size >= 96) {
        fuzz_xyz_from_data(Data + 72, Size - 72, &xyz);
    }

    // Parse weighting factors (12 bytes total: 3 doubles)
    // Offset: 96. Need 96 + 24 = 120 bytes total.
    if (Size >= 120) {
        kl = *(const cmsFloat64Number*)(Data + 96);
        kc = *(const cmsFloat64Number*)(Data + 104);
        kh = *(const cmsFloat64Number*)(Data + 112);
    }

    if (kl < 0.0) kl = 0.0;
    if (kc < 0.0) kc = 0.0;
    if (kh < 0.0) kh = 0.0;

    // 1. Test cmsCIE2000DeltaE
    delta_e = cmsCIE2000DeltaE(&lab1, &lab2, kl, kc, kh);
    (void)delta_e;

    // 2. Test cmsGDBCheckPoint and cmsGDBAddPoint
    // Skipped as cmsGBDCreate is not in the public API to create a valid handle.

    // 3. Test cmsDesaturateLab
    cmsCIELab lab_copy = lab1;
    double amax = 100.0, amin = -100.0;
    double bmax = 100.0, bmin = -100.0;
    
    // Need 16 bytes for 4 doubles (120 + 32 = 152)
    // Offsets: 120, 128, 136, 144. Last byte needed is 144+7=151.
    if (Size >= 152) {
        amax = *(const double*)(Data + 120);
        amin = *(const double*)(Data + 128);
        bmax = *(const double*)(Data + 136);
        bmin = *(const double*)(Data + 144);
    }
    
    result = cmsDesaturateLab(&lab_copy, amax, amin, bmax, bmin);
    (void)result;

    // 4. Test cmsBFDdeltaE
    delta_e = cmsBFDdeltaE(&lab1, &lab2);
    (void)delta_e;

    // 5. Test cmsXYZ2Lab
    cmsCIELab lab_result;
    cmsXYZ2Lab(&white_point, &lab_result, &xyz);
    (void)lab_result;

    // Additional edge case: Test with NULL white point
    cmsXYZ2Lab(NULL, &lab_result, &xyz);
    (void)lab_result;

    return 0;
}