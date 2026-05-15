// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_test_tone_curve(cmsContext context) {
    // Create a simple gamma curve for testing
    return cmsBuildGamma(context, 2.2);
}

static void cleanup_tone_curves(cmsToneCurve* curves[3]) {
    for (int i = 0; i < 3; i++) {
        if (curves[i]) {
            cmsFreeToneCurve(curves[i]);
        }
    }
}

int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
    // Create a context first
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

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

    // Test cmsCreate_sRGBProfileTHR
    cmsHPROFILE srgb_profile = cmsCreate_sRGBProfileTHR(context);
    if (srgb_profile) {
        cmsCloseProfile(srgb_profile);
    }

    // Test cmsCreateProfilePlaceholder
    cmsHPROFILE placeholder_profile = cmsCreateProfilePlaceholder(context);
    if (placeholder_profile) {
        cmsCloseProfile(placeholder_profile);
    }

    // Test cmsCreateRGBProfileTHR with various inputs
    if (Size >= sizeof(cmsCIExyY) + sizeof(cmsCIExyYTRIPLE)) {
        // Use fuzz data to populate structures
        cmsCIExyY white_point;
        cmsCIExyYTRIPLE primaries;
        cmsToneCurve* transfer_function[3] = {NULL, NULL, NULL};
        
        // Copy data into structures (with bounds checking)
        size_t offset = 0;
        
        // Copy white point
        size_t white_point_size = sizeof(cmsCIExyY);
        if (offset + white_point_size <= Size) {
            memcpy(&white_point, Data + offset, white_point_size);
            offset += white_point_size;
        } else {
            // Use default values if not enough data
            white_point.x = 0.3127;
            white_point.y = 0.3290;
            white_point.Y = 1.0;
        }
        
        // Copy primaries
        size_t primaries_size = sizeof(cmsCIExyYTRIPLE);
        if (offset + primaries_size <= Size) {
            memcpy(&primaries, Data + offset, primaries_size);
            offset += primaries_size;
        } else {
            // Use sRGB primaries as defaults
            primaries.Red.x = 0.64;
            primaries.Red.y = 0.33;
            primaries.Red.Y = 1.0;
            primaries.Green.x = 0.30;
            primaries.Green.y = 0.60;
            primaries.Green.Y = 1.0;
            primaries.Blue.x = 0.15;
            primaries.Blue.y = 0.06;
            primaries.Blue.Y = 1.0;
        }
        
        // Create tone curves for transfer function
        for (int i = 0; i < 3; i++) {
            transfer_function[i] = create_test_tone_curve(context);
        }
        
        // Create RGB profile
        cmsHPROFILE rgb_profile = cmsCreateRGBProfileTHR(context, &white_point, &primaries, transfer_function);
        if (rgb_profile) {
            cmsCloseProfile(rgb_profile);
        }
        
        // Clean up tone curves
        cleanup_tone_curves(transfer_function);
    } else {
        // Test with default values when insufficient data
        cmsCIExyY white_point = {0.3127, 0.3290, 1.0}; // D65
        cmsCIExyYTRIPLE primaries = {
            {0.64, 0.33, 1.0},   // Red
            {0.30, 0.60, 1.0},   // Green
            {0.15, 0.06, 1.0}    // Blue
        };
        cmsToneCurve* transfer_function[3] = {NULL, NULL, NULL};
        
        for (int i = 0; i < 3; i++) {
            transfer_function[i] = create_test_tone_curve(context);
        }
        
        cmsHPROFILE rgb_profile = cmsCreateRGBProfileTHR(context, &white_point, &primaries, transfer_function);
        if (rgb_profile) {
            cmsCloseProfile(rgb_profile);
        }
        
        cleanup_tone_curves(transfer_function);
    }

    // Clean up context
    cmsDeleteContext(context);
    
    return 0;
}