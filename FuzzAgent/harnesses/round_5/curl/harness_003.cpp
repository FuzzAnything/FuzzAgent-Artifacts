#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_003 {
/*
 * Fuzzing harness for libcurl's deprecated form data API subsystem
 * Targets: curl_formadd, curl_formget, curl_formfree
 * Helpers: curl_easy_init, curl_easy_cleanup
 * Semantic differentiation from previous harnesses:
 *   - harness_000: Basic HTTP operations
 *   - harness_001: Header manipulation  
 *   - harness_002: MIME (multipart/form-data) operations
 *   - harness_003: Deprecated form data API (curl_form* functions)
 * Focus: Tests HTTP POST form data building and serialization with various field types
 */


// Callback for curl_formget to collect serialized form data
static size_t formget_callback(void *arg, const char *buf, size_t len) {
    std::vector<char> *output = static_cast<std::vector<char>*>(arg);
    if (buf && len > 0) {
        output->insert(output->end(), buf, buf + len);
    }
    return len;  // Return the length we processed
}

// Simple read callback for CURLFORM_CALLBACK posts
static size_t read_callback(char *buffer, size_t size, size_t nitems, void *userdata) {
    FuzzedDataProvider *fdp = static_cast<FuzzedDataProvider*>(userdata);
    if (!fdp || fdp->remaining_bytes() == 0) {
        return 0;
    }
    
    size_t max_read = size * nitems;
    size_t to_read = std::min(max_read, fdp->remaining_bytes());
    if (to_read > 0) {
        auto bytes = fdp->ConsumeBytes<uint8_t>(to_read);
        memcpy(buffer, bytes.data(), bytes.size());
        return bytes.size();
    }
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for meaningful form data fuzzing
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters
    uint8_t num_fields = fdp.ConsumeIntegralInRange<uint8_t>(1, 8);
    uint8_t use_file_fields = fdp.ConsumeBool();
    uint8_t use_buffer_fields = fdp.ConsumeBool();
    uint8_t use_callback_fields = fdp.ConsumeBool();
    uint8_t max_name_len = fdp.ConsumeIntegralInRange<uint8_t>(1, 128);
    uint16_t max_data_len = fdp.ConsumeIntegralInRange<uint16_t>(1, 1024);
    
    // Initialize curl global state
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create easy handle (required for form operations context)
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // Form post structure
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    // Add form fields based on configuration
    for (uint8_t i = 0; i < num_fields && fdp.remaining_bytes() > 0; i++) {
        // Consume field name
        std::string field_name = fdp.ConsumeRandomLengthString(max_name_len);
        if (field_name.empty()) {
            field_name = "field_" + std::to_string(i);
        }
        
        // Determine field type based on configuration
        uint8_t field_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        CURLFORMcode result;
        
        if (field_type == 0 || !use_file_fields) {
            // Text field
            std::string field_value = fdp.ConsumeRandomLengthString(max_data_len);
            if (field_value.empty() && fdp.remaining_bytes() > 0) {
                field_value = fdp.ConsumeBytesAsString(std::min((size_t)16, fdp.remaining_bytes()));
            }
            
            result = curl_formadd(&formpost, &lastptr,
                CURLFORM_COPYNAME, field_name.c_str(),
                CURLFORM_COPYCONTENTS, field_value.c_str(),
                CURLFORM_END);
        }
        else if (field_type == 1 && use_buffer_fields) {
            // Buffer field - use COPYCONTENTS to ensure data remains valid
            size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, max_data_len);
            if (buffer_size > fdp.remaining_bytes()) {
                buffer_size = std::min((size_t)max_data_len, fdp.remaining_bytes());
            }
            
            if (buffer_size > 0) {
                std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
                
                // Convert vector to string for COPYCONTENTS (which expects null-terminated)
                // or use PTRCONTENTS with explicit length
                std::string buffer_str(reinterpret_cast<const char*>(buffer.data()), buffer.size());
                
                result = curl_formadd(&formpost, &lastptr,
                    CURLFORM_COPYNAME, field_name.c_str(),
                    CURLFORM_BUFFER, field_name.c_str(),
                    CURLFORM_COPYCONTENTS, buffer_str.c_str(),
                    CURLFORM_BUFFERLENGTH, buffer.size(),
                    CURLFORM_END);
            } else {
                continue;
            }
        }
        else if (field_type == 2 && use_callback_fields) {
            // Callback field - simulate file upload via callback
            size_t callback_size = fdp.ConsumeIntegralInRange<size_t>(1, max_data_len);
            
            result = curl_formadd(&formpost, &lastptr,
                CURLFORM_COPYNAME, field_name.c_str(),
                CURLFORM_FILENAME, "upload.dat",
                CURLFORM_STREAM, &fdp,
                CURLFORM_CONTENTSLENGTH, callback_size,
                CURLFORM_END);
        }
        else {
            // File field (simulated with buffer)
            size_t file_size = fdp.ConsumeIntegralInRange<size_t>(1, max_data_len);
            if (file_size > fdp.remaining_bytes()) {
                file_size = std::min((size_t)max_data_len, fdp.remaining_bytes());
            }
            
            if (file_size > 0) {
                std::vector<uint8_t> file_data = fdp.ConsumeBytes<uint8_t>(file_size);
                // Note: In real usage, this would point to an actual file
                // For fuzzing, we use a buffer with the FILE flag
                // Convert vector to string for COPYCONTENTS to ensure data remains valid
                std::string file_data_str(reinterpret_cast<const char*>(file_data.data()), file_data.size());
                
                result = curl_formadd(&formpost, &lastptr,
                    CURLFORM_COPYNAME, field_name.c_str(),
                    CURLFORM_FILENAME, "testfile.dat",
                    CURLFORM_BUFFER, "testfile.dat",
                    CURLFORM_COPYCONTENTS, file_data_str.c_str(),
                    CURLFORM_BUFFERLENGTH, file_data.size(),
                    CURLFORM_END);
            } else {
                continue;
            }
        }
        
        // Check formadd result
        if (result != CURL_FORMADD_OK) {
            // Continue with next field even if one fails
            continue;
        }
    }
    
    // Test curl_formget if we have a form
    if (formpost) {
        std::vector<char> serialized_data;
        int get_result = curl_formget(formpost, &serialized_data, formget_callback);
        
        // The result should be 0 on success, but we don't fail the harness on error
        (void)get_result;
        
        // Also test with NULL callback
        if (fdp.ConsumeBool()) {
            curl_formget(formpost, NULL, NULL);
        }
    }
    
    // Clean up form data
    if (formpost) {
        curl_formfree(formpost);
    }
    
    // Clean up curl handle
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_003
