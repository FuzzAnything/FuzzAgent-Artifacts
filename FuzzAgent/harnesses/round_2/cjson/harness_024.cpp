#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "cjson/cJSON.h"

namespace harness_024 {

extern "C" {
}

// Helper to create overlapping string scenarios with guaranteed proper null-termination
void create_overlapping_scenario(FuzzedDataProvider& fdp, 
                                 std::vector<char>& buffer,
                                 const char** source_str,
                                 const char** target_str,
                                 size_t& source_len,
                                 size_t& target_len) {
    // Create a buffer large enough for both strings with room for null terminators
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(256, 2048);
    buffer.resize(buffer_size);
    
    // Fill entire buffer with a pattern (helps detect buffer overreads)
    for (size_t i = 0; i < buffer_size; i++) {
        buffer[i] = static_cast<char>(fdp.ConsumeIntegral<uint8_t>());
    }
    
    // Determine string lengths (ensure room for null terminators)
    size_t max_len = (buffer_size / 3) - 1;  // Conservative maximum
    source_len = fdp.ConsumeIntegralInRange<size_t>(1, max_len);
    target_len = fdp.ConsumeIntegralInRange<size_t>(1, max_len);
    
    // Choose overlap scenario
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Create two properly null-terminated strings at safe locations first
    size_t safe_source_offset = 0;
    size_t safe_target_offset = buffer_size / 2;
    
    // Ensure safe_target_offset + target_len + 1 <= buffer_size
    if (safe_target_offset + target_len + 1 > buffer_size) {
        safe_target_offset = buffer_size - target_len - 1;
    }
    
    // Create source string at safe location
    for (size_t i = 0; i < source_len; i++) {
        buffer[safe_source_offset + i] = 'A' + (i % 26);
    }
    buffer[safe_source_offset + source_len] = '\0';
    
    // Create target string at safe location  
    for (size_t i = 0; i < target_len; i++) {
        buffer[safe_target_offset + i] = 'a' + (i % 26);
    }
    buffer[safe_target_offset + target_len] = '\0';
    
    // Now adjust pointers based on overlap scenario
    size_t source_offset = safe_source_offset;
    size_t target_offset = safe_target_offset;
    
    switch (scenario) {
        case 0: // No overlap - source before target (already set up)
            break;
            
        case 1: // No overlap - target before source
            source_offset = safe_target_offset;
            target_offset = safe_source_offset;
            break;
            
        case 2: // Partial overlap - source starts within target
            target_offset = safe_source_offset;
            source_offset = safe_source_offset + (target_len / 2);
            // Ensure source doesn't overflow buffer
            if (source_offset + source_len + 1 > buffer_size) {
                source_offset = buffer_size - source_len - 1;
            }
            break;
            
        case 3: // Partial overlap - target starts within source
            source_offset = safe_source_offset;
            target_offset = safe_source_offset + (source_len / 2);
            // Ensure target doesn't overflow buffer
            if (target_offset + target_len + 1 > buffer_size) {
                target_offset = buffer_size - target_len - 1;
            }
            break;
            
        case 4: // Complete overlap - same memory
            source_offset = safe_source_offset;
            target_offset = safe_source_offset;
            // For complete overlap, ensure both strings have same null terminator
            // Use the shorter length to avoid buffer overflow
            if (source_len > target_len) {
                source_len = target_len;
            } else {
                target_len = source_len;
            }
            break;
            
        case 5: // Adjacent memory - source ends where target begins
            source_offset = safe_source_offset;
            target_offset = safe_source_offset + source_len;
            // Ensure target doesn't overflow buffer
            if (target_offset + target_len + 1 > buffer_size) {
                target_offset = buffer_size - target_len - 1;
            }
            break;
    }
    
    // Final sanity check: ensure null terminators are in place
    if (source_offset + source_len < buffer_size) {
        buffer[source_offset + source_len] = '\0';
    } else {
        // Adjust source_len to fit
        source_len = buffer_size - source_offset - 1;
        if (source_len > 0) {
            buffer[source_offset + source_len] = '\0';
        } else {
            source_len = 1;
            buffer[source_offset] = '\0';
        }
    }
    
    if (target_offset + target_len < buffer_size) {
        buffer[target_offset + target_len] = '\0';
    } else {
        // Adjust target_len to fit
        target_len = buffer_size - target_offset - 1;
        if (target_len > 0) {
            buffer[target_offset + target_len] = '\0';
        } else {
            target_len = 1;
            buffer[target_offset] = '\0';
        }
    }
    
    // Set the string pointers
    *source_str = buffer.data() + source_offset;
    *target_str = buffer.data() + target_offset;
    
    // Verify strings are null-terminated (for debugging/safety)
    if ((*source_str)[source_len] != '\0') {
        // Emergency null termination
        const_cast<char*>(*source_str)[source_len] = '\0';
    }
    if ((*target_str)[target_len] != '\0') {
        // Emergency null termination
        const_cast<char*>(*target_str)[target_len] = '\0';
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Minimum input size for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Test 1: Basic cJSON_SetValuestring functionality with normal strings
    {
        std::string initial_str = fdp.ConsumeRandomLengthString(32);
        cJSON* string_item = cJSON_CreateString(initial_str.c_str());
        if (string_item != NULL) {
            std::string new_string = fdp.ConsumeRandomLengthString(128);
            char* result = cJSON_SetValuestring(string_item, new_string.c_str());
            // Result can be NULL if allocation fails, but we don't check here
            cJSON_Delete(string_item);
        }
    }
    
    // Test 2: Test with overlapping string scenarios
    if (fdp.remaining_bytes() > 100) {
        std::vector<char> buffer;
        const char* source_str = nullptr;
        const char* target_str = nullptr;
        size_t source_len = 0;
        size_t target_len = 0;
        
        create_overlapping_scenario(fdp, buffer, &source_str, &target_str, source_len, target_len);
        
        // Create a string item with target string
        cJSON* string_item = cJSON_CreateString(target_str);
        if (string_item != NULL && string_item->valuestring != NULL) {
            // Now try to set it with potentially overlapping source string
            char* result = cJSON_SetValuestring(string_item, source_str);
            // The function should handle overlapping strings properly
            // If strings overlap and source_len <= target_len, it should return NULL
            cJSON_Delete(string_item);
        }
    }
    
    // Test 3: Test edge cases and error conditions
    
    // Test with NULL object
    if (fdp.remaining_bytes() > 10) {
        std::string test_str = fdp.ConsumeRandomLengthString(16);
        char* result = cJSON_SetValuestring(NULL, test_str.c_str());
        // Should return NULL
    }
    
    // Test with non-string object
    if (fdp.remaining_bytes() > 10) {
        double number_value = fdp.ConsumeFloatingPoint<double>();
        cJSON* number_item = cJSON_CreateNumber(number_value);
        if (number_item != NULL) {
            std::string test_str = fdp.ConsumeRandomLengthString(16);
            char* result = cJSON_SetValuestring(number_item, test_str.c_str());
            // Should return NULL since it's not a string type
            cJSON_Delete(number_item);
        }
    }
    
    // Test with NULL valuestring
    if (fdp.remaining_bytes() > 10) {
        std::string initial_str = fdp.ConsumeRandomLengthString(32);
        cJSON* string_item = cJSON_CreateString(initial_str.c_str());
        if (string_item != NULL) {
            char* result = cJSON_SetValuestring(string_item, NULL);
            // Should return NULL
            cJSON_Delete(string_item);
        }
    }

    // Test 5: Test various string length combinations to hit different code paths
    if (fdp.remaining_bytes() > 200) {
        for (int i = 0; i < 5 && fdp.remaining_bytes() > 40; i++) {
            size_t initial_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
            size_t new_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
            
            // Create initial string using FDP to ensure proper null termination
            std::string initial_str = fdp.ConsumeRandomLengthString(initial_len);
            if (initial_str.empty()) {
                initial_str = "a";  // Ensure non-empty
            }
            
            // Create new string using FDP to ensure proper null termination
            std::string new_str = fdp.ConsumeRandomLengthString(new_len);
            if (new_str.empty()) {
                new_str = "b";  // Ensure non-empty
            }
            
            cJSON* string_item = cJSON_CreateString(initial_str.c_str());
            if (string_item != NULL) {
                char* result = cJSON_SetValuestring(string_item, new_str.c_str());
                // Test covers:
                // - new_len <= initial_len (strcpy path)
                // - new_len > initial_len (allocation path)
                cJSON_Delete(string_item);
            }
        }
    }
    
    // Test 6: Test with empty strings
    if (fdp.remaining_bytes() > 5) {
        cJSON* string_item = cJSON_CreateString("");
        if (string_item != NULL) {
            char* result = cJSON_SetValuestring(string_item, "");
            cJSON_Delete(string_item);
        }
    }
    
    // Test 7: Test with very long strings (potential allocation failures)
    if (fdp.remaining_bytes() > 500) {
        size_t short_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
        size_t long_len = fdp.ConsumeIntegralInRange<size_t>(1000, 10000);
        
        // Use FDP to create strings with guaranteed null termination
        std::string short_str = fdp.ConsumeRandomLengthString(short_len);
        std::string long_str = fdp.ConsumeRandomLengthString(long_len);
        
        // Ensure strings are not empty
        if (short_str.empty()) short_str = "short";
        if (long_str.empty()) long_str = std::string(long_len, 'x');
        
        cJSON* string_item = cJSON_CreateString(short_str.c_str());
        if (string_item != NULL) {
            char* result = cJSON_SetValuestring(string_item, long_str.c_str());
            // May return NULL if allocation fails
            cJSON_Delete(string_item);
        }
    }

    
    // Test 8: Test cleanup and memory management
    // Create multiple string items and modify them repeatedly
    if (fdp.remaining_bytes() > 300) {
        const int num_items = fdp.ConsumeIntegralInRange<int>(2, 10);
        std::vector<cJSON*> items;
        
        for (int i = 0; i < num_items && fdp.remaining_bytes() > 30; i++) {
            std::string initial = fdp.ConsumeRandomLengthString(50);
            cJSON* item = cJSON_CreateString(initial.c_str());
            if (item != NULL) {
                items.push_back(item);
            }
        }
        
        // Modify each item multiple times
        for (auto& item : items) {
            int num_modifications = fdp.ConsumeIntegralInRange<int>(1, 5);
            for (int j = 0; j < num_modifications && fdp.remaining_bytes() > 20; j++) {
                std::string new_val = fdp.ConsumeRandomLengthString(100);
                char* result = cJSON_SetValuestring(item, new_val.c_str());
            }
        }
        
        // Clean up
        for (auto& item : items) {
            cJSON_Delete(item);
        }
    }

    return 0;
}

} // namespace harness_024
