// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCIECAM02Init at cmscam02.c:358:22 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsGetContextUserData at cmsplugin.c:1021:17 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// cmsCIECAM02Init at cmscam02.c:358:22 in lcms2.h
// cmsDictAlloc at cmsnamed.c:1090:21 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void cleanup_resources(cmsContext ctx, cmsHANDLE handle, cmsHANDLE dict, cmsHANDLE gdb) {
    if (handle != NULL) {
        _cmsFree(ctx, handle);
    }
    if (dict != NULL) {
        _cmsFree(ctx, dict);
    }
    if (gdb != NULL) {
        _cmsFree(ctx, gdb);
    }
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = NULL;
    cmsContext dupCtx = NULL;
    cmsHANDLE ciecamHandle = NULL;
    cmsHANDLE dictHandle = NULL;
    cmsHANDLE gdbHandle = NULL;
    cmsViewingConditions vc;

    // Initialize viewing conditions safely
    // Since we don't have the exact struct definition, we zero-initialize it.
    // We will rely on the API to handle default values if fields are zero,
    // or we can try to set known public fields if they exist in the header.
    // For safety, we just zero init and let the API use defaults or fail gracefully.
    memset(&vc, 0, sizeof(vc));

    // If we have enough data, we can try to copy raw bytes into the struct
    // to trigger different internal states, assuming the struct is compatible.
    if (Size >= sizeof(vc)) {
        memcpy(&vc, Data, sizeof(vc));
    } else {
        // Partial copy
        memcpy(&vc, Data, Size);
    }

    // Create a context
    ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Test cmsCIECAM02Init
    ciecamHandle = cmsCIECAM02Init(ctx, &vc);

    // Test cmsGBDAlloc
    gdbHandle = cmsGBDAlloc(ctx);

    // Test cmsDictAlloc
    dictHandle = cmsDictAlloc(ctx);

    // Test cmsGetContextUserData
    void *userData = cmsGetContextUserData(ctx);
    (void)userData; // Use to avoid unused warning

    // Test cmsDupContext
    // Use a byte from input as user data pointer (casted)
    void *newUserData = (void*)(uintptr_t)(Data[0] & 0xFF);
    dupCtx = cmsDupContext(ctx, newUserData);
    
    if (dupCtx != NULL) {
        // Try operations on duplicated context
        cmsHANDLE dupCiecam = cmsCIECAM02Init(dupCtx, &vc);
        cmsHANDLE dupDict = cmsDictAlloc(dupCtx);
        cmsHANDLE dupGdb = cmsGBDAlloc(dupCtx);

        _cmsFree(dupCtx, dupCiecam);
        _cmsFree(dupCtx, dupDict);
        _cmsFree(dupCtx, dupGdb);

        cmsDeleteContext(dupCtx);
    }

    // Cleanup
    cleanup_resources(ctx, ciecamHandle, dictHandle, gdbHandle);

    return 0;
}