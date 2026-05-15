// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8TableCount at cmscgats.c:2981:27 in lcms2.h
// cmsIT8EnumProperties at cmscgats.c:2665:27 in lcms2.h
// cmsIT8EnumPropertyMulti at cmscgats.c:2700:27 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_property_names(char ***names, cmsUInt32Number count) {
    if (names && *names) {
        for (cmsUInt32Number i = 0; i < count; i++) {
            if ((*names)[i]) {
                free((*names)[i]);
            }
        }
        free(*names);
        *names = NULL;
    }
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    // Create a context for the IT8 operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Load IT8 data from memory
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(context, Data, (cmsUInt32Number)Size);
    if (!hIT8) {
        cmsDeleteContext(context);
        return 0;
    }

    // Test cmsIT8TableCount
    cmsUInt32Number table_count = cmsIT8TableCount(hIT8);
    (void)table_count; // Use result to avoid unused variable warning

    // Test cmsIT8EnumProperties
    char **property_names = NULL;
    cmsUInt32Number prop_count = cmsIT8EnumProperties(hIT8, &property_names);
    
    if (prop_count > 0 && property_names) {
        // For each property, test cmsIT8EnumPropertyMulti
        for (cmsUInt32Number i = 0; i < prop_count && i < 5; i++) { // Limit to 5 properties
            const char **subproperty_names = NULL;
            cmsUInt32Number subprop_count = cmsIT8EnumPropertyMulti(hIT8, property_names[i], &subproperty_names);
            
            if (subprop_count > 0 && subproperty_names) {
                // Try to set some multi-properties if we have subproperties
                if (Size > 0) {
                    // Create dummy buffer from fuzzer data
                    char dummy_buffer[256];
                    size_t copy_size = Size < 255 ? Size : 255;
                    memcpy(dummy_buffer, Data, copy_size);
                    dummy_buffer[copy_size] = '\0';
                    
                    // Try to set property with first subproperty
                    cmsIT8SetPropertyMulti(hIT8, property_names[i], subproperty_names[0], dummy_buffer);
                    
                    // Try with empty subkey
                    cmsIT8SetPropertyMulti(hIT8, property_names[i], "", dummy_buffer);
                    
                    // Try with NULL buffer
                    cmsIT8SetPropertyMulti(hIT8, property_names[i], subproperty_names[0], NULL);
                }
                
                // Free subproperty names array
                if (subproperty_names) {
                    free(subproperty_names);
                }
            }
        }
        
        // Clean up property names
        cleanup_property_names(&property_names, prop_count);
    }

    // Test cmsIT8SaveToMem
    cmsUInt32Number bytes_needed = 0;
    cmsBool save_result = cmsIT8SaveToMem(hIT8, NULL, &bytes_needed);
    
    if (save_result && bytes_needed > 0 && bytes_needed < 10 * 1024 * 1024) { // Limit to 10MB
        void *buffer = malloc(bytes_needed);
        if (buffer) {
            cmsIT8SaveToMem(hIT8, buffer, &bytes_needed);
            free(buffer);
        }
    }

    // Clean up
    cmsIT8Free(hIT8);
    cmsDeleteContext(context);
    
    return 0;
}