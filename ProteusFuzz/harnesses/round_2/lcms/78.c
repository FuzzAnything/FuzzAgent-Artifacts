// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsMD5alloc at cmsmd5.c:154:21 in lcms2_plugin.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsUnregisterPluginsTHR(ctx);
        cmsDeleteContext(ctx);
    }
}

static void cleanup_handle(cmsContext ctx, cmsHANDLE handle) {
    if (handle != NULL) {
        _cmsFree(ctx, handle);
    }
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = NULL;
    cmsContext dupCtx = NULL;
    cmsHANDLE md5Handle = NULL;
    cmsHANDLE dictHandle = NULL;
    void *userData = NULL;

    // Create a context using the fuzzer data to influence behavior if possible
    // We use the first byte to decide whether to create a new context or duplicate the default one
    if (Data[0] % 2 == 0) {
        ctx = cmsCreateContext(NULL, NULL);
    } else {
        // Try to duplicate the default context (NULL)
        dupCtx = cmsDupContext(NULL, NULL);
        ctx = dupCtx;
    }

    if (ctx == NULL) {
        return 0;
    }

    // Set user data from the input buffer if size allows
    // Note: cmsSetContextUserData is not in the target list, so we skip setting it
    // to avoid compilation errors, but we can still test retrieval if it was set externally
    // For this driver, we will not set it since the function is not declared in the target list
    // and might not be available in the specific build configuration.

    // Test cmsGetContextUserData
    void *retrievedData = cmsGetContextUserData(ctx);
    (void)retrievedData; // Suppress unused variable warning

    // Test cmsMD5alloc
    md5Handle = cmsMD5alloc(ctx);
    if (md5Handle != NULL) {
        // Simulate some usage if possible, though we don't have update/finalize in the target list
        // We just ensure it was allocated
    }

    // Test cmsDictAlloc
    dictHandle = cmsDictAlloc(ctx);
    if (dictHandle != NULL) {
        // Simulate some usage
    }

    // Test cmsDupContext with the created context
    cmsContext clonedCtx = cmsDupContext(ctx, NULL);
    if (clonedCtx != NULL) {
        // Test cmsUnregisterPluginsTHR on the cloned context
        cmsUnregisterPluginsTHR(clonedCtx);
        cmsDeleteContext(clonedCtx);
    }

    // Test cmsUnregisterPluginsTHR on the main context
    cmsUnregisterPluginsTHR(ctx);

    // Cleanup handles
    cleanup_handle(ctx, md5Handle);
    cleanup_handle(ctx, dictHandle);

    // Cleanup context
    cleanup_context(ctx);

    return 0;
}