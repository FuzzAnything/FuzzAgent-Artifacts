// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
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
    // Do nothing
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Ensure minimum size for basic operations
    if (Size < 1) return 0;
    
    // Create initial context with user data
    void* user_data1 = (void*)(uintptr_t)Data[0];
    cmsContext ctx1 = cmsCreateContext(NULL, user_data1);
    if (!ctx1) return 0;
    
    // Test cmsGetContextUserData
    void* retrieved_data = cmsGetContextUserData(ctx1);
    (void)retrieved_data; // Use to avoid unused variable warning
    
    // Test cmsSetLogErrorHandlerTHR
    cmsSetLogErrorHandlerTHR(ctx1, dummy_error_handler);
    
    // Test cmsDictAlloc
    cmsHANDLE dict = cmsDictAlloc(ctx1);
    if (dict) {
        // Free dictionary if allocated
        // Note: cmsDictFree is not in provided API but exists in lcms
        // We'll skip explicit free to avoid dependency on non-provided function
    }
    
    // Test cmsDupContext with new user data
    void* user_data2 = (Size > 1) ? (void*)(uintptr_t)Data[1] : NULL;
    cmsContext ctx2 = cmsDupContext(ctx1, user_data2);
    
    if (ctx2) {
        // Test operations on duplicated context
        void* dup_retrieved = cmsGetContextUserData(ctx2);
        (void)dup_retrieved;
        
        cmsSetLogErrorHandlerTHR(ctx2, NULL); // Set to default handler
        
        // Create transform to test cmsGetTransformContextID
        // First create dummy profiles
        cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
        if (hProfile) {
            cmsHTRANSFORM hTransform = cmsCreateTransform(hProfile, TYPE_RGB_8,
                                                          hProfile, TYPE_RGB_8,
                                                          INTENT_PERCEPTUAL, 0);
            if (hTransform) {
                // Test cmsGetTransformContextID
                cmsContext transform_ctx = cmsGetTransformContextID(hTransform);
                (void)transform_ctx;
                
                // Clean up transform
                cmsDeleteTransform(hTransform);
            }
            // Clean up profile
            cmsCloseProfile(hProfile);
        }
        
        // Clean up duplicated context
        cmsDeleteContext(ctx2);
    }
    
    // Test with file operations if needed
    if (Size > 2) {
        FILE* f = fopen("./dummy_file", "wb");
        if (f) {
            size_t write_size = (Size > 1024) ? 1024 : Size;
            fwrite(Data, 1, write_size, f);
            fclose(f);
            
            // Could use the file here if needed
            remove("./dummy_file");
        }
    }
    
    // Clean up original context
    cmsDeleteContext(ctx1);
    
    return 0;
}