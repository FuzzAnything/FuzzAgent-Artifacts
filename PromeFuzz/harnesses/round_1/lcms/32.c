// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetNamedColorList at cmsnamed.c:967:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_named_color_list_operations(cmsNAMEDCOLORLIST* list) {
    if (!list) return;
    
    // Get the count of named colors
    cmsUInt32Number count = cmsNamedColorCount(list);
    
    // Try to get info for some indices
    if (count > 0) {
        // Allocate buffers for color info
        char name[256];
        char prefix[256];
        char suffix[256];
        cmsUInt16Number pcs[3];
        cmsUInt16Number colorant[16]; // Assuming max 16 colorants
        
        // Try to get info for first, middle, and last indices
        for (cmsUInt32Number i = 0; i < count && i < 3; i++) {
            cmsUInt32Number index = (i == 0) ? 0 : 
                                   (i == 1 && count > 1) ? count / 2 : 
                                   count - 1;
            
            if (index < count) {
                cmsNamedColorInfo(list, index, name, prefix, suffix, pcs, colorant);
            }
        }
        
        // Also try out-of-range indices
        cmsNamedColorInfo(list, count, name, prefix, suffix, pcs, colorant);
        cmsNamedColorInfo(list, count + 1, name, prefix, suffix, pcs, colorant);
    }
}

static cmsNAMEDCOLORLIST* create_named_color_list_from_fuzz(const uint8_t* data, size_t size) {
    if (size < 8) return NULL;
    
    // Use fuzz data to determine parameters
    cmsUInt32Number n = *(cmsUInt32Number*)data % 100; // Limit to reasonable size
    cmsUInt32Number colorant_count = *(cmsUInt32Number*)(data + 4) % 16; // Max 16 colorants
    
    // Extract prefix and suffix from fuzz data if available
    const char* prefix = NULL;
    const char* suffix = NULL;
    
    char prefix_buf[33] = {0};
    char suffix_buf[33] = {0};
    
    if (size >= 8 + 32) {
        memcpy(prefix_buf, data + 8, 32);
        prefix_buf[32] = '\0';
        prefix = prefix_buf;
    }
    
    if (size >= 8 + 64) {
        memcpy(suffix_buf, data + 40, 32);
        suffix_buf[32] = '\0';
        suffix = suffix_buf;
    }
    
    // Allocate the named color list
    return cmsAllocNamedColorList(NULL, n, colorant_count, prefix, suffix);
}

int LLVMFuzzerTestOneInput_32(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Part 1: Test cmsAllocNamedColorList and related operations
    cmsNAMEDCOLORLIST* list1 = create_named_color_list_from_fuzz(Data, Size);
    
    if (list1) {
        // Test operations on the allocated list
        fuzz_named_color_list_operations(list1);
        
        // Test duplication
        cmsNAMEDCOLORLIST* list2 = cmsDupNamedColorList(list1);
        if (list2) {
            fuzz_named_color_list_operations(list2);
            cmsFreeNamedColorList(list2);
        }
        
        // Free the original list
        cmsFreeNamedColorList(list1);
    }
    
    // Part 2: Test cmsGetNamedColorList with a dummy transform
    // Create a simple transform to test cmsGetNamedColorList
    // First create some profiles
    cmsHPROFILE hProfile1 = cmsCreate_sRGBProfile();
    cmsHPROFILE hProfile2 = cmsCreate_sRGBProfile();
    
    if (hProfile1 && hProfile2) {
        // Create a transform
        cmsHTRANSFORM hTransform = cmsCreateTransform(hProfile1, TYPE_RGB_8, 
                                                     hProfile2, TYPE_RGB_8, 
                                                     INTENT_PERCEPTUAL, 0);
        
        if (hTransform) {
            // Try to get named color list from transform
            cmsNAMEDCOLORLIST* transform_list = cmsGetNamedColorList(hTransform);
            
            if (transform_list) {
                // This shouldn't happen with sRGB profiles, but handle it if it does
                fuzz_named_color_list_operations(transform_list);
                cmsFreeNamedColorList(transform_list);
            }
            
            cmsDeleteTransform(hTransform);
        }
        
        cmsCloseProfile(hProfile1);
        cmsCloseProfile(hProfile2);
    }
    
    // Part 3: Test edge cases
    // Test with NULL parameters
    cmsNamedColorInfo(NULL, 0, NULL, NULL, NULL, NULL, NULL);
    cmsNamedColorCount(NULL);
    cmsDupNamedColorList(NULL);
    cmsFreeNamedColorList(NULL);
    
    // Test allocation with extreme values
    if (Size >= 8) {
        cmsUInt32Number extreme_n = *(cmsUInt32Number*)Data;
        cmsUInt32Number extreme_colorant = *(cmsUInt32Number*)(Data + 4);
        
        // These may fail, which is fine
        cmsNAMEDCOLORLIST* extreme_list = cmsAllocNamedColorList(NULL, 
                                                                extreme_n, 
                                                                extreme_colorant, 
                                                                NULL, NULL);
        if (extreme_list) {
            cmsFreeNamedColorList(extreme_list);
        }
    }
    
    return 0;
}