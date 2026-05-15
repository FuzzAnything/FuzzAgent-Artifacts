#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "curl/curl.h"

namespace harness_001 {
/*
 * Fuzzing harness for libcurl targeting MIME (Multipart Internet Mail Extensions) API
 * Harness ID: 001
 * Targets: curl_mime APIs for email attachments and multipart form data
 * APIs: curl_mime_init, curl_mime_addpart, curl_mime_data, curl_mime_filename, curl_mime_type, curl_mime_free
 * Differentiated from harness_000.cpp which focuses on curl_url and curl_easy APIs
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic MIME operations
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create an easy handle (required for curl_mime_init)
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // ==================== MIME API Testing ====================
    
    // Initialize MIME structure
    curl_mime* mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Determine number of parts to create (1-8 parts to avoid excessive resource usage)
    size_t num_parts = fdp.ConsumeIntegralInRange<size_t>(1, 8);
    
    for (size_t i = 0; i < num_parts && fdp.remaining_bytes() > 16; i++) {
        // Add a new part to the MIME structure
        curl_mimepart* part = curl_mime_addpart(mime);
        if (!part) {
            continue;  // Skip if part creation fails
        }
        
        // Consume data for this part
        std::string part_data = fdp.ConsumeRandomLengthString(1024);
        
        // Set MIME data (actual content)
        if (!part_data.empty()) {
            curl_mime_data(part, part_data.c_str(), part_data.size());
        } else {
            // Even empty data can be interesting
            curl_mime_data(part, "", 0);
        }
        
        // Consume and set filename (if any)
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
            std::string filename = fdp.ConsumeRandomLengthString(128);
            if (!filename.empty()) {
                curl_mime_filename(part, filename.c_str());
            }
        }
        
        // Consume and set MIME type (if any)
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
            // Common MIME types plus some random ones
            static const char* mime_types[] = {
                "text/plain",
                "text/html",
                "application/json",
                "application/xml",
                "image/jpeg",
                "image/png",
                "application/octet-stream",
                "multipart/form-data",
                "message/rfc822"
            };
            
            size_t type_idx = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(mime_types)/sizeof(mime_types[0]) - 1);
            curl_mime_type(part, mime_types[type_idx]);
            
            // Also try some fuzzed MIME types
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 8) {
                std::string custom_type = fdp.ConsumeRandomLengthString(64);
                curl_mime_type(part, custom_type.c_str());
            }
        }
        
        // Sometimes set part name (form field name)
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
            std::string part_name = fdp.ConsumeRandomLengthString(64);
            if (!part_name.empty()) {
                curl_mime_name(part, part_name.c_str());
            }
        }
        
        // Test encoder for some parts
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
            static const char* encodings[] = {
                "base64",
                "quoted-printable",
                "8bit",
                "7bit"
            };
            
            size_t enc_idx = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(encodings)/sizeof(encodings[0]) - 1);
            curl_mime_encoder(part, encodings[enc_idx]);
        }
    }
    
    // ==================== Test MIME with Easy Handle ====================
    
    // Set the MIME structure as POST data for the easy handle
    // This exercises the integration between MIME and easy APIs
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    
    // Set some other easy options to test integration
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
    
    // Create a write callback that discards data (since we're not making actual network calls)
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, 
                     [](void* buffer, size_t size, size_t nmemb, void* userp) -> size_t {
                         return size * nmemb;  // Pretend we wrote everything
                     });
    
    // Try to perform (will fail since URL is not set, but exercises MIME processing)
    // Don't actually call curl_easy_perform since it would try to make network connections
    // Instead, we test the MIME structure creation and manipulation
    
    // ==================== Test Additional MIME Scenarios ====================
    
    // Create a second MIME structure for nested multipart testing
    if (fdp.remaining_bytes() > 64) {
        curl_mime* alt_mime = curl_mime_init(easy_handle);
        if (alt_mime) {
            // Add a simple part to the alternative MIME
            curl_mimepart* alt_part = curl_mime_addpart(alt_mime);
            if (alt_part) {
                std::string alt_data = fdp.ConsumeRandomLengthString(512);
                curl_mime_data(alt_part, alt_data.c_str(), alt_data.size());
                curl_mime_type(alt_part, "text/plain");
                
                // Test subparts (nested MIME)
                if (fdp.ConsumeBool()) {
                    // Create a part in the main MIME that contains the alternative MIME as subparts
                    curl_mimepart* container_part = curl_mime_addpart(mime);
                    if (container_part) {
                        curl_mime_subparts(container_part, alt_mime);
                        curl_mime_type(container_part, "multipart/alternative");
                    }
                } else {
                    // Free the alternative MIME if not used as subparts
                    curl_mime_free(alt_mime);
                }
            } else {
                curl_mime_free(alt_mime);
            }
        }
    }
    
    // Test filedata API (simulating file attachment)
    if (fdp.remaining_bytes() > 32) {
        curl_mimepart* file_part = curl_mime_addpart(mime);
        if (file_part) {
            // Use filedata with a filename (simulating file attachment)
            std::string filename = fdp.ConsumeRandomLengthString(64);
            if (!filename.empty()) {
                // Note: curl_mime_filedata would try to open the file, which we don't want
                // in a fuzzer. Instead, we'll use data API for fuzzing.
                // But we can still test the API with an invalid/non-existent filename
                // to exercise error paths.
                curl_mime_filedata(file_part, "/dev/null");  // Safe file that exists
            }
        }
    }
    
    // ==================== Cleanup ====================
    
    // Free the MIME structure (this also frees all parts)
    curl_mime_free(mime);
    
    // Clean up easy handle
    curl_easy_cleanup(easy_handle);
    
    // Clean up global curl state
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_001
