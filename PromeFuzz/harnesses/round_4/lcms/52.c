// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetNamedColorList at cmsnamed.c:967:30 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsNamedColorCount at cmsnamed.c:848:27 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"

static void fuzz_cmsNamedColorInfo(const cmsNAMEDCOLORLIST* list) {
    if (list == NULL) return;
    
    cmsUInt32Number count = cmsNamedColorCount(list);
    if (count == 0) return;
    
    // Test with valid indices
    for (cmsUInt32Number i = 0; i < count && i < 5; i++) {
        char Name[256];
        char Prefix[33];
        char Suffix[33];
        cmsUInt16Number PCS[3];
        cmsUInt16Number Colorant[16]; // Use fixed size array
        
        cmsNamedColorInfo(list, i, Name, Prefix, Suffix, PCS, Colorant);
    }
    
    // Test with out-of-range index
    if (count > 0) {
        char Name[256];
        char Prefix[33];
        char Suffix[33];
        cmsUInt16Number PCS[3];
        cmsUInt16Number Colorant[16];
        cmsNamedColorInfo(list, count + 100, Name, Prefix, Suffix, PCS, Colorant);
    }
}

static cmsNAMEDCOLORLIST* create_test_named_color_list(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 8) return NULL;
    
    // Extract parameters from fuzzer input
    cmsUInt32Number n = *(cmsUInt32Number*)data % 100;
    cmsUInt32Number colorant_count = *(cmsUInt32Number*)(data + 4) % 16;
    if (colorant_count == 0) colorant_count = 1;
    
    // Extract prefix and suffix strings (limited to 32 chars + null terminator)
    char prefix[33] = {0};
    char suffix[33] = {0};
    
    size_t offset = 8;
    if (offset < size) {
        size_t prefix_len = data[offset] % 32;
        if (prefix_len > 0 && offset + prefix_len < size) {
            memcpy(prefix, data + offset, prefix_len);
            prefix[prefix_len] = '\0';
        }
        offset += prefix_len + 1;
    }
    
    if (offset < size) {
        size_t suffix_len = data[offset] % 32;
        if (suffix_len > 0 && offset + suffix_len < size) {
            memcpy(suffix, data + offset, suffix_len);
            suffix[suffix_len] = '\0';
        }
    }
    
    return cmsAllocNamedColorList(ctx, n, colorant_count, prefix, suffix);
}

int LLVMFuzzerTestOneInput_52(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;
    
    // Test 1: Create and test a named color list
    cmsNAMEDCOLORLIST* list = create_test_named_color_list(ctx, Data, Size);
    if (list != NULL) {
        // Test cmsNamedColorCount
        cmsUInt32Number count = cmsNamedColorCount(list);
        
        // Test cmsNamedColorInfo
        fuzz_cmsNamedColorInfo(list);
        
        // Test cmsDupNamedColorList
        cmsNAMEDCOLORLIST* dup_list = cmsDupNamedColorList(list);
        if (dup_list != NULL) {
            fuzz_cmsNamedColorInfo(dup_list);
            cmsFreeNamedColorList(dup_list);
        }
        
        // Test with NULL input
        cmsNamedColorCount(NULL);
        cmsDupNamedColorList(NULL);
        cmsFreeNamedColorList(NULL);
        
        // Clean up
        cmsFreeNamedColorList(list);
    }
    
    // Test 2: Test cmsGetNamedColorList with NULL transform
    cmsNAMEDCOLORLIST* transform_list = cmsGetNamedColorList(NULL);
    
    // Test 3: Test edge cases for cmsAllocNamedColorList
    // Test with excessive colorant count (should return NULL)
    cmsNAMEDCOLORLIST* excessive_list = cmsAllocNamedColorList(ctx, 10, 1000, "test", "test");
    if (excessive_list != NULL) {
        cmsFreeNamedColorList(excessive_list);
    }
    
    // Test with NULL context (uses global context)
    cmsNAMEDCOLORLIST* null_ctx_list = cmsAllocNamedColorList(NULL, 5, 3, "pre", "suf");
    if (null_ctx_list != NULL) {
        cmsFreeNamedColorList(null_ctx_list);
    }
    
    // Clean up context
    cmsDeleteContext(ctx);
    
    return 0;
}