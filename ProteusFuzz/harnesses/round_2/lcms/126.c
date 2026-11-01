// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFloat2LabEncodedV2 at cmspcs.c:254:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static void fuzz_lab_values(cmsCIELab *lab, const uint8_t *data, size_t size) {
    if (size >= 12) {
        memcpy(&lab->L, data, sizeof(double));
        memcpy(&lab->a, data + 8, sizeof(double));
        memcpy(&lab->b, data + 16, sizeof(double));
    } else {
        lab->L = 0.0;
        lab->a = 0.0;
        lab->b = 0.0;
    }
}

static void fuzz_xyz_values(cmsCIEXYZ *xyz, const uint8_t *data, size_t size) {
    if (size >= 12) {
        memcpy(&xyz->X, data, sizeof(double));
        memcpy(&xyz->Y, data + 8, sizeof(double));
        memcpy(&xyz->Z, data + 16, sizeof(double));
    } else {
        xyz->X = 0.0;
        xyz->Y = 0.0;
        xyz->Z = 0.0;
    }
}

static void fuzz_lch_values(cmsCIELCh *lch, const uint8_t *data, size_t size) {
    if (size >= 12) {
        memcpy(&lch->L, data, sizeof(double));
        memcpy(&lch->C, data + 8, sizeof(double));
        memcpy(&lch->h, data + 16, sizeof(double));
    } else {
        lch->L = 0.0;
        lch->C = 0.0;
        lch->h = 0.0;
    }
}

static void fuzz_white_point(cmsCIEXYZ *wp, const uint8_t *data, size_t size) {
    if (size >= 12) {
        memcpy(&wp->X, data, sizeof(double));
        memcpy(&wp->Y, data + 8, sizeof(double));
        memcpy(&wp->Z, data + 16, sizeof(double));
    } else {
        wp->X = 0.9642;
        wp->Y = 1.0000;
        wp->Z = 0.8251;
    }
}

int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
    cmsCIELab lab;
    cmsCIEXYZ xyz;
    cmsCIEXYZ whitePoint;
    cmsCIELCh lch;
    cmsUInt16Number wLab[3];
    cmsBool result;

    if (Size < 12) {
        return 0;
    }

    fuzz_lab_values(&lab, Data, Size);
    fuzz_xyz_values(&xyz, Data, Size);
    fuzz_white_point(&whitePoint, Data, Size);
    fuzz_lch_values(&lch, Data, Size);

    // Test cmsLab2XYZ
    cmsCIEXYZ xyz_out;
    cmsLab2XYZ(&whitePoint, &xyz_out, &lab);
    cmsLab2XYZ(NULL, &xyz_out, &lab);

    // Test cmsXYZ2Lab
    cmsCIELab lab_out;
    cmsXYZ2Lab(&whitePoint, &lab_out, &xyz);
    cmsXYZ2Lab(NULL, &lab_out, &xyz);

    // Test cmsDesaturateLab
    cmsCIELab lab_copy = lab;
    result = cmsDesaturateLab(&lab_copy, 127.0, -128.0, 127.0, -128.0);
    (void)result;

    // Test cmsLCh2Lab
    cmsCIELab lab_from_lch;
    cmsLCh2Lab(&lab_from_lch, &lch);

    // Test cmsLab2LCh
    cmsCIELCh lch_from_lab;
    cmsLab2LCh(&lch_from_lab, &lab);

    // Test cmsFloat2LabEncodedV2
    cmsFloat2LabEncodedV2(wLab, &lab);

    return 0;
}