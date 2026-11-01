// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCIECAM02Init at cmscam02.c:358:22 in lcms2.h
// cmsCIECAM02Forward at cmscam02.c:435:16 in lcms2.h
// cmsCIECAM02Reverse at cmscam02.c:461:16 in lcms2.h
// cmsCIECAM02Done at cmscam02.c:427:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void fuzz_cmsAdaptToIlluminant(const uint8_t *Data, size_t Size) {
    if (Size < 3 * sizeof(cmsCIEXYZ)) return;

    cmsCIEXYZ sourceWhite, illuminant, value, result;
    memcpy(&sourceWhite, Data, sizeof(cmsCIEXYZ));
    memcpy(&illuminant, Data + sizeof(cmsCIEXYZ), sizeof(cmsCIEXYZ));
    memcpy(&value, Data + 2 * sizeof(cmsCIEXYZ), sizeof(cmsCIEXYZ));

    cmsBool ret = cmsAdaptToIlluminant(&result, &sourceWhite, &illuminant, &value);
    (void)ret;
}

static void fuzz_cmsLab2XYZ(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsCIEXYZ) + sizeof(cmsCIELab)) return;

    cmsCIEXYZ whitePoint, xyz;
    cmsCIELab lab;

    memcpy(&whitePoint, Data, sizeof(cmsCIEXYZ));
    memcpy(&lab, Data + sizeof(cmsCIEXYZ), sizeof(cmsCIELab));

    cmsLab2XYZ(&whitePoint, &xyz, &lab);
}

static void fuzz_cmsD50_XYZ(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    const cmsCIEXYZ *d50 = cmsD50_XYZ();
    if (d50) {
        volatile double x = d50->X;
        volatile double y = d50->Y;
        volatile double z = d50->Z;
        (void)x; (void)y; (void)z;
    }
}

static void fuzz_cmsCIECAM02(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsViewingConditions) + sizeof(cmsCIEXYZ)) return;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return;

    cmsViewingConditions vc;
    cmsCIEXYZ inputXYZ, outputXYZ;
    cmsJCh jch;
    cmsHANDLE hModel = NULL;

    memcpy(&vc, Data, sizeof(cmsViewingConditions));
    memcpy(&inputXYZ, Data + sizeof(cmsViewingConditions), sizeof(cmsCIEXYZ));

    hModel = cmsCIECAM02Init(ctx, &vc);
    if (hModel) {
        cmsCIECAM02Forward(hModel, &inputXYZ, &jch);
        cmsCIECAM02Reverse(hModel, &jch, &outputXYZ);
        cmsCIECAM02Done(hModel);
    }

    cmsDeleteContext(ctx);
}

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_cmsAdaptToIlluminant(Data, Size);
    fuzz_cmsLab2XYZ(Data, Size);
    fuzz_cmsD50_XYZ(Data, Size);
    fuzz_cmsCIECAM02(Data, Size);

    return 0;
}