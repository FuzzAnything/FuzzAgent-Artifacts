// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void dummy_cleanup(cmsNAMEDCOLORLIST *list) {
    if (list != NULL) {
        cmsFreeNamedColorList(list);
    }
}

int LLVMFuzzerTestOneInput_198(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Create a named color list using the standard API
    // cmsAllocNamedColorList requires: ContextID, n, ColorantCount, Prefix, Suffix
    cmsNAMEDCOLORLIST *original_list = cmsAllocNamedColorList(ctx, 10, 3, "", "");
    if (original_list == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    cmsNAMEDCOLORLIST *copy_list = NULL;
    cmsBool append_result = FALSE;
    cmsInt32Number index_result = -1;
    cmsBool info_result = FALSE;

    // Fuzz data usage for name and color values
    char name[256];
    cmsUInt16Number pcs[3];
    cmsUInt16Number colorant[32]; 

    size_t name_len = (Size > 255) ? 255 : Size;
    if (name_len > 0) {
        memcpy(name, Data, name_len);
        name[name_len] = '\0';
    } else {
        strcpy(name, "TestColor");
    }

    // Initialize PCS and Colorant with fuzz data or defaults
    for (int i = 0; i < 3; i++) {
        pcs[i] = (Size > i) ? Data[i] : 0;
        colorant[i] = (Size > i + 3) ? Data[i + 3] : 0;
    }
    for (int i = 3; i < 32; i++) {
        colorant[i] = 0;
    }

    // 1. Test cmsAppendNamedColor
    append_result = cmsAppendNamedColor(original_list, name, pcs, colorant);

    // 2. Test cmsDupNamedColorList
    copy_list = cmsDupNamedColorList(original_list);

    // 3. Test cmsNamedColorIndex on original and copy
    index_result = cmsNamedColorIndex(original_list, name);
    if (copy_list != NULL) {
        cmsNamedColorIndex(copy_list, name);
    }

    // 4. Test cmsNamedColorInfo
    char info_name[256] = {0};
    char info_prefix[33] = {0};
    char info_suffix[33] = {0};
    cmsUInt16Number info_pcs[3] = {0};
    cmsUInt16Number info_colorant[32] = {0};

    if (index_result >= 0) {
        info_result = cmsNamedColorInfo(original_list, index_result, info_name, info_prefix, info_suffix, info_pcs, info_colorant);
        if (copy_list != NULL) {
            cmsNamedColorInfo(copy_list, index_result, NULL, NULL, NULL, NULL, NULL);
        }
    }

    // Cleanup
    dummy_cleanup(original_list);
    dummy_cleanup(copy_list);
    cmsDeleteContext(ctx);

    return 0;
}