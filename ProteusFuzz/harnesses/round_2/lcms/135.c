// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCIECAM02Done at cmscam02.c:427:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsCIECAM02Init at cmscam02.c:358:22 in lcms2.h
// cmsCIECAM02Forward at cmscam02.c:435:16 in lcms2.h
// cmsCIECAM02Reverse at cmscam02.c:461:16 in lcms2.h
// cmsCIECAM02Done at cmscam02.c:427:16 in lcms2.h
// cmsCIECAM02Init at cmscam02.c:358:22 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsCIECAM02Done at cmscam02.c:427:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void cleanup_resources(cmsContext ctx, cmsHANDLE hModel) {
    if (hModel != NULL) {
        cmsCIECAM02Done(hModel);
    }
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size) {
    if (Size < 20) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    cmsViewingConditions vc = {0};
    cmsCIEXYZ sourceWhite = {0};
    cmsCIEXYZ targetIlluminant = {0};
    cmsCIEXYZ inputXYZ = {0};
    cmsCIEXYZ outputXYZ = {0};
    cmsCIEXYZ outputXYZ2 = {0};
    cmsCIELab inputLab = {0};
    cmsJCh inputJCh = {0};
    cmsJCh outputJCh = {0};
    cmsHANDLE hModel = NULL;

    // Parse input data to populate structures
    size_t offset = 0;
    if (offset + sizeof(vc) <= Size) {
        memcpy(&vc, Data + offset, sizeof(vc));
        offset += sizeof(vc);
    }
    if (offset + sizeof(sourceWhite) <= Size) {
        memcpy(&sourceWhite, Data + offset, sizeof(sourceWhite));
        offset += sizeof(sourceWhite);
    }
    if (offset + sizeof(targetIlluminant) <= Size) {
        memcpy(&targetIlluminant, Data + offset, sizeof(targetIlluminant));
        offset += sizeof(targetIlluminant);
    }
    if (offset + sizeof(inputXYZ) <= Size) {
        memcpy(&inputXYZ, Data + offset, sizeof(inputXYZ));
        offset += sizeof(inputXYZ);
    }
    if (offset + sizeof(inputLab) <= Size) {
        memcpy(&inputLab, Data + offset, sizeof(inputLab));
        offset += sizeof(inputLab);
    }
    if (offset + sizeof(inputJCh) <= Size) {
        memcpy(&inputJCh, Data + offset, sizeof(inputJCh));
        offset += sizeof(inputJCh);
    }

    // 1. Test cmsD50_XYZ (No args, just call to ensure it doesn't crash)
    const cmsCIEXYZ *d50 = cmsD50_XYZ();
    if (d50 == NULL) {
        cleanup_resources(ctx, hModel);
        return 0;
    }

    // 2. Test cmsCIECAM02Init
    hModel = cmsCIECAM02Init(ctx, &vc);
    if (hModel != NULL) {
        // 3. Test cmsCIECAM02Forward
        cmsCIECAM02Forward(hModel, &inputXYZ, &outputJCh);

        // 4. Test cmsCIECAM02Reverse
        cmsCIECAM02Reverse(hModel, &inputJCh, &outputXYZ);

        // Cleanup model before next steps to test re-initialization or null handling
        cmsCIECAM02Done(hModel);
        hModel = NULL;
    }

    // Re-initialize model for further tests if needed, or test with NULL handle (handled by asserts in lib)
    // Let's try to init again with potentially modified data if size allows
    if (Size > 40) {
        // Modify viewing conditions slightly to explore state
        vc.surround = (Data[30] % 4); 
        hModel = cmsCIECAM02Init(ctx, &vc);
    }

    // 5. Test cmsLab2XYZ
    // Use D50 as white point, or NULL to trigger default
    cmsLab2XYZ(d50, &outputXYZ2, &inputLab);
    cmsLab2XYZ(NULL, &outputXYZ2, &inputLab);

    // 6. Test cmsAdaptToIlluminant
    cmsBool adaptResult = cmsAdaptToIlluminant(&outputXYZ2, &sourceWhite, &targetIlluminant, &inputXYZ);
    (void)adaptResult;

    // Cleanup
    if (hModel != NULL) {
        cmsCIECAM02Done(hModel);
    }
    cmsDeleteContext(ctx);

    return 0;
}