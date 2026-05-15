// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Custom error handler for cmsSetLogErrorHandlerTHR
static void custom_error_handler(cmsContext context, cmsUInt32Number error_code, const char* error_text) {
    // Do nothing - just a stub for fuzzing
    (void)context;
    (void)error_code;
    (void)error_text;
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    // We need at least some data to make decisions
    if (Size < 1) return 0;
    
    // Create a context to work with
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Use the first byte to decide which functions to call
    uint8_t selector = Data[0];
    
    // Test cmsGetSupportedIntentsTHR
    if (selector & 0x01) {
        cmsUInt32Number max_intents = 10;
        if (Size > 1) {
            max_intents = (Data[1] % 20) + 1; // Limit to reasonable size
        }
        
        cmsUInt32Number* codes = (cmsUInt32Number*)malloc(max_intents * sizeof(cmsUInt32Number));
        char** descriptions = (char**)malloc(max_intents * sizeof(char*));
        
        if (codes && descriptions) {
            // Initialize descriptions array
            for (cmsUInt32Number i = 0; i < max_intents; i++) {
                descriptions[i] = NULL;
            }
            
            cmsUInt32Number count = cmsGetSupportedIntentsTHR(ctx, max_intents, codes, descriptions);
            (void)count; // Use count to avoid unused variable warning
            
            // Clean up
            free(codes);
            for (cmsUInt32Number i = 0; i < max_intents; i++) {
                if (descriptions[i]) {
                    free(descriptions[i]);
                }
            }
            free(descriptions);
        }
    }
    
    // Test cmsGetTransformContextID
    if (selector & 0x02) {
        // Create a simple transform to test with
        cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
        if (hProfile) {
            cmsHTRANSFORM hTransform = cmsCreateTransform(hProfile, TYPE_RGB_8, hProfile, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);
            if (hTransform) {
                cmsContext transform_ctx = cmsGetTransformContextID(hTransform);
                (void)transform_ctx; // Use to avoid unused variable warning
                cmsDeleteTransform(hTransform);
            }
            
            // Also test with NULL transform
            cmsContext null_ctx = cmsGetTransformContextID(NULL);
            (void)null_ctx; // Use to avoid unused variable warning
            
            cmsCloseProfile(hProfile);
        }
    }
    
    // Test cmsDupContext
    if (selector & 0x04) {
        // Create dummy user data
        int dummy_data = 42;
        void* user_data = (Size > 2) ? (void*)(Data + 2) : (void*)&dummy_data;
        
        cmsContext dup_ctx = cmsDupContext(ctx, user_data);
        if (dup_ctx) {
            // Test cmsGetContextUserData on duplicated context
            void* retrieved_data = cmsGetContextUserData(dup_ctx);
            (void)retrieved_data; // Use to avoid unused variable warning
            
            // Test cmsSetLogErrorHandlerTHR on duplicated context
            cmsSetLogErrorHandlerTHR(dup_ctx, custom_error_handler);
            
            // Test cmsDictAlloc on duplicated context
            cmsHANDLE dict = cmsDictAlloc(dup_ctx);
            if (dict) {
                // Dictionary would normally be freed with cmsDictFree, but we don't have that function
                // For fuzzing, we'll just leave it allocated
            }
            
            cmsDeleteContext(dup_ctx);
        }
    }
    
    // Test cmsSetLogErrorHandlerTHR
    if (selector & 0x08) {
        // Set custom error handler
        cmsSetLogErrorHandlerTHR(ctx, custom_error_handler);
        
        // Also test with NULL handler (default)
        cmsSetLogErrorHandlerTHR(ctx, NULL);
    }
    
    // Test cmsGetContextUserData
    if (selector & 0x10) {
        void* user_data = cmsGetContextUserData(ctx);
        (void)user_data; // Use to avoid unused variable warning
    }
    
    // Test cmsDictAlloc
    if (selector & 0x20) {
        cmsHANDLE dict = cmsDictAlloc(ctx);
        if (dict) {
            // Dictionary would normally be freed with cmsDictFree, but we don't have that function
            // For fuzzing, we'll just leave it allocated
        }
    }
    
    // Clean up
    cmsDeleteContext(ctx);
    
    return 0;
}