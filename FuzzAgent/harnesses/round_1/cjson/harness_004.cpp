#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "cjson/cJSON.h"

namespace harness_004 {

extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful raw JSON testing: 
    // 1 byte for operation type + 1 byte for string length + some data
    if (size < 8) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation type to determine testing strategy
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume number of raw items to create (1-10)
    int num_raw_items = fdp.ConsumeIntegralInRange<int>(1, 10);
    
    // Create parent object
    cJSON* parent_obj = cJSON_CreateObject();
    if (parent_obj == nullptr) {
        return 0;
    }
    
    // Test different raw JSON creation and addition strategies
    switch (operation) {
        case 0: {
            // Test cJSON_CreateRaw with separate creation and addition
            for (int i = 0; i < num_raw_items && fdp.remaining_bytes() > 0; i++) {
                // Consume key name for the raw item
                std::string key_name = fdp.ConsumeRandomLengthString(64);
                
                // Consume raw JSON string (could be valid or invalid JSON)
                std::string raw_json = fdp.ConsumeRandomLengthString(512);
                
                // Create raw JSON item
                cJSON* raw_item = cJSON_CreateRaw(raw_json.c_str());
                if (raw_item != nullptr) {
                    // Add raw item to parent object
                    cJSON_AddItemToObject(parent_obj, key_name.c_str(), raw_item);
                    
                    // Verify the item is raw
                    (void)cJSON_IsRaw(raw_item);
                }
            }
            break;
        }
        
        case 1: {
            // Test cJSON_AddRawToObject directly
            for (int i = 0; i < num_raw_items && fdp.remaining_bytes() > 0; i++) {
                // Consume key name
                std::string key_name = fdp.ConsumeRandomLengthString(64);
                
                // Consume raw JSON string
                std::string raw_json = fdp.ConsumeRandomLengthString(512);
                
                // Add raw JSON directly to object
                cJSON* raw_item = cJSON_AddRawToObject(parent_obj, key_name.c_str(), raw_json.c_str());
                
                // Verify if addition succeeded
                if (raw_item != nullptr) {
                    (void)cJSON_IsRaw(raw_item);
                }
            }
            break;
        }
        
        case 2: {
            // Test mixing raw JSON items with other item types
            for (int i = 0; i < num_raw_items && fdp.remaining_bytes() > 0; i++) {
                std::string key_name = fdp.ConsumeRandomLengthString(64);
                
                // Decide whether to create raw or other type based on input
                uint8_t item_type = fdp.ConsumeIntegral<uint8_t>() % 3;
                
                if (item_type == 0) {
                    // Create raw JSON item
                    std::string raw_json = fdp.ConsumeRandomLengthString(512);
                    cJSON* raw_item = cJSON_CreateRaw(raw_json.c_str());
                    if (raw_item != nullptr) {
                        cJSON_AddItemToObject(parent_obj, key_name.c_str(), raw_item);
                    }
                } else if (item_type == 1) {
                    // Create string item for comparison
                    std::string string_val = fdp.ConsumeRandomLengthString(256);
                    cJSON* string_item = cJSON_CreateString(string_val.c_str());
                    if (string_item != nullptr) {
                        cJSON_AddItemToObject(parent_obj, key_name.c_str(), string_item);
                    }
                } else {
                    // Create number item
                    double num_val = fdp.ConsumeFloatingPoint<double>();
                    cJSON* num_item = cJSON_CreateNumber(num_val);
                    if (num_item != nullptr) {
                        cJSON_AddItemToObject(parent_obj, key_name.c_str(), num_item);
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Test raw JSON with special characters and edge cases
            for (int i = 0; i < num_raw_items && fdp.remaining_bytes() > 0; i++) {
                std::string key_name = fdp.ConsumeRandomLengthString(32);
                
                // Consume raw JSON with limited length for edge cases
                size_t raw_length = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                std::string raw_json = fdp.ConsumeBytesAsString(raw_length);
                
                // Test with empty string
                if (i == 0 && raw_json.empty()) {
                    cJSON* empty_raw = cJSON_CreateRaw("");
                    if (empty_raw != nullptr) {
                        cJSON_AddItemToObject(parent_obj, key_name.c_str(), empty_raw);
                    }
                } else {
                    cJSON* raw_item = cJSON_CreateRaw(raw_json.c_str());
                    if (raw_item != nullptr) {
                        cJSON_AddItemToObject(parent_obj, key_name.c_str(), raw_item);
                    }
                }
            }
            break;
        }
    }
    
    // Optionally print the object to verify raw JSON is preserved
    if (fdp.remaining_bytes() > 0) {
        bool should_print = fdp.ConsumeBool();
        if (should_print) {
            char* printed_formatted = cJSON_Print(parent_obj);
            if (printed_formatted != nullptr) {
                free(printed_formatted);
            }
            
            char* printed_unformatted = cJSON_PrintUnformatted(parent_obj);
            if (printed_unformatted != nullptr) {
                free(printed_unformatted);
            }
        }
    }
    
    // Cleanup
    cJSON_Delete(parent_obj);
    
    return 0;
}

} // namespace harness_004
