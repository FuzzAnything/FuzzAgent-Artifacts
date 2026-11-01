// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsLCh2Lab at cmspcs.c:358:16 in lcms2.h
// cmsLab2LCh at cmspcs.c:349:16 in lcms2.h
// cmsFloat2LabEncodedV2 at cmspcs.c:254:16 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <lcms2.h>

static void fuzz_cmsXYZ2Lab(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIEXYZ) * 2) return;

    cmsCIEXYZ xyz;
    cmsCIEXYZ whitePoint;
    cmsCIELab lab;

    memcpy(&xyz, Data, sizeof(xyz));
    memcpy(&whitePoint, Data + sizeof(xyz), sizeof(whitePoint));
    
    memset(&lab, 0, sizeof(lab));

    cmsXYZ2Lab(&whitePoint, &lab, &xyz);
    cmsXYZ2Lab(NULL, &lab, &xyz);
}

static void fuzz_cmsLCh2Lab(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELCh)) return;

    cmsCIELCh lch;
    cmsCIELab lab;

    memcpy(&lch, Data, sizeof(lch));
    memset(&lab, 0, sizeof(lab));

    cmsLCh2Lab(&lab, &lch);
}

static void fuzz_cmsLab2LCh(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELab)) return;

    cmsCIELab lab;
    cmsCIELCh lch;

    memcpy(&lab, Data, sizeof(lab));
    memset(&lch, 0, sizeof(lch));

    cmsLab2LCh(&lch, &lab);
}

static void fuzz_cmsFloat2LabEncodedV2(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELab)) return;

    cmsCIELab lab;
    cmsUInt16Number wLab[3];

    memcpy(&lab, Data, sizeof(lab));
    memset(wLab, 0, sizeof(wLab));

    cmsFloat2LabEncodedV2(wLab, &lab);
}

static void fuzz_cmsDesaturateLab(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELab) + 4 * sizeof(double)) return;

    cmsCIELab lab;
    double amax, amin, bmax, bmin;

    memcpy(&lab, Data, sizeof(lab));
    memcpy(&amax, Data + sizeof(lab), sizeof(double));
    memcpy(&amin, Data + sizeof(lab) + sizeof(double), sizeof(double));
    memcpy(&bmax, Data + sizeof(lab) + 2 * sizeof(double), sizeof(double));
    memcpy(&bmin, Data + sizeof(lab) + 3 * sizeof(double), sizeof(double));

    cmsBool result = cmsDesaturateLab(&lab, amax, amin, bmax, bmin);
    (void)result;
}

static void fuzz_cmsLab2XYZ(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELab) + sizeof(cmsCIEXYZ)) return;

    cmsCIELab lab;
    cmsCIEXYZ xyz;
    cmsCIEXYZ whitePoint;

    memcpy(&lab, Data, sizeof(lab));
    memcpy(&whitePoint, Data + sizeof(lab), sizeof(whitePoint));
    memset(&xyz, 0, sizeof(xyz));

    cmsLab2XYZ(&whitePoint, &xyz, &lab);
    cmsLab2XYZ(NULL, &xyz, &lab);
}

int LLVMFuzzerTestOneInput_170(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_cmsXYZ2Lab(Data, Size);
    fuzz_cmsLCh2Lab(Data, Size);
    fuzz_cmsLab2LCh(Data, Size);
    fuzz_cmsFloat2LabEncodedV2(Data, Size);
    fuzz_cmsDesaturateLab(Data, Size);
    fuzz_cmsLab2XYZ(Data, Size);

    return 0;
}