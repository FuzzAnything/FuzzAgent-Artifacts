// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsUnregisterPlugins at cmsplugin.c:627:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPlugin at cmsplugin.c:541:19 in lcms2.h
// cmsUnregisterPlugins at cmsplugin.c:627:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void* create_plugin_data(const uint8_t *Data, size_t Size) {
    // Allocate heap memory for plugin data to avoid stack-use-after-return
    void* plugin_data = malloc(Size > 0 ? Size : 1);
    if (!plugin_data) return NULL;
    if (Size > 0) {
        memcpy(plugin_data, Data, Size);
    }
    return plugin_data;
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    // Clean global plugins at start
    cmsUnregisterPlugins();
    
    // Create heap-allocated plugin data from fuzzer input
    void* plugin_data = create_plugin_data(Data, Size);
    if (!plugin_data) return 0;
    
    // Create context with heap-allocated plugin data
    cmsContext ctx = cmsCreateContext(plugin_data, plugin_data);
    
    if (ctx) {
        // Try to register plugin in thread context
        cmsPluginTHR(ctx, plugin_data);
        
        // Try to create mutex
        void* mutex = _cmsCreateMutex(ctx);
        (void)mutex; // Use mutex to avoid unused variable warning
        
        // Unregister plugins for this context
        cmsUnregisterPluginsTHR(ctx);
        
        // Clean up context
        cmsDeleteContext(ctx);
    }
    
    // Try to register global plugin
    cmsPlugin(plugin_data);
    
    // Clean global plugins at end
    cmsUnregisterPlugins();
    
    // Free the heap-allocated plugin data
    free(plugin_data);
    
    return 0;
}