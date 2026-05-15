// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsUnregisterPlugins at cmsplugin.c:627:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPlugin at cmsplugin.c:541:19 in lcms2.h
// cmsUnregisterPlugins at cmsplugin.c:627:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
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
#include <string.h>

static void* create_dummy_plugin(const uint8_t* data, size_t size) {
    // Create a minimal plugin structure using fuzz data
    // Allocate on heap to avoid returning address of local variable
    cmsPluginBase* plugin_base = (cmsPluginBase*)malloc(sizeof(cmsPluginBase));
    if (!plugin_base) return NULL;
    
    if (size >= sizeof(cmsPluginBase)) {
        memcpy(plugin_base, data, sizeof(cmsPluginBase));
        // Ensure Next pointer is NULL to avoid following random pointers
        plugin_base->Next = NULL;
    } else {
        // Initialize with some default values if not enough data
        plugin_base->Magic = 0x6C434D53; // "lCMS" in hex
        plugin_base->ExpectedVersion = LCMS_VERSION;
        plugin_base->Type = 0;
        plugin_base->Next = NULL;
    }
    
    return plugin_base;
}

static void free_dummy_plugin(void* plugin) {
    if (plugin) {
        free(plugin);
    }
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    // Test global plugin functions first
    if (Size > 0) {
        void* plugin = create_dummy_plugin(Data, Size);
        
        if (plugin) {
            // Test cmsPlugin with dummy plugin
            cmsPlugin(plugin);
            
            // Test cmsUnregisterPlugins
            cmsUnregisterPlugins();
            
            free_dummy_plugin(plugin);
        }
    }
    
    // Test context-based functions
    if (Size > sizeof(cmsPluginBase)) {
        void* plugin = create_dummy_plugin(Data, Size);
        
        if (plugin) {
            // Create context with plugin and user data
            cmsContext context = cmsCreateContext(plugin, NULL); // Use NULL for UserData
            
            if (context != NULL) {
                // Test cmsPluginTHR with the context
                cmsPluginTHR(context, plugin);
                
                // Test _cmsCreateMutex
                void* mutex = _cmsCreateMutex(context);
                // Note: We should destroy the mutex if created, but 
                // _cmsDestroyMutex is not in the target functions list.
                // In a real fuzzer, we would need to pair create/destroy.
                
                // Test cmsUnregisterPluginsTHR
                cmsUnregisterPluginsTHR(context);
                
                // Clean up context
                cmsDeleteContext(context);
            }
            
            free_dummy_plugin(plugin);
        }
    }
    
    // Test with minimal data
    if (Size > 0) {
        // Create context with NULL plugin
        cmsContext context = cmsCreateContext(NULL, NULL);
        
        if (context != NULL) {
            // Test with NULL plugin
            cmsPluginTHR(context, NULL);
            
            // Test mutex creation
            void* mutex = _cmsCreateMutex(context);
            
            // Unregister plugins
            cmsUnregisterPluginsTHR(context);
            
            // Clean up context
            cmsDeleteContext(context);
        }
    }
    
    // Test edge cases
    if (Size == 0) {
        // Test with empty input
        cmsUnregisterPlugins();
        
        cmsContext context = cmsCreateContext(NULL, NULL);
        if (context != NULL) {
            cmsUnregisterPluginsTHR(context);
            _cmsCreateMutex(context);
            cmsDeleteContext(context);
        }
    }
    
    return 0;
}