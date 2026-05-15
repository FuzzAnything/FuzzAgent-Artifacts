#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "cjson/cJSON.h"

namespace harness_018 {

extern "C" {
}

// Custom allocator functions for testing memory hooks
static void* custom_malloc(size_t size) {
    return malloc(size);
}

static void custom_free(void* ptr) {
    free(ptr);
}

static void* custom_realloc(void* ptr, size_t size) {
    return realloc(ptr, size);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 8) {
        return 0;  // Minimum size for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume test scenario selection
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (scenario) {
        case 0: {
            // Test 1: Memory hook functions with custom allocators
            // Set up custom hooks
            cJSON_Hooks hooks;
            hooks.malloc_fn = custom_malloc;
            hooks.free_fn = custom_free;
            
            // Initialize hooks with custom allocators
            cJSON_InitHooks(&hooks);
            
            // Test cJSON_malloc and cJSON_free with various sizes
            size_t alloc_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            void* mem_block = cJSON_malloc(alloc_size);
            if (mem_block != nullptr) {
                // Write some test data
                memset(mem_block, 0xAA, alloc_size);
                cJSON_free(mem_block);
            }
            
            // Test with zero size (edge case)
            void* zero_block = cJSON_malloc(0);
            if (zero_block != nullptr) {
                cJSON_free(zero_block);
            }
            
            // Reset hooks to default
            cJSON_InitHooks(nullptr);
            break;
        }
        
        case 1: {
            // Test 2: Type checking functions with NULL inputs (edge cases)
            // All type checking functions should handle NULL gracefully
            (void)cJSON_IsInvalid(nullptr);
            (void)cJSON_IsFalse(nullptr);
            (void)cJSON_IsTrue(nullptr);
            (void)cJSON_IsBool(nullptr);
            (void)cJSON_IsNull(nullptr);
            (void)cJSON_IsNumber(nullptr);
            (void)cJSON_IsString(nullptr);
            (void)cJSON_IsArray(nullptr);
            (void)cJSON_IsObject(nullptr);
            (void)cJSON_IsRaw(nullptr);
            
            // Also test with various valid object types
            // Consume object type to create
            uint8_t obj_type = fdp.ConsumeIntegral<uint8_t>() % 10;
            cJSON* test_obj = nullptr;
            
            switch (obj_type) {
                case 0: test_obj = cJSON_CreateNull(); break;
                case 1: test_obj = cJSON_CreateTrue(); break;
                case 2: test_obj = cJSON_CreateFalse(); break;
                case 3: {
                    double num_val = fdp.ConsumeFloatingPoint<double>();
                    test_obj = cJSON_CreateNumber(num_val);
                    break;
                }
                case 4: {
                    std::string str_val = fdp.ConsumeRandomLengthString(100);
                    test_obj = cJSON_CreateString(str_val.c_str());
                    break;
                }
                case 5: test_obj = cJSON_CreateArray(); break;
                case 6: test_obj = cJSON_CreateObject(); break;
                case 7: {
                    std::string raw_val = fdp.ConsumeRandomLengthString(100);
                    test_obj = cJSON_CreateRaw(raw_val.c_str());
                    break;
                }
                case 8: {
                    bool bool_val = fdp.ConsumeBool();
                    test_obj = cJSON_CreateBool(bool_val ? 1 : 0);
                    break;
                }
                case 9: {
                    // Create invalid item by creating then modifying type
                    test_obj = cJSON_CreateNull();
                    if (test_obj) {
                        test_obj->type = cJSON_Invalid;
                    }
                    break;
                }
            }
            
            if (test_obj != nullptr) {
                // Test all type checking functions on the valid object
                (void)cJSON_IsInvalid(test_obj);
                (void)cJSON_IsFalse(test_obj);
                (void)cJSON_IsTrue(test_obj);
                (void)cJSON_IsBool(test_obj);
                (void)cJSON_IsNull(test_obj);
                (void)cJSON_IsNumber(test_obj);
                (void)cJSON_IsString(test_obj);
                (void)cJSON_IsArray(test_obj);
                (void)cJSON_IsObject(test_obj);
                (void)cJSON_IsRaw(test_obj);
                
                cJSON_Delete(test_obj);
            }
            break;
        }
        
        case 2: {
            // Test 3: Memory hooks with partial customization (only malloc or only free)
            cJSON_Hooks partial_hooks;
            memset(&partial_hooks, 0, sizeof(partial_hooks));
            
            // Consume which hooks to set
            uint8_t hook_config = fdp.ConsumeIntegral<uint8_t>() % 4;
            
            if (hook_config == 0) {
                // Only malloc custom
                partial_hooks.malloc_fn = custom_malloc;
            } else if (hook_config == 1) {
                // Only free custom
                partial_hooks.free_fn = custom_free;
            } else if (hook_config == 2) {
                // Both custom
                partial_hooks.malloc_fn = custom_malloc;
                partial_hooks.free_fn = custom_free;
            }
            // hook_config == 3: no hooks set (all NULL)
            
            cJSON_InitHooks(&partial_hooks);
            
            // Perform some allocations to test the hooks
            size_t test_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
            void* test_mem = cJSON_malloc(test_size);
            if (test_mem != nullptr) {
                cJSON_free(test_mem);
            }
            
            // Reset to default
            cJSON_InitHooks(nullptr);
            break;
        }
        
        case 3: {
            // Test 4: Integrated test - parse JSON with custom hooks, then type check
            // Set custom hooks first
            cJSON_Hooks test_hooks;
            test_hooks.malloc_fn = custom_malloc;
            test_hooks.free_fn = custom_free;
            cJSON_InitHooks(&test_hooks);
            
            // Consume JSON input
            std::string json_input = fdp.ConsumeRemainingBytesAsString();
            
            // Try to parse with various methods
            uint8_t parse_method = fdp.ConsumeIntegral<uint8_t>() % 2;
            cJSON* parsed_obj = nullptr;
            
            if (parse_method == 0) {
                parsed_obj = cJSON_Parse(json_input.c_str());
            } else {
                parsed_obj = cJSON_ParseWithLength(json_input.c_str(), json_input.size());
            }
            
            if (parsed_obj != nullptr) {
                // Test type checking on parsed object
                (void)cJSON_IsInvalid(parsed_obj);
                (void)cJSON_IsFalse(parsed_obj);
                (void)cJSON_IsTrue(parsed_obj);
                (void)cJSON_IsBool(parsed_obj);
                (void)cJSON_IsNull(parsed_obj);
                (void)cJSON_IsNumber(parsed_obj);
                (void)cJSON_IsString(parsed_obj);
                (void)cJSON_IsArray(parsed_obj);
                (void)cJSON_IsObject(parsed_obj);
                (void)cJSON_IsRaw(parsed_obj);
                
                // Also test on child items if any
                cJSON* child = parsed_obj->child;
                while (child != nullptr) {
                    (void)cJSON_IsInvalid(child);
                    (void)cJSON_IsFalse(child);
                    (void)cJSON_IsTrue(child);
                    (void)cJSON_IsBool(child);
                    (void)cJSON_IsNull(child);
                    (void)cJSON_IsNumber(child);
                    (void)cJSON_IsString(child);
                    (void)cJSON_IsArray(child);
                    (void)cJSON_IsObject(child);
                    (void)cJSON_IsRaw(child);
                    
                    child = child->next;
                }
                
                cJSON_Delete(parsed_obj);
            }
            
            // Reset hooks
            cJSON_InitHooks(nullptr);
            break;
        }
    }
    
    return 0;
}

} // namespace harness_018
