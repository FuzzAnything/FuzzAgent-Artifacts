#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <list>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "curl/curl.h"

namespace harness_004 {
/*
 * Fuzzing harness for libcurl targeting Form API family for multipart form data
 * Harness ID: 004
 * Targets: curl_formadd, curl_formget, curl_formfree
 * Focus: Multipart form data creation, serialization, and cleanup operations
 * Differentiated from:
 * - harness_000.cpp (curl_url/curl_easy APIs)
 * - harness_001.cpp (MIME APIs) 
 * - harness_002.cpp (curl_multi APIs)
 * - harness_003.cpp (FTP protocol)
 * This harness specifically targets Form API coverage gap with 74+ undiscovered complexity
 * FIXED: CURLFORM_CONTENTHEADER lifecycle management - headers now persist until after curl_formget and curl_easy_cleanup
 */



// Simple callback for curl_formget to collect serialized form data
static size_t formget_callback(void* arg, const char* buf, size_t len) {
    // Just return the length to indicate success
    (void)arg;
    (void)buf;
    return len;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create form post structure
    struct curl_httppost* formpost = NULL;
    struct curl_httppost* lastptr = NULL;
    // Lists to store persistent data for CURLFORM_PTRCONTENTS and CURLFORM_BUFFERPTR
    // Using std::list instead of std::vector for pointer stability - vectors can reallocate
    // These must persist until after curl_easy_cleanup()
    std::list<std::string> persistent_strings;
    std::list<std::vector<uint8_t>> persistent_buffers;
    std::vector<struct curl_slist*> persistent_headers;
    
    // ==================== Form API Testing ====================
    
    // Determine number of form fields to create (1-10 to avoid excessive resource usage)
    size_t num_fields = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    for (size_t i = 0; i < num_fields && fdp.remaining_bytes() > 16; i++) {
        // Consume field type (0-7 for different field types)
        int field_type = fdp.ConsumeIntegralInRange<int>(0, 7);
        
        // Consume field name for all field types
        std::string field_name = fdp.ConsumeRandomLengthString(64);
        if (field_name.empty()) {
            field_name = "field" + std::to_string(i);
        }
        
        // Create different types of form fields based on fuzzed data
        switch (field_type) {
            case 0: {  // Regular text field with COPYCONTENTS (safe - libcurl copies)
                std::string field_value = fdp.ConsumeRandomLengthString(256);
                CURL_IGNORE_DEPRECATION(
                    curl_formadd(&formpost, &lastptr,
                                 CURLFORM_COPYNAME, field_name.c_str(),
                                 CURLFORM_COPYCONTENTS, field_value.c_str(),
                                 CURLFORM_END);
                )
                break;
            }
            
            case 1: {  // File field with FILENAME
                std::string filename = fdp.ConsumeRandomLengthString(128);
                if (filename.empty()) {
                    filename = "testfile.txt";
                }
                CURL_IGNORE_DEPRECATION(
                    curl_formadd(&formpost, &lastptr,
                                 CURLFORM_COPYNAME, field_name.c_str(),
                                 CURLFORM_FILE, filename.c_str(),
                                 CURLFORM_END);
                )
                break;
            }
            
            case 2: {  // Text field with PTRCONTENTS (pointer to data) - FIXED: Use persistent string
                std::string field_value = fdp.ConsumeRandomLengthString(128);
                if (!field_value.empty()) {
                    // Store string persistently to satisfy CURLFORM_PTRCONTENTS contract
                    persistent_strings.push_back(std::move(field_value));
                    const std::string& persistent_string = persistent_strings.back();
                    
                    CURL_IGNORE_DEPRECATION(
                        curl_formadd(&formpost, &lastptr,
                                     CURLFORM_COPYNAME, field_name.c_str(),
                                     CURLFORM_PTRCONTENTS, persistent_string.c_str(),
                                     CURLFORM_CONTENTSLENGTH, (long)persistent_string.size(),
                                     CURLFORM_END);
                    )
                }
                break;
            }
            
            case 3: {  // Field with content type (uses COPYCONTENTS - safe)
                std::string field_value = fdp.ConsumeRandomLengthString(128);
                std::string content_type = fdp.ConsumeRandomLengthString(64);
                if (content_type.empty()) {
                    content_type = "text/plain";
                }
                CURL_IGNORE_DEPRECATION(
                    curl_formadd(&formpost, &lastptr,
                                 CURLFORM_COPYNAME, field_name.c_str(),
                                 CURLFORM_COPYCONTENTS, field_value.c_str(),
                                 CURLFORM_CONTENTTYPE, content_type.c_str(),
                                 CURLFORM_END);
                )
                break;
            }
            
            case 4: {  // Field with filename override (uses COPYCONTENTS - safe)
                std::string field_value = fdp.ConsumeRandomLengthString(128);
                std::string show_filename = fdp.ConsumeRandomLengthString(64);
                if (show_filename.empty()) {
                    show_filename = "uploaded.txt";
                }
                CURL_IGNORE_DEPRECATION(
                    curl_formadd(&formpost, &lastptr,
                                 CURLFORM_COPYNAME, field_name.c_str(),
                                 CURLFORM_COPYCONTENTS, field_value.c_str(),
                                 CURLFORM_FILENAME, show_filename.c_str(),
                                 CURLFORM_END);
                )
                break;
            }
            
            case 5: {  // Buffer field (in-memory file) - FIXED: Use persistent buffer
                size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
                std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
                std::string filename = fdp.ConsumeRandomLengthString(64);
                if (filename.empty()) {
                    filename = "bufferfile.dat";
                }
                if (!buffer.empty()) {
                    // Store buffer persistently to satisfy CURLFORM_BUFFERPTR contract
                    persistent_buffers.push_back(std::move(buffer));
                    const std::vector<uint8_t>& persistent_buffer = persistent_buffers.back();
                    
                    CURL_IGNORE_DEPRECATION(
                        curl_formadd(&formpost, &lastptr,
                                     CURLFORM_COPYNAME, field_name.c_str(),
                                     CURLFORM_BUFFER, filename.c_str(),
                                     CURLFORM_BUFFERPTR, persistent_buffer.data(),
                                     CURLFORM_BUFFERLENGTH, (long)persistent_buffer.size(),
                                     CURLFORM_END);
                    )
                }
                break;
            }
            
            case 6: {  // Multiple fields using array approach (simplified, uses COPYCONTENTS - safe)
                std::string field_value1 = fdp.ConsumeRandomLengthString(64);
                std::string field_value2 = fdp.ConsumeRandomLengthString(64);
                if (!field_value1.empty()) {
                    CURL_IGNORE_DEPRECATION(
                        curl_formadd(&formpost, &lastptr,
                                     CURLFORM_COPYNAME, field_name.c_str(),
                                     CURLFORM_COPYCONTENTS, field_value1.c_str(),
                                     CURLFORM_END);
                    )
                }
                // Add another field with different name
                std::string field_name2 = "field2_" + std::to_string(i);
                if (!field_value2.empty()) {
                    CURL_IGNORE_DEPRECATION(
                        curl_formadd(&formpost, &lastptr,
                                     CURLFORM_COPYNAME, field_name2.c_str(),
                                     CURLFORM_COPYCONTENTS, field_value2.c_str(),
                                     CURLFORM_END);
                    )
                }
                break;
            }
            
            case 7: {  // Field with custom headers (uses COPYCONTENTS - safe)
                std::string field_value = fdp.ConsumeRandomLengthString(128);
                struct curl_slist* headers = NULL;
                std::string header1 = fdp.ConsumeRandomLengthString(64);
                if (!header1.empty()) {
                    headers = curl_slist_append(headers, header1.c_str());
                }
                std::string header2 = fdp.ConsumeRandomLengthString(64);
                if (!header2.empty()) {
                    headers = curl_slist_append(headers, header2.c_str());
                }
                
                if (!field_value.empty() && headers) {
                    // Store headers persistently to satisfy CURLFORM_CONTENTHEADER contract
                    // Headers must persist until after curl_formget and curl_easy_cleanup
                    persistent_headers.push_back(headers);
                    
                    CURL_IGNORE_DEPRECATION(
                        curl_formadd(&formpost, &lastptr,
                                     CURLFORM_COPYNAME, field_name.c_str(),
                                     CURLFORM_COPYCONTENTS, field_value.c_str(),
                                     CURLFORM_CONTENTHEADER, headers,
                                     CURLFORM_END);
                    )
                }
                break;
            }
        }
    }
    
    // ==================== Form Serialization Testing ====================
    
    // Test curl_formget if we have a form
    if (formpost) {
        // Test serialization with callback
        int formget_result = curl_formget(formpost, NULL, formget_callback);
        (void)formget_result;  // Result not used, but call tests the API
        
        // Test with a custom context
        struct TestContext {
            size_t total_size;
        } context = {0};
        
        // Custom callback that accumulates size
        auto accumulating_callback = [](void* arg, const char* buf, size_t len) -> size_t {
            TestContext* ctx = static_cast<TestContext*>(arg);
            ctx->total_size += len;
            return len;
        };
        
        curl_formget(formpost, &context, accumulating_callback);
    }
    
    // ==================== Easy Handle Integration Testing ====================
    
    // Create an easy handle and test form integration
    CURL* easy_handle = curl_easy_init();
    if (easy_handle && formpost) {
        // Set form data on easy handle
        CURL_IGNORE_DEPRECATION(
            curl_easy_setopt(easy_handle, CURLOPT_HTTPPOST, formpost);
        )
        
        // Test that the form is properly associated with the easy handle
        // by getting info back from it (without actually performing a transfer)
        char* effective_url = NULL;
        curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &effective_url);
        
        // Clean up easy handle (buffers/strings must persist until after this)
        curl_easy_cleanup(easy_handle);
    } else if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    // ==================== Cleanup ====================
    
    // Free form data structure
    if (formpost) {
        curl_formfree(formpost);
    }
    
    // Free persistent headers
    for (auto headers : persistent_headers) {
        if (headers) {
            curl_slist_free_all(headers);
        }
    }
    
    // Global cleanup
    curl_global_cleanup();
    return 0;
}

} // namespace harness_004
