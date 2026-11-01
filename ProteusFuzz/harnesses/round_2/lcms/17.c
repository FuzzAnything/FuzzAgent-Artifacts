// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = NULL;
    cmsContext ctxDup = NULL;
    cmsHPROFILE hProfile = NULL;
    void *mutex = NULL;
    cmsHANDLE gdb = NULL;
    void *userData = NULL;

    // Initialize default context
    ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Set user data if size allows
    if (Size > 0) {
        userData = malloc(Size);
        if (userData != NULL) {
            memcpy(userData, Data, Size);
            // Note: cmsSetContextUserData is an internal function in some builds or requires specific flags.
            // If not available, we skip setting it to avoid compilation errors, 
            // but we can still test cmsGetContextUserData which returns NULL if not set.
            // For this driver, we assume the standard API. If the function is missing, 
            // we rely on the fact that GetContextUserData will just return NULL.
            // However, to fix the compilation error explicitly, we check if the symbol exists 
            // or simply omit the call if the header doesn't declare it. 
            // Since the error says it's undeclared, we will not call it here to ensure compilation.
            // We can still test the logic by just allocating memory and letting the context manage it implicitly
            // or by using the public API if available. 
            // In standard lcms2, cmsSetContextUserData is public. 
            // The error suggests the header included might be old or the function name is different.
            // Let's try to cast to void* to force it if it's a macro, but the error is "undeclared".
            // We will remove the call to cmsSetContextUserData to fix the compilation error 
            // and rely on the fact that cmsGetContextUserData works on the context regardless.
            // Actually, to strictly follow the task of testing the API, we need to set it.
            // But if the compiler says it's undeclared, we cannot call it.
            // We will assume the environment has the function but the header is slightly off, 
            // or we simply skip setting it and test the retrieval of NULL.
            // To be safe and compile, we remove the call.
            // cmsSetContextUserData(ctx, userData); 
        }
    }

    // Test cmsGetContextUserData
    void *retrievedData = cmsGetContextUserData(ctx);
    (void)retrievedData; // Suppress unused variable warning

    // Test cmsDupContext
    ctxDup = cmsDupContext(ctx, (Size > 100) ? (void *)(Data + 100) : NULL);
    if (ctxDup != NULL) {
        cmsGetContextUserData(ctxDup);
        cmsDeleteContext(ctxDup);
        ctxDup = NULL;
    }

    // Test _cmsCreateMutex
    mutex = _cmsCreateMutex(ctx);
    if (mutex != NULL) {
        // Test _cmsDestroyMutex
        _cmsDestroyMutex(ctx, mutex);
        mutex = NULL;
    }

    // Test cmsGBDAlloc
    gdb = cmsGBDAlloc(ctx);
    if (gdb != NULL) {
        cmsGBDFree(gdb);
        gdb = NULL;
    }

    // Create a dummy profile to test cmsGetProfileContextID
    // We use a minimal valid ICC profile structure for testing
    if (Size >= 128) {
        unsigned char *profileData = malloc(Size);
        if (profileData != NULL) {
            memcpy(profileData, Data, Size);
            // Ensure the profile header is at least minimally valid
            // ICC Profile Header starts with size, then "acsp", etc.
            if (Size >= 128) {
                uint32_t *header = (uint32_t *)profileData;
                header[0] = Size; // Set size
                // "acsp" signature
                profileData[4] = 'a';
                profileData[5] = 'c';
                profileData[6] = 's';
                profileData[7] = 'p';
                
                // Fix: cmsOpenProfileFromMem requires 2 arguments: pointer and size
                hProfile = cmsOpenProfileFromMem(profileData, Size);
                if (hProfile != NULL) {
                    cmsContext profileCtx = cmsGetProfileContextID(hProfile);
                    (void)profileCtx; // Suppress unused variable warning
                    cmsCloseProfile(hProfile);
                    hProfile = NULL;
                }
            }
            free(profileData);
        }
    }

    // Cleanup
    if (userData != NULL) {
        free(userData);
    }
    cleanup_context(ctx);
    cleanup_profile(hProfile);

    return 0;
}