// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// _cmsGetTransformMaxWorkers at cmsxform.c:794:26 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
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

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Create context with NULL plugin and user data
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        // If context creation fails, return early
        return 0;
    }

    // Create a mutex using the context
    void* mutex = _cmsCreateMutex(context);
    
    // Try to lock the mutex if it was created successfully
    if (mutex != NULL) {
        cmsBool lock_result = _cmsLockMutex(context, mutex);
        // Lock result is not used further, but we check it
        (void)lock_result;
    }

    // Create a simple transform to test transform-related functions
    // First create sRGB profiles
    cmsHPROFILE src_profile = cmsCreate_sRGBProfile();
    cmsHPROFILE dst_profile = cmsCreate_sRGBProfile();
    
    if (src_profile != NULL && dst_profile != NULL) {
        // Create a simple transform
        cmsHTRANSFORM transform = cmsCreateTransform(src_profile, 
                                                     TYPE_RGB_8, 
                                                     dst_profile, 
                                                     TYPE_RGB_8, 
                                                     INTENT_PERCEPTUAL, 
                                                     0);
        
        if (transform != NULL) {
            // Get context ID from transform
            cmsContext transform_context = cmsGetTransformContextID(transform);
            (void)transform_context; // Use void to avoid unused variable warning
            
            // Get max workers for transform (requires casting to internal type)
            cmsInt32Number max_workers = _cmsGetTransformMaxWorkers((struct _cmstransform_struct*)transform);
            (void)max_workers; // Use void to avoid unused variable warning
            
            // Clean up transform
            cmsDeleteTransform(transform);
        }
        
        // Clean up profiles
        cmsCloseProfile(src_profile);
        cmsCloseProfile(dst_profile);
    }

    // Destroy mutex if it was created
    if (mutex != NULL) {
        _cmsDestroyMutex(context, mutex);
    }

    // Clean up context
    cmsDeleteContext(context);

    return 0;
}