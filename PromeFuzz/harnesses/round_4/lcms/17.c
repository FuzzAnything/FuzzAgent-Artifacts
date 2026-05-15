// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsGetNamedColorList at cmsnamed.c:967:30 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsNamedColorInfo at cmsnamed.c:855:20 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsDupNamedColorList at cmsnamed.c:787:30 in lcms2.h
// cmsAppendNamedColor at cmsnamed.c:815:20 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"

static cmsNAMEDCOLORLIST* create_test_list(cmsContext context, const uint8_t* data, size_t size) {
    if (size < 10) return NULL;
    
    // Create a named color list with some initial capacity
    cmsNAMEDCOLORLIST* list = cmsAllocNamedColorList(context, 0, 3, "PREFIX", "SUFFIX");
    if (!list) return NULL;
    
    // Add a few colors using data from fuzzer input
    for (int i = 0; i < 3 && size >= 20; i++) {
        char name[32];
        size_t name_len = (data[i] % 31) + 1;
        if (name_len > 31) name_len = 31;
        
        // Extract name from fuzzer data
        for (size_t j = 0; j < name_len; j++) {
            name[j] = data[(i * 10 + j) % size];
        }
        name[name_len] = '\0';
        
        cmsUInt16Number PCS[3] = {
            (cmsUInt16Number)(data[(i * 10 + 0) % size] << 8 | data[(i * 10 + 1) % size]),
            (cmsUInt16Number)(data[(i * 10 + 2) % size] << 8 | data[(i * 10 + 3) % size]),
            (cmsUInt16Number)(data[(i * 10 + 4) % size] << 8 | data[(i * 10 + 5) % size])
        };
        
        cmsUInt16Number Colorant[3] = {
            (cmsUInt16Number)(data[(i * 10 + 6) % size] << 8 | data[(i * 10 + 7) % size]),
            (cmsUInt16Number)(data[(i * 10 + 8) % size] << 8 | data[(i * 10 + 9) % size]),
            0
        };
        
        cmsAppendNamedColor(list, name, PCS, Colorant);
    }
    
    return list;
}

static void test_named_color_functions(cmsNAMEDCOLORLIST* list, const uint8_t* data, size_t size) {
    if (!list || size == 0) return;
    
    // Test cmsNamedColorInfo for various indices
    // We'll test indices 0-4 to cover both valid and potentially invalid indices
    for (cmsUInt32Number i = 0; i < 5; i++) {
        char name[256];
        char prefix[33];
        char suffix[33];
        cmsUInt16Number PCS[3];
        cmsUInt16Number Colorant[3];
        
        cmsNamedColorInfo(list, i, name, prefix, suffix, PCS, Colorant);
    }
    
    // Test cmsNamedColorIndex with names derived from fuzzer data
    if (size > 10) {
        char search_name[32];
        size_t name_len = (data[0] % 31) + 1;
        if (name_len > 31) name_len = 31;
        
        for (size_t i = 0; i < name_len; i++) {
            search_name[i] = data[(i + 5) % size];
        }
        search_name[name_len] = '\0';
        
        cmsNamedColorIndex(list, search_name);
    }
    
    // Test with empty string
    cmsNamedColorIndex(list, "");
    
    // Test with NULL name
    cmsNamedColorIndex(list, NULL);
}

static void test_duplicate_and_free(cmsNAMEDCOLORLIST* list) {
    if (!list) return;
    
    // Test duplication
    cmsNAMEDCOLORLIST* dup = cmsDupNamedColorList(list);
    
    // Test operations on duplicate
    if (dup) {
        // Add another color to duplicate
        char name[] = "DuplicatedColor";
        cmsUInt16Number PCS[3] = {0x8000, 0x8000, 0x8000};
        cmsUInt16Number Colorant[3] = {0x4000, 0x4000, 0x4000};
        
        cmsAppendNamedColor(dup, name, PCS, Colorant);
        
        // Free the duplicate
        cmsFreeNamedColorList(dup);
    }
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Create a context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Create a test named color list
    cmsNAMEDCOLORLIST* list = create_test_list(context, Data, Size);
    
    if (list) {
        // Test all the API functions
        test_named_color_functions(list, Data, Size);
        test_duplicate_and_free(list);
        
        // Test cmsAppendNamedColor with various inputs
        if (Size >= 20) {
            char new_name[32];
            size_t name_len = (Data[10] % 31) + 1;
            if (name_len > 31) name_len = 31;
            
            for (size_t i = 0; i < name_len; i++) {
                new_name[i] = Data[(i + 15) % Size];
            }
            new_name[name_len] = '\0';
            
            cmsUInt16Number PCS[3] = {
                (cmsUInt16Number)(Data[0] << 8 | Data[1]),
                (cmsUInt16Number)(Data[2] << 8 | Data[3]),
                (cmsUInt16Number)(Data[4] << 8 | Data[5])
            };
            
            cmsUInt16Number Colorant[3] = {
                (cmsUInt16Number)(Data[6] << 8 | Data[7]),
                (cmsUInt16Number)(Data[8] << 8 | Data[9]),
                0
            };
            
            // Test with valid data
            cmsAppendNamedColor(list, new_name, PCS, Colorant);
            
            // Test with NULL name
            cmsAppendNamedColor(list, NULL, PCS, Colorant);
            
            // Test with NULL PCS and Colorant
            cmsAppendNamedColor(list, "TestColor", NULL, NULL);
            
            // Test with all NULL except list
            cmsAppendNamedColor(list, NULL, NULL, NULL);
        }
        
        // Test cmsFreeNamedColorList
        cmsFreeNamedColorList(list);
        
        // Test freeing NULL
        cmsFreeNamedColorList(NULL);
    }
    
    // Test cmsDupNamedColorList with NULL
    cmsNAMEDCOLORLIST* null_dup = cmsDupNamedColorList(NULL);
    if (null_dup) {
        cmsFreeNamedColorList(null_dup);
    }
    
    // Note: cmsGetNamedColorList requires a transform handle
    // We can't easily create a valid transform for fuzzing, so we'll test with NULL
    cmsGetNamedColorList(NULL);
    
    // Clean up context
    cmsDeleteContext(context);
    
    return 0;
}