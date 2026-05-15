#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_010 {
// Formget callback that just counts bytes
static size_t formget_callback(void *arg, const char *buf, size_t len) {
    // Just return len to indicate success
    return len;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum size for comprehensive form operations
    // We need data for name, value, filenames, content types, etc.
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize curl library - must be called once
    CURLcode global_init_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_result != CURLE_OK) {
        return 0;
    }
    
    // Create form post chain
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastpost = NULL;
    
    // Consume operation type to determine what kind of form to create
    uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // ============================================
    // TEST: curl_formadd - PRIMARY TARGET
    // ============================================
    CURLFORMcode formadd_result = CURL_FORMADD_OK;
    
    switch (operation_type) {
        case 0: {
            // Simple name/value pair with copied contents
            std::string name = fdp.ConsumeRandomLengthString(100);
            std::string value = fdp.ConsumeRandomLengthString(1024);
            
            formadd_result = curl_formadd(&formpost, &lastpost,
                CURLFORM_COPYNAME, name.c_str(),
                CURLFORM_COPYCONTENTS, value.c_str(),
                CURLFORM_END);
            break;
        }
        
        case 1: {
            // Name/value pair with pointer contents and content length
            std::string name = fdp.ConsumeRandomLengthString(100);
            std::string value = fdp.ConsumeRandomLengthString(1024);
            long content_len = static_cast<long>(value.length());
            
            // Allocate heap memory for the content to ensure it stays valid
            char* content_ptr = static_cast<char*>(malloc(value.length() + 1));
            if (content_ptr) {
                memcpy(content_ptr, value.c_str(), value.length());
                content_ptr[value.length()] = '\0';
                
                formadd_result = curl_formadd(&formpost, &lastpost,
                    CURLFORM_COPYNAME, name.c_str(),
                    CURLFORM_PTRCONTENTS, content_ptr,
                    CURLFORM_CONTENTSLENGTH, content_len,
                    CURLFORM_END);
                
                // Note: content_ptr memory will be freed by curl_formfree
                // when formpost is freed
            } else {
                formadd_result = CURL_FORMADD_MEMORY;
            }
            break;
        }
        case 2: {
            // File upload with filename
            std::string name = fdp.ConsumeRandomLengthString(100);
            std::string filename = fdp.ConsumeRandomLengthString(200);
            std::string content_type = fdp.ConsumeRandomLengthString(100);
            
            // Allocate heap memory for filename and content_type to ensure they stay valid
            char* filename_ptr = static_cast<char*>(malloc(filename.length() + 1));
            char* content_type_ptr = static_cast<char*>(malloc(content_type.length() + 1));
            
            if (filename_ptr && content_type_ptr) {
                memcpy(filename_ptr, filename.c_str(), filename.length());
                filename_ptr[filename.length()] = '\0';
                memcpy(content_type_ptr, content_type.c_str(), content_type.length());
                content_type_ptr[content_type.length()] = '\0';
                
                formadd_result = curl_formadd(&formpost, &lastpost,
                    CURLFORM_COPYNAME, name.c_str(),  // COPYNAME copies the string
                    CURLFORM_FILE, filename_ptr,
                    CURLFORM_CONTENTTYPE, content_type_ptr,
                    CURLFORM_END);
                
                // Note: filename_ptr and content_type_ptr memory will be freed by curl_formfree
                // when formpost is freed
            } else {
                formadd_result = CURL_FORMADD_MEMORY;
                // Free any allocated memory on failure
                if (filename_ptr) free(filename_ptr);
                if (content_type_ptr) free(content_type_ptr);
            }
            break;
        }
        
        case 3: {
            // Multiple files for same field name
            std::string name = fdp.ConsumeRandomLengthString(100);
            std::string filename1 = fdp.ConsumeRandomLengthString(200);
            std::string filename2 = fdp.ConsumeRandomLengthString(200);
            
            // Allocate heap memory for filenames to ensure they stay valid
            char* filename1_ptr = static_cast<char*>(malloc(filename1.length() + 1));
            char* filename2_ptr = static_cast<char*>(malloc(filename2.length() + 1));
            
            if (filename1_ptr && filename2_ptr) {
                memcpy(filename1_ptr, filename1.c_str(), filename1.length());
                filename1_ptr[filename1.length()] = '\0';
                memcpy(filename2_ptr, filename2.c_str(), filename2.length());
                filename2_ptr[filename2.length()] = '\0';
                
                formadd_result = curl_formadd(&formpost, &lastpost,
                    CURLFORM_COPYNAME, name.c_str(),  // COPYNAME copies the string
                    CURLFORM_FILE, filename1_ptr,
                    CURLFORM_FILE, filename2_ptr,
                    CURLFORM_END);
                
                // Note: filename1_ptr and filename2_ptr memory will be freed by curl_formfree
                // when formpost is freed
            } else {
                formadd_result = CURL_FORMADD_MEMORY;
                // Free any allocated memory on failure
                if (filename1_ptr) free(filename1_ptr);
                if (filename2_ptr) free(filename2_ptr);
            }
            break;
        }
        case 4: {
            // Buffer-based file upload
            std::string name = fdp.ConsumeRandomLengthString(100);
            std::string filename = fdp.ConsumeRandomLengthString(200);
            std::string buffer_data = fdp.ConsumeRandomLengthString(1024);
            
            // Allocate heap memory for buffer data and filename to ensure they stay valid
            char* buffer_ptr = static_cast<char*>(malloc(buffer_data.length() + 1));
            char* filename_ptr = static_cast<char*>(malloc(filename.length() + 1));
            
            if (buffer_ptr && filename_ptr) {
                memcpy(buffer_ptr, buffer_data.c_str(), buffer_data.length());
                buffer_ptr[buffer_data.length()] = '\0';
                memcpy(filename_ptr, filename.c_str(), filename.length());
                filename_ptr[filename.length()] = '\0';
                
                formadd_result = curl_formadd(&formpost, &lastpost,
                    CURLFORM_COPYNAME, name.c_str(),  // COPYNAME copies the string
                    CURLFORM_BUFFER, filename_ptr,
                    CURLFORM_BUFFERPTR, buffer_ptr,
                    CURLFORM_BUFFERLENGTH, static_cast<long>(buffer_data.length()),
                    CURLFORM_END);
                
                // Note: buffer_ptr and filename_ptr memory will be freed by curl_formfree
                // when formpost is freed
            } else {
                formadd_result = CURL_FORMADD_MEMORY;
                // Free any allocated memory on failure
                if (buffer_ptr) free(buffer_ptr);
                if (filename_ptr) free(filename_ptr);
            }
            break;
        }
        case 5: {
            // Form with content headers
            std::string name = fdp.ConsumeRandomLengthString(100);
            std::string value = fdp.ConsumeRandomLengthString(1024);
            std::string content_header = "Content-Disposition: form-data";
            
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, content_header.c_str());
            
            formadd_result = curl_formadd(&formpost, &lastpost,
                CURLFORM_COPYNAME, name.c_str(),
                CURLFORM_COPYCONTENTS, value.c_str(),
                CURLFORM_CONTENTHEADER, headers,
                CURLFORM_END);
            
            // Only free headers if formadd failed
            // If formadd succeeded, libcurl takes ownership of the headers
            if (formadd_result != CURL_FORMADD_OK && headers) {
                curl_slist_free_all(headers);
            }
            break;
        }
        case 6: {
            // Mixed form with multiple fields
            std::string name1 = fdp.ConsumeRandomLengthString(100);
            std::string value1 = fdp.ConsumeRandomLengthString(512);
            std::string name2 = fdp.ConsumeRandomLengthString(100);
            std::string filename = fdp.ConsumeRandomLengthString(200);
            
            // Allocate heap memory for filename to ensure it stays valid
            char* filename_ptr = static_cast<char*>(malloc(filename.length() + 1));
            if (filename_ptr) {
                memcpy(filename_ptr, filename.c_str(), filename.length());
                filename_ptr[filename.length()] = '\0';
                
                // First form with name1/value1
                formadd_result = curl_formadd(&formpost, &lastpost,
                    CURLFORM_COPYNAME, name1.c_str(),  // COPYNAME copies the string
                    CURLFORM_COPYCONTENTS, value1.c_str(),  // COPYCONTENTS copies the string
                    CURLFORM_END);
                    
                if (formadd_result == CURL_FORMADD_OK) {
                    // Second form with name2/filename
                    formadd_result = curl_formadd(&formpost, &lastpost,
                        CURLFORM_COPYNAME, name2.c_str(),  // COPYNAME copies the string
                        CURLFORM_FILE, filename_ptr,  // FILE needs persistent pointer
                        CURLFORM_END);
                }
                
                // Note: filename_ptr memory will be freed by curl_formfree
                // when formpost is freed
            } else {
                formadd_result = CURL_FORMADD_MEMORY;
            }
            break;
        }
        case 7: {
            // Array-based form (using curl_forms array)
            std::string name = fdp.ConsumeRandomLengthString(100);
            std::string value = fdp.ConsumeRandomLengthString(1024);
            std::string content_type = fdp.ConsumeRandomLengthString(50);
            
            struct curl_forms forms[4];
            // For CURLFORM_COPYNAME and CURLFORM_COPYCONTENTS, libcurl copies the strings
            // So we can safely use .c_str() pointers
            forms[0].option = CURLFORM_COPYNAME;
            forms[0].value = name.c_str();
            forms[1].option = CURLFORM_COPYCONTENTS;
            forms[1].value = value.c_str();
            forms[2].option = CURLFORM_CONTENTTYPE;
            forms[2].value = content_type.c_str();
            forms[3].option = CURLFORM_END;
            forms[3].value = NULL;
            
            formadd_result = curl_formadd(&formpost, &lastpost,
                CURLFORM_ARRAY, forms,
                CURLFORM_END);
            break;
        }
    }
    
    // If formadd failed, clean up and return
    if (formadd_result != CURL_FORMADD_OK) {
        if (formpost) {
            curl_formfree(formpost);
        }
        curl_global_cleanup();
        return 0;
    }
    // ============================================
    // TEST: curl_formget - SECONDARY TARGET
    // ============================================
    if (formpost) {
        // Test curl_formget with simple callback
        int formget_result = curl_formget(formpost, NULL, formget_callback);
        (void)formget_result; // Ignore result for fuzzing
        
        // Test with different callback scenarios
        uint8_t formget_test = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        switch (formget_test) {
            case 0: {
                // Normal formget
                curl_formget(formpost, NULL, formget_callback);
                break;
            }
            case 1: {
                // Formget with user pointer
                char user_data[16];
                std::string user_str = fdp.ConsumeRandomLengthString(15);
                strncpy(user_data, user_str.c_str(), sizeof(user_data) - 1);
                user_data[sizeof(user_data) - 1] = '\0';
                curl_formget(formpost, user_data, formget_callback);
                break;
            }
            case 2: {
                // Try formget on NULL form (should handle gracefully)
                curl_formget(NULL, NULL, formget_callback);
                break;
            }
        }
    }
    
    // ============================================
    // TEST: curl_formfree - TERTIARY TARGET
    // ============================================
    if (formpost) {
        // Test normal formfree
        curl_formfree(formpost);
        
        // Test formfree on NULL (should handle gracefully)
        curl_formfree(NULL);
    }
    
    // ============================================
    // TEST: Additional edge cases
    // ============================================
    
    // Test 1: Form with empty strings
    if (fdp.remaining_bytes() > 10) {
        struct curl_httppost *edge_form = NULL;
        struct curl_httppost *edge_last = NULL;
        
        curl_formadd(&edge_form, &edge_last,
            CURLFORM_COPYNAME, "",
            CURLFORM_COPYCONTENTS, "",
            CURLFORM_END);
        
        if (edge_form) {
            curl_formfree(edge_form);
        }
    }
    
    // Test 2: Form with very long strings (test buffer boundaries)
    if (fdp.remaining_bytes() > 100) {
        struct curl_httppost *long_form = NULL;
        struct curl_httppost *long_last = NULL;
        std::string long_name = fdp.ConsumeRandomLengthString(5000);
        std::string long_value = fdp.ConsumeRandomLengthString(10000);
        
        curl_formadd(&long_form, &long_last,
            CURLFORM_COPYNAME, long_name.c_str(),
            CURLFORM_COPYCONTENTS, long_value.c_str(),
            CURLFORM_END);
        
        if (long_form) {
            curl_formfree(long_form);
        }
    }
    
    // Test 3: Multiple sequential formadd calls
    if (fdp.remaining_bytes() > 50) {
        struct curl_httppost *multi_form = NULL;
        struct curl_httppost *multi_last = NULL;
        
        for (int i = 0; i < 3 && fdp.remaining_bytes() > 20; i++) {
            std::string name = fdp.ConsumeRandomLengthString(50);
            std::string value = fdp.ConsumeRandomLengthString(100);
            
            curl_formadd(&multi_form, &multi_last,
                CURLFORM_COPYNAME, name.c_str(),
                CURLFORM_COPYCONTENTS, value.c_str(),
                CURLFORM_END);
        }
        
        if (multi_form) {
            curl_formfree(multi_form);
        }
    }
    
    // Test 4: Invalid formadd combinations (should still be handled)
    if (fdp.remaining_bytes() > 10) {
        struct curl_httppost *invalid_form = NULL;
        struct curl_httppost *invalid_last = NULL;
        
        std::string test_name = fdp.ConsumeRandomLengthString(20);
        std::string test_value = fdp.ConsumeRandomLengthString(20);
        
        // Missing CURLFORM_END (invalid but should be handled)
        curl_formadd(&invalid_form, &invalid_last,
            CURLFORM_COPYNAME, test_name.c_str(),
            CURLFORM_COPYCONTENTS, test_value.c_str());
            // Note: intentionally missing CURLFORM_END
        
        if (invalid_form) {
            curl_formfree(invalid_form);
        }
    }
    
    curl_global_cleanup();
    return 0;
}

} // namespace harness_010
