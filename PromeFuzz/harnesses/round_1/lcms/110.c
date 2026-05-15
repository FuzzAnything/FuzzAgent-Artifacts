// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// _cmsGetTransformMaxWorkers at cmsxform.c:794:26 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Test _cmsCreateMutex
    void* mutex = _cmsCreateMutex(ctx);
    
    // Test _cmsLockMutex if mutex was created
    if (mutex) {
        cmsBool lock_result = _cmsLockMutex(ctx, mutex);
        (void)lock_result; // Use result to avoid unused variable warning
    }
    
    // Test _cmsUnlockMutex if mutex was created
    if (mutex) {
        _cmsUnlockMutex(ctx, mutex);
    }

    // Create a simple transform to test _cmsGetTransformMaxWorkers
    // We'll create a simple sRGB profile and make a transform from it
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile) {
        // Create a simple transform
        cmsHTRANSFORM hTransform = cmsCreateTransform(hProfile, TYPE_RGB_8, 
                                                      hProfile, TYPE_RGB_8, 
                                                      INTENT_PERCEPTUAL, 0);
        if (hTransform) {
            // Cast to the internal structure type for the function call
            struct _cmstransform_struct* transform_struct = 
                (struct _cmstransform_struct*)hTransform;
            
            // Test _cmsGetTransformMaxWorkers
            cmsInt32Number max_workers = _cmsGetTransformMaxWorkers(transform_struct);
            (void)max_workers; // Use result to avoid unused variable warning
            
            // Clean up the transform
            cmsDeleteTransform(hTransform);
        }
        
        // Clean up the profile
        cmsCloseProfile(hProfile);
    }

    // Test _cmsDestroyMutex if mutex was created
    if (mutex) {
        _cmsDestroyMutex(ctx, mutex);
    }

    // Clean up context
    cmsDeleteContext(ctx);
    
    return 0;
}