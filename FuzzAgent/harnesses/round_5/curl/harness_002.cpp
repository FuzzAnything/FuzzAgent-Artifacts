#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_002 {
/*
 * Fuzzing harness for libcurl's MIME (multipart/form-data) operations
 * Targets: curl_mime_init, curl_mime_addpart, curl_mime_data, curl_mime_filedata,
 *          curl_mime_headers, curl_mime_name, curl_mime_type, curl_mime_encoder,
 *          curl_mime_subparts, curl_mime_free
 * Helpers: curl_easy_init, curl_easy_setopt with CURLOPT_MIMEPOST, 
 *          curl_easy_perform, curl_easy_cleanup
 * Semantic differentiation from harness_000.cpp and harness_001.cpp: 
 *   Focuses on MIME multipart operations rather than basic HTTP or header manipulation
 */


// Simple write callback that discards data
static size_t discard_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;  // Always report success
}

// Header callback that discards headers
static size_t discard_header_cb(char *buffer, size_t size, size_t nitems, void *userdata) {
    (void)buffer;
    (void)userdata;
    return size * nitems;  // Always report success
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for meaningful MIME fuzzing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first for configuration
    uint8_t num_parts = fdp.ConsumeIntegralInRange<uint8_t>(1, 5);
    uint8_t use_nested = fdp.ConsumeBool();
    uint8_t max_name_len = fdp.ConsumeIntegralInRange<uint8_t>(1, 64);
    uint16_t max_data_len = fdp.ConsumeIntegralInRange<uint16_t>(1, 256);
    uint8_t use_file_data = fdp.ConsumeBool();
    uint8_t add_headers = fdp.ConsumeBool();
    uint8_t set_content_type = fdp.ConsumeBool();
    uint8_t set_encoder = fdp.ConsumeBool();
    
    // Consume URL string
    std::string url = fdp.ConsumeRandomLengthString(128);
    
    // Initialize curl global state
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // Set URL - use a dummy URL since we're fuzzing API usage
    curl_easy_setopt(curl, CURLOPT_URL, url.empty() ? "http://example.com/upload" : url.c_str());
    
    // Set write callback to discard response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, nullptr);
    
    // Set header callback to discard headers
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, discard_header_cb);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, nullptr);
    
    // Set timeout to prevent hanging
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    
    // Create MIME structure
    curl_mime *mime = curl_mime_init(curl);
    if (!mime) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }
    
    // Create nested MIME structure if requested
    curl_mime *nested_mime = nullptr;
    if (use_nested) {
        nested_mime = curl_mime_init(curl);
    }
    
    // Add MIME parts according to configuration
    for (uint8_t i = 0; i < num_parts && fdp.remaining_bytes() > 0; i++) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (!part) {
            continue;
        }
        
        // Set part name from fuzzer input
        std::string part_name = fdp.ConsumeRandomLengthString(max_name_len);
        if (!part_name.empty()) {
            curl_mime_name(part, part_name.c_str());
        }
        
        // Decide what type of data to add to this part
        uint8_t data_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        switch (data_type) {
            case 0: {  // Regular data
                std::string part_data = fdp.ConsumeRandomLengthString(max_data_len);
                if (!part_data.empty()) {
                    curl_mime_data(part, part_data.c_str(), part_data.size());
                }
                break;
            }
            
            case 1: {  // File data (simulated with string)
                if (use_file_data && fdp.remaining_bytes() > 0) {
                    std::string filename = fdp.ConsumeRandomLengthString(max_name_len);
                    if (!filename.empty()) {
                        // Note: In real usage, this would be a real file path
                        // For fuzzing, we simulate with a string
                        curl_mime_filedata(part, filename.c_str());
                    }
                }
                break;
            }
            
            case 2: {  // Zero-terminated string data
                std::string zero_term_data = fdp.ConsumeRandomLengthString(max_data_len);
                if (!zero_term_data.empty()) {
                    curl_mime_data(part, zero_term_data.c_str(), CURL_ZERO_TERMINATED);
                }
                break;
            }
        }
        
        // Set content type if requested
        if (set_content_type && fdp.remaining_bytes() > 0) {
            std::string content_type = fdp.ConsumeRandomLengthString(32);
            if (!content_type.empty()) {
                curl_mime_type(part, content_type.c_str());
            }
        }
        
        // Set encoder if requested
        if (set_encoder && fdp.remaining_bytes() > 0) {
            std::string encoding = fdp.ConsumeRandomLengthString(16);
            if (!encoding.empty()) {
                curl_mime_encoder(part, encoding.c_str());
            }
        }
        
        // Add headers if requested
        if (add_headers && fdp.remaining_bytes() > 0) {
            struct curl_slist *headers = nullptr;
            uint8_t num_headers = fdp.ConsumeIntegralInRange<uint8_t>(0, 3);
            
            for (uint8_t h = 0; h < num_headers && fdp.remaining_bytes() > 0; h++) {
                std::string header_name = fdp.ConsumeRandomLengthString(32);
                std::string header_value = fdp.ConsumeRandomLengthString(64);
                
                if (!header_name.empty() && !header_value.empty()) {
                    std::string header = header_name + ": " + header_value;
                    headers = curl_slist_append(headers, header.c_str());
                }
            }
            
            if (headers) {
                // Take ownership of headers
                curl_mime_headers(part, headers, 1);
            }
        }
        
        // Add to nested structure if we have one
        if (nested_mime && i < num_parts / 2) {
            curl_mimepart *nested_part = curl_mime_addpart(nested_mime);
            if (nested_part) {
                std::string nested_data = fdp.ConsumeRandomLengthString(max_data_len);
                if (!nested_data.empty()) {
                    curl_mime_data(nested_part, nested_data.c_str(), nested_data.size());
                }
            }
        }
    }
    
    // Attach nested MIME structure if created
    if (nested_mime) {
        curl_mimepart *container_part = curl_mime_addpart(mime);
        if (container_part) {
            curl_mime_subparts(container_part, nested_mime);
            curl_mime_type(container_part, "multipart/mixed");
        }
    }
    
    // Attach MIME to easy handle
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    
    // Set POST method since we're sending MIME data
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    
    // Perform the request (ignore result, we're fuzzing API usage)
    curl_easy_perform(curl);
    
    // Clean up nested MIME if created
    if (nested_mime) {
        // Note: nested_mime will be freed automatically when parent mime is freed
        // because it's attached via curl_mime_subparts
    }
    
    // Clean up MIME structure
    curl_mime_free(mime);
    
    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_002
