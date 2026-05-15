// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// cmsPlugin at cmsplugin.c:541:19 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsUnregisterPlugins at cmsplugin.c:627:16 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
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

static void* create_dummy_plugin(cmsUInt32Number plugin_type) {
    // Create a minimal plugin structure with valid base fields
    static struct {
        cmsUInt32Number Magic;
        cmsUInt32Number ExpectedVersion;
        cmsUInt32Number Type;
        void* Next;
    } dummy_plugin;
    
    dummy_plugin.Magic = cmsPluginMagicNumber;
    dummy_plugin.ExpectedVersion = LCMS_VERSION;
    dummy_plugin.Type = plugin_type;
    dummy_plugin.Next = NULL;
    
    return (void*)&dummy_plugin;
}

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    // Create contexts for testing
    cmsContext context1 = cmsCreateContext(NULL, NULL);
    cmsContext context2 = cmsCreateContext(NULL, NULL);
    
    if (context1 == NULL || context2 == NULL) {
        if (context1) cmsDeleteContext(context1);
        if (context2) cmsDeleteContext(context2);
        return 0;
    }
    
    // Test _cmsCreateMutex
    void* mutex1 = _cmsCreateMutex(context1);
    void* mutex2 = _cmsCreateMutex(context2);
    // Note: Mutexes are created via plugin, may return NULL if unsupported
    
    // Test cmsPluginTHR with different plugin types
    if (Size > 0) {
        // Use first byte to determine plugin type
        cmsUInt32Number plugin_type = Data[0] % 10; // Various plugin types
        void* dummy_plugin = create_dummy_plugin(plugin_type);
        
        cmsBool result1 = cmsPluginTHR(context1, dummy_plugin);
        cmsBool result2 = cmsPluginTHR(context2, dummy_plugin);
        
        // Test global plugin registration
        cmsBool global_result = cmsPlugin(dummy_plugin);
        
        // Test cmsGetSupportedIntentsTHR
        cmsUInt32Number max_intents = 10;
        cmsUInt32Number codes[10];
        char* descriptions[10];
        
        // Initialize arrays
        for (cmsUInt32Number i = 0; i < max_intents; i++) {
            codes[i] = 0;
            descriptions[i] = NULL;
        }
        
        cmsUInt32Number count1 = cmsGetSupportedIntentsTHR(context1, max_intents, codes, descriptions);
        cmsUInt32Number count2 = cmsGetSupportedIntentsTHR(context2, max_intents, codes, descriptions);
        
        // Test with different max values
        if (Size > 1) {
            cmsUInt32Number dynamic_max = Data[1] % 20;
            cmsUInt32Number dynamic_codes[20];
            char* dynamic_descriptions[20];
            
            for (cmsUInt32Number i = 0; i < 20; i++) {
                dynamic_codes[i] = 0;
                dynamic_descriptions[i] = NULL;
            }
            
            cmsUInt32Number dynamic_count = cmsGetSupportedIntentsTHR(context1, dynamic_max, dynamic_codes, dynamic_descriptions);
        }
    }
    
    // Test cmsUnregisterPluginsTHR
    cmsUnregisterPluginsTHR(context1);
    cmsUnregisterPluginsTHR(context2);
    
    // Test cmsUnregisterPlugins
    cmsUnregisterPlugins();
    
    // Create mutex again after unregistering plugins
    void* mutex3 = _cmsCreateMutex(context1);
    
    // Cleanup contexts
    cmsDeleteContext(context1);
    cmsDeleteContext(context2);
    
    return 0;
}