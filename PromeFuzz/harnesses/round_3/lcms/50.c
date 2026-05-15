// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsMD5add at cmsmd5.c:172:16 in lcms2_plugin.h
// cmsMD5finish at cmsmd5.c:215:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSetLogErrorHandler at cmserr.c:503:16 in lcms2.h
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
// cmsMD5add at cmsmd5.c:172:16 in lcms2_plugin.h
// cmsMD5add at cmsmd5.c:172:16 in lcms2_plugin.h
// cmsMD5add at cmsmd5.c:172:16 in lcms2_plugin.h
// cmsMD5finish at cmsmd5.c:215:16 in lcms2_plugin.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
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

static void dummy_error_handler(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *Text) {
    // Silently ignore errors during fuzzing
    (void)ContextID;
    (void)ErrorCode;
    (void)Text;
}

static cmsHPROFILE create_test_profile(const uint8_t *data, size_t size) {
    // Try to open profile from memory if we have enough data
    if (size > 128) {
        cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
        if (hProfile != NULL) {
            return hProfile;
        }
    }
    
    // Fallback: create a simple sRGB profile
    return cmsCreate_sRGBProfile();
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    // Initialize error handler
    cmsSetLogErrorHandler(dummy_error_handler);
    
    // Create a test profile
    cmsHPROFILE hProfile = create_test_profile(Data, Size);
    if (hProfile == NULL) {
        return 0;
    }
    
    // Test cmsGetHeaderProfileID
    cmsUInt8Number profileID[16];
    cmsGetHeaderProfileID(hProfile, profileID);
    
    // Test cmsSetHeaderProfileID with fuzzed data
    cmsUInt8Number newProfileID[16];
    if (Size >= 16) {
        memcpy(newProfileID, Data, 16);
    } else {
        memset(newProfileID, 0, 16);
        if (Size > 0) {
            memcpy(newProfileID, Data, Size);
        }
    }
    cmsSetHeaderProfileID(hProfile, newProfileID);
    
    // Test cmsMD5computeID
    cmsMD5computeID(hProfile);
    
    // Test MD5 functions
    cmsHANDLE md5Context = cmsMD5alloc(NULL);
    if (md5Context != NULL) {
        // Add data to MD5 context in chunks
        if (Size > 0) {
            size_t chunk_size = Size > 1024 ? 1024 : Size;
            cmsMD5add(md5Context, Data, (cmsUInt32Number)chunk_size);
            
            // Add remaining data if any
            if (Size > chunk_size) {
                size_t remaining = Size - chunk_size;
                if (remaining > 0) {
                    cmsMD5add(md5Context, Data + chunk_size, (cmsUInt32Number)remaining);
                }
            }
        } else {
            // Test with empty data
            cmsMD5add(md5Context, Data, 0);
        }
        
        // Finalize MD5
        cmsProfileID md5Result;
        cmsMD5finish(&md5Result, md5Context);
    }
    
    // Test with different profile configurations
    cmsHPROFILE hProfile2 = cmsCreateLab4Profile(NULL);
    if (hProfile2 != NULL) {
        cmsGetHeaderProfileID(hProfile2, profileID);
        cmsSetHeaderProfileID(hProfile2, newProfileID);
        cmsMD5computeID(hProfile2);
        cmsCloseProfile(hProfile2);
    }
    
    // Test MD5 with another context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx != NULL) {
        cmsHANDLE md5Context2 = cmsMD5alloc(ctx);
        if (md5Context2 != NULL) {
            if (Size > 0) {
                cmsMD5add(md5Context2, Data, (cmsUInt32Number)(Size > 512 ? 512 : Size));
            }
            cmsProfileID md5Result2;
            cmsMD5finish(&md5Result2, md5Context2);
        }
        cmsDeleteContext(ctx);
    }
    
    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}