// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsGetNamedColorList at cmsnamed.c:967:30 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void cleanup_named_color_list(cmsNAMEDCOLORLIST* list) {
    if (list != NULL) {
        cmsFreeNamedColorList(list);
    }
}

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = NULL;
    cmsNAMEDCOLORLIST* list = NULL;
    cmsNAMEDCOLORLIST* dup_list = NULL;
    cmsBool res;
    cmsUInt32Number count;
    char name[256] = {0};
    char prefix[256] = {0};
    char suffix[256] = {0};
    cmsUInt16Number pcs[256] = {0};
    cmsUInt16Number colorant[256] = {0};
    cmsUInt32Number nColors;
    cmsUInt32Number colorantCount;
    const char* prefixStr;
    const char* suffixStr;

    // Extract parameters from fuzz input
    if (Size >= 8) {
        nColors = *(const cmsUInt32Number*)(Data);
        colorantCount = *(const cmsUInt32Number*)(Data + 4);
        prefixStr = "FuzzPrefix";
        suffixStr = "FuzzSuffix";
    } else {
        nColors = 0;
        colorantCount = 0;
        prefixStr = "FuzzPrefix";
        suffixStr = "FuzzSuffix";
    }

    // Clamp values to reasonable limits to prevent allocation failures or crashes
    if (nColors > 1000) nColors = 1000;
    if (colorantCount > 20) colorantCount = 20; 

    // Create a context
    ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // 1. Test cmsAllocNamedColorList
    list = cmsAllocNamedColorList(ctx, nColors, colorantCount, prefixStr, suffixStr);
    
    if (list != NULL) {
        // 2. Test cmsNamedColorCount on valid list
        count = cmsNamedColorCount(list);

        // 3. Test cmsDupNamedColorList
        dup_list = cmsDupNamedColorList(list);
        if (dup_list != NULL) {
            // Test cmsNamedColorCount on duplicate
            count = cmsNamedColorCount(dup_list);
            
            // Test cmsNamedColorInfo on duplicate (index 0 if count > 0)
            if (count > 0) {
                res = cmsNamedColorInfo(dup_list, 0, name, prefix, suffix, pcs, colorant);
            }
        }

        // 4. Test cmsNamedColorInfo on original list
        count = cmsNamedColorCount(list);
        if (count > 0) {
            // Try valid index
            res = cmsNamedColorInfo(list, 0, name, prefix, suffix, pcs, colorant);
            // Try invalid index (out of bounds)
            res = cmsNamedColorInfo(list, count + 1, name, prefix, suffix, pcs, colorant);
        }

        // Test with NULL buffers
        if (count > 0) {
            res = cmsNamedColorInfo(list, 0, NULL, NULL, NULL, NULL, NULL);
        }

        // Test cmsNamedColorInfo with NULL list (should return FALSE)
        res = cmsNamedColorInfo(NULL, 0, name, prefix, suffix, pcs, colorant);

        // Cleanup duplicate
        cleanup_named_color_list(dup_list);
        // Cleanup original
        cleanup_named_color_list(list);
    }

    // 5. Test cmsNamedColorCount with NULL
    count = cmsNamedColorCount(NULL);

    // 6. Test cmsDupNamedColorList with NULL
    dup_list = cmsDupNamedColorList(NULL);
    cleanup_named_color_list(dup_list);

    // 7. Test cmsFreeNamedColorList with NULL
    cmsFreeNamedColorList(NULL);

    // 8. Test cmsGetNamedColorList with NULL transform
    cmsNAMEDCOLORLIST* retrieved = cmsGetNamedColorList(NULL);
    if (retrieved != NULL) {
        cleanup_named_color_list(retrieved);
    }

    // Cleanup context
    cleanup_context(ctx);

    return 0;
}