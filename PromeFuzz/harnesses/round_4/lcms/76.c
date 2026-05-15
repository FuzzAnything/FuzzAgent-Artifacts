// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPlugin at cmsplugin.c:541:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    // Create plugin structure from fuzzer data
    cmsPluginBase base_plugin;
    if (Size >= sizeof(base_plugin)) {
        memcpy(&base_plugin, Data, sizeof(base_plugin));
        Data += sizeof(base_plugin);
        Size -= sizeof(base_plugin);
    } else {
        // Not enough data for full plugin, use defaults
        memset(&base_plugin, 0, sizeof(base_plugin));
    }

    // Test cmsPlugin (global plugin registration)
    cmsPlugin(&base_plugin);

    // Create context with plugin data
    cmsContext context = cmsCreateContext(&base_plugin, NULL);
    if (!context) {
        // If context creation fails, continue with default context
        context = cmsCreateContext(NULL, NULL);
        if (!context) {
            return 0; // Can't proceed without context
        }
    }

    // Test cmsPluginTHR (context-specific plugin)
    cmsPluginTHR(context, &base_plugin);

    // Test _cmsCreateMutex
    void* mutex = _cmsCreateMutex(context);
    // Mutex cleanup would normally be done by _cmsDestroyMutex,
    // but we don't have that function in our API list

    // Test cmsGetSupportedIntentsTHR
    cmsUInt32Number max_intents = 10;
    cmsUInt32Number codes[10];
    char* descriptions[10];
    char desc_buffers[10][256];
    
    // Initialize description pointers
    for (cmsUInt32Number i = 0; i < max_intents; i++) {
        descriptions[i] = desc_buffers[i];
    }
    
    cmsUInt32Number count = cmsGetSupportedIntentsTHR(context, max_intents, codes, descriptions);

    // Test cmsUnregisterPluginsTHR
    cmsUnregisterPluginsTHR(context);

    // Cleanup
    cmsDeleteContext(context);
    
    return 0;
}