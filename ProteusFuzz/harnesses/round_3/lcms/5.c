// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsHPROFILE create_dummy_profile(const uint8_t *Data, size_t Size) {
    if (Size < 128) return NULL;
    
    // Create a minimal valid ICC profile structure for testing
    unsigned char *profile_data = (unsigned char *)malloc(Size);
    if (!profile_data) return NULL;
    
    memcpy(profile_data, Data, Size);
    
    // Ensure minimum header validity (basic checks)
    // In real scenarios, you'd validate more thoroughly
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(profile_data, Size);
    free(profile_data);
    
    return hProfile;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    cmsContext ContextID = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number Intent = 0;
    cmsUInt32Number dwFlags = 0;
    void *Buffer = NULL;
    cmsUInt32Number dwBufferLen = 0;
    cmsUInt32Number ret1, ret2;
    void *malloc_ptr;
    
    // Create a context
    ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;
    
    // Create a dummy profile from input data
    if (Size > 0) {
        hProfile = create_dummy_profile(Data, Size);
    }
    
    // Only proceed if we have a valid profile
    if (hProfile == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }
    
    // First call to cmsGetPostScriptCSA to get required size
    ret1 = cmsGetPostScriptCSA(ContextID, hProfile, Intent, dwFlags, NULL, 0);
    
    // If we got a size, allocate memory and call again
    if (ret1 > 0) {
        Buffer = _cmsMalloc(ContextID, ret1);
        if (Buffer) {
            ret2 = cmsGetPostScriptCSA(ContextID, hProfile, Intent, dwFlags, Buffer, ret1);
            // Buffer is allocated via _cmsMalloc, will be freed later
        }
    }
    
    // Test _cmsMalloc with a size derived from input
    if (Size > 0) {
        cmsUInt32Number alloc_size = (Size % 1024) + 1;
        malloc_ptr = _cmsMalloc(ContextID, alloc_size);
        if (malloc_ptr) {
            // Use the allocated memory to avoid unused variable warnings
            memset(malloc_ptr, 0, alloc_size);
            _cmsFree(ContextID, malloc_ptr);
        }
    }
    
    // Cleanup
    if (Buffer) {
        _cmsFree(ContextID, Buffer);
    }
    
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    cmsDeleteContext(ContextID);
    
    return 0;
}