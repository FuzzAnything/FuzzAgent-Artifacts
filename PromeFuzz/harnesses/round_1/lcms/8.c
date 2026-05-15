// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
// cmsMD5add at cmsmd5.c:172:16 in lcms2_plugin.h
// cmsMD5finish at cmsmd5.c:215:16 in lcms2_plugin.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
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

static cmsHPROFILE create_test_profile(const uint8_t *data, size_t size) {
    // Try to open profile from memory first
    if (size > 0) {
        cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, (cmsUInt32Number)size);
        if (hProfile != NULL) {
            return hProfile;
        }
    }
    
    // Fallback to creating a simple sRGB profile
    return cmsCreate_sRGBProfile();
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create a test profile
    cmsHPROFILE hProfile = create_test_profile(Data, Size);
    if (hProfile == NULL) {
        return 0;
    }
    
    // Test cmsMD5alloc with different contexts
    cmsContext contexts[] = {NULL, cmsCreateContext(NULL, NULL)};
    
    for (int i = 0; i < 2; i++) {
        cmsContext ctx = contexts[i];
        
        // Test cmsMD5alloc
        cmsHANDLE md5Handle = cmsMD5alloc(ctx);
        if (md5Handle != NULL) {
            // Test cmsMD5add with various data chunks
            if (Size > 0) {
                // Add data in different chunk sizes
                size_t chunk_size = Size > 4 ? 4 : Size;
                for (size_t offset = 0; offset < Size; offset += chunk_size) {
                    size_t remaining = Size - offset;
                    size_t add_size = remaining < chunk_size ? remaining : chunk_size;
                    cmsMD5add(md5Handle, Data + offset, (cmsUInt32Number)add_size);
                }
            }
            
            // Test cmsMD5finish
            union {
                cmsProfileID profile_id;
                cmsUInt8Number bytes[16];
            } digest;
            
            cmsMD5finish(&digest.profile_id, md5Handle);
            
            // Test cmsSetHeaderProfileID with the computed digest
            cmsSetHeaderProfileID(hProfile, digest.bytes);
            
            // Also test with random data if available
            if (Size >= 16) {
                cmsSetHeaderProfileID(hProfile, (cmsUInt8Number*)Data);
            }
        }
        
        // Clean up context if we created one
        if (i == 1 && ctx != NULL) {
            cmsDeleteContext(ctx);
        }
    }
    
    // Test cmsMD5computeID
    (void)cmsMD5computeID(hProfile);
    
    // Test cmsGetHeaderProfileID
    cmsUInt8Number profile_id[16];
    cmsGetHeaderProfileID(hProfile, profile_id);
    
    // Test with different profile states
    // Create another profile and test ID operations
    cmsHPROFILE hProfile2 = cmsCreate_sRGBProfile();
    if (hProfile2 != NULL) {
        // Copy ID from first profile to second
        cmsSetHeaderProfileID(hProfile2, profile_id);
        
        // Get ID from second profile
        cmsUInt8Number profile_id2[16];
        cmsGetHeaderProfileID(hProfile2, profile_id2);
        
        // Compute MD5 ID for second profile
        (void)cmsMD5computeID(hProfile2);
        
        cmsCloseProfile(hProfile2);
    }
    
    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}