// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static cmsHPROFILE create_dummy_profile(const uint8_t *data, size_t size) {
    cmsHPROFILE hProfile = NULL;
    cmsContext ContextID = NULL;

    // Create a context if needed, though NULL is often valid for default context
    ContextID = cmsCreateContext(NULL, 0);
    if (ContextID == NULL) {
        return NULL;
    }

    hProfile = cmsCreateProfilePlaceholder(ContextID);
    if (hProfile == NULL) {
        cmsDeleteContext(ContextID);
        return NULL;
    }

    // Store context in profile or handle cleanup later? 
    // cmsCloseProfile handles the profile, but context needs explicit deletion if not shared.
    // For fuzzing, we can leak the context slightly or try to delete it at the end of the test.
    // However, cmsCloseProfile does not delete the context.
    // We will return the profile and let the caller (LLVMFuzzerTestOneInput) handle context cleanup 
    // or just leak it for simplicity in a fuzzing harness where memory is reset per run.
    // To be safe, we can attach the context ID to the profile or just delete it here if we don't need it.
    // But cmsCreateProfilePlaceholder requires the context to exist while the profile is alive.
    // So we must keep the context alive.
    // We can't easily pass the context back. 
    // Standard practice in lcms2 fuzzing: create context, create profile, close profile, delete context.
    // Since we return the profile, we need a way to pass the context.
    // Let's change the approach: create context in LLVMFuzzerTestOneInput and pass it or use NULL context if supported.
    // The error says cmsCreateProfilePlaceholder needs ContextID.
    // If we pass NULL, it might work if the library supports default context, but the signature requires it.
    // Let's create the context in the main function and pass it to a helper or just do it inline.
    
    // Revised plan: Do everything in LLVMFuzzerTestOneInput to manage context lifecycle properly.
    // We will remove the helper function or modify it to accept context.
    // For now, let's just return NULL from here and do it inline in the main function to avoid complexity.
    cmsDeleteContext(ContextID);
    return NULL; 
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    cmsContext ContextID = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsUInt8Number profileID[16] = {0};
    cmsBool ret;
    cmsUInt32Number bytesNeeded = 0;
    void *memPtr = NULL;
    cmsProfileClassSignature deviceClass;

    if (Size == 0) {
        return 0;
    }

    // Initialize context
    ContextID = cmsCreateContext(NULL, 0);
    if (ContextID == NULL) {
        return 0;
    }

    // Create profile placeholder with context
    hProfile = cmsCreateProfilePlaceholder(ContextID);
    if (hProfile == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    if (Size > 0) {
        // Cast input byte to cmsProfileClassSignature
        deviceClass = (cmsProfileClassSignature)(Data[0] & 0xFF);
        cmsSetDeviceClass(hProfile, deviceClass);
    }

    // Compute MD5 ID for the profile
    ret = cmsMD5computeID(hProfile);
    (void)ret;

    if (Size >= 16) {
        memcpy(profileID, Data, 16);
        cmsSetHeaderProfileID(hProfile, profileID);
    }

    // Retrieve the profile ID
    cmsGetHeaderProfileID(hProfile, profileID);

    // Attempt to save profile to memory
    ret = cmsSaveProfileToMem(hProfile, NULL, &bytesNeeded);
    if (ret && bytesNeeded > 0) {
        memPtr = malloc(bytesNeeded);
        if (memPtr != NULL) {
            ret = cmsSaveProfileToMem(hProfile, memPtr, &bytesNeeded);
            (void)ret;
            free(memPtr);
        }
    }

    // Cleanup
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ContextID);

    return 0;
}