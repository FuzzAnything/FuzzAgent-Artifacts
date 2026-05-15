// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateGrayProfile at cmsvirt.c:280:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
// cmsMD5add at cmsmd5.c:172:16 in lcms2_plugin.h
// cmsMD5add at cmsmd5.c:172:16 in lcms2_plugin.h
// cmsMD5finish at cmsmd5.c:215:16 in lcms2_plugin.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
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

static cmsHPROFILE create_test_profile(void) {
    // Create a simple sRGB profile instead of custom RGB profile
    // This avoids the tone curve duplication issue
    return cmsCreate_sRGBProfile();
}

static cmsHPROFILE create_simple_profile(void) {
    // Alternative: create a simple gray profile
    cmsToneCurve* gamma = cmsBuildGamma(NULL, 2.2);
    if (!gamma) return NULL;
    
    cmsHPROFILE profile = cmsCreateGrayProfile(NULL, gamma);
    cmsFreeToneCurve(gamma);
    return profile;
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a test profile for functions that need it
    cmsHPROFILE profile = NULL;
    
    // Try different profile creation methods based on input
    if (Size % 3 == 0) {
        profile = create_test_profile();  // sRGB profile
    } else {
        profile = create_simple_profile();  // Gray profile
    }
    
    if (!profile) {
        // Fallback to sRGB if both fail
        profile = cmsCreate_sRGBProfile();
        if (!profile) return 0;
    }

    // Test cmsMD5alloc and cmsMD5add/cmsMD5finish
    cmsHANDLE md5_handle = cmsMD5alloc(NULL);
    if (md5_handle) {
        // Use first half of data for MD5 operations
        size_t md5_size = Size / 2;
        if (md5_size > 0) {
            cmsMD5add(md5_handle, Data, md5_size);
        }
        
        // Test with different chunk sizes
        if (Size > 10) {
            size_t chunk_size = Size > 20 ? 10 : Size - 5;
            if (chunk_size > 0 && 5 < Size) {
                cmsMD5add(md5_handle, Data + 5, chunk_size);
            }
        }
        
        cmsProfileID profile_id;
        cmsMD5finish(&profile_id, md5_handle);
    }

    // Test cmsSetHeaderProfileID and cmsGetHeaderProfileID
    if (Size >= 16) {
        cmsUInt8Number profile_id_buffer[16];
        memcpy(profile_id_buffer, Data, 16);
        cmsSetHeaderProfileID(profile, profile_id_buffer);
        
        cmsUInt8Number retrieved_id[16];
        cmsGetHeaderProfileID(profile, retrieved_id);
    }

    // Test cmsMD5computeID
    if (profile) {
        // Only compute ID if we have a valid profile
        cmsMD5computeID(profile);
    }

    // Cleanup
    if (profile) {
        cmsCloseProfile(profile);
    }

    return 0;
}