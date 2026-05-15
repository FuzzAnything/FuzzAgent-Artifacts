// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8EnumPropertyMulti at cmscgats.c:2700:27 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8TableCount at cmscgats.c:2981:27 in lcms2.h
// cmsIT8EnumProperties at cmscgats.c:2665:27 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8EnumPropertyMulti at cmscgats.c:2700:27 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"

static void cleanup_property_names(char **names, cmsUInt32Number count) {
    if (names) {
        for (cmsUInt32Number i = 0; i < count; i++) {
            if (names[i]) {
                free(names[i]);
            }
        }
        free(names);
    }
}

static void cleanup_subproperty_names(const char **names, cmsUInt32Number count) {
    if (names) {
        for (cmsUInt32Number i = 0; i < count; i++) {
            if (names[i]) {
                free((void*)names[i]);
            }
        }
        free(names);
    }
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create a dummy context for IT8 operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Test cmsIT8LoadFromMem
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(context, Data, (cmsUInt32Number)Size);
    if (!hIT8) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Test cmsIT8TableCount
    cmsUInt32Number table_count = cmsIT8TableCount(hIT8);
    (void)table_count;
    
    // Test cmsIT8EnumProperties
    char **property_names = NULL;
    cmsUInt32Number prop_count = cmsIT8EnumProperties(hIT8, &property_names);
    
    if (prop_count > 0 && property_names) {
        // Test cmsIT8GetProperty with each property name
        for (cmsUInt32Number i = 0; i < prop_count; i++) {
            if (property_names[i]) {
                const char *prop_value = cmsIT8GetProperty(hIT8, property_names[i]);
                (void)prop_value;
                
                // Test cmsIT8EnumPropertyMulti for this property
                const char **subprop_names = NULL;
                cmsUInt32Number subprop_count = cmsIT8EnumPropertyMulti(hIT8, property_names[i], &subprop_names);
                
                if (subprop_count > 0 && subprop_names) {
                    cleanup_subproperty_names(subprop_names, subprop_count);
                }
            }
        }
        
        cleanup_property_names(property_names, prop_count);
    }
    
    // Test cmsIT8SaveToMem
    cmsUInt32Number bytes_needed = 0;
    cmsBool size_result = cmsIT8SaveToMem(hIT8, NULL, &bytes_needed);
    
    if (size_result && bytes_needed > 0) {
        // Try to save with a buffer if we have reasonable size
        if (bytes_needed < 10 * 1024 * 1024) {
            void *buffer = malloc(bytes_needed);
            if (buffer) {
                cmsUInt32Number bytes_written = bytes_needed;
                cmsBool save_result = cmsIT8SaveToMem(hIT8, buffer, &bytes_written);
                (void)save_result;
                free(buffer);
            }
        }
    }
    
    // Test with some random property names from the input data
    if (Size > 10) {
        // Create a null-terminated property name from the first part of the input
        size_t prop_len = Size > 100 ? 100 : Size;
        char *test_prop = (char *)malloc(prop_len + 1);
        if (test_prop) {
            memcpy(test_prop, Data, prop_len);
            test_prop[prop_len] = '\0';
            
            const char *prop_value = cmsIT8GetProperty(hIT8, test_prop);
            (void)prop_value;
            
            // Test cmsIT8EnumPropertyMulti with this property
            const char **subprop_names = NULL;
            cmsUInt32Number subprop_count = cmsIT8EnumPropertyMulti(hIT8, test_prop, &subprop_names);
            
            if (subprop_count > 0 && subprop_names) {
                cleanup_subproperty_names(subprop_names, subprop_count);
            }
            
            free(test_prop);
        }
    }
    
    // Clean up
    cmsIT8Free(hIT8);
    cmsDeleteContext(context);
    
    return 0;
}