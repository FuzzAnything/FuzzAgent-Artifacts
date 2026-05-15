// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
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

static void* dummy_plugin = NULL;
static void* dummy_userdata = NULL;

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    // Create a minimal sRGB profile for testing
    return cmsCreate_sRGBProfileTHR(ctx);
}

static cmsHTRANSFORM create_dummy_transform(cmsContext ctx) {
    cmsHPROFILE src = create_dummy_profile(ctx);
    cmsHPROFILE dst = create_dummy_profile(ctx);
    if (!src || !dst) {
        if (src) cmsCloseProfile(src);
        if (dst) cmsCloseProfile(dst);
        return NULL;
    }
    
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, src, TYPE_RGB_8, 
                                                    dst, TYPE_RGB_8, 
                                                    INTENT_PERCEPTUAL, 0);
    cmsCloseProfile(src);
    cmsCloseProfile(dst);
    return transform;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Use first byte to determine which functions to call
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0];
    size_t offset = 1;
    
    // Test cmsCreateContext with various inputs
    void* plugin_arg = NULL;
    void* userdata_arg = NULL;
    
    if (Size > offset + sizeof(void*)) {
        // Use fuzzer data to simulate plugin pointer (won't be dereferenced)
        plugin_arg = (void*)(uintptr_t)*(uint64_t*)(Data + offset);
        offset += sizeof(void*);
    }
    
    if (Size > offset + sizeof(void*)) {
        // Use fuzzer data to simulate user data pointer
        userdata_arg = (void*)(uintptr_t)*(uint64_t*)(Data + offset);
        offset += sizeof(void*);
    }
    
    cmsContext ctx1 = cmsCreateContext(plugin_arg, userdata_arg);
    
    // Test cmsGetContextUserData
    if (ctx1) {
        void* retrieved_data = cmsGetContextUserData(ctx1);
        (void)retrieved_data; // Use result to avoid unused variable warning
    }
    
    // Test cmsDupContext with various user data
    void* new_userdata = NULL;
    if (Size > offset + sizeof(void*)) {
        new_userdata = (void*)(uintptr_t)*(uint64_t*)(Data + offset);
        offset += sizeof(void*);
    }
    
    cmsContext ctx2 = NULL;
    if (ctx1) {
        ctx2 = cmsDupContext(ctx1, new_userdata);
    }
    
    // Test cmsGetProfileContextID
    cmsHPROFILE profile = NULL;
    if (selector & 0x01) {
        profile = create_dummy_profile(ctx1 ? ctx1 : NULL);
        if (profile) {
            cmsContext profile_ctx = cmsGetProfileContextID(profile);
            (void)profile_ctx; // Use result
            cmsCloseProfile(profile);
        }
    }
    
    // Test cmsGetTransformContextID
    cmsHTRANSFORM transform = NULL;
    if (selector & 0x02) {
        transform = create_dummy_transform(ctx1 ? ctx1 : NULL);
        if (transform) {
            cmsContext transform_ctx = cmsGetTransformContextID(transform);
            (void)transform_ctx; // Use result
            cmsDeleteTransform(transform);
        }
    }
    
    // Test _cmsDestroyMutex - requires a mutex to destroy
    // We'll create and destroy a mutex if context is valid
    if (ctx1) {
        void* mutex = _cmsCreateMutex(ctx1);
        if (mutex) {
            _cmsDestroyMutex(ctx1, mutex);
        }
    }
    
    // Cleanup contexts
    if (ctx2) {
        cmsDeleteContext(ctx2);
    }
    if (ctx1) {
        cmsDeleteContext(ctx1);
    }
    
    return 0;
}