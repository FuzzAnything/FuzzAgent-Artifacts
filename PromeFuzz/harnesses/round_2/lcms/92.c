// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
// cmsSetEncodedICCversion at cmsio0.c:1111:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "lcms2.h"

static void dummy_fuzz_operations(cmsHPROFILE hProfile, const uint8_t *data, size_t size) {
    // Use the data to generate parameters for the API calls
    // We'll extract values from the fuzz data in a safe way
    
    // For cmsSetProfileVersion - need a double value
    cmsFloat64Number version = 2.0; // Default
    if (size >= sizeof(cmsFloat64Number)) {
        // Copy bytes to create a double (may be invalid/NaN, but function should handle)
        memcpy(&version, data, sizeof(cmsFloat64Number));
        // Ensure version is reasonable (positive, not too large)
        if (version < 0.0 || version > 10.0) {
            version = 2.0;
        }
    }
    
    // For cmsSetEncodedICCversion and cmsSetHeaderRenderingIntent - need 32-bit values
    cmsUInt32Number encoded_version = 0x02000000; // Default to version 2.0
    cmsUInt32Number rendering_intent = 0; // Perceptual
    
    if (size >= sizeof(cmsUInt32Number)) {
        memcpy(&encoded_version, data, sizeof(cmsUInt32Number));
    }
    
    if (size >= 2 * sizeof(cmsUInt32Number)) {
        memcpy(&rendering_intent, data + sizeof(cmsUInt32Number), sizeof(cmsUInt32Number));
        // Rendering intent should be 0-3 for standard intents
        rendering_intent = rendering_intent % 4;
    }
    
    // Call the target API functions
    cmsFloat64Number retrieved_version = cmsGetProfileVersion(hProfile);
    (void)retrieved_version; // Use result to avoid compiler warnings
    
    cmsProfileClassSignature device_class = cmsGetDeviceClass(hProfile);
    (void)device_class;
    
    cmsSetProfileVersion(hProfile, version);
    
    cmsSetEncodedICCversion(hProfile, encoded_version);
    
    cmsSetHeaderRenderingIntent(hProfile, rendering_intent);
    
    cmsUInt32Number retrieved_encoded_version = cmsGetEncodedICCversion(hProfile);
    (void)retrieved_encoded_version;
}

int LLVMFuzzerTestOneInput_92(const uint8_t *data, size_t size) {
    // Create a dummy ICC profile to work with
    // We'll create a simple sRGB profile as a baseline
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    
    if (hProfile == NULL) {
        // If we can't create a profile, nothing to do
        return 0;
    }
    
    // Perform fuzzing operations on the profile
    dummy_fuzz_operations(hProfile, data, size);
    
    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}