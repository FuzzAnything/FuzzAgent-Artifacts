// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// cmsIT8TableCount at cmscgats.c:2981:27 in lcms2.h
// cmsIT8EnumProperties at cmscgats.c:2665:27 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8EnumPropertyMulti at cmscgats.c:2700:27 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    // Initialize with global context (NULL)
    cmsContext context = NULL;
    
    // Create IT8 handle from memory input
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(context, Data, Size);
    
    if (hIT8 != NULL) {
        // Test cmsIT8TableCount
        cmsUInt32Number tableCount = cmsIT8TableCount(hIT8);
        
        // Test cmsIT8EnumProperties
        char **propertyNames = NULL;
        cmsUInt32Number propCount = cmsIT8EnumProperties(hIT8, &propertyNames);
        
        if (propCount > 0 && propertyNames != NULL) {
            // Test cmsIT8GetProperty for each property
            for (cmsUInt32Number i = 0; i < propCount; i++) {
                const char *propValue = cmsIT8GetProperty(hIT8, propertyNames[i]);
                // Test cmsIT8EnumPropertyMulti for each property
                const char **subpropertyNames = NULL;
                cmsUInt32Number subpropCount = cmsIT8EnumPropertyMulti(hIT8, propertyNames[i], &subpropertyNames);
                
                // Free subproperty names if allocated
                if (subpropertyNames != NULL) {
                    free(subpropertyNames);
                }
            }
            
            // Free property names array
            for (cmsUInt32Number i = 0; i < propCount; i++) {
                free(propertyNames[i]);
            }
            free(propertyNames);
        }
        
        // Test cmsIT8SaveToMem - first call to get size
        cmsUInt32Number bytesNeeded = 0;
        cmsBool saveResult = cmsIT8SaveToMem(hIT8, NULL, &bytesNeeded);
        
        if (saveResult && bytesNeeded > 0) {
            // Allocate buffer and save
            void *buffer = malloc(bytesNeeded);
            if (buffer != NULL) {
                saveResult = cmsIT8SaveToMem(hIT8, buffer, &bytesNeeded);
                free(buffer);
            }
        }
        
        // Clean up IT8 handle
        cmsIT8Free(hIT8);
    }
    
    return 0;
}

#ifdef __cplusplus
}
#endif