// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void* customMalloc(cmsContext ContextID, cmsUInt32Number size) {
    return malloc(size);
}

static void customFree(cmsContext ContextID, void* ptr) {
    free(ptr);
}

static void* customRealloc(cmsContext ContextID, void* ptr, cmsUInt32Number newSize) {
    return realloc(ptr, newSize);
}

static void* customCalloc(cmsContext ContextID, cmsUInt32Number num, cmsUInt32Number size) {
    return calloc(num, size);
}

static void* customDup(cmsContext ContextID, const void* org, cmsUInt32Number size) {
    void* ptr = malloc(size);
    if (ptr) {
        memcpy(ptr, org, size);
    }
    return ptr;
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    // Create initial context with default memory management
    cmsContext ctx1 = cmsCreateContext(NULL, NULL);
    if (!ctx1) return 0;
    
    // Test cmsOpenProfileFromMemTHR with fuzz data
    cmsHPROFILE profile = cmsOpenProfileFromMemTHR(ctx1, Data, Size);
    if (profile) {
        cmsCloseProfile(profile);
    }
    
    // Test cmsCreateNULLProfileTHR
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(ctx1);
    if (nullProfile) {
        cmsCloseProfile(nullProfile);
    }
    
    // Test cmsDupContext
    cmsContext ctx2 = cmsDupContext(ctx1, NULL);
    
    // Test _cmsRealloc with various scenarios
    if (Size > 0) {
        // Allocate initial block
        void* ptr = _cmsMalloc(ctx1, 16);
        if (ptr) {
            // Reallocate to different sizes based on fuzz data
            cmsUInt32Number newSize = (Size > 4) ? (Data[0] % 1024) + 1 : 32;
            void* newPtr = _cmsRealloc(ctx1, ptr, newSize);
            if (newPtr) {
                // Try realloc again with different size
                cmsUInt32Number anotherSize = (Size > 8) ? (Data[1] % 2048) + 1 : 64;
                void* anotherPtr = _cmsRealloc(ctx1, newPtr, anotherSize);
                if (anotherPtr) {
                    _cmsFree(ctx1, anotherPtr);
                } else {
                    _cmsFree(ctx1, newPtr);
                }
            } else {
                _cmsFree(ctx1, ptr);
            }
        }
    }
    
    // Test cmsPluginTHR with memory plugin using cmsPluginMemHandler
    cmsPluginMemHandler memPlugin;
    memset(&memPlugin, 0, sizeof(memPlugin));
    memPlugin.base.Magic = cmsPluginMagicNumber;
    memPlugin.base.ExpectedVersion = LCMS_VERSION;
    memPlugin.base.Type = cmsPluginMemHandlerSig;
    memPlugin.base.Next = NULL;
    memPlugin.MallocPtr = customMalloc;
    memPlugin.FreePtr = customFree;
    memPlugin.ReallocPtr = customRealloc;
    memPlugin.CallocPtr = customCalloc;
    memPlugin.DupPtr = customDup;
    
    cmsPluginTHR(ctx1, (void*)&memPlugin);
    
    // Test operations with duplicated context if available
    if (ctx2) {
        // Try opening profile with duplicated context
        if (Size > 0) {
            cmsHPROFILE profile2 = cmsOpenProfileFromMemTHR(ctx2, Data, Size);
            if (profile2) {
                cmsCloseProfile(profile2);
            }
        }
        
        // Try creating NULL profile with duplicated context
        cmsHPROFILE nullProfile2 = cmsCreateNULLProfileTHR(ctx2);
        if (nullProfile2) {
            cmsCloseProfile(nullProfile2);
        }
        
        cmsDeleteContext(ctx2);
    }
    
    // Test creating another context with custom user data
    int userData = 42;
    cmsContext ctx3 = cmsCreateContext(NULL, &userData);
    if (ctx3) {
        // Try some operations with this context
        cmsHPROFILE profile3 = cmsCreateNULLProfileTHR(ctx3);
        if (profile3) {
            cmsCloseProfile(profile3);
        }
        cmsDeleteContext(ctx3);
    }
    
    // Cleanup
    cmsDeleteContext(ctx1);
    
    return 0;
}