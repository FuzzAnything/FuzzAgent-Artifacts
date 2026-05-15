// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetNamedColorList at cmsnamed.c:967:30 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_cleanup(cmsNAMEDCOLORLIST* list1, cmsNAMEDCOLORLIST* list2) {
    if (list1) cmsFreeNamedColorList(list1);
    if (list2) cmsFreeNamedColorList(list2);
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    // Create a dummy transform by writing data to file and creating profile
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!hProfile) {
        remove("./dummy_file");
        return 0;
    }
    
    // Create transform to get named color list
    cmsHTRANSFORM xform = cmsCreateTransform(hProfile, TYPE_RGB_8, hProfile, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);
    if (!xform) {
        cmsCloseProfile(hProfile);
        remove("./dummy_file");
        return 0;
    }
    
    // Test cmsGetNamedColorList
    cmsNAMEDCOLORLIST* named_list = cmsGetNamedColorList(xform);
    
    // Test cmsDupNamedColorList
    cmsNAMEDCOLORLIST* dup_list = NULL;
    if (named_list) {
        dup_list = cmsDupNamedColorList(named_list);
    }
    
    // Test cmsNamedColorInfo with various indices
    if (named_list) {
        char name[256], prefix[256], suffix[256];
        cmsUInt16Number PCS[3];
        cmsUInt16Number Colorant[cmsMAXCHANNELS];
        
        // Try first few indices
        for (cmsUInt32Number i = 0; i < 5 && i < Size; i++) {
            cmsNamedColorInfo(named_list, i, name, prefix, suffix, PCS, Colorant);
        }
        
        // Try index from fuzzer data
        if (Size >= 4) {
            cmsUInt32Number idx = *(cmsUInt32Number*)Data;
            cmsNamedColorInfo(named_list, idx, name, prefix, suffix, PCS, Colorant);
        }
    }
    
    // Test cmsNamedColorIndex
    if (named_list && Size > 0) {
        // Create null-terminated name from fuzzer data
        size_t name_len = Size < 255 ? Size : 255;
        char* search_name = (char*)malloc(name_len + 1);
        if (search_name) {
            memcpy(search_name, Data, name_len);
            search_name[name_len] = '\0';
            cmsNamedColorIndex(named_list, search_name);
            free(search_name);
        }
    }
    
    // Test cmsAppendNamedColor on duplicated list
    if (dup_list && Size >= 20) {
        // Extract name from data
        size_t name_len = Size < 100 ? Size : 100;
        char* new_name = (char*)malloc(name_len + 1);
        if (new_name) {
            memcpy(new_name, Data, name_len);
            new_name[name_len] = '\0';
            
            cmsUInt16Number PCS[3] = {0};
            cmsUInt16Number Colorant[cmsMAXCHANNELS] = {0};
            
            // Fill with some data if available
            if (Size >= 10) {
                for (int i = 0; i < 3 && i < cmsMAXCHANNELS; i++) {
                    if (Size > i) PCS[i] = Data[i] << 8;
                }
            }
            
            cmsAppendNamedColor(dup_list, new_name, PCS, Colorant);
            free(new_name);
        }
    }
    
    // Cleanup
    dummy_cleanup(named_list, dup_list);
    cmsDeleteTransform(xform);
    cmsCloseProfile(hProfile);
    remove("./dummy_file");
    
    return 0;
}