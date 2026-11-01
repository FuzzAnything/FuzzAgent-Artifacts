// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsGetNamedColorList at cmsnamed.c:967:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void safe_free_list(cmsNAMEDCOLORLIST *list) {
    if (list != NULL) {
        cmsFreeNamedColorList(list);
    }
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        ctx = NULL;
    }

    cmsNAMEDCOLORLIST *list = NULL;
    cmsNAMEDCOLORLIST *dup_list = NULL;
    cmsHTRANSFORM xform = NULL;
    cmsUInt32Number nColors = 0;
    cmsUInt32Number channels = 0;
    cmsUInt16Number pcs = 0;
    cmsUInt16Number colorant[8] = {0};
    char name[256] = {0};
    char prefix[64] = {0};
    char suffix[64] = {0};
    cmsBool info_result = FALSE;

    // Extract parameters from fuzz data safely
    if (Size > 0) nColors = Data[0] % 100; 
    if (Size > 1) channels = Data[1] % 8;  

    // Prepare safe prefix and suffix strings
    char safe_prefix[33] = {0};
    char safe_suffix[33] = {0};

    // Only copy if we have enough data and it's not empty
    if (Size > 2) {
        size_t prefix_len = 0;
        // Find null terminator or limit to available size
        for (size_t i = 0; i < Size - 2 && i < 32; i++) {
            if (Data[2 + i] == 0) break;
            safe_prefix[i] = (char)Data[2 + i];
            prefix_len = i + 1;
        }
        safe_prefix[32] = '\0';
    } else {
        strcpy(safe_prefix, "Prefix");
    }

    if (Size > 4) {
        size_t suffix_len = 0;
        for (size_t i = 0; i < Size - 4 && i < 32; i++) {
            if (Data[4 + i] == 0) break;
            safe_suffix[i] = (char)Data[4 + i];
            suffix_len = i + 1;
        }
        safe_suffix[32] = '\0';
    } else {
        strcpy(safe_suffix, "Suffix");
    }

    // 1. Test cmsAllocNamedColorList
    list = cmsAllocNamedColorList(ctx, nColors, channels, safe_prefix, safe_suffix);
    
    if (list != NULL) {
        // 2. Test cmsNamedColorCount
        cmsUInt32Number count = cmsNamedColorCount(list);
        (void)count;

        // 3. Test cmsDupNamedColorList
        dup_list = cmsDupNamedColorList(list);
        if (dup_list != NULL) {
            cmsUInt32Number dup_count = cmsNamedColorCount(dup_list);
            (void)dup_count;
        }

        // 4. Test cmsNamedColorInfo with various indices
        // We test index 0 and index nColors (out of bounds)
        info_result = cmsNamedColorInfo(list, 0, name, prefix, suffix, &pcs, colorant);
        (void)info_result;
        
        if (nColors > 0) {
            info_result = cmsNamedColorInfo(list, nColors, name, prefix, suffix, &pcs, colorant);
            (void)info_result;
        }

        // Test with NULL buffers
        info_result = cmsNamedColorInfo(list, 0, NULL, NULL, NULL, NULL, NULL);
        (void)info_result;

        // 5. Test cmsFreeNamedColorList on original
        cmsFreeNamedColorList(list);
        list = NULL;
    }

    // 6. Test cmsGetNamedColorList
    // Attempt to create a transform. Note: Without valid profiles, this often returns NULL,
    // which is a valid state to test for robustness.
    if (Size > 6) {
        // We try to create a transform, but without valid profile data it will likely fail.
        // This is acceptable for fuzzing to test the NULL path.
        xform = cmsCreateTransform(NULL, cmsSigXYZData, NULL, cmsSigXYZData, 8, 0);
    }
    
    cmsNAMEDCOLORLIST *xform_list = cmsGetNamedColorList(xform);
    if (xform_list != NULL) {
        cmsNamedColorCount(xform_list);
        // Do NOT free xform_list here as it is owned by the transform.
    }

    // Cleanup
    if (xform != NULL) {
        cmsDeleteTransform(xform);
    }

    if (dup_list != NULL) {
        cmsFreeNamedColorList(dup_list);
    }

    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }

    return 0;
}