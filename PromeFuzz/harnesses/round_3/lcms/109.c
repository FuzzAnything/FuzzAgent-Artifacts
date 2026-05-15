// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// cmsIT8TableCount at cmscgats.c:2981:27 in lcms2.h
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

#define MAXSTR 1024

int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    // Use default context
    cmsContext contextID = NULL;
    
    // Load IT8 data from fuzzer input
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(contextID, Data, (cmsUInt32Number)Size);
    
    if (hIT8 == NULL) {
        // If loading fails, we still want to test other functions with NULL handle
        // to explore error handling paths
        cmsIT8TableCount(NULL);
        return 0;
    }
    
    // Test cmsIT8TableCount
    cmsUInt32Number tableCount = cmsIT8TableCount(hIT8);
    
    // Test cmsIT8EnumProperties
    char **propertyNames = NULL;
    cmsUInt32Number propCount = cmsIT8EnumProperties(hIT8, &propertyNames);
    
    if (propertyNames != NULL) {
        // Test cmsIT8EnumPropertyMulti for each property
        for (cmsUInt32Number i = 0; i < propCount; i++) {
            const char **subpropertyNames = NULL;
            cmsUInt32Number subpropCount = cmsIT8EnumPropertyMulti(hIT8, propertyNames[i], &subpropertyNames);
            
            // Note: subpropertyNames points to internal data, don't free
        }
        
        // Free the property names array as required by documentation
        if (propertyNames != NULL) {
            free(propertyNames);
        }
    }
    
    // Test cmsIT8GetPatchName for some patches
    if (tableCount > 0) {
        // Try a few patch indices, including negative and out-of-bounds
        for (int patchIdx = -1; patchIdx < 10; patchIdx++) {
            char buffer[MAXSTR];
            const char *patchName = cmsIT8GetPatchName(hIT8, patchIdx, buffer);
            
            // Also test with NULL buffer
            const char *internalName = cmsIT8GetPatchName(hIT8, patchIdx, NULL);
        }
    }
    
    // Test cmsIT8SaveToMem
    cmsUInt32Number bytesNeeded = 0;
    
    // First call to get required size
    cmsBool sizeResult = cmsIT8SaveToMem(hIT8, NULL, &bytesNeeded);
    
    if (sizeResult && bytesNeeded > 0) {
        // Allocate buffer and save
        void *saveBuffer = malloc(bytesNeeded);
        if (saveBuffer != NULL) {
            cmsBool saveResult = cmsIT8SaveToMem(hIT8, saveBuffer, &bytesNeeded);
            free(saveBuffer);
        }
    }
    
    // Clean up
    if (hIT8 != NULL) {
        // Note: cmsIT8Free is not in the provided API list, but we need to free the handle
        // Looking at lcms2.h, the function to free IT8 handle is cmsIT8Free
        cmsIT8Free(hIT8);
    }
    
    return 0;
}