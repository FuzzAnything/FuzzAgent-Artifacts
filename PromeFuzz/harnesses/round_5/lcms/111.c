// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummyErrorHandler(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *Text) {
    // Do nothing - just a dummy handler for fuzzing
}

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    // Create initial context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Test cmsGetSupportedIntentsTHR
    if (Size > 0) {
        cmsUInt32Number maxIntents = (Size % 10) + 1;  // 1-10 intents
        cmsUInt32Number *codes = (cmsUInt32Number*)calloc(maxIntents, sizeof(cmsUInt32Number));
        char **descriptions = (char**)calloc(maxIntents, sizeof(char*));
        
        if (codes && descriptions) {
            // Only allocate descriptions for indices that exist in the input data
            size_t descriptionsToAllocate = (maxIntents < Size) ? maxIntents : Size;
            for (size_t i = 0; i < descriptionsToAllocate; i++) {
                descriptions[i] = (char*)calloc(100, sizeof(char));
                if (descriptions[i] && i < Size) {
                    descriptions[i][0] = Data[i];
                }
            }
            
            cmsGetSupportedIntentsTHR(ctx, maxIntents, codes, descriptions);
            
            // Only free descriptions that were actually allocated
            for (size_t i = 0; i < descriptionsToAllocate; i++) {
                if (descriptions[i]) {
                    free(descriptions[i]);
                }
            }
        }
        free(codes);
        free(descriptions);
    }

    // Test cmsDupContext
    void *userData = NULL;
    if (Size > 0) {
        userData = (void*)Data;
    }
    cmsContext dupCtx = cmsDupContext(ctx, userData);
    
    if (dupCtx) {
        // Test cmsSetLogErrorHandlerTHR with the duplicated context
        cmsSetLogErrorHandlerTHR(dupCtx, dummyErrorHandler);
        
        // Test cmsGetContextUserData
        void *retrievedData = cmsGetContextUserData(dupCtx);
        (void)retrievedData;  // Use to avoid unused variable warning
        
        // Test cmsDictAlloc
        cmsHANDLE dict = cmsDictAlloc(dupCtx);
        (void)dict;  // Use to avoid unused variable warning
        
        // Clean up duplicated context
        cmsDeleteContext(dupCtx);
    }

    // Test cmsSetLogErrorHandlerTHR with original context
    cmsSetLogErrorHandlerTHR(ctx, dummyErrorHandler);
    
    // Test cmsGetContextUserData with original context
    void *origData = cmsGetContextUserData(ctx);
    (void)origData;  // Use to avoid unused variable warning
    
    // Test cmsDictAlloc with original context
    cmsHANDLE dict2 = cmsDictAlloc(ctx);
    (void)dict2;  // Use to avoid unused variable warning

    // Test cmsGetTransformContextID with NULL
    cmsContext transformCtx = cmsGetTransformContextID(NULL);
    (void)transformCtx;  // Use to avoid unused variable warning

    // Create a dummy transform to test cmsGetTransformContextID with valid handle
    // We need to create a simple transform first
    cmsHPROFILE srcProfile = cmsCreate_sRGBProfile();
    cmsHPROFILE dstProfile = cmsCreate_sRGBProfile();
    
    if (srcProfile && dstProfile) {
        cmsHTRANSFORM transform = cmsCreateTransform(srcProfile, TYPE_RGB_8,
                                                     dstProfile, TYPE_RGB_8,
                                                     INTENT_PERCEPTUAL, 0);
        if (transform) {
            cmsContext transformCtx2 = cmsGetTransformContextID(transform);
            (void)transformCtx2;  // Use to avoid unused variable warning
            cmsDeleteTransform(transform);
        }
    }
    
    if (srcProfile) cmsCloseProfile(srcProfile);
    if (dstProfile) cmsCloseProfile(dstProfile);

    // Clean up original context
    cmsDeleteContext(ctx);

    return 0;
}