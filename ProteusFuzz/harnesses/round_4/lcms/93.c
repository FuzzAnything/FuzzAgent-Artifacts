// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsSetHeaderProfileID at cmsio0.c:1056:16 in lcms2.h
// cmsGetHeaderProfileID at cmsio0.c:1050:16 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsUInt8Number profileID[16];
    cmsUInt8Number profileIDOut[16];
    cmsUInt32Number bytesNeeded = 0;
    void *memPtr = NULL;
    cmsContext ContextID = NULL;

    // Initialize profile ID from input data
    memcpy(profileID, Data, 16);

    // Create a context if needed, though NULL is often acceptable for default context
    // However, cmsCreateProfilePlaceholder requires a context.
    // We can create a new context or use NULL if the library allows it for default.
    // To be safe and explicit, we create a new context.
    ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) {
        return 0;
    }

    // Create a dummy profile using the correct API
    hProfile = cmsCreateProfilePlaceholder(ContextID);
    if (!hProfile) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Test cmsSetDeviceClass
    if (Size >= 17) {
        cmsProfileClassSignature sig = (cmsProfileClassSignature)(Data[16] & 0xFF);
        cmsSetDeviceClass(hProfile, sig);
    }

    // Test cmsSetHeaderProfileID
    cmsSetHeaderProfileID(hProfile, profileID);

    // Test cmsGetHeaderProfileID
    cmsGetHeaderProfileID(hProfile, profileIDOut);

    // Test cmsMD5computeID
    cmsBool md5Result = cmsMD5computeID(hProfile);
    (void)md5Result; // Suppress unused warning

    // Test cmsSaveProfileToMem to get size
    if (!cmsSaveProfileToMem(hProfile, NULL, &bytesNeeded)) {
        cleanup_profile(hProfile);
        cmsDeleteContext(ContextID);
        return 0;
    }

    if (bytesNeeded > 0) {
        memPtr = malloc(bytesNeeded);
        if (memPtr) {
            cmsBool saveResult = cmsSaveProfileToMem(hProfile, memPtr, &bytesNeeded);
            (void)saveResult; // Suppress unused warning
            free(memPtr);
        }
    }

    cleanup_profile(hProfile);
    cmsDeleteContext(ContextID);
    return 0;
}