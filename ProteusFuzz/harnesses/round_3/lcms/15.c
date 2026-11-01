// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void fuzz_cmsXYZ2Lab(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIEXYZ)) return;

    cmsCIEXYZ xyz;
    cmsCIELab lab;
    cmsCIEXYZ whitePoint;

    memcpy(&xyz, Data, sizeof(cmsCIEXYZ));
    memset(&lab, 0, sizeof(lab));
    memset(&whitePoint, 0, sizeof(whitePoint));

    if (Size >= sizeof(cmsCIEXYZ) * 2) {
        memcpy(&whitePoint, Data + sizeof(cmsCIEXYZ), sizeof(cmsCIEXYZ));
    }

    cmsXYZ2Lab(&whitePoint, &lab, &xyz);
}

static void fuzz_cmsAdaptToIlluminant(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIEXYZ) * 3) return;

    cmsCIEXYZ result;
    cmsCIEXYZ sourceWhitePt;
    cmsCIEXYZ illuminant;
    cmsCIEXYZ value;

    memcpy(&sourceWhitePt, Data, sizeof(cmsCIEXYZ));
    memcpy(&illuminant, Data + sizeof(cmsCIEXYZ), sizeof(cmsCIEXYZ));
    memcpy(&value, Data + sizeof(cmsCIEXYZ) * 2, sizeof(cmsCIEXYZ));

    cmsBool ret = cmsAdaptToIlluminant(&result, &sourceWhitePt, &illuminant, &value);
    (void)ret;
}

static void fuzz_cmsD50_XYZ(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    const cmsCIEXYZ *d50 = cmsD50_XYZ();
    if (d50) {
        cmsCIEXYZ temp;
        memcpy(&temp, d50, sizeof(cmsCIEXYZ));
        (void)temp;
    }
}

static void fuzz_cmsxyY2XYZ(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIExyY)) return;

    cmsCIExyY source;
    cmsCIEXYZ dest;

    memcpy(&source, Data, sizeof(cmsCIExyY));
    memset(&dest, 0, sizeof(dest));

    cmsxyY2XYZ(&dest, &source);
}

static void fuzz_cmsXYZ2xyY(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIEXYZ)) return;

    cmsCIEXYZ source;
    cmsCIExyY dest;

    memcpy(&source, Data, sizeof(cmsCIEXYZ));
    memset(&dest, 0, sizeof(dest));

    cmsXYZ2xyY(&dest, &source);
}

static void fuzz_cmsLab2XYZ(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIELab)) return;

    cmsCIELab lab;
    cmsCIEXYZ xyz;
    cmsCIEXYZ whitePoint;

    memcpy(&lab, Data, sizeof(cmsCIELab));
    memset(&xyz, 0, sizeof(xyz));
    memset(&whitePoint, 0, sizeof(whitePoint));

    if (Size >= sizeof(cmsCIELab) + sizeof(cmsCIEXYZ)) {
        memcpy(&whitePoint, Data + sizeof(cmsCIELab), sizeof(cmsCIEXYZ));
    }

    cmsLab2XYZ(&whitePoint, &xyz, &lab);
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    uint8_t type = Data[0] % 6;

    switch (type) {
        case 0:
            fuzz_cmsXYZ2Lab(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
        case 1:
            fuzz_cmsAdaptToIlluminant(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
        case 2:
            fuzz_cmsD50_XYZ(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
        case 3:
            fuzz_cmsxyY2XYZ(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
        case 4:
            fuzz_cmsXYZ2xyY(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
        case 5:
            fuzz_cmsLab2XYZ(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
    }

    return 0;
}