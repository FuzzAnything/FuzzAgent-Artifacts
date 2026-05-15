// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCIECAM02Init at cmscam02.c:363:22 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCIECAM02Done at cmscam02.c:432:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

static void dummy_error_handler(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *Text) {
    // Do nothing - just a dummy handler for error signals
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    // Create a context first
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Set a dummy error handler to capture errors
    cmsSetLogErrorHandlerTHR(ctx, dummy_error_handler);

    // Test cmsOpenProfileFromMemTHR with fuzzer data
    cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(ctx, Data, (cmsUInt32Number)Size);
    
    // Test cmsCIECAM02Init with dummy viewing conditions
    cmsViewingConditions vc;
    memset(&vc, 0, sizeof(vc));
    vc.whitePoint.X = 95.05;
    vc.whitePoint.Y = 100.0;
    vc.whitePoint.Z = 108.88;
    vc.La = 100.0;  // Adapting luminance
    vc.Yb = 20.0;   // Background luminance factor
    vc.surround = 1; // Average surround
    
    cmsHANDLE hCAM = cmsCIECAM02Init(ctx, &vc);
    
    // Test cmsCreateTransformTHR if we got a valid profile
    if (hProfile) {
        cmsHTRANSFORM hTransform = cmsCreateTransformTHR(
            ctx,
            hProfile,          // Input profile
            TYPE_RGB_8,        // Input format
            hProfile,          // Output profile (same as input)
            TYPE_RGB_8,        // Output format
            INTENT_PERCEPTUAL, // Rendering intent
            0                  // Flags
        );
        
        // Also test with NULL output profile (device link)
        if (hTransform) {
            cmsDeleteTransform(hTransform);
        }
        
        hTransform = cmsCreateTransformTHR(
            ctx,
            hProfile,          // Input profile
            TYPE_RGB_8,        // Input format
            NULL,              // Output profile (NULL for device link)
            TYPE_RGB_8,        // Output format
            INTENT_PERCEPTUAL, // Rendering intent
            0                  // Flags
        );
        
        if (hTransform) {
            cmsDeleteTransform(hTransform);
        }
        
        // Close the profile
        cmsCloseProfile(hProfile);
    }
    
    // Test cmsSignalError with various error codes
    cmsSignalError(ctx, cmsERROR_UNDEFINED, "Fuzzer generated error: %s", "test");
    cmsSignalError(ctx, cmsERROR_RANGE, "Range error during fuzzing");
    cmsSignalError(ctx, cmsERROR_NOT_SUITABLE, "Profile not suitable");
    
    // Test cmsDeleteContext with NULL (cleans global context)
    cmsDeleteContext(NULL);
    
    // Clean up CIECAM02 handle if created
    if (hCAM) {
        // Use cmsCIECAM02Done to properly free the allocated memory
        cmsCIECAM02Done(hCAM);
    }
    
    // Finally delete our context
    cmsDeleteContext(ctx);
    
    return 0;
}