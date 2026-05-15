// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetNamedColorList at cmsnamed.c:967:30 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

#define MAX_NAME_LEN 256
#define MAX_PREFIX_SUFFIX_LEN 32

static void dummy_cleanup(void) {
    // No-op cleanup function
}

static cmsNAMEDCOLORLIST* create_test_named_color_list(const uint8_t* data, size_t size) {
    // We need to create a transform first to get a named color list
    // Create dummy profiles for testing
    cmsHPROFILE src_profile = cmsCreate_sRGBProfile();
    cmsHPROFILE dst_profile = cmsCreate_sRGBProfile();
    
    if (!src_profile || !dst_profile) {
        if (src_profile) cmsCloseProfile(src_profile);
        if (dst_profile) cmsCloseProfile(dst_profile);
        return NULL;
    }
    
    cmsHTRANSFORM transform = cmsCreateTransform(src_profile, TYPE_RGB_8,
                                                 dst_profile, TYPE_RGB_8,
                                                 INTENT_PERCEPTUAL, 0);
    
    cmsCloseProfile(src_profile);
    cmsCloseProfile(dst_profile);
    
    if (!transform) {
        return NULL;
    }
    
    // Get the named color list from the transform
    cmsNAMEDCOLORLIST* list = cmsGetNamedColorList(transform);
    
    // Duplicate the list to have our own copy
    cmsNAMEDCOLORLIST* duplicated_list = NULL;
    if (list) {
        duplicated_list = cmsDupNamedColorList(list);
    }
    
    cmsDeleteTransform(transform);
    
    // If we got a duplicated list, try to append some colors using fuzz data
    if (duplicated_list && size > 0) {
        size_t offset = 0;
        for (int i = 0; i < 3 && offset + 10 < size; i++) {
            char name[32];
            cmsUInt16Number pcs[3] = {0x8000, 0x8000, 0x8000};
            cmsUInt16Number colorant[16] = {0};
            
            // Create a simple name
            snprintf(name, sizeof(name), "Color%d", i);
            
            // Use some fuzz data for color values if available
            if (offset + 6 < size) {
                for (int j = 0; j < 3; j++) {
                    pcs[j] = (data[offset] << 8) | data[offset + 1];
                    offset += 2;
                }
            }
            
            cmsAppendNamedColor(duplicated_list, name, pcs, colorant);
        }
    }
    
    return duplicated_list;
}

int LLVMFuzzerTestOneInput_18(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Test cmsDupNamedColorList with NULL input
    cmsNAMEDCOLORLIST* null_dup = cmsDupNamedColorList(NULL);
    if (null_dup) {
        cmsFreeNamedColorList(null_dup);
    }
    
    // Create a test named color list using fuzz data
    cmsNAMEDCOLORLIST* original_list = create_test_named_color_list(Data, Size);
    
    if (original_list) {
        // Test cmsDupNamedColorList with valid input
        cmsNAMEDCOLORLIST* duplicated_list = cmsDupNamedColorList(original_list);
        
        if (duplicated_list) {
            // Test cmsNamedColorInfo on both lists
            char name[MAX_NAME_LEN];
            char prefix[MAX_PREFIX_SUFFIX_LEN];
            char suffix[MAX_PREFIX_SUFFIX_LEN];
            cmsUInt16Number pcs[3];
            cmsUInt16Number colorant[16];
            
            // Try to get color info for indices 0-9
            for (cmsUInt32Number i = 0; i < 10; i++) {
                cmsNamedColorInfo(original_list, i, name, prefix, suffix, pcs, colorant);
                cmsNamedColorInfo(duplicated_list, i, name, prefix, suffix, pcs, colorant);
            }
            
            // Test cmsNamedColorIndex
            for (int i = 0; i < 3; i++) {
                char search_name[32];
                snprintf(search_name, sizeof(search_name), "Color%d", i);
                cmsNamedColorIndex(original_list, search_name);
                cmsNamedColorIndex(duplicated_list, search_name);
            }
            
            // Test with non-existent name
            cmsNamedColorIndex(original_list, "NonExistentColorName123");
            
            // Test cmsAppendNamedColor with some fuzz data
            if (Size > 20) {
                char new_name[32] = "FuzzColor";
                cmsUInt16Number new_pcs[3] = {
                    (Data[0] << 8) | Data[1],
                    (Data[2] << 8) | Data[3],
                    (Data[4] << 8) | Data[5]
                };
                cmsUInt16Number new_colorant[16] = {0};
                
                // Fill colorant array with fuzz data if available
                size_t colorant_bytes = 16 * sizeof(cmsUInt16Number);
                if (Size > 20 + colorant_bytes) {
                    for (int i = 0; i < 16 && (20 + i*2 + 1) < Size; i++) {
                        new_colorant[i] = (Data[20 + i*2] << 8) | Data[20 + i*2 + 1];
                    }
                }
                
                cmsAppendNamedColor(original_list, new_name, new_pcs, new_colorant);
                
                // Try to append with NULL name
                cmsAppendNamedColor(original_list, NULL, new_pcs, new_colorant);
            }
            
            // Clean up duplicated list
            cmsFreeNamedColorList(duplicated_list);
        }
        
        // Clean up original list
        cmsFreeNamedColorList(original_list);
    }
    
    // Test cmsNamedColorIndex with NULL list
    cmsNamedColorIndex(NULL, "Test");
    
    // Test cmsNamedColorInfo with NULL list
    char dummy_name[MAX_NAME_LEN];
    char dummy_prefix[MAX_PREFIX_SUFFIX_LEN];
    char dummy_suffix[MAX_PREFIX_SUFFIX_LEN];
    cmsUInt16Number dummy_pcs[3];
    cmsUInt16Number dummy_colorant[16];
    cmsNamedColorInfo(NULL, 0, dummy_name, dummy_prefix, dummy_suffix, dummy_pcs, dummy_colorant);
    
    // Test cmsFreeNamedColorList with NULL
    cmsFreeNamedColorList(NULL);
    
    dummy_cleanup();
    return 0;
}