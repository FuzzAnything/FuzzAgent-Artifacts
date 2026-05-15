// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetLogErrorHandlerTHR at cmserr.c:489:16 in lcms2.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_error_handler(cmsContext context, cmsUInt32Number error_code, const char* error_text) {
    // Silently ignore errors during fuzzing
    (void)context;
    (void)error_code;
    (void)error_text;
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    // Create a context first
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }
    
    // Set error handler to ignore errors
    cmsSetLogErrorHandlerTHR(context, dummy_error_handler);
    
    // Test cmsCreateNULLProfileTHR
    cmsHPROFILE null_profile = cmsCreateNULLProfileTHR(context);
    if (null_profile) {
        cmsCloseProfile(null_profile);
    }
    
    // Test cmsCreateXYZProfileTHR
    cmsHPROFILE xyz_profile = cmsCreateXYZProfileTHR(context);
    if (xyz_profile) {
        cmsCloseProfile(xyz_profile);
    }
    
    // Test cmsCreateProfilePlaceholder
    cmsHPROFILE placeholder_profile = cmsCreateProfilePlaceholder(context);
    if (placeholder_profile) {
        cmsCloseProfile(placeholder_profile);
    }
    
    // Test cmsCreate_sRGBProfileTHR
    cmsHPROFILE srgb_profile = cmsCreate_sRGBProfileTHR(context);
    if (srgb_profile) {
        cmsCloseProfile(srgb_profile);
    }
    
    // Test cmsCreateRGBProfileTHR with fuzzed data
    if (Size >= sizeof(cmsCIExyY) + sizeof(cmsCIExyYTRIPLE)) {
        // Parse white point from fuzz data
        cmsCIExyY white_point;
        size_t offset = 0;
        
        if (offset + sizeof(cmsCIExyY) <= Size) {
            memcpy(&white_point, Data + offset, sizeof(cmsCIExyY));
            offset += sizeof(cmsCIExyY);
        }
        
        // Parse primaries from fuzz data
        cmsCIExyYTRIPLE primaries;
        if (offset + sizeof(cmsCIExyYTRIPLE) <= Size) {
            memcpy(&primaries, Data + offset, sizeof(cmsCIExyYTRIPLE));
            offset += sizeof(cmsCIExyYTRIPLE);
        }
        
        // Create tone curves using fuzz data
        cmsToneCurve* transfer_function[3] = {NULL, NULL, NULL};
        
        // Use remaining data to create tone curves if enough data exists
        if (Size - offset >= 3 * sizeof(cmsUInt16Number) * 256) {
            // Create simple gamma curves using fuzz data
            for (int i = 0; i < 3; i++) {
                if (offset + 256 * sizeof(cmsUInt16Number) <= Size) {
                    cmsUInt16Number* table = (cmsUInt16Number*)malloc(256 * sizeof(cmsUInt16Number));
                    if (table) {
                        // Copy fuzz data into table
                        size_t copy_size = 256 * sizeof(cmsUInt16Number);
                        if (offset + copy_size > Size) {
                            copy_size = Size - offset;
                        }
                        memcpy(table, Data + offset, copy_size);
                        offset += copy_size;
                        
                        // Create tone curve with the table
                        transfer_function[i] = cmsBuildTabulatedToneCurve16(context, 256, table);
                        free(table);
                    }
                }
            }
        } else {
            // Create simple identity curves if not enough data
            for (int i = 0; i < 3; i++) {
                transfer_function[i] = cmsBuildGamma(context, 1.0);
            }
        }
        
        // Create RGB profile with the parsed data
        cmsHPROFILE rgb_profile = cmsCreateRGBProfileTHR(context, &white_point, &primaries, transfer_function);
        if (rgb_profile) {
            cmsCloseProfile(rgb_profile);
        }
        
        // Clean up tone curves
        for (int i = 0; i < 3; i++) {
            if (transfer_function[i]) {
                cmsFreeToneCurve(transfer_function[i]);
            }
        }
    }
    
    // Clean up context
    cmsDeleteContext(context);
    
    return 0;
}