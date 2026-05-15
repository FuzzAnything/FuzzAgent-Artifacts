// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void custom_error_handler(cmsContext context, cmsUInt32Number error_code, const char* error_text) {
    // Custom error handler that does nothing to avoid interfering with fuzzing
    (void)context;
    (void)error_code;
    (void)error_text;
}

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Set custom error handler
    cmsSetLogErrorHandlerTHR(context, custom_error_handler);

    // Test cmsOpenProfileFromMemTHR
    cmsHPROFILE mem_profile = cmsOpenProfileFromMemTHR(context, Data, (cmsUInt32Number)Size);
    if (mem_profile) {
        cmsCloseProfile(mem_profile);
    }

    // Test cmsOpenProfileFromFileTHR
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
        
        // Test with read mode
        cmsHPROFILE file_profile = cmsOpenProfileFromFileTHR(context, "./dummy_file", "r");
        if (file_profile) {
            cmsCloseProfile(file_profile);
        }
        
        // Test with write mode (may fail but that's okay)
        file_profile = cmsOpenProfileFromFileTHR(context, "./dummy_file", "w");
        if (file_profile) {
            cmsCloseProfile(file_profile);
        }
        
        remove("./dummy_file");
    }

    // Test cmsSignalError with various error codes
    cmsSignalError(context, 0, "Test error message");
    cmsSignalError(context, 1, "Another error: %d", 42);
    cmsSignalError(context, (cmsUInt32Number)Size, "Error with size: %zu", Size);

    // Test cmsCreateTransformTHR
    if (mem_profile) {
        // Re-open profile if needed
        mem_profile = cmsOpenProfileFromMemTHR(context, Data, (cmsUInt32Number)Size);
    } else {
        // Use minimal valid data for profile creation
        uint8_t minimal_profile[128] = {0};
        if (Size > sizeof(minimal_profile)) {
            memcpy(minimal_profile, Data, sizeof(minimal_profile));
            mem_profile = cmsOpenProfileFromMemTHR(context, minimal_profile, sizeof(minimal_profile));
        }
    }
    
    if (mem_profile) {
        // Create transform with same profile for input and output
        cmsHTRANSFORM transform = cmsCreateTransformTHR(
            context,
            mem_profile,
            TYPE_RGB_8,      // Input format
            mem_profile,     // Output profile (same as input)
            TYPE_RGB_8,      // Output format
            INTENT_PERCEPTUAL, // Rendering intent
            cmsFLAGS_NOCACHE  // Flags
        );
        
        if (transform) {
            cmsDeleteTransform(transform);
        }
        
        // Test with NULL output profile (device link)
        transform = cmsCreateTransformTHR(
            context,
            mem_profile,
            TYPE_RGB_8,
            NULL,           // Output profile NULL for device link
            TYPE_RGB_8,
            INTENT_PERCEPTUAL,
            cmsFLAGS_NOCACHE
        );
        
        if (transform) {
            cmsDeleteTransform(transform);
        }
        
        cmsCloseProfile(mem_profile);
    }

    // Clean up
    cmsDeleteContext(context);
    
    return 0;
}