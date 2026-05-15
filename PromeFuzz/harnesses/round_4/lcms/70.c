// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
// cmsMD5add at cmsmd5.c:172:16 in lcms2_plugin.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsMD5finish at cmsmd5.c:215:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    // Create a dummy profile for testing
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (!hProfile) return 0;
    
    // Test cmsGetHeaderProfileID
    cmsUInt8Number profileID[16];
    if (Size >= 16) {
        memcpy(profileID, Data, 16);
        cmsSetHeaderProfileID(hProfile, profileID);
    }
    cmsGetHeaderProfileID(hProfile, profileID);
    
    // Test cmsMD5alloc and related functions
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context) {
        cmsHANDLE md5Handle = cmsMD5alloc(context);
        if (md5Handle) {
            // Use fuzz data for MD5 hashing
            if (Size > 0) {
                cmsMD5add(md5Handle, Data, Size > 0xFFFFFFFF ? 0xFFFFFFFF : (cmsUInt32Number)Size);
            }
            
            // Test cmsMD5computeID
            cmsMD5computeID(hProfile);
            
            // Test cmsMD5finish
            cmsProfileID computedID;
            cmsMD5finish(&computedID, md5Handle);
            
            // Clean up MD5 handle
            _cmsFree(context, md5Handle);
        }
        cmsDeleteContext(context);
    }
    
    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}