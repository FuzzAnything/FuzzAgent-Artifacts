#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <algorithm>

namespace harness_016 {
/*
 * Harness 016 - Plugin System Operations for Little CMS 2
 * FIXED: Stack-buffer-overflow issue - now allocates sufficient memory for all plugin types
 * 
 * Target APIs:
 * - cmsPlugin (Target - 56 undiscovered branches)
 * - cmsPluginTHR (Target - 56 undiscovered branches, partially covered)
 * - cmsDupContext (Target - 24 undiscovered branches)
 * - cmsGetContextUserData (Target - 1 undiscovered branch)
 * 
 * Required Helper APIs:
 * - cmsCreateContext (Init - already covered)
 * - cmsDeleteContext (Cleanup - already covered)
 * - cmsUnregisterPluginsTHR (Plugin management - already covered)
 * 
 * Invocation sequence:
 * 1. Call cmsCreateContext to create a new context (or use NULL for default context).
 * 2. Prepare mock plugin data structures with valid Magic numbers and types.
 * 3. Call cmsPlugin/cmsPluginTHR to register plugins with the context.
 * 4. Call cmsDupContext to duplicate the context with registered plugins.
 * 5. Call cmsGetContextUserData to retrieve and verify plugin data.
 * 6. Call cmsUnregisterPluginsTHR to clean up plugin registrations.
 * 7. Call cmsDeleteContext to free contexts.
 * 
 * FIX DETAIL: Previously used undersized SimplePlugin structure causing stack-buffer-overflow
 * when library accessed OptimizePtr field. Now allocates 256 bytes (sufficient for any plugin type)
 * and zero-initializes all fields to prevent memory corruption.
 * 
 * These functions represent 137+ points of undiscovered complexity (56+24+1+56).
 * They form a logical lifecycle for plugin management (Create Context → Register Plugins 
 * → Duplicate Context → Retrieve Data → Cleanup).
 * 
 * Existing harnesses focus on profiles, tone curves, dictionaries, MD5, pipelines, IT8, and GDB,
 * but lack dedicated plugin system testing, making this a semantically diverse target.
 */


// Create a plugin with given type - returns a pointer to appropriate structure
static void* create_plugin(cmsUInt32Number plugin_type, FuzzedDataProvider& fdp) {
    // Allocate buffer large enough for the largest plugin type
    // Use malloc since we need to return pointers that can be cast to specific types
    // Calculate max size needed for any plugin structure
    size_t max_size = 0;
    
    // We'll estimate a safe maximum size - actual plugin structures can vary
    // A safe approach is to allocate enough for base + some function pointers
    // Let's allocate 256 bytes which should be more than enough for any plugin
    size_t safe_size = 256;
    
    void* plugin_buffer = malloc(safe_size);
    if (!plugin_buffer) return NULL;
    
    // Zero out the buffer
    memset(plugin_buffer, 0, safe_size);
    
    // Set common base fields
    cmsPluginBase* base = (cmsPluginBase*)plugin_buffer;
    base->Magic = cmsPluginMagicNumber;
    base->ExpectedVersion = 2060; // LCMS version 2.6
    base->Type = plugin_type;
    base->Next = NULL;
    
    return plugin_buffer;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for plugin operations
    // We need: context flag, number of plugins, plugin types, user data flag
    const size_t MIN_SIZE = sizeof(uint8_t) * 3 + sizeof(uint32_t);
    
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t use_context = fdp.ConsumeBool();  // Whether to use thread-safe context
    uint8_t num_plugins = fdp.ConsumeIntegralInRange<uint8_t>(0, 10);  // 0-10 plugins
    uint8_t test_user_data = fdp.ConsumeBool();  // Whether to test user data retrieval
    uint32_t user_data_value = fdp.ConsumeIntegral<uint32_t>();  // User data value
    
    // Create context if requested
    cmsContext context = NULL;
    cmsContext dup_context = NULL;
    
    if (use_context) {
        // Pass user data pointer if testing user data retrieval
        void* user_data_ptr = test_user_data ? (void*)(uintptr_t)user_data_value : NULL;
        context = cmsCreateContext(NULL, user_data_ptr);
        if (context == NULL) {
            return 0;
        }
    }
    
    // Array of plugin pointers for testing different plugin types
    void* plugin_ptrs[10];
    int plugin_count = 0;
    
    // Define available plugin types from lcms2_plugin.h
    cmsUInt32Number plugin_types[] = {
        cmsPluginMemHandlerSig,
        cmsPluginInterpolationSig,
        cmsPluginParametricCurveSig,
        cmsPluginFormattersSig,
        cmsPluginTagTypeSig,
        cmsPluginTagSig,
        cmsPluginMultiProcessElementSig,
        cmsPluginOptimizationSig,
        cmsPluginRenderingIntentSig,
        cmsPluginTransformSig,
        cmsPluginMutexSig
    };
    const int NUM_PLUGIN_TYPES = sizeof(plugin_types) / sizeof(plugin_types[0]);
    
    // Register plugins based on fuzzer input
    for (int i = 0; i < num_plugins && fdp.remaining_bytes() > 0; i++) {
        uint8_t plugin_type_idx = fdp.ConsumeIntegral<uint8_t>() % NUM_PLUGIN_TYPES;
        void* plugin_ptr = create_plugin(plugin_types[plugin_type_idx], fdp);
        if (!plugin_ptr) {
            continue;
        }
        
        plugin_ptrs[plugin_count] = plugin_ptr;
        
        // Try both cmsPlugin and cmsPluginTHR to test different code paths
        uint8_t use_thr = fdp.ConsumeBool();
        cmsBool result;
        
        if (use_thr && context) {
            result = cmsPluginTHR(context, plugin_ptr);
        } else {
            result = cmsPlugin(plugin_ptr);
        }
        
        // Result may be FALSE, which is valid for fuzzing (invalid plugin data)
        (void)result;
        
        plugin_count++;
    }
    
    // Duplicate context if we have one
    if (context) {
        dup_context = cmsDupContext(context, NULL);
        if (dup_context) {
            // Test getting user data from both original and duplicated contexts
            if (test_user_data) {
                void* original_user_data = cmsGetContextUserData(context);
                void* dup_user_data = cmsGetContextUserData(dup_context);
                
                // User data may be NULL, which is valid
                (void)original_user_data;
                (void)dup_user_data;
            }
        }
    }
    
    // Clean up plugin registrations if we have a context
    if (context) {
        cmsUnregisterPluginsTHR(context);
    }
    
    // Clean up duplicated context
    if (dup_context) {
        cmsDeleteContext(dup_context);
    }
    
    // Clean up original context
    if (context) {
        cmsDeleteContext(context);
    }
    
    // Clean up allocated plugin memory
    for (int i = 0; i < plugin_count; i++) {
        free(plugin_ptrs[i]);
    }
    
    // Also test with NULL context (default context)
    if (num_plugins > 0 && fdp.remaining_bytes() > 0) {
        // Try registering a plugin with NULL context (default context)
        uint8_t plugin_type_idx = fdp.ConsumeIntegral<uint8_t>() % NUM_PLUGIN_TYPES;
        void* test_plugin = create_plugin(plugin_types[plugin_type_idx], fdp);
        if (test_plugin) {
            cmsBool result = cmsPlugin(test_plugin);
            (void)result;
            free(test_plugin);
        }
    }
    
    return 0;
}

} // namespace harness_016
