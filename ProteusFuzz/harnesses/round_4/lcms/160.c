#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

/* 
 * Internal structure definition for cmsPluginBase is required to construct the input.
 * The header lcms2_plugin.h already defines 'cmsPluginBase', so we define a local struct
 * with a unique name to avoid redefinition errors, then cast to the public type.
 */
typedef struct {
    cmsUInt32Number Magic;
    cmsUInt32Number ExpectedVersion;
    cmsUInt32Number Type;
    cmsUInt32Number Flags;
    cmsUInt32Number Version;
    void* Next;
} PluginBaseInternal;

/* 
 * Helper to create a plugin with specific properties to trigger uncovered lines.
 * Returns a pointer to a newly allocated PluginBaseInternal.
 * Caller is responsible for freeing it.
 */
static PluginBaseInternal* create_test_plugin(cmsUInt32Number magic, cmsUInt32Number version, cmsUInt32Number type) {
    PluginBaseInternal* plugin = (PluginBaseInternal*)malloc(sizeof(PluginBaseInternal));
    if (!plugin) return NULL;

    memset(plugin, 0, sizeof(PluginBaseInternal));
    plugin->Magic = magic;
    plugin->ExpectedVersion = version;
    plugin->Type = type;
    plugin->Next = NULL;
    
    return plugin;
}

int LLVMFuzzerTestOneInput_160(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* Create a context for the plugin registration */
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }

    /* Use the first byte to select the test scenario */
    uint8_t scenario = Data[0] % 5;

    PluginBaseInternal* plugin = NULL;
    cmsBool result = FALSE;

    switch (scenario) {
        case 0:
            /* Scenario 0: Trigger Line 554 (Invalid Magic) */
            /* Use a magic number different from cmsPluginMagicNumber */
            plugin = create_test_plugin(0xDEADBEEF, LCMS_VERSION, 0);
            break;

        case 1:
            /* Scenario 1: Trigger Line 559 (Version too high) */
            /* Use valid magic, but version > LCMS_VERSION */
            plugin = create_test_plugin(cmsPluginMagicNumber, LCMS_VERSION + 100, 0);
            break;

        case 2:
            /* Scenario 2: Trigger specific switch cases (Lines 567-612) */
            /* Use valid magic and version, but set Type to a specific signature */
            /* We cycle through known signatures based on remaining data if available */
            cmsUInt32Number type = 0;
            if (Size > 1) {
                type = Data[1] % 15; /* 15 known cases roughly */
            }
            
            /* Map type to known signatures to hit specific lines */
            switch (type) {
                case 0: type = cmsPluginMemHandlerSig; break;
                case 1: type = cmsPluginInterpolationSig; break;
                case 2: type = cmsPluginTagTypeSig; break;
                case 3: type = cmsPluginTagSig; break;
                case 4: type = cmsPluginFormattersSig; break;
                case 5: type = cmsPluginRenderingIntentSig; break;
                case 6: type = cmsPluginParametricCurveSig; break;
                case 7: type = cmsPluginMultiProcessElementSig; break;
                case 8: type = cmsPluginOptimizationSig; break;
                case 9: type = cmsPluginTransformSig; break;
                case 10: type = cmsPluginMutexSig; break;
                case 11: type = cmsPluginParalellizationSig; break;
                default: type = 0; break;
            }
            plugin = create_test_plugin(cmsPluginMagicNumber, LCMS_VERSION, type);
            break;

        case 3:
            /* Scenario 3: Trigger Line 615 (Default case - Unknown Type) */
            /* Valid magic/version, but an invalid Type */
            plugin = create_test_plugin(cmsPluginMagicNumber, LCMS_VERSION, 0xFFFFFFFF);
            break;

        case 4:
            /* Scenario 4: Valid Plugin (Happy Path) */
            /* This tests the loop and successful return, though specific lines might be covered by others */
            plugin = create_test_plugin(cmsPluginMagicNumber, LCMS_VERSION, cmsPluginMemHandlerSig);
            break;
    }

    if (plugin) {
        /* Call the target function */
        /* Cast to cmsPluginBase* as expected by the API */
        result = cmsPluginTHR(ctx, (cmsPluginBase*)plugin);
        (void)result; /* Suppress unused warning */
        free(plugin);
    }

    /* Cleanup */
    cmsDeleteContext(ctx);

    return 0;
}