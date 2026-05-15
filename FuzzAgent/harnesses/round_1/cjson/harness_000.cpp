#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "cjson/cJSON.h"

namespace harness_000 {

extern "C" {
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 4) {
        return 0;  // Minimum size for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume parsing method selection
    uint8_t method = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume require_null_terminated flag
    bool require_null_terminated = fdp.ConsumeBool();
    
    // Consume buffer size for buffered printing
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    
    // Consume formatted flag for buffered printing
    bool formatted = fdp.ConsumeBool();
    
    // Consume the remaining bytes as JSON input string
    std::string json_input = fdp.ConsumeRemainingBytesAsString();
    
    cJSON* json_obj = nullptr;
    const char* parse_end = nullptr;
    switch (method) {
        case 0:
            // Test cJSON_Parse - basic parsing
            json_obj = cJSON_Parse(json_input.c_str());
            break;
            
        case 1:
            // Test cJSON_ParseWithLength - parsing with explicit length
            json_obj = cJSON_ParseWithLength(json_input.c_str(), json_input.size());
            break;
            
        case 2:
            // Test cJSON_ParseWithOpts - parsing with options
            json_obj = cJSON_ParseWithOpts(json_input.c_str(), &parse_end, require_null_terminated);
            break;
            
        case 3:
            // Test cJSON_ParseWithLengthOpts - parsing with length and options
            json_obj = cJSON_ParseWithLengthOpts(json_input.c_str(), json_input.size(), &parse_end, require_null_terminated);
            break;
    }
    
    // If parsing succeeded, perform basic operations on the parsed object
    if (json_obj != nullptr) {
        // Test basic type checking functions
        (void)cJSON_IsInvalid(json_obj);
        (void)cJSON_IsFalse(json_obj);
        (void)cJSON_IsTrue(json_obj);
        (void)cJSON_IsNull(json_obj);
        (void)cJSON_IsNumber(json_obj);
        (void)cJSON_IsString(json_obj);
        (void)cJSON_IsArray(json_obj);
        (void)cJSON_IsObject(json_obj);
        (void)cJSON_IsRaw(json_obj);
        
        // Test item count functions
        (void)cJSON_GetArraySize(json_obj);
        
        // Test printing functions with different formats
        char* printed_formatted = cJSON_Print(json_obj);
        if (printed_formatted != nullptr) {
            free(printed_formatted);
        }
        
        char* printed_unformatted = cJSON_PrintUnformatted(json_obj);
        if (printed_unformatted != nullptr) {
            free(printed_unformatted);
        }
        
        // Test buffered printing with consumed buffer size
        char* printed_buffered = cJSON_PrintBuffered(json_obj, buffer_size, formatted);
        if (printed_buffered != nullptr) {
            free(printed_buffered);
        }
        
        // Test minify on a copy of the input
        if (!json_input.empty()) {
            char* input_copy = (char*)malloc(json_input.size() + 1);
            if (input_copy != nullptr) {
                memcpy(input_copy, json_input.c_str(), json_input.size());
                input_copy[json_input.size()] = '\0';
                cJSON_Minify(input_copy);
                free(input_copy);
            }
        }
        
        // Clean up
        cJSON_Delete(json_obj);
    }
    
    // Test parsing error retrieval when parsing fails
    // We need to test with malformed input to trigger parse errors
    if (json_input.size() > 0 && json_input[0] == '{' && !json_input.empty()) {
        // Intentionally create malformed JSON by truncating
        std::string malformed = json_input.substr(0, json_input.size() / 2);
        cJSON* bad_parse = cJSON_Parse(malformed.c_str());
        if (bad_parse == nullptr) {
            // Get parse error
            const char* error_ptr = cJSON_GetErrorPtr();
            (void)error_ptr; // Use error pointer
        } else {
            cJSON_Delete(bad_parse);
        }
    }
    
    return 0;
}

} // namespace harness_000
