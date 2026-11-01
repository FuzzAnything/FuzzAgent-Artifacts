// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Forward declaration for internal types if not exposed in public headers
// Note: _cmsMemPluginChunkType is internal. We rely on the default context behavior
// or use the public API to create a context. Since we cannot easily set a custom
// internal plugin without internal headers, we will use the standard context
// which uses the default memory manager. The _cmsMalloc and _cmsFree functions
// will then use the default system allocators associated with the context.

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a standard context. This initializes the default memory manager.
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Create a dummy profile for testing
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (!hProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }

    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;

    // First call to cmsGetPostScriptCSA to get required size
    cmsUInt32Number requiredSize = cmsGetPostScriptCSA(ctx, hProfile, intent, flags, NULL, 0);
    
    // If initial call fails or returns 0, try with different parameters
    if (requiredSize == 0) {
        intent = 1; // Relative colorimetric
        flags = 0;
        requiredSize = cmsGetPostScriptCSA(ctx, hProfile, intent, flags, NULL, 0);
    }

    if (requiredSize > 0 && requiredSize < 1024 * 1024) {
        // Allocate memory using _cmsMalloc with the context
        void* buffer = _cmsMalloc(ctx, requiredSize);
        if (buffer) {
            // Second call to cmsGetPostScriptCSA to fill the buffer
            cmsUInt32Number written = cmsGetPostScriptCSA(ctx, hProfile, intent, flags, buffer, requiredSize);
            (void)written; // Suppress unused variable warning

            // Free memory using _cmsFree with the context
            _cmsFree(ctx, buffer);
        }
    }

    // Cleanup
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);

    return 0;
}