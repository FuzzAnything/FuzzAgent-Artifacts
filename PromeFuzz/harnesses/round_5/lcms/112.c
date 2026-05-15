// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
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
    // Dummy error handler for testing
    (void)ContextID;
    (void)ErrorCode;
    (void)Text;
}

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Use first byte to control behavior variations
    uint8_t mode = Data[0];
    
    // Create initial context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Test cmsGetSupportedIntentsTHR
    {
        cmsUInt32Number max_intents = (mode % 10) + 1;
        cmsUInt32Number *codes = (cmsUInt32Number*)malloc(max_intents * sizeof(cmsUInt32Number));
        char **descriptions = (char**)malloc(max_intents * sizeof(char*));
        
        if (codes && descriptions) {
            // The function fills the descriptions array with pointers to static strings
            // We don't need to allocate memory for each string
            cmsUInt32Number count = cmsGetSupportedIntentsTHR(context, max_intents, codes, descriptions);
            (void)count; // Use count to avoid unused variable warning
        }
        
        free(codes);
        free(descriptions);
    }

    // Test cmsDupContext
    {
        void *user_data = (mode & 0x01) ? (void*)(intptr_t)mode : NULL;
        cmsContext dup_context = cmsDupContext(context, user_data);
        if (dup_context) {
            // Test cmsGetContextUserData on duplicated context
            void *retrieved_data = cmsGetContextUserData(dup_context);
            (void)retrieved_data;
            
            cmsDeleteContext(dup_context);
        }
    }

    // Test cmsSetLogErrorHandlerTHR
    {
        if (mode & 0x02) {
            cmsSetLogErrorHandlerTHR(context, dummy_error_handler);
        } else {
            cmsSetLogErrorHandlerTHR(context, NULL);
        }
    }

    // Test cmsGetContextUserData on original context
    {
        void *user_data = cmsGetContextUserData(context);
        (void)user_data;
    }

    // Test cmsDictAlloc
    {
        cmsHANDLE dict = cmsDictAlloc(context);
        if (dict) {
            // Note: cmsDictFree is not in the API list, so we can't free it properly
            // This might leak memory during fuzzing but that's acceptable for fuzzing
        }
    }

    // Test cmsGetTransformContextID with NULL transform
    {
        cmsContext transform_context = cmsGetTransformContextID(NULL);
        (void)transform_context;
    }

    // Create a dummy transform to test cmsGetTransformContextID with valid transform
    // We need to create profiles first
    {
        // Create simple sRGB profiles
        cmsHPROFILE src_profile = cmsCreate_sRGBProfile();
        cmsHPROFILE dst_profile = cmsCreate_sRGBProfile();
        
        if (src_profile && dst_profile) {
            // Create transform
            cmsHTRANSFORM transform = cmsCreateTransform(src_profile, 
                                                         TYPE_RGB_8, 
                                                         dst_profile, 
                                                         TYPE_RGB_8, 
                                                         INTENT_PERCEPTUAL, 
                                                         0);
            
            if (transform) {
                // Test cmsGetTransformContextID
                cmsContext transform_context = cmsGetTransformContextID(transform);
                (void)transform_context;
                
                cmsDeleteTransform(transform);
            }
        }
        
        if (src_profile) {
            cmsCloseProfile(src_profile);
        }
        if (dst_profile) {
            cmsCloseProfile(dst_profile);
        }
    }

    // Cleanup
    cmsDeleteContext(context);
    
    return 0;
}