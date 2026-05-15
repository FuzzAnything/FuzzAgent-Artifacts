#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_005 {
/*
 * Fuzzing harness for libcurl's deprecated form data API operations
 * Targets: curl_formget (14 branches, 0% coverage), 
 *          curl_formadd (builds form data structures),
 *          curl_formfree (cleans up form data)
 * Following invocation sequence from coverage guidance:
 * curl_easy_init() -> curl_formadd() multiple times with different form field types
 * (text, file, buffer) -> curl_formget() with callback function to serialize form data
 * -> curl_formfree() -> curl_easy_cleanup()
 * Focus on HTTP multipart form data manipulation lifecycle using deprecated API
 */


// Callback function for curl_formget()
static size_t formget_callback(void *arg, const char *buf, size_t len) {
    // Just consume the data, no actual processing needed for fuzzing
    (void)arg;
    (void)buf;
    return len;  // Return len to indicate success
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum input for meaningful fuzzing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }
    
    // Step 1: Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }
    
    // Step 2: Create formpost structures
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    // Determine number of form parts to add (1-5)
    size_t num_parts = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    
    // Consume some data for various form field values
    std::string field_name1 = fdp.ConsumeRandomLengthString(50);
    std::string field_value1 = fdp.ConsumeRandomLengthString(100);
    std::string field_name2 = fdp.ConsumeRandomLengthString(50);
    std::string field_value2 = fdp.ConsumeRandomLengthString(100);
    std::string buffer_name = fdp.ConsumeRandomLengthString(50);
    std::vector<uint8_t> buffer_data = fdp.ConsumeBytes<uint8_t>(
        fdp.ConsumeIntegralInRange<size_t>(1, 200));
    
    // Use CURL_IGNORE_DEPRECATION to suppress deprecation warnings
    CURL_IGNORE_DEPRECATION(
        // Add first form part with CURLFORM_COPYNAME and CURLFORM_COPYCONTENTS
        CURLFORMcode rc1 = curl_formadd(&formpost, &lastptr,
                                       CURLFORM_COPYNAME, field_name1.c_str(),
                                       CURLFORM_COPYCONTENTS, field_value1.c_str(),
                                       CURLFORM_END);
        
        // Add second form part with CURLFORM_PTRNAME and CURLFORM_PTRCONTENTS
        if (num_parts >= 2) {
            CURLFORMcode rc2 = curl_formadd(&formpost, &lastptr,
                                           CURLFORM_PTRNAME, field_name2.c_str(),
                                           CURLFORM_PTRCONTENTS, field_value2.c_str(),
                                           CURLFORM_END);
        }
        
        // Add buffer-based form part with CURLFORM_BUFFERPTR
        if (num_parts >= 3 && !buffer_data.empty()) {
            std::string buffer_filename = fdp.ConsumeRandomLengthString(50);
            CURLFORMcode rc3 = curl_formadd(&formpost, &lastptr,
                                           CURLFORM_COPYNAME, buffer_name.c_str(),
                                           CURLFORM_BUFFERPTR, buffer_data.data(),
                                           CURLFORM_BUFFERLENGTH, buffer_data.size(),
                                           CURLFORM_FILENAME, buffer_filename.c_str(),
                                           CURLFORM_END);
        }
        
        // Try to add a form part with content type
        if (num_parts >= 4) {
            std::string content_field = fdp.ConsumeRandomLengthString(50);
            std::string content_value = fdp.ConsumeRandomLengthString(100);
            std::string content_type = fdp.ConsumeRandomLengthString(50);
            
            CURLFORMcode rc4 = curl_formadd(&formpost, &lastptr,
                                           CURLFORM_COPYNAME, content_field.c_str(),
                                           CURLFORM_COPYCONTENTS, content_value.c_str(),
                                           CURLFORM_CONTENTTYPE, content_type.c_str(),
                                           CURLFORM_END);
        }
        
        // Try using CURLFORM_ARRAY if we have enough data
        if (num_parts >= 5) {
            std::string array_field = fdp.ConsumeRandomLengthString(50);
            std::string array_value = fdp.ConsumeRandomLengthString(100);
            
            // Create forms array (simplified for fuzzing)
            struct curl_forms forms[3];
            forms[0].option = CURLFORM_COPYNAME;
            forms[0].value = array_field.c_str();
            forms[1].option = CURLFORM_COPYCONTENTS;
            forms[1].value = array_value.c_str();
            forms[2].option = CURLFORM_END;
            forms[2].value = NULL;
            
            CURLFORMcode rc5 = curl_formadd(&formpost, &lastptr,
                                           CURLFORM_ARRAY, forms,
                                           CURLFORM_END);
        }
    );
    
    // Step 3: Use curl_formget() to serialize the form data
    if (formpost) {
        void *callback_arg = NULL;
        CURL_IGNORE_DEPRECATION(
            int formget_res = curl_formget(formpost, callback_arg, formget_callback);
            // Result is ignored for fuzzing purposes
        );
    }
    
    // Step 4: Clean up form data
    CURL_IGNORE_DEPRECATION(
        curl_formfree(formpost);
    );
    
    // Step 5: Clean up easy handle and global libcurl
    curl_easy_cleanup(easy);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_005
