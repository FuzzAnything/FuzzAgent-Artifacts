// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsOpenProfileFromFileTHR at cmsio0.c:1203:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
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

static void custom_error_handler(cmsContext context_id, cmsUInt32Number error_code, const char* text)
{
    // Custom error handler that does nothing to avoid silent ignores
    (void)context_id;
    (void)error_code;
    (void)text;
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    // Create a context with default memory management
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    // Set a custom error handler for the context
    cmsSetLogErrorHandlerTHR(ctx, custom_error_handler);

    // Test cmsOpenProfileFromMemTHR with fuzzer data
    cmsHPROFILE mem_profile = cmsOpenProfileFromMemTHR(ctx, Data, (cmsUInt32Number)Size);
    
    // Test cmsOpenProfileFromFileTHR by writing data to a file
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
        
        // Try different access modes
        const char* access_modes[] = {"r", "rb", "r+", "rb+", NULL};
        for (int i = 0; access_modes[i] != NULL; i++) {
            cmsHPROFILE file_profile = cmsOpenProfileFromFileTHR(ctx, "./dummy_file", access_modes[i]);
            if (file_profile) {
                cmsCloseProfile(file_profile);
            }
        }
        
        remove("./dummy_file");
    }

    // If we got a valid profile from memory, try creating transforms
    if (mem_profile) {
        // Test various input/output formats and intents
        cmsUInt32Number formats[] = {
            TYPE_RGB_8, TYPE_CMYK_8, TYPE_Lab_8, TYPE_XYZ_16,
            TYPE_GRAY_8, TYPE_RGBA_8, TYPE_BGR_8
        };
        cmsUInt32Number intents[] = {
            INTENT_PERCEPTUAL, INTENT_RELATIVE_COLORIMETRIC,
            INTENT_SATURATION, INTENT_ABSOLUTE_COLORIMETRIC
        };
        
        for (size_t i = 0; i < sizeof(formats)/sizeof(formats[0]); i++) {
            for (size_t j = 0; j < sizeof(formats)/sizeof(formats[0]); j++) {
                for (size_t k = 0; k < sizeof(intents)/sizeof(intents[0]); k++) {
                    // Test with single profile (device link) and dual profiles
                    cmsHTRANSFORM transform1 = cmsCreateTransformTHR(
                        ctx, mem_profile, formats[i], NULL, formats[j], 
                        intents[k], cmsFLAGS_NOCACHE);
                    if (transform1) {
                        cmsDeleteTransform(transform1);
                    }
                    
                    // Test with same profile for input and output
                    cmsHTRANSFORM transform2 = cmsCreateTransformTHR(
                        ctx, mem_profile, formats[i], mem_profile, formats[j], 
                        intents[k], cmsFLAGS_NOCACHE);
                    if (transform2) {
                        cmsDeleteTransform(transform2);
                    }
                }
            }
        }
        
        cmsCloseProfile(mem_profile);
    }

    // Test error signaling with various error codes
    cmsSignalError(ctx, 0, "Test error message");
    cmsSignalError(ctx, 1, "Another error: %d", 42);
    cmsSignalError(ctx, 2, "Format: %s", "string");

    // Cleanup
    cmsDeleteContext(ctx);
    
    return 0;
}