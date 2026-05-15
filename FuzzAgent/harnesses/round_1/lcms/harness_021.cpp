#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

namespace harness_021 {
// Fuzzing harness for Little CMS 2 library
// Target: Context duplication and user data management operations
// Strategy: Implement context lifecycle management focusing on duplication operations
//           that are currently 0% covered (24 undiscovered branches in cmsDupContext)
// Target APIs:
//   - cmsDupContext (Primary Target - 24 undiscovered branches)
//   - cmsGetContextUserData (Secondary Target - 1 undiscovered branch)
// Required Helper APIs:
//   - cmsCreateContext (Required Helper: Initialization)
//   - cmsDeleteContext (Required Helper: Cleanup)
// Invocation sequence:
//   1. Call cmsCreateContext to create an initial context with optional plugin structure
//   2. Call cmsDupContext to duplicate the context, testing both with and without NewUserData parameter
//   3. Call cmsGetContextUserData to retrieve user data from duplicated context
//   4. Call cmsDeleteContext to clean up both original and duplicated contexts
// Error handling scenarios included:
//   - Testing with NULL plugin structures
//   - Testing with valid user data
//   - Testing duplicate context retrieval with NULL user data
//   - Proper resource cleanup in all code paths
// Note: Uses FuzzedDataProvider for input-driven testing of all parameters
// Semantic diversity from harnesses 000-020: This harness focuses exclusively on context
// management operations, which are not covered in any previous harnesses (search confirms
// no existing harness uses cmsDupContext or cmsGetContextUserData). Previous harnesses
// target profile operations, color transforms, deltaE calculations, IT8 operations, etc.


// Simple user data structure for testing context user data functionality
typedef struct {
    int id;
    char name[32];
    float value;
    bool active;
} TestUserData;

// Static wrapper functions with C linkage for memory handler plugin
extern "C" {
    static void* malloc_wrapper(cmsContext ContextID, cmsUInt32Number size) {
        if (size == 0 || size > 1024 * 1024 * 1024) return NULL; // Limit to 1GB for safety
        return malloc(size);
    }
    
    static void free_wrapper(cmsContext ContextID, void *Ptr) {
        if (Ptr) free(Ptr);
    }
    
    static void* realloc_wrapper(cmsContext ContextID, void* Ptr, cmsUInt32Number NewSize) {
        if (NewSize > 1024 * 1024 * 1024) return NULL; // Limit to 1GB for safety
        return realloc(Ptr, NewSize);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for operation selection and user data
    // We need: operation type + user data fields + plugin decision
    const size_t MIN_SIZE = sizeof(int) + sizeof(float) + 1; // id + value + 1 byte for flags
    if (size < MIN_SIZE) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Determine operation mode from fuzzer input
    // 0: Create context with NULL plugin, test duplication without new user data
    // 1: Create context with NULL plugin, test duplication with new user data  
    // 2: Create context with simple memory handler plugin, test duplication
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Step 2: Prepare user data from fuzzer input
    TestUserData user_data;
    user_data.id = fdp.ConsumeIntegral<int>();
    user_data.value = fdp.ConsumeFloatingPoint<float>();
    user_data.active = fdp.ConsumeBool();
    
    // Consume a string for name (limited to 31 chars + null terminator)
    std::string name_str = fdp.ConsumeRandomLengthString(31);
    strncpy(user_data.name, name_str.c_str(), sizeof(user_data.name) - 1);
    user_data.name[sizeof(user_data.name) - 1] = '\0';
    
    // Step 3: Create initial context based on operation mode
    cmsContext original_ctx = NULL;
    cmsPluginMemHandler* mem_handler_plugin = NULL;
    
    if (operation_mode == 2) {
        // Create a simple memory handler plugin for testing with valid function pointers
        // We need to provide at least the required function pointers (MallocPtr, FreePtr, ReallocPtr)
        mem_handler_plugin = (cmsPluginMemHandler*)malloc(sizeof(cmsPluginMemHandler));
        if (mem_handler_plugin) {
            memset(mem_handler_plugin, 0, sizeof(cmsPluginMemHandler));
            mem_handler_plugin->base.Magic = cmsPluginMagicNumber;
            mem_handler_plugin->base.ExpectedVersion = LCMS_VERSION;
            mem_handler_plugin->base.Type = cmsPluginMemHandlerSig;
            mem_handler_plugin->base.Next = NULL;
            
            // Provide valid function pointers for required callbacks
            // Use the static wrapper functions with C linkage
            mem_handler_plugin->MallocPtr = malloc_wrapper;
            mem_handler_plugin->FreePtr = free_wrapper;
            mem_handler_plugin->ReallocPtr = realloc_wrapper;
            
            // Optional function pointers can remain NULL (will use defaults)
        }
    }
    
    // Create the context with optional plugin and user data
    original_ctx = cmsCreateContext(mem_handler_plugin, &user_data);
    
    // Clean up the plugin structure if we created it
    if (mem_handler_plugin) {
        free(mem_handler_plugin);
        mem_handler_plugin = NULL;
    }
    
    // If context creation failed, we can't proceed
    if (original_ctx == NULL) {
        return 0;
    }
    // Step 4: Test cmsGetContextUserData on original context
    void* retrieved_data = cmsGetContextUserData(original_ctx);
    if (retrieved_data != NULL) {
        // Verify it's our user data structure (optional check)
        TestUserData* retrieved_user_data = (TestUserData*)retrieved_data;
        // We could compare fields, but just accessing is enough for coverage
        (void)retrieved_user_data->id;
        (void)retrieved_user_data->value;
        (void)retrieved_user_data->active;
    }
    // Step 5: Test cmsDupContext with different parameters based on operation mode
    cmsContext duplicated_ctx = NULL;
    
    if (operation_mode == 0) {
        // Test duplication WITHOUT new user data (NULL parameter)
        duplicated_ctx = cmsDupContext(original_ctx, NULL);
    } else {
        // Test duplication WITH new user data
        // Create a different user data structure for the duplicated context
        TestUserData new_user_data;
        new_user_data.id = fdp.ConsumeIntegral<int>();
        new_user_data.value = fdp.ConsumeFloatingPoint<float>();
        new_user_data.active = fdp.ConsumeBool();
        
        std::string new_name_str = fdp.ConsumeRandomLengthString(31);
        strncpy(new_user_data.name, new_name_str.c_str(), sizeof(new_user_data.name) - 1);
        new_user_data.name[sizeof(new_user_data.name) - 1] = '\0';
        
        duplicated_ctx = cmsDupContext(original_ctx, &new_user_data);
        
        // If duplication succeeded, test retrieving the new user data
        if (duplicated_ctx != NULL) {
            void* new_retrieved_data = cmsGetContextUserData(duplicated_ctx);
            if (new_retrieved_data != NULL) {
                TestUserData* new_retrieved_user_data = (TestUserData*)new_retrieved_data;
                (void)new_retrieved_user_data->id;
                (void)new_retrieved_user_data->value;
                (void)new_retrieved_user_data->active;
            }
        }
    }
    
    // Step 6: Additional edge case testing if we have both contexts
    if (original_ctx != NULL && duplicated_ctx != NULL) {
        // Test that contexts are different (they should be)
        if (original_ctx != duplicated_ctx) {
            // Get user data from both to ensure they work independently
            void* orig_data = cmsGetContextUserData(original_ctx);
            void* dup_data = cmsGetContextUserData(duplicated_ctx);
            
            // Access the data (optional, for coverage)
            (void)orig_data;
            (void)dup_data;
        }
        
        // Test creating a second-level duplication (context of duplicated context)
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
            cmsContext second_dup_ctx = cmsDupContext(duplicated_ctx, NULL);
            if (second_dup_ctx != NULL) {
                // Get user data from second-level duplication
                void* second_dup_data = cmsGetContextUserData(second_dup_ctx);
                (void)second_dup_data;
                
                // Clean up second-level duplication
                cmsDeleteContext(second_dup_ctx);
            }
        }
    }
    
    // Step 7: Clean up contexts in reverse order of creation
    // Duplicated context should be deleted first if it exists
    if (duplicated_ctx != NULL) {
        cmsDeleteContext(duplicated_ctx);
    }
    
    // Original context should be deleted last
    if (original_ctx != NULL) {
        cmsDeleteContext(original_ctx);
    }
    
    // Step 8: Test edge cases with invalid/null contexts (if we have enough input left)
    if (fdp.remaining_bytes() > 0) {
        // Try to get user data from NULL context (should handle gracefully)
        // Note: According to LittleCMS docs, this should return NULL
        void* null_data = cmsGetContextUserData(NULL);
        (void)null_data;
        
        // Try to duplicate NULL context (should return NULL according to docs)
        cmsContext null_dup = cmsDupContext(NULL, NULL);
        if (null_dup != NULL) {
            // Shouldn't happen, but clean up if it does
            cmsDeleteContext(null_dup);
        }
        
        // Try to delete NULL context (should be safe)
        cmsDeleteContext(NULL);
    }
    
    return 0;
}

} // namespace harness_021
