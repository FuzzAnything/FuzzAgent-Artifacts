// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// cmsIT8EnumProperties at cmscgats.c:2665:27 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8EnumPropertyMulti at cmscgats.c:2700:27 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8TableCount at cmscgats.c:2981:27 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    // Create a dummy context for the IT8 operations
    cmsContext context = NULL;
    
    // First, try to load IT8 data from the fuzzing input
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(context, Data, (cmsUInt32Number)Size);
    
    // If loading failed, we can't proceed with other operations
    if (hIT8 == NULL) {
        return 0;
    }
    
    // Test cmsIT8EnumProperties
    char **propertyNames = NULL;
    cmsUInt32Number propCount = cmsIT8EnumProperties(hIT8, &propertyNames);
    
    // Free the property names array if it was allocated
    if (propertyNames != NULL) {
        // The strings themselves are owned by the library, only free the array
        free(propertyNames);
    }
    
    // Test cmsIT8SaveToMem - first call to get required size
    cmsUInt32Number bytesNeeded = 0;
    cmsBool saveResult = cmsIT8SaveToMem(hIT8, NULL, &bytesNeeded);
    
    // If we got the size, try to actually save the data
    if (saveResult && bytesNeeded > 0) {
        void *buffer = malloc(bytesNeeded);
        if (buffer != NULL) {
            cmsIT8SaveToMem(hIT8, buffer, &bytesNeeded);
            free(buffer);
        }
    }
    
    // Test cmsIT8EnumPropertyMulti for a dummy property
    const char *dummyProp = "SAMPLE_FIELD";
    const char **subpropertyNames = NULL;
    cmsUInt32Number subpropCount = cmsIT8EnumPropertyMulti(hIT8, dummyProp, &subpropertyNames);
    
    if (subpropertyNames != NULL) {
        // The strings are owned by the library, only free the array
        free((void*)subpropertyNames);
    }
    
    // Test cmsIT8SetPropertyMulti with some dummy data
    const char *key = "TEST_KEY";
    const char *subKey = "TEST_SUBKEY";
    const char *buffer = "TEST_VALUE";
    cmsIT8SetPropertyMulti(hIT8, key, subKey, buffer);
    
    // Test cmsIT8TableCount
    cmsUInt32Number tableCount = cmsIT8TableCount(hIT8);
    
    // Clean up - use cmsIT8Free which should be available via lcms2.h
    // Note: This function is not in the provided list but is standard in lcms
    // If it causes issues, we can comment it out and let the fuzzer exit
    cmsIT8Free(hIT8);
    
    return 0;
}