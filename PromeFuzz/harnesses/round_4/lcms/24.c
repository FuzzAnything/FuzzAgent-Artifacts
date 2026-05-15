// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsSetLogErrorHandler at cmserr.c:503:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// cmsIT8TableCount at cmscgats.c:2981:27 in lcms2.h
// cmsIT8EnumProperties at cmscgats.c:2665:27 in lcms2.h
// cmsIT8EnumPropertyMulti at cmscgats.c:2700:27 in lcms2.h
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

static void dummy_error_handler(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *Text) {
    // Silently ignore errors during fuzzing
    (void)ContextID;
    (void)ErrorCode;
    (void)Text;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    // Set up error handler to avoid error messages during fuzzing
    cmsSetLogErrorHandler(dummy_error_handler);
    
    // Create a default context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0;
    }
    
    // Try to load IT8 data from memory
    cmsHANDLE it8_handle = cmsIT8LoadFromMem(context, Data, (cmsUInt32Number)Size);
    
    if (it8_handle != NULL) {
        // Test cmsIT8TableCount
        cmsUInt32Number table_count = cmsIT8TableCount(it8_handle);
        (void)table_count;
        
        // Test cmsIT8EnumProperties
        char **property_names = NULL;
        cmsUInt32Number prop_count = cmsIT8EnumProperties(it8_handle, &property_names);
        
        if (prop_count > 0 && property_names != NULL) {
            // Test cmsIT8EnumPropertyMulti for each property
            for (cmsUInt32Number i = 0; i < prop_count; i++) {
                const char **subproperty_names = NULL;
                cmsUInt32Number subprop_count = cmsIT8EnumPropertyMulti(it8_handle, property_names[i], &subproperty_names);
                (void)subprop_count;
                // subproperty_names is allocated by the library, we don't free it
            }
            
            // Free the property names array
            if (property_names) {
                // Use free() instead of _cmsFree() which is an internal function
                free(property_names);
            }
        }
        
        // Test cmsIT8SetPropertyMulti with some dummy data
        // Use parts of the input data for property values
        if (Size > 0) {
            // Create null-terminated strings from input data
            size_t key_len = Size > 100 ? 100 : Size;
            char *key_buf = (char *)malloc(key_len + 1);
            if (key_buf) {
                memcpy(key_buf, Data, key_len);
                key_buf[key_len] = '\0';
                
                // Try to set a property
                cmsIT8SetPropertyMulti(it8_handle, "FUZZ_KEY", "FUZZ_SUBKEY", key_buf);
                
                free(key_buf);
            }
        }
        
        // Test cmsIT8SaveToMem
        cmsUInt32Number bytes_needed = 0;
        cmsBool save_result = cmsIT8SaveToMem(it8_handle, NULL, &bytes_needed);
        
        if (save_result && bytes_needed > 0) {
            void *save_buffer = malloc(bytes_needed);
            if (save_buffer) {
                cmsIT8SaveToMem(it8_handle, save_buffer, &bytes_needed);
                free(save_buffer);
            }
        }
        
        // Clean up IT8 handle
        cmsIT8Free(it8_handle);
    }
    
    // Clean up context
    cmsDeleteContext(context);
    
    return 0;
}