// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsSetLogErrorHandler at cmserr.c:503:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// cmsIsMatrixShaper at cmsio1.c:806:20 in lcms2.h
// cmsDetectBlackPoint at cmssamp.c:194:19 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsIsCLUT at cmsio1.c:830:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_error_handler(cmsContext context, cmsUInt32Number error_code, const char *text) {
    // Suppress error messages during fuzzing
    (void)context;
    (void)error_code;
    (void)text;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    // Initialize variables
    cmsHPROFILE hProfile = NULL;
    cmsCIEXYZ black_point;
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    cmsUInt32Number direction = 0;
    cmsUInt32Number buffer_size = 0;
    cmsContext context = NULL;
    char *buffer = NULL;
    
    // Set error handler to suppress messages
    cmsSetLogErrorHandler(dummy_error_handler);
    
    // Create a profile from the fuzz data
    if (Size > 0) {
        hProfile = cmsOpenProfileFromMem(Data, Size);
    }
    
    // If we couldn't create a profile, use a NULL handle for testing
    // Extract parameters from fuzz data if available
    if (Size >= sizeof(cmsUInt32Number) * 4) {
        // Use first bytes for intent, flags, and direction
        intent = *((cmsUInt32Number*)Data);
        flags = *((cmsUInt32Number*)(Data + sizeof(cmsUInt32Number)));
        direction = *((cmsUInt32Number*)(Data + sizeof(cmsUInt32Number) * 2));
        
        // Ensure intent is within valid range (0-3)
        intent = intent % 4;
        // Ensure direction is 0 or 1 (input or output)
        direction = direction % 2;
    }
    
    // Test cmsDetectDestinationBlackPoint
    if (hProfile != NULL) {
        cmsDetectDestinationBlackPoint(&black_point, hProfile, intent, flags);
    }
    
    // Test cmsGetPostScriptCSA
    if (hProfile != NULL) {
        // First call to get required buffer size
        buffer_size = cmsGetPostScriptCSA(context, hProfile, intent, flags, NULL, 0);
        
        if (buffer_size > 0 && buffer_size < 1024 * 1024) { // Limit buffer size to 1MB
            buffer = (char*)malloc(buffer_size);
            if (buffer != NULL) {
                cmsGetPostScriptCSA(context, hProfile, intent, flags, buffer, buffer_size);
                free(buffer);
            }
        }
    }
    
    // Test cmsIsMatrixShaper
    if (hProfile != NULL) {
        cmsIsMatrixShaper(hProfile);
    }
    
    // Test cmsDetectBlackPoint
    if (hProfile != NULL) {
        cmsDetectBlackPoint(&black_point, hProfile, intent, flags);
    }
    
    // Test cmsIsIntentSupported
    if (hProfile != NULL) {
        cmsIsIntentSupported(hProfile, intent, direction);
    }
    
    // Test cmsIsCLUT
    if (hProfile != NULL) {
        cmsIsCLUT(hProfile, intent, direction);
    }
    
    // Cleanup
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
    
    return 0;
}