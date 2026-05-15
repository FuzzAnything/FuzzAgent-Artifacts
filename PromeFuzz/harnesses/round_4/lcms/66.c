// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void* create_dummy_plugin(const uint8_t* data, size_t size) {
    // Allocate a buffer for the plugin structure
    size_t plugin_size = 256; // Reasonable size for a plugin structure
    void* plugin = malloc(plugin_size);
    if (!plugin) {
        return NULL;
    }
    
    // Initialize with zeros
    memset(plugin, 0, plugin_size);
    
    // Copy fuzzer data into the plugin buffer, but ensure we don't overflow
    size_t copy_size = size < plugin_size ? size : plugin_size;
    if (copy_size > 0) {
        memcpy(plugin, data, copy_size);
    }
    
    // Set the Magic field to a valid value to avoid immediate rejection
    // cmsPluginMagicNumber is defined in lcms2_plugin.h
    uint32_t* magic_ptr = (uint32_t*)plugin;
    *magic_ptr = cmsPluginMagicNumber;
    
    return plugin;
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    // Create context with default memory management
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    // Create a dummy plugin structure with valid Magic field
    void* plugin = create_dummy_plugin(Data, Size);
    if (plugin != NULL) {
        cmsPluginTHR(ctx, plugin);
        free(plugin);
    }

    // Try to open a profile from memory using the fuzzer input
    cmsHPROFILE profile = NULL;
    if (Size > 0) {
        profile = cmsOpenProfileFromMemTHR(ctx, Data, (cmsUInt32Number)(Size < 1024 ? Size : 1024));
    }

    // Test mutex operations
    void* mutex = _cmsCreateMutex(ctx);
    if (mutex != NULL) {
        _cmsLockMutex(ctx, mutex);
        _cmsDestroyMutex(ctx, mutex);
    }

    // Clean up resources
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
    
    // Destroy the context
    cmsDeleteContext(ctx);

    return 0;
}