// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsGetNamedColorList at cmsnamed.c:967:30 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void safe_free_list(cmsNAMEDCOLORLIST *list) {
    if (list != NULL) {
        cmsFreeNamedColorList(list);
    }
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Extract parameters from input data
    cmsUInt32Number n = Data[0];
    cmsUInt32Number ColorantCount = Data[1];

    // Limit values to prevent excessive allocation or invalid states
    if (n > 1000) n = 1000;
    if (ColorantCount > cmsMAXCHANNELS) ColorantCount = cmsMAXCHANNELS;

    // Create dummy prefix and suffix strings based on input
    char prefix[33] = {0};
    char suffix[33] = {0};
    
    // Ensure we have enough data for lengths and content
    if (Size >= 4) {
        size_t prefix_len = Data[2] % 32;
        size_t suffix_len = Data[3] % 32;
        
        // Check if there is enough data for the actual string content
        size_t available_data = Size - 4;
        
        if (prefix_len > 0) {
            if (prefix_len > available_data) prefix_len = available_data;
            if (prefix_len > 32) prefix_len = 32;
            memcpy(prefix, Data + 4, prefix_len);
            prefix[prefix_len] = '\0';
        }
        
        if (suffix_len > 0 && prefix_len < available_data) {
            size_t remaining = available_data - prefix_len;
            if (suffix_len > remaining) suffix_len = remaining;
            if (suffix_len > 32) suffix_len = 32;
            memcpy(suffix, Data + 4 + prefix_len, suffix_len);
            suffix[suffix_len] = '\0';
        }
    }

    // Allocate named color list
    cmsNAMEDCOLORLIST *list = cmsAllocNamedColorList(ctx, n, ColorantCount, prefix, suffix);
    if (list == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Test cmsNamedColorCount
    cmsUInt32Number count = cmsNamedColorCount(list);
    (void)count;

    // Test cmsNamedColorInfo with various indices
    char name[256] = {0};
    char pfx[33] = {0};
    char sfx[33] = {0};
    cmsUInt16Number pcs[8] = {0};
    cmsUInt16Number colorant[8] = {0};

    // Try valid index if count > 0
    if (count > 0) {
        cmsNamedColorInfo(list, 0, name, pfx, sfx, pcs, colorant);
    }

    // Try invalid index
    cmsNamedColorInfo(list, count + 1, name, pfx, sfx, pcs, colorant);

    // Test cmsDupNamedColorList
    cmsNAMEDCOLORLIST *dup_list = cmsDupNamedColorList(list);
    if (dup_list != NULL) {
        cmsUInt32Number dup_count = cmsNamedColorCount(dup_list);
        (void)dup_count;
        if (dup_count > 0) {
            cmsNamedColorInfo(dup_list, 0, name, pfx, sfx, pcs, colorant);
        }
        safe_free_list(dup_list);
    }

    // Test cmsGetNamedColorList with a dummy transform (will likely return NULL)
    cmsHTRANSFORM xform = NULL;
    cmsNAMEDCOLORLIST *from_xform = cmsGetNamedColorList(xform);
    (void)from_xform;

    // Cleanup
    safe_free_list(list);
    cmsDeleteContext(ctx);

    return 0;
}