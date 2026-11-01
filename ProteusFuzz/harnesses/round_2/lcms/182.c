#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

// Helper to create a context
static cmsContext create_test_context(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    return ctx;
}

static void cleanup_test_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

// Helper to build a fake plugin struct based on input
static void* build_fake_plugin(const uint8_t *Data, size_t Size) {
    // Estimate size of cmsPluginBase (Magic, ExpectedVersion, Type, Next)
    size_t min_size = 32; 
    
    if (Size < 4) {
        // Fallback for small inputs
        static cmsPluginBase static_plugin = {0};
        static_plugin.Magic = 0; 
        static_plugin.ExpectedVersion = 0;
        static_plugin.Type = 0;
        static_plugin.Next = NULL;
        return &static_plugin;
    }

    cmsPluginBase *plugin = (cmsPluginBase *)malloc(min_size);
    if (!plugin) return NULL;

    memset(plugin, 0, min_size);

    // Byte 0: Magic validity (0 = invalid, 1 = valid)
    uint8_t magic_valid = (Data[0] % 2);
    if (magic_valid) {
        plugin->Magic = cmsPluginMagicNumber;
    } else {
        plugin->Magic = 0xDEADBEEF; // Invalid magic to hit line 554
    }

    // Byte 1: Version check (0 = valid, 1 = too high)
    uint8_t version_high = (Data[1] % 2);
    if (version_high) {
        plugin->ExpectedVersion = LCMS_VERSION + 100; // Hit line 559
    } else {
        plugin->ExpectedVersion = LCMS_VERSION;
    }

    // Byte 2: Plugin Type selection
    uint8_t type_selector = Data[2];
    
    switch (type_selector % 14) {
        case 0: plugin->Type = cmsPluginMemHandlerSig; break;
        case 1: plugin->Type = cmsPluginInterpolationSig; break;
        case 2: plugin->Type = cmsPluginTagTypeSig; break;
        case 3: plugin->Type = cmsPluginTagSig; break;
        case 4: plugin->Type = cmsPluginFormattersSig; break;
        case 5: plugin->Type = cmsPluginRenderingIntentSig; break;
        case 6: plugin->Type = cmsPluginParametricCurveSig; break;
        case 7: plugin->Type = cmsPluginMultiProcessElementSig; break;
        case 8: plugin->Type = cmsPluginOptimizationSig; break;
        case 9: plugin->Type = cmsPluginTransformSig; break;
        case 10: plugin->Type = cmsPluginMutexSig; break;
        case 11: plugin->Type = cmsPluginParalellizationSig; break;
        case 12: plugin->Type = 0xFFFFFFFF; // Hit default case (line 615)
        case 13: plugin->Type = 0x12345678; // Another invalid type
        default: plugin->Type = 0;
    }

    // Byte 3: Chain length (0 or 1)
    uint8_t has_next = (Size > 3) ? (Data[3] % 2) : 0;
    if (has_next) {
        cmsPluginBase *plugin2 = (cmsPluginBase *)malloc(min_size);
        if (plugin2) {
            memset(plugin2, 0, min_size);
            plugin2->Magic = cmsPluginMagicNumber;
            plugin2->ExpectedVersion = LCMS_VERSION;
            plugin2->Type = cmsPluginMemHandlerSig; 
            plugin2->Next = NULL;
            plugin->Next = plugin2;
        }
    } else {
        plugin->Next = NULL;
    }

    return plugin;
}

int LLVMFuzzerTestOneInput_182(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Create a context
    cmsContext ctx = create_test_context(Data, Size);
    
    // 2. Build a fake plugin based on input
    void *fake_plugin = build_fake_plugin(Data, Size);
    
    if (fake_plugin == NULL) {
        cleanup_test_context(ctx);
        return 0;
    }

    // 3. Invoke cmsPluginTHR with context to hit validation and switch cases
    cmsPluginTHR(ctx, fake_plugin);
    
    // 4. Cleanup plugin
    cmsPluginBase *p = (cmsPluginBase *)fake_plugin;
    while (p != NULL) {
        cmsPluginBase *next = p->Next;
        free(p);
        p = next;
    }

    cleanup_test_context(ctx);

    // 5. Test global version (cmsPlugin) which calls cmsPluginTHR with NULL context
    fake_plugin = build_fake_plugin(Data, Size);
    if (fake_plugin != NULL) {
        cmsPlugin(fake_plugin);
        
        p = (cmsPluginBase *)fake_plugin;
        while (p != NULL) {
            cmsPluginBase *next = p->Next;
            free(p);
            p = next;
        }
    }

    return 0;
}