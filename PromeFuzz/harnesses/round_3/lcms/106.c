// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// cmsIT8TableCount at cmscgats.c:2981:27 in lcms2.h
// cmsIT8EnumProperties at cmscgats.c:2665:27 in lcms2.h
// cmsIT8EnumPropertyMulti at cmscgats.c:2700:27 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATCH_INDEX 100
#define MAX_PROPERTY_NAME_LEN 100
#define MAX_STR 1024

static void cleanup_property_names(char ***names, cmsUInt32Number count) {
    if (names && *names) {
        // Use standard free since lcms doesn't provide cmsFree in the public API
        free(*names);
        *names = NULL;
    }
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    // Return early if input is empty to avoid assertion error
    if (Size == 0) {
        return 0;
    }
    
    // Use default context
    cmsContext contextID = NULL;
    
    // Load IT8 data from memory
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(contextID, Data, (cmsUInt32Number)Size);
    if (hIT8 == NULL) {
        return 0;
    }
    
    // Test cmsIT8TableCount
    cmsUInt32Number table_count = cmsIT8TableCount(hIT8);
    
    // Test cmsIT8EnumProperties
    char **property_names = NULL;
    cmsUInt32Number prop_count = cmsIT8EnumProperties(hIT8, &property_names);
    
    // Test cmsIT8EnumPropertyMulti for some properties
    if (prop_count > 0 && property_names != NULL) {
        // Try first few properties
        for (cmsUInt32Number i = 0; i < prop_count && i < 5; i++) {
            if (property_names[i] != NULL) {
                const char **subproperty_names = NULL;
                cmsUInt32Number subprop_count = cmsIT8EnumPropertyMulti(hIT8, property_names[i], &subproperty_names);
                
                // Clean up subproperty names array if allocated
                if (subproperty_names != NULL) {
                    // Use standard free for memory allocated by lcms
                    free((void*)subproperty_names);
                }
            }
        }
    }
    
    // Test cmsIT8GetPatchName for some indices
    for (int i = 0; i < MAX_PATCH_INDEX; i++) {
        // First call with NULL buffer to get pointer to internal data
        const char *internal_name = cmsIT8GetPatchName(hIT8, i, NULL);
        
        // Then call with buffer to copy
        if (internal_name != NULL) {
            char buffer[MAX_STR];
            const char *copied_name = cmsIT8GetPatchName(hIT8, i, buffer);
        }
    }
    
    // Test cmsIT8SaveToMem
    cmsUInt32Number bytes_needed = 0;
    
    // First call to get required size
    cmsBool success = cmsIT8SaveToMem(hIT8, NULL, &bytes_needed);
    
    // Second call to actually save if we have enough data
    if (success && bytes_needed > 0 && bytes_needed < 10 * 1024 * 1024) { // Limit to 10MB
        void *buffer = malloc(bytes_needed);
        if (buffer != NULL) {
            success = cmsIT8SaveToMem(hIT8, buffer, &bytes_needed);
            free(buffer);
        }
    }
    
    // Clean up property names array using standard free
    cleanup_property_names(&property_names, prop_count);
    
    // Clean up IT8 handle
    if (hIT8 != NULL) {
        cmsIT8Free(hIT8);
    }
    
    return 0;
}