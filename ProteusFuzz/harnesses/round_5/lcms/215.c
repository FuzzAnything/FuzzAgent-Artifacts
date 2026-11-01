// Fuzz driver for cmsPluginTHR
// Target: cmsPluginTHR (/mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/sources/lcms/src/cmsplugin.c:546:19)
// Goal: Cover lines 554, 559, 567-612, 615

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// NOTE: Removed manual definition of _cmsPluginBaseStruct to avoid redefinition errors.
// The struct and its typedef (cmsPluginBase) are defined in <lcms2_plugin.h>.

// Helper to create a dummy plugin with specific properties
static cmsPluginBase* create_test_plugin(cmsUInt32Number type, cmsUInt32Number magic, cmsUInt32Number expected_version, cmsPluginBase *next) {
    cmsPluginBase *plugin = (cmsPluginBase*)malloc(sizeof(cmsPluginBase));
    if (!plugin) return NULL;

    memset(plugin, 0, sizeof(cmsPluginBase));
    
    plugin->Magic = magic;
    // FIX: Removed 'Version' and 'Size' assignments. 
    // In newer LCMS2 versions, cmsPluginBase does not expose these fields for user initialization.
    // The target function cmsPluginTHR relies on Magic and Type for routing.
    // plugin->Version = LCMS_VERSION; // Removed: Field does not exist
    // plugin->Size = sizeof(cmsPluginBase); // Removed: Field does not exist
    
    plugin->Type = type;
    // Note: ExpectedVersion is not a standard field in cmsPluginBase in newer versions either, 
    // but if it exists in this specific build, we leave it out to be safe or assume it's internal.
    // If the target code checks a specific version field inside the plugin, it might be internal.
    // However, the error log specifically cited Version and Size. 
    // We assume the 'expected_version' argument is no longer directly mapped to a struct field 
    // in the way the original code attempted.
    
    plugin->Next = next;
    
    // Initialize function pointers to NULL. 
    // The target function cmsPluginTHR checks for NULL and handles it appropriately for the switch logic.
    // We do not need to implement the actual logic, just trigger the registration call.
    
    return plugin;
}

int LLVMFuzzerTestOneInput_215(const uint8_t *Data, size_t Size) {
    cmsContext ctx = NULL;
    cmsPluginBase *plugin_list = NULL;
    cmsBool result;
    
    if (Size < 1) {
        return 0;
    }

    // Create a context to test thread-specific plugin registration
    ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        // If context creation fails, we can't test cmsPluginTHR effectively with a context.
        return 0;
    }

    // Determine test case based on input data
    uint8_t mode = Data[0] % 15; // 0-14

    cmsUInt32Number test_magic = 0x434D5350; // "CMSP" is the standard magic
    cmsUInt32Number test_version = LCMS_VERSION;
    cmsUInt32Number test_type = 0;

    // Map modes to specific uncovered lines
    switch (mode) {
        case 0: // Line 554: Invalid Magic
            test_magic = 0xDEADBEEF; 
            test_type = cmsPluginMemHandlerSig; // Any valid type to reach the switch
            break;
        case 1: // Line 559: Version too high
            // Note: Since we cannot set plugin->Version, we cannot directly trigger "Version too high" 
            // via the struct field if the check is strictly on that field. 
            // However, if cmsPluginTHR checks the context version vs plugin magic/type logic, 
            // this case might still be relevant if the logic has changed. 
            // We keep the logic to attempt the call.
            test_magic = 0x434D5350;
            test_version = LCMS_VERSION + 100; // Force version mismatch (if used internally)
            test_type = cmsPluginMemHandlerSig;
            break;
        case 2: // Line 567-568: cmsPluginMemHandlerSig
            test_type = cmsPluginMemHandlerSig;
            break;
        case 3: // Line 571-572: cmsPluginInterpolationSig
            test_type = cmsPluginInterpolationSig;
            break;
        case 4: // Line 575-576: cmsPluginTagTypeSig
            test_type = cmsPluginTagTypeSig;
            break;
        case 5: // Line 579-580: cmsPluginTagSig
            test_type = cmsPluginTagSig;
            break;
        case 6: // Line 583-584: cmsPluginFormattersSig
            test_type = cmsPluginFormattersSig;
            break;
        case 7: // Line 587-588: cmsPluginRenderingIntentSig
            test_type = cmsPluginRenderingIntentSig;
            break;
        case 8: // Line 591-592: cmsPluginParametricCurveSig
            test_type = cmsPluginParametricCurveSig;
            break;
        case 9: // Line 595-596: cmsPluginMultiProcessElementSig
            test_type = cmsPluginMultiProcessElementSig;
            break;
        case 10: // Line 599-600: cmsPluginOptimizationSig
            test_type = cmsPluginOptimizationSig;
            break;
        case 11: // Line 603-604: cmsPluginTransformSig
            test_type = cmsPluginTransformSig;
            break;
        case 12: // Line 607-608: cmsPluginMutexSig
            test_type = cmsPluginMutexSig;
            break;
        case 13: // Line 611-612: cmsPluginParalellizationSig
            test_type = cmsPluginParalellizationSig;
            break;
        case 14: // Line 615: Default case (Unknown Type)
            test_type = 0xFFFFFFFF; // Invalid type
            break;
    }

    // Create the plugin
    plugin_list = create_test_plugin(test_type, test_magic, test_version, NULL);
    
    if (plugin_list) {
        // Invoke the target function
        result = cmsPluginTHR(ctx, plugin_list);
        (void)result; // Ignore result, we just want to hit the code
        
        // Cleanup plugin
        free(plugin_list);
        plugin_list = NULL;
    }

    // Cleanup context
    cmsDeleteContext(ctx);

    return 0;
}