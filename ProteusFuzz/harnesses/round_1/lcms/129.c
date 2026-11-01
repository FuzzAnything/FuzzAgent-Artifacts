// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsGetNamedColorList at cmsnamed.c:967:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    // Extract parameters from fuzz input
    cmsUInt32Number n = 0;
    cmsUInt32Number ColorantCount = 0;
    char Prefix[33] = "";
    char Suffix[33] = "";

    if (Size >= 8) {
        n = (cmsUInt32Number)Data[0];
        ColorantCount = (cmsUInt32Number)Data[1];
        // Limit to reasonable values to avoid immediate allocation failures or overflow
        if (n > 1000) n = 1000;
        if (ColorantCount > cmsMAXCHANNELS) ColorantCount = cmsMAXCHANNELS;
        
        if (Size >= 16) {
            size_t prefix_len = Data[8] % 32;
            size_t suffix_len = Data[9] % 32;
            if (Size >= 10 + prefix_len) {
                memcpy(Prefix, Data + 10, prefix_len);
                Prefix[prefix_len] = '\0';
            }
            if (Size >= 10 + prefix_len + suffix_len) {
                memcpy(Suffix, Data + 10 + prefix_len, suffix_len);
                Suffix[suffix_len] = '\0';
            }
        }
    }

    // Test cmsAllocNamedColorList
    cmsNAMEDCOLORLIST *list = cmsAllocNamedColorList(ContextID, n, ColorantCount, Prefix, Suffix);
    
    if (list) {
        // Test cmsNamedColorCount
        cmsUInt32Number count = cmsNamedColorCount(list);
        
        // Test cmsDupNamedColorList
        cmsNAMEDCOLORLIST *dup_list = cmsDupNamedColorList(list);
        if (dup_list) {
            cmsNamedColorCount(dup_list);
            cmsFreeNamedColorList(dup_list);
        }

        // Test cmsNamedColorInfo with various indices
        char name[256] = {0};
        char prefix[33] = {0};
        char suffix[33] = {0};
        cmsUInt16Number pcs[4] = {0};
        cmsUInt16Number colorant[8] = {0};

        // Try index 0
        cmsNamedColorInfo(list, 0, name, prefix, suffix, pcs, colorant);
        
        // Try index out of bounds (should return FALSE safely)
        cmsNamedColorInfo(list, count + 10, name, prefix, suffix, pcs, colorant);
        
        // Try with NULL buffers
        cmsNamedColorInfo(list, 0, NULL, NULL, NULL, NULL, NULL);

        // Free the original list
        cmsFreeNamedColorList(list);
    }

    // Test cmsFreeNamedColorList with NULL (should be safe)
    cmsFreeNamedColorList(NULL);

    // Test cmsGetNamedColorList with NULL transform
    cmsNAMEDCOLORLIST *null_list = cmsGetNamedColorList(NULL);
    if (null_list) {
        cmsFreeNamedColorList(null_list);
    }

    // Cleanup context
    cmsDeleteContext(ContextID);

    // Write dummy file if data exists
    if (Size > 0) {
        write_dummy_file(Data, Size);
    }

    return 0;
}