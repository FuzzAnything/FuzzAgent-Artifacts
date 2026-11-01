// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "lcms2.h"

static void safe_fuzz_xyY_from_data(const uint8_t *Data, size_t Size, cmsCIExyY *dest) {
    if (Size >= 12) {
        dest->x = *(const double *)(Data);
        dest->y = *(const double *)(Data + 8);
        dest->Y = *(const double *)(Data + 16);
    } else if (Size >= 8) {
        dest->x = *(const double *)(Data);
        dest->y = *(const double *)(Data + 8);
        dest->Y = 0.0;
    } else if (Size >= 4) {
        dest->x = *(const float *)(Data);
        dest->y = *(const float *)(Data + 4);
        dest->Y = 0.0;
    } else {
        dest->x = 0.0;
        dest->y = 0.0;
        dest->Y = 0.0;
    }
}

static void safe_fuzz_XYZ_from_data(const uint8_t *Data, size_t Size, cmsCIEXYZ *dest) {
    if (Size >= 24) {
        dest->X = *(const double *)(Data);
        dest->Y = *(const double *)(Data + 8);
        dest->Z = *(const double *)(Data + 16);
    } else if (Size >= 12) {
        dest->X = *(const double *)(Data);
        dest->Y = *(const double *)(Data + 8);
        dest->Z = 0.0;
    } else if (Size >= 8) {
        dest->X = *(const double *)(Data);
        dest->Y = *(const double *)(Data + 8);
        dest->Z = 0.0;
    } else if (Size >= 4) {
        dest->X = *(const float *)(Data);
        dest->Y = *(const float *)(Data + 4);
        dest->Z = 0.0;
    } else {
        dest->X = 0.0;
        dest->Y = 0.0;
        dest->Z = 0.0;
    }
}

static void safe_fuzz_Lab_from_data(const uint8_t *Data, size_t Size, cmsCIELab *dest) {
    if (Size >= 12) {
        dest->L = *(const double *)(Data);
        dest->a = *(const double *)(Data + 8);
        dest->b = *(const double *)(Data + 16);
    } else if (Size >= 8) {
        dest->L = *(const double *)(Data);
        dest->a = *(const double *)(Data + 8);
        dest->b = 0.0;
    } else if (Size >= 4) {
        dest->L = *(const float *)(Data);
        dest->a = *(const float *)(Data + 4);
        dest->b = 0.0;
    } else {
        dest->L = 0.0;
        dest->a = 0.0;
        dest->b = 0.0;
    }
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsCIEXYZ xyz_in, xyz_out, xyz_result;
    cmsCIExyY xyY_in, xyY_out;
    cmsCIELab lab_in, lab_out;
    const cmsCIEXYZ *d50;
    cmsBool adapt_result;

    // Initialize structures with fuzzed data
    safe_fuzz_XYZ_from_data(Data, Size, &xyz_in);
    safe_fuzz_xyY_from_data(Data, Size, &xyY_in);
    safe_fuzz_Lab_from_data(Data, Size, &lab_in);

    // Get D50 white point
    d50 = cmsD50_XYZ();
    if (d50 == NULL) return 0;

    // Test cmsLab2XYZ
    cmsLab2XYZ(d50, &xyz_out, &lab_in);
    cmsLab2XYZ(NULL, &xyz_out, &lab_in);

    // Test cmsAdaptToIlluminant
    adapt_result = cmsAdaptToIlluminant(&xyz_result, d50, d50, &xyz_in);
    (void)adapt_result;

    // Test cmsxyY2XYZ
    cmsxyY2XYZ(&xyz_out, &xyY_in);

    // Test cmsXYZ2xyY
    cmsXYZ2xyY(&xyY_out, &xyz_in);

    // Test cmsXYZ2Lab
    cmsXYZ2Lab(d50, &lab_out, &xyz_in);
    cmsXYZ2Lab(NULL, &lab_out, &xyz_in);

    // Additional state exploration: use parts of input as white point
    cmsCIEXYZ custom_white;
    safe_fuzz_XYZ_from_data(Data, Size, &custom_white);
    cmsLab2XYZ(&custom_white, &xyz_out, &lab_in);
    cmsXYZ2Lab(&custom_white, &lab_out, &xyz_in);
    adapt_result = cmsAdaptToIlluminant(&xyz_result, &custom_white, d50, &xyz_in);
    (void)adapt_result;

    return 0;
}