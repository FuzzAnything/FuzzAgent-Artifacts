// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetNamedColorList at cmsnamed.c:967:30 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_resources(cmsHTRANSFORM xform, cmsNAMEDCOLORLIST* list, cmsNAMEDCOLORLIST* dup_list, cmsHPROFILE hProfile) {
    if (xform) {
        cmsDeleteTransform(xform);
    }
    if (list) {
        cmsFreeNamedColorList(list);
    }
    if (dup_list) {
        cmsFreeNamedColorList(dup_list);
    }
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (!hProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Corrected arguments for cmsCreateTransform: Input, InputFormat, Output, OutputFormat, Intent, Flags
    cmsHTRANSFORM xform = cmsCreateTransform(hProfile, TYPE_RGB_8, hProfile, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);
    if (!xform) {
        cleanup_resources(NULL, NULL, NULL, hProfile);
        cmsDeleteContext(ctx);
        return 0;
    }

    cmsNAMEDCOLORLIST* list = NULL;
    cmsNAMEDCOLORLIST* dup_list = NULL;
    cmsBool success = FALSE;
    cmsInt32Number index = -1;
    char name[256];
    char prefix[33] = {0};
    char suffix[33] = {0};
    cmsUInt16Number pcs[3] = {0, 0, 0};
    cmsUInt16Number colorant[cmsMAXCHANNELS] = {0};

    // Initialize name buffer from input data
    size_t nameLen = (Size < 255) ? Size : 255;
    memcpy(name, Data, nameLen);
    name[nameLen] = '\0';

    // Try to get named color list from transform (likely NULL unless transform has named color stage)
    cmsNAMEDCOLORLIST* retrievedList = cmsGetNamedColorList(xform);
    if (retrievedList) {
        list = retrievedList;
    }

    // If no list retrieved, we cannot easily create one without a specific profile containing named colors.
    // However, we can test the functions with a NULL list or a list created via other means if available.
    // Since cmsCreateNamedColorList is not a standard public API in lcms2.h for direct creation,
    // we will proceed to test the functions with the retrieved list (if any) or NULL.
    
    // If list is NULL, we can still test functions that handle NULL gracefully or skip specific tests.
    // For cmsAppendNamedColor, cmsNamedColorIndex, cmsDupNamedColorList, cmsNamedColorInfo:
    // They generally handle NULL or invalid inputs by returning FALSE or -1.
    
    if (list) {
        // Fuzz cmsAppendNamedColor
        success = cmsAppendNamedColor(list, name, pcs, colorant);
        
        // Fuzz cmsNamedColorIndex
        index = cmsNamedColorIndex(list, name);
        
        // Fuzz cmsDupNamedColorList
        dup_list = cmsDupNamedColorList(list);
        
        // Fuzz cmsNamedColorInfo
        if (index >= 0) {
            cmsNamedColorInfo(list, (cmsUInt32Number)index, name, prefix, suffix, pcs, colorant);
        }
        
        // Try with invalid index
        cmsNamedColorInfo(list, 99999, name, prefix, suffix, pcs, colorant);
    } else {
        // Test behavior with NULL list
        success = cmsAppendNamedColor(NULL, name, pcs, colorant);
        index = cmsNamedColorIndex(NULL, name);
        dup_list = cmsDupNamedColorList(NULL);
        cmsNamedColorInfo(NULL, 0, name, prefix, suffix, pcs, colorant);
    }

    // Cleanup
    if (list && !retrievedList) {
        // This case shouldn't happen as we only assign retrievedList to list if it's not NULL
        // But if we had created a list manually, we would free it here.
        // Since we didn't create one manually, this block is effectively dead code for this logic.
        cmsFreeNamedColorList(list);
    }
    if (dup_list) {
        cmsFreeNamedColorList(dup_list);
    }
    
    cleanup_resources(xform, NULL, NULL, hProfile);
    cmsDeleteContext(ctx);

    return 0;
}