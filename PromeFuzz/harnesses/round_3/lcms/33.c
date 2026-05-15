// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsGetNamedColorList at cmsnamed.c:967:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static cmsHTRANSFORM create_dummy_transform(cmsContext ctx) {
    // Create simple RGB profiles for a dummy transform
    cmsHPROFILE hSRGB = cmsCreate_sRGBProfile();
    if (!hSRGB) return NULL;
    
    cmsHPROFILE hLab = cmsCreateLab4Profile(NULL);
    if (!hLab) {
        cmsCloseProfile(hSRGB);
        return NULL;
    }
    
    cmsHTRANSFORM xform = cmsCreateTransform(hSRGB, TYPE_RGB_8,
                                             hLab, TYPE_Lab_8,
                                             INTENT_PERCEPTUAL, 0);
    cmsCloseProfile(hSRGB);
    cmsCloseProfile(hLab);
    
    return xform;
}

static void initialize_named_color_list(cmsNAMEDCOLORLIST* list, const uint8_t* data, size_t size) {
    if (!list || size < 1) return;
    
    // Use data to potentially fill some entries if the API allows
    // Note: cmsAllocNamedColorList only allocates, doesn't populate
    // We would need cmsAppendNamedColor to populate, but it's not in our API list
    // So we'll just test the query functions on empty/partially filled lists
}

int LLVMFuzzerTestOneInput_33(const uint8_t* Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = create_test_context();
    if (!ctx) return 0;
    
    // Extract parameters from fuzz data
    size_t offset = 0;
    
    // Get n (number of slots) - use first 4 bytes
    cmsUInt32Number n = 0;
    if (offset + 4 <= Size) {
        memcpy(&n, Data + offset, 4);
        offset += 4;
    }
    
    // Get ColorantCount - next 4 bytes
    cmsUInt32Number colorant_count = 0;
    if (offset + 4 <= Size) {
        memcpy(&colorant_count, Data + offset, 4);
        offset += 4;
    }
    
    // Limit values to reasonable ranges to avoid excessive memory allocation
    n = n % 1024;  // Max 1024 slots
    colorant_count = colorant_count % 16;  // Max 16 colorants
    
    // Extract prefix and suffix strings
    const char* prefix = NULL;
    const char* suffix = NULL;
    
    // Find null-terminated strings in the data
    if (offset < Size) {
        prefix = (const char*)(Data + offset);
        size_t prefix_len = strnlen(prefix, Size - offset);
        offset += prefix_len + 1;
        
        if (offset < Size) {
            suffix = (const char*)(Data + offset);
            size_t suffix_len = strnlen(suffix, Size - offset);
            offset += suffix_len + 1;
        }
    }
    
    // Test cmsAllocNamedColorList
    cmsNAMEDCOLORLIST* list1 = cmsAllocNamedColorList(ctx, n, colorant_count, prefix, suffix);
    
    if (list1) {
        // Test cmsNamedColorCount
        cmsUInt32Number count = cmsNamedColorCount(list1);
        
        // Test cmsNamedColorInfo for some indices
        if (count > 0) {
            // Try to get info for a few indices
            for (cmsUInt32Number i = 0; i < count && i < 5; i++) {
                char name[256];
                char pref[33];
                char suff[33];
                cmsUInt16Number pcs[3];
                cmsUInt16Number colorant[16];  // Max 16 colorants
                
                cmsNamedColorInfo(list1, i, name, pref, suff, pcs, colorant);
            }
        }
        
        // Test cmsDupNamedColorList
        cmsNAMEDCOLORLIST* list2 = cmsDupNamedColorList(list1);
        
        if (list2) {
            // Test functions on the duplicated list
            cmsUInt32Number count2 = cmsNamedColorCount(list2);
            
            // Test cmsNamedColorInfo on duplicated list
            if (count2 > 0) {
                char name[256];
                char pref[33];
                char suff[33];
                cmsUInt16Number pcs[3];
                cmsUInt16Number colorant[16];
                
                // Try first and last indices if available
                cmsNamedColorInfo(list2, 0, name, pref, suff, pcs, colorant);
                if (count2 > 1) {
                    cmsNamedColorInfo(list2, count2 - 1, name, pref, suff, pcs, colorant);
                }
            }
            
            // Clean up duplicated list
            cmsFreeNamedColorList(list2);
        }
        
        // Test cmsGetNamedColorList
        cmsHTRANSFORM xform = create_dummy_transform(ctx);
        if (xform) {
            cmsNAMEDCOLORLIST* list3 = cmsGetNamedColorList(xform);
            // Note: list3 may be NULL as dummy transform likely has no named colors
            if (list3) {
                cmsUInt32Number count3 = cmsNamedColorCount(list3);
                // Don't free list3 as it belongs to the transform
            }
            cmsDeleteTransform(xform);
        }
        
        // Clean up original list
        cmsFreeNamedColorList(list1);
    }
    
    // Test edge cases with NULL
    cmsNAMEDCOLORLIST* null_list = NULL;
    cmsFreeNamedColorList(null_list);  // Should handle NULL safely
    cmsDupNamedColorList(null_list);   // Should return NULL
    
    // Test cmsNamedColorInfo with NULL list
    if (Size > 0) {
        char name[256];
        char pref[33];
        char suff[33];
        cmsUInt16Number pcs[3];
        cmsUInt16Number colorant[16];
        
        cmsNamedColorInfo(NULL, 0, name, pref, suff, pcs, colorant);
    }
    
    // Test cmsNamedColorCount with NULL
    cmsUInt32Number null_count = cmsNamedColorCount(NULL);
    
    cmsDeleteContext(ctx);
    
    return 0;
}