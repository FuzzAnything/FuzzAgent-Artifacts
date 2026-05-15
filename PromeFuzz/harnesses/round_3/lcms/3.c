// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Use default context for simplicity
    cmsContext context = NULL;
    
    // Create a dummy profile handle - in real usage this would be a valid profile
    // For fuzzing we use a simple placeholder
    cmsHPROFILE profile = NULL;
    
    // Use fuzz data to generate intent and flags parameters
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    
    if (Size >= sizeof(cmsUInt32Number)) {
        memcpy(&intent, Data, sizeof(cmsUInt32Number));
        if (Size >= 2 * sizeof(cmsUInt32Number)) {
            memcpy(&flags, Data + sizeof(cmsUInt32Number), sizeof(cmsUInt32Number));
        }
    }
    
    // First call to cmsGetPostScriptCSA with NULL buffer to get required size
    cmsUInt32Number bufferSize = cmsGetPostScriptCSA(context, profile, intent, flags, NULL, 0);
    
    // Call _cmsMalloc to allocate buffer
    void* buffer = _cmsMalloc(context, bufferSize);
    
    // Second call to cmsGetPostScriptCSA with allocated buffer
    if (buffer != NULL) {
        cmsUInt32Number result = cmsGetPostScriptCSA(context, profile, intent, flags, buffer, bufferSize);
        (void)result; // Use result to avoid unused variable warning
    }
    
    // Clean up allocated memory
    _cmsFree(context, buffer);
    
    return 0;
}