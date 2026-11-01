// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLabEncoded2FloatV2 at cmspcs.c:218:16 in lcms2.h
// cmsFloat2LabEncoded at cmspcs.c:298:16 in lcms2.h
// cmsFloat2LabEncodedV2 at cmspcs.c:254:16 in lcms2.h
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// cmsLabEncoded2Float at cmspcs.c:226:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_cmsLab2XYZ(const uint8_t *Data, size_t Size) {
    cmsCIEXYZ whitePoint;
    cmsCIEXYZ xyz;
    cmsCIELab lab;

    if (Size < 12) return;

    memcpy(&whitePoint, Data, sizeof(cmsCIEXYZ));
    memcpy(&lab, Data + sizeof(cmsCIEXYZ), sizeof(cmsCIELab));

    cmsLab2XYZ(&whitePoint, &xyz, &lab);
    cmsLab2XYZ(NULL, &xyz, &lab);
}

static void fuzz_cmsLabEncoded2FloatV2(const uint8_t *Data, size_t Size) {
    cmsCIELab lab;
    cmsUInt16Number wLab[3];

    if (Size < 6) return;

    memcpy(wLab, Data, sizeof(wLab));
    cmsLabEncoded2FloatV2(&lab, wLab);
}

static void fuzz_cmsFloat2LabEncoded(const uint8_t *Data, size_t Size) {
    cmsUInt16Number wLab[3];
    cmsCIELab lab;

    if (Size < sizeof(cmsCIELab)) return;

    memcpy(&lab, Data, sizeof(cmsCIELab));
    cmsFloat2LabEncoded(wLab, &lab);
}

static void fuzz_cmsFloat2LabEncodedV2(const uint8_t *Data, size_t Size) {
    cmsUInt16Number wLab[3];
    cmsCIELab lab;

    if (Size < sizeof(cmsCIELab)) return;

    memcpy(&lab, Data, sizeof(cmsCIELab));
    cmsFloat2LabEncodedV2(wLab, &lab);
}

static void fuzz_cmsXYZEncoded2Float(const uint8_t *Data, size_t Size) {
    cmsCIEXYZ fxyz;
    cmsUInt16Number XYZ[3];

    if (Size < 6) return;

    memcpy(XYZ, Data, sizeof(XYZ));
    cmsXYZEncoded2Float(&fxyz, XYZ);
}

static void fuzz_cmsLabEncoded2Float(const uint8_t *Data, size_t Size) {
    cmsCIELab lab;
    cmsUInt16Number wLab[3];

    if (Size < 6) return;

    memcpy(wLab, Data, sizeof(wLab));
    cmsLabEncoded2Float(&lab, wLab);
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_cmsLab2XYZ(Data, Size);
    fuzz_cmsLabEncoded2FloatV2(Data, Size);
    fuzz_cmsFloat2LabEncoded(Data, Size);
    fuzz_cmsFloat2LabEncodedV2(Data, Size);
    fuzz_cmsXYZEncoded2Float(Data, Size);
    fuzz_cmsLabEncoded2Float(Data, Size);

    return 0;
}