// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsGetNamedColorList at cmsnamed.c:967:30 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    // Use default context
    cmsContext context = NULL;
    
    // Parse parameters from fuzzing input
    if (Size < 12) return 0; // Need at least 12 bytes for basic parameters
    
    // Extract parameters for cmsAllocNamedColorList
    cmsUInt32Number n = ((cmsUInt32Number*)Data)[0] % 1000; // Limit to reasonable size
    cmsUInt32Number colorantCount = ((cmsUInt32Number*)Data)[1] % 16; // Limit to reasonable size
    
    // Move pointer past the two integers
    const uint8_t* strData = Data + 8;
    size_t strSize = Size - 8;
    
    // Define buffers in the outer scope to avoid use-after-scope
    char prefix_buf[33] = {0};
    char suffix_buf[33] = {0};
    const char* prefix = "";
    const char* suffix = "";
    
    if (strSize > 0) {
        // Extract prefix string
        size_t prefix_len = 0;
        while (prefix_len < strSize && prefix_len < 32 && strData[prefix_len] != 0) {
            prefix_buf[prefix_len] = strData[prefix_len];
            prefix_len++;
        }
        prefix_buf[prefix_len] = '\0';
        prefix = prefix_buf;
        
        // Extract suffix string if there's more data
        size_t suffix_start = prefix_len + 1;
        if (suffix_start < strSize) {
            size_t suffix_len = 0;
            while (suffix_start + suffix_len < strSize && suffix_len < 32 && 
                   strData[suffix_start + suffix_len] != 0) {
                suffix_buf[suffix_len] = strData[suffix_start + suffix_len];
                suffix_len++;
            }
            suffix_buf[suffix_len] = '\0';
            suffix = suffix_buf;
        }
    }
    
    // Test cmsAllocNamedColorList
    cmsNAMEDCOLORLIST* namedColorList = cmsAllocNamedColorList(context, n, colorantCount, prefix, suffix);
    
    if (namedColorList != NULL) {
        // Test cmsNamedColorCount
        cmsUInt32Number count = cmsNamedColorCount(namedColorList);
        
        // Test cmsNamedColorInfo for some indices
        if (count > 0) {
            // Test first entry
            char name[256] = {0};
            char prefix_info[33] = {0};
            char suffix_info[33] = {0};
            cmsUInt16Number pcs[3] = {0};
            cmsUInt16Number colorant[16] = {0}; // Max 16 colorants
            
            cmsBool result = cmsNamedColorInfo(namedColorList, 0, name, prefix_info, suffix_info, pcs, colorant);
            
            // Test middle entry if exists
            if (count > 1) {
                cmsUInt32Number middle = count / 2;
                if (middle < count) {
                    cmsNamedColorInfo(namedColorList, middle, name, prefix_info, suffix_info, pcs, colorant);
                }
            }
            
            // Test last entry
            if (count > 2) {
                cmsNamedColorInfo(namedColorList, count - 1, name, prefix_info, suffix_info, pcs, colorant);
            }
            
            // Test out of bounds index
            cmsNamedColorInfo(namedColorList, count, name, prefix_info, suffix_info, pcs, colorant);
        }
        
        // Test cmsDupNamedColorList
        cmsNAMEDCOLORLIST* dupList = cmsDupNamedColorList(namedColorList);
        if (dupList != NULL) {
            // Verify duplicate has same count
            cmsUInt32Number dupCount = cmsNamedColorCount(dupList);
            
            // Test cmsNamedColorInfo on duplicate
            if (dupCount > 0) {
                char name[256] = {0};
                char prefix_info[33] = {0};
                char suffix_info[33] = {0};
                cmsUInt16Number pcs[3] = {0};
                cmsUInt16Number colorant[16] = {0};
                
                cmsNamedColorInfo(dupList, 0, name, prefix_info, suffix_info, pcs, colorant);
            }
            
            // Clean up duplicate
            cmsFreeNamedColorList(dupList);
        }
        
        // Clean up original list
        cmsFreeNamedColorList(namedColorList);
    }
    
    // Test cmsGetNamedColorList - requires a transform
    // Create a dummy transform handle (simplified for fuzzing)
    cmsHTRANSFORM dummyTransform = NULL;
    
    // This will typically return NULL since we don't have a real transform
    cmsNAMEDCOLORLIST* transformList = cmsGetNamedColorList(dummyTransform);
    
    // Test cmsNamedColorInfo with NULL list
    if (transformList == NULL) {
        char name[256] = {0};
        char prefix_info[33] = {0};
        char suffix_info[33] = {0};
        cmsUInt16Number pcs[3] = {0};
        cmsUInt16Number colorant[16] = {0};
        
        cmsNamedColorInfo(NULL, 0, name, prefix_info, suffix_info, pcs, colorant);
    }
    
    // Test cmsNamedColorCount with NULL list
    cmsUInt32Number nullCount = cmsNamedColorCount(NULL);
    
    // Test cmsDupNamedColorList with NULL list
    cmsNAMEDCOLORLIST* nullDup = cmsDupNamedColorList(NULL);
    
    // Test cmsFreeNamedColorList with NULL (should be safe)
    cmsFreeNamedColorList(NULL);
    
    return 0;
}

#ifdef __cplusplus
}
#endif