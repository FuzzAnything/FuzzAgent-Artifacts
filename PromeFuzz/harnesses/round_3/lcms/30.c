// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// cmsPlugin at cmsplugin.c:541:19 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    // Create a context with NULL plugin and user data
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0;
    }

    // Use fuzz data to create a dummy plugin structure
    // We'll use a minimal structure that matches the plugin base
    struct _cmsPluginBase {
        cmsUInt32Number Magic;
        cmsUInt32Number ExpectedVersion;
        cmsUInt32Number Type;
        cmsUInt32Number Next;
    } dummy_plugin;

    // Initialize with some values from fuzz data
    if (Size >= sizeof(dummy_plugin)) {
        memcpy(&dummy_plugin, Data, sizeof(dummy_plugin));
    } else {
        // Use default values if not enough data
        dummy_plugin.Magic = 0x6163706C; // 'acpl' magic number
        dummy_plugin.ExpectedVersion = LCMS_VERSION;
        dummy_plugin.Type = 0;
        dummy_plugin.Next = 0;
    }

    // Try to register the plugin with the context
    cmsPluginTHR(context, &dummy_plugin);

    // Try to register globally
    cmsPlugin(&dummy_plugin);

    // Get supported intents
    cmsUInt32Number max_intents = 10;
    cmsUInt32Number codes[10];
    char* descriptions[10];
    
    // Initialize description pointers
    for (cmsUInt32Number i = 0; i < max_intents; i++) {
        descriptions[i] = NULL;
    }
    
    cmsUInt32Number count = cmsGetSupportedIntentsTHR(context, max_intents, codes, descriptions);

    // Create a mutex
    void* mutex = _cmsCreateMutex(context);
    
    // Destroy the mutex to prevent memory leak
    if (mutex != NULL) {
        _cmsDestroyMutex(context, mutex);
    }

    // Unregister plugins before cleanup
    cmsUnregisterPluginsTHR(context);

    // Delete the context
    cmsDeleteContext(context);

    return 0;
}