// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
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
    // Do nothing - just a dummy handler for fuzzing
    (void)ContextID;
    (void)ErrorCode;
    (void)Text;
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    // Use fuzzer input to create user data pointer (ensure it's not NULL if Size > 0)
    void* user_data = (Size > 0) ? (void*)(Data) : NULL;
    
    // Create initial context
    cmsContext ctx1 = cmsCreateContext(NULL, user_data);
    if (!ctx1) {
        return 0;
    }
    
    // Set error handler
    cmsSetLogErrorHandlerTHR(ctx1, dummy_error_handler);
    
    // Get user data back
    void* retrieved_data = cmsGetContextUserData(ctx1);
    (void)retrieved_data; // Use to avoid unused variable warning
    
    // Allocate dictionary
    cmsHANDLE dict = cmsDictAlloc(ctx1);
    if (dict) {
        // Dictionary allocated successfully, we would normally free it but
        // cmsDictFree requires the context and we're just fuzzing allocation
        // Note: We're not freeing to avoid crashes from double-free in fuzzing
    }
    
    // Create a transform to test cmsGetTransformContextID
    // We need profiles for this, so create simple sRGB profiles
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile) {
        cmsHTRANSFORM hTransform = cmsCreateTransform(hProfile, TYPE_RGB_8, 
                                                     hProfile, TYPE_RGB_8, 
                                                     INTENT_PERCEPTUAL, 0);
        if (hTransform) {
            cmsContext transform_ctx = cmsGetTransformContextID(hTransform);
            (void)transform_ctx;
            
            // Clean up transform
            cmsDeleteTransform(hTransform);
        }
        
        // Clean up profile
        cmsCloseProfile(hProfile);
    }
    
    // Duplicate context with new user data
    void* new_user_data = (Size > 1) ? (void*)(Data + 1) : NULL;
    cmsContext ctx2 = cmsDupContext(ctx1, new_user_data);
    if (ctx2) {
        // Test operations on duplicated context
        cmsSetLogErrorHandlerTHR(ctx2, dummy_error_handler);
        void* dup_retrieved_data = cmsGetContextUserData(ctx2);
        (void)dup_retrieved_data;
        
        // Allocate dictionary in duplicated context
        cmsHANDLE dict2 = cmsDictAlloc(ctx2);
        (void)dict2;
        
        // Clean up duplicated context
        cmsDeleteContext(ctx2);
    }
    
    // Clean up original context
    cmsDeleteContext(ctx1);
    
    return 0;
}