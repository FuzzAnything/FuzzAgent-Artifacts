#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

namespace harness_012 {
// Fuzzing harness for LittleCMS (lcms) plugin management system
// This harness specifically targets plugin architecture lifecycle functions
// Target APIs: cmsPlugin [0% covered, 58 branches], cmsDupContext [0% covered, 14 branches], cmsPluginTHR [thread-safe variant, 56 branches]
// MODULE: cmsplugin.c and related plugin management code
// STRATEGY: Target plugin architecture lifecycle for library extensibility
// DIFFERENTIATION: Unlike existing harnesses, focus on plugin system for custom memory management, tag types, formatters, and optimization routines
// API SEQUENCE: Create context with NULL plugin bundle -> Create context with custom plugin bundle -> Configure with various plugin types -> Duplicate context -> Delete context


// Dummy function pointers for plugin testing (we don't need actual implementations)
static cmsInterpFunction dummyInterpFactory(cmsUInt32Number nInputChannels, cmsUInt32Number nOutputChannels, cmsUInt32Number dwFlags) {
    cmsInterpFunction result;
    memset(&result, 0, sizeof(result));
    return result;
}

// Dummy memory management functions that just call standard C library functions
static void* dummyMalloc(cmsContext ContextID, cmsUInt32Number size) {
    (void)ContextID;
    return malloc(size);
}

static void dummyFree(cmsContext ContextID, void *Ptr) {
    (void)ContextID;
    free(Ptr);
}

static void* dummyRealloc(cmsContext ContextID, void* Ptr, cmsUInt32Number NewSize) {
    (void)ContextID;
    return realloc(Ptr, NewSize);
}

static void* dummyMallocZero(cmsContext ContextID, cmsUInt32Number size) {
    (void)ContextID;
    void* ptr = malloc(size);
    if (ptr != NULL) {
        memset(ptr, 0, size);
    }
    return ptr;
}

static void* dummyCalloc(cmsContext ContextID, cmsUInt32Number num, cmsUInt32Number size) {
    (void)ContextID;
    return calloc(num, size);
}

static void* dummyDup(cmsContext ContextID, const void* Org, cmsUInt32Number size) {
    (void)ContextID;
    void* ptr = malloc(size);
    if (ptr != NULL && Org != NULL) {
        memcpy(ptr, Org, size);
    }
    return ptr;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for meaningful testing of plugin system
    // Minimum: plugin type selection, plugin parameters, and operations
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Create context with NULL plugin bundle (basic setup)
    cmsContext ctx1 = cmsCreateContext(NULL, NULL);
    if (ctx1 == NULL) {
        return 0; // Cannot proceed without context
    }
    
    // 2. Test cmsDupContext on the first context
    cmsContext ctx2 = cmsDupContext(ctx1, NULL);
    if (ctx2 == NULL) {
        cmsDeleteContext(ctx1);
        return 0; // Cannot duplicate context
    }
    
    // 3. Create simple plugin structures to test plugin registration
    // We'll create different types of plugins based on fuzzer input
    
    // Determine which plugin types to test
    uint8_t plugin_mask = fdp.ConsumeIntegral<uint8_t>();
    
    // Test memory handler plugin (if bit 0 is set)
    if ((plugin_mask & 0x01) && fdp.remaining_bytes() >= 16) {
        // Create a simple memory handler plugin structure
        // Based on DebugMemHandler from testcms2.c
        static cmsPluginMemHandler mem_handler = {
            { cmsPluginMagicNumber, 2060, cmsPluginMemHandlerSig, NULL },
            dummyMalloc, // MallocPtr - use dummy implementation
            dummyFree,   // FreePtr
            dummyRealloc, // ReallocPtr
            dummyMallocZero, // MallocZeroPtr
            dummyCalloc, // CallocPtr
            dummyDup     // DupPtr
        };
        
        // Register the plugin using thread-safe variant
        cmsBool result = cmsPluginTHR(ctx1, &mem_handler);
        (void)result; // Ignore result, just testing the API call
    }
    
    // Test interpolation plugin (if bit 1 is set)
    if ((plugin_mask & 0x02) && fdp.remaining_bytes() >= 16) {
        // Create a simple interpolation plugin
        static cmsPluginInterpolation interp_plugin = {
            { cmsPluginMagicNumber, 2060, cmsPluginInterpolationSig, NULL },
            NULL // InterpolatorsFactory - leave as NULL for testing
        };
        
        cmsBool result = cmsPluginTHR(ctx2, &interp_plugin);
        (void)result;
    }
    
    // Test formatters plugin (if bit 2 is set)
    if ((plugin_mask & 0x04) && fdp.remaining_bytes() >= 16) {
        // Create a simple formatters plugin
        static cmsPluginFormatters fmt_plugin = {
            { cmsPluginMagicNumber, 2060, cmsPluginFormattersSig, NULL },
            NULL, // FormattersFactory - leave as NULL for testing
        };
        
        cmsBool result = cmsPluginTHR(ctx1, &fmt_plugin);
        (void)result;
    }
    
    // Test tag type plugin (if bit 3 is set)
    if ((plugin_mask & 0x08) && fdp.remaining_bytes() >= 16) {
        // Create a simple tag type plugin
        static cmsPluginTagType tagtype_plugin = {
            { cmsPluginMagicNumber, 2060, cmsPluginTagTypeSig, NULL },
            (cmsTagTypeSignature)0, // TypeSignature - use 0 for testing
            NULL, // DeserializePtr
            NULL, // SerializePtr
            NULL, // DupPtr
            NULL, // FreePtr
            NULL  // SamplePtr
        };
        
        cmsBool result = cmsPluginTHR(ctx2, &tagtype_plugin);
        (void)result;
    }
    
    // 4. Create another context with a plugin bundle (if we have enough data)
    if (fdp.remaining_bytes() >= 32) {
        // Create a proper memory handler plugin structure with valid function pointers
        static cmsPluginMemHandler mem_handler_plugin = {
            { cmsPluginMagicNumber, 2060, cmsPluginMemHandlerSig, NULL },
            dummyMalloc,   // MallocPtr - required, must be non-NULL
            dummyFree,     // FreePtr - required, must be non-NULL
            dummyRealloc,  // ReallocPtr - required, must be non-NULL
            dummyMallocZero, // MallocZeroPtr
            dummyCalloc,   // CallocPtr
            dummyDup       // DupPtr
        };
        
        // Create context with the plugin bundle
        cmsContext ctx3 = cmsCreateContext((cmsPluginBase*)&mem_handler_plugin, NULL);
        if (ctx3 != NULL) {
            // Test plugin registration on this context too
            if (fdp.remaining_bytes() >= 16) {
                static cmsPluginInterpolation interp_plugin2 = {
                    { cmsPluginMagicNumber, 2060, cmsPluginInterpolationSig, NULL },
                    NULL // InterpolatorsFactory
                };
                
                // Use thread-safe variant for context-specific registration
                cmsBool result = cmsPluginTHR(ctx3, &interp_plugin2);
                (void)result;
            }
            
            // Duplicate this context as well
            cmsContext ctx4 = cmsDupContext(ctx3, NULL);
            if (ctx4 != NULL) {
                cmsDeleteContext(ctx4);
            }
            
            cmsDeleteContext(ctx3);
        }
    }
    
    // 5. Test global plugin registration (non-thread-safe) if we have data
    if (fdp.remaining_bytes() >= 16) {
        static cmsPluginOptimization opt_plugin = {
            { cmsPluginMagicNumber, 2060, cmsPluginOptimizationSig, NULL },
            NULL, // OptimizationPtr - leave as NULL for testing
        };
        
        cmsBool result = cmsPlugin(&opt_plugin);
        (void)result;
    }
    
    // 6. Test more complex plugin chains (if we have enough data)
    if (fdp.remaining_bytes() >= 48) {
        // Create a chain of two plugins
        static cmsPluginInterpolation interp_plugin_chain = {
            { cmsPluginMagicNumber, 2060, cmsPluginInterpolationSig, NULL },
            NULL // InterpolatorsFactory
        };
        
        static cmsPluginParametricCurves curve_plugin = {
            { cmsPluginMagicNumber, 2060, cmsPluginParametricCurveSig, (cmsPluginBase*)&interp_plugin_chain },
            0, // nFunctions - 0 means no functions
            {0}, // FunctionTypes
            {0}, // ParameterCount
            NULL // Evaluator
        };
        
        // Register the chain (curve plugin links to interpolation plugin)
        cmsBool result = cmsPluginTHR(ctx1, &curve_plugin);
        (void)result;
    }
    
    // 7. Test context-specific user data (if we have data)
    if (fdp.remaining_bytes() >= 8) {
        int user_data = fdp.ConsumeIntegral<int>();
        int new_user_data = fdp.ConsumeIntegral<int>(); // Use fuzzer input instead of user_data + 1
        cmsContext ctx_with_data = cmsCreateContext(NULL, &user_data);
        if (ctx_with_data != NULL) {
            // Duplicate context with different user data from fuzzer input
            cmsContext ctx_dup_with_data = cmsDupContext(ctx_with_data, &new_user_data);
            if (ctx_dup_with_data != NULL) {
                cmsDeleteContext(ctx_dup_with_data);
            }
            cmsDeleteContext(ctx_with_data);
        }
    }
    
    // 8. Test plugin registration on duplicated contexts
    if (fdp.remaining_bytes() >= 16) {
        static cmsPluginMutex mutex_plugin = {
            { cmsPluginMagicNumber, 2060, cmsPluginMutexSig, NULL },
            NULL, // CreateMutexPtr
            NULL, // DestroyMutexPtr
            NULL, // LockMutexPtr
            NULL  // UnlockMutexPtr
        };
        
        // Register on duplicated context
        cmsBool result = cmsPluginTHR(ctx2, &mutex_plugin);
        (void)result;
    }
    
    // 9. Cleanup - delete contexts
    cmsDeleteContext(ctx2);
    cmsDeleteContext(ctx1);
    
    return 0;
}

} // namespace harness_012
