// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsUnregisterPlugins at cmsplugin.c:627:16 in lcms2.h
// cmsPlugin at cmsplugin.c:541:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsUnregisterPlugins at cmsplugin.c:627:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    // Test cmsUnregisterPlugins (global plugin cleanup)
    cmsUnregisterPlugins();
    
    // Test cmsPlugin (global plugin registration)
    // We'll use a minimal plugin structure from fuzzer data
    if (Size >= sizeof(cmsPluginBase)) {
        cmsPluginBase plugin_base;
        memcpy(&plugin_base, Data, sizeof(cmsPluginBase));
        
        // Ensure we have valid magic number for plugin
        plugin_base.Magic = cmsPluginMagicNumber;
        plugin_base.ExpectedVersion = LCMS_VERSION;
        
        cmsPlugin((void*)&plugin_base);
    }
    
    // Test cmsCreateContext with various inputs
    cmsContext context = NULL;
    
    // Try creating context with NULL plugin and user data
    context = cmsCreateContext(NULL, NULL);
    
    if (context != NULL) {
        // Test cmsPluginTHR (thread-specific plugin registration)
        if (Size >= sizeof(cmsPluginBase)) {
            cmsPluginBase thr_plugin;
            memcpy(&thr_plugin, Data, sizeof(cmsPluginBase));
            thr_plugin.Magic = cmsPluginMagicNumber;
            thr_plugin.ExpectedVersion = LCMS_VERSION;
            
            cmsPluginTHR(context, (void*)&thr_plugin);
        }
        
        // Test _cmsCreateMutex
        void* mutex = _cmsCreateMutex(context);
        if (mutex != NULL) {
            // Mutex created successfully
            // Note: We don't have destroy function in API list, so we can't clean it up
        }
        
        // Test cmsUnregisterPluginsTHR
        cmsUnregisterPluginsTHR(context);
        
        // Clean up context
        cmsDeleteContext(context);
    }
    
    // Try creating context with plugin data if we have enough
    if (Size >= sizeof(cmsPluginBase)) {
        cmsPluginBase context_plugin;
        memcpy(&context_plugin, Data, sizeof(cmsPluginBase));
        context_plugin.Magic = cmsPluginMagicNumber;
        context_plugin.ExpectedVersion = LCMS_VERSION;
        
        // Use remaining data as user data if available
        void* user_data = NULL;
        if (Size > sizeof(cmsPluginBase)) {
            user_data = malloc(Size - sizeof(cmsPluginBase));
            if (user_data) {
                memcpy(user_data, Data + sizeof(cmsPluginBase), Size - sizeof(cmsPluginBase));
            }
        }
        
        context = cmsCreateContext((void*)&context_plugin, user_data);
        
        if (context != NULL) {
            // Test plugin registration with this context
            if (Size >= sizeof(cmsPluginBase) * 2) {
                cmsPluginBase another_plugin;
                memcpy(&another_plugin, Data + sizeof(cmsPluginBase), sizeof(cmsPluginBase));
                another_plugin.Magic = cmsPluginMagicNumber;
                another_plugin.ExpectedVersion = LCMS_VERSION;
                
                cmsPluginTHR(context, (void*)&another_plugin);
            }
            
            // Test mutex creation again
            _cmsCreateMutex(context);
            
            // Clean up
            cmsDeleteContext(context);
        }
        
        if (user_data) {
            free(user_data);
        }
    }
    
    // Final cleanup of global plugins
    cmsUnregisterPlugins();
    
    return 0;
}