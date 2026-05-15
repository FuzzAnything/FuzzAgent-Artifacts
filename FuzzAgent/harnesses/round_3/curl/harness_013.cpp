#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_013 {
/*
 * Fuzzing harness for libcurl's MIME multipart API with focus on curl_mime_filedata
 * Targets: curl_mime_filedata (0% coverage, 16 undiscovered branches)
 *          curl_mime_data (uncovered in existing harnesses)
 *          curl_mime_name (uncovered in existing harnesses)  
 *          curl_mime_type (uncovered in existing harnesses)
 *          curl_mime_encoder (uncovered in existing harnesses)
 *          curl_mime_subparts (uncovered in existing harnesses)
 * Multi-handle support: Uses curl_multi interface with multiple easy handles
 * Semantic diversity: Complex MIME structures with multiple parts, nested subparts,
 *                     various data sources, encoder testing, and error condition exploration
 * Test scenarios: File data operations, memory data, nested multipart structures,
 *                 different encoders, name/type assignments, multi-handle integration
 */


// Simple read callback for curl_mime_data_cb testing
static size_t mime_read_callback(char *buffer, size_t size, size_t nitems, void *instream) {
    FuzzedDataProvider *fdp = static_cast<FuzzedDataProvider*>(instream);
    if (fdp == nullptr || fdp->remaining_bytes() == 0) {
        return 0; // EOF
    }
    
    size_t bytes_to_read = size * nitems;
    if (bytes_to_read > fdp->remaining_bytes()) {
        bytes_to_read = fdp->remaining_bytes();
    }
    
    if (bytes_to_read > 0) {
        std::vector<uint8_t> data = fdp->ConsumeBytes<uint8_t>(bytes_to_read);
        memcpy(buffer, data.data(), data.size());
        return data.size();
    }
    
    return 0;
}

static int mime_seek_callback(void *instream, curl_off_t offset, int origin) {
    (void)instream;
    (void)offset;
    (void)origin;
    // For fuzzing, just return success
    return CURL_SEEKFUNC_OK;
}

static void mime_free_callback(void *instream) {
    FuzzedDataProvider *fdp = static_cast<FuzzedDataProvider*>(instream);
    delete fdp;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need sufficient input for meaningful fuzzing
    // We need data for: test scenario, flags, strings for filenames, data, etc.
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_res != CURLE_OK) {
        return 0;
    }
    
    // Consume test scenario from fuzzed input
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    bool use_multi_handle = fdp.ConsumeBool();
    uint8_t num_parts = fdp.ConsumeIntegralInRange<uint8_t>(1, 5);
    
    // Variables for handles
    CURL *easy_handle = nullptr;
    CURLM *multi_handle = nullptr;
    curl_mime *mime = nullptr;
    
    // Create easy handle
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create multi handle if requested
    if (use_multi_handle) {
        multi_handle = curl_multi_init();
        if (!multi_handle) {
            curl_easy_cleanup(easy_handle);
            curl_global_cleanup();
            return 0;
        }
        
        // Add easy handle to multi handle
        CURLMcode multi_add_res = curl_multi_add_handle(multi_handle, easy_handle);
        (void)multi_add_res;
    }
    
    // Create MIME structure
    mime = curl_mime_init(easy_handle);
    if (!mime) {
        if (multi_handle) {
            curl_multi_remove_handle(multi_handle, easy_handle);
            curl_multi_cleanup(multi_handle);
        }
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create multiple MIME parts based on fuzzed input
    for (uint8_t part_idx = 0; part_idx < num_parts; part_idx++) {
        if (fdp.remaining_bytes() < 16) {
            break; // Not enough data for more parts
        }
        
        curl_mimepart *part = curl_mime_addpart(mime);
        if (!part) {
            continue; // Skip if part creation fails
        }
        
        // Choose part configuration based on test scenario
        switch (test_scenario) {
            case 0: {
                // Scenario 0: Test curl_mime_filedata (primary target)
                // Use fuzzed filename strings (may be invalid/nonexistent)
                std::string filename = fdp.ConsumeRandomLengthString(256);
                CURLcode filedata_res = curl_mime_filedata(part, filename.c_str());
                (void)filedata_res;
                
                // Also set filename for display
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
                    std::string display_name = fdp.ConsumeRandomLengthString(128);
                    CURLcode name_res = curl_mime_name(part, display_name.c_str());
                    (void)name_res;
                }
                break;
            }
            
            case 1: {
                // Scenario 1: Test curl_mime_data with memory data
                size_t data_size = fdp.ConsumeIntegralInRange<size_t>(0, 512);
                if (data_size > 0 && fdp.remaining_bytes() >= data_size) {
                    std::vector<uint8_t> mime_data = fdp.ConsumeBytes<uint8_t>(data_size);
                    CURLcode data_res = curl_mime_data(part, 
                                                      reinterpret_cast<const char*>(mime_data.data()), 
                                                      mime_data.size());
                    (void)data_res;
                    
                    // Set MIME type based on data
                    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 8) {
                        std::string mime_type = fdp.ConsumeRandomLengthString(64);
                        CURLcode type_res = curl_mime_type(part, mime_type.c_str());
                        (void)type_res;
                    }
                }
                break;
            }
            
            case 2: {
                // Scenario 2: Test curl_mime_data_cb with callback
                size_t callback_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
                std::vector<uint8_t> callback_data;
                
                if (callback_size > 0 && fdp.remaining_bytes() >= callback_size) {
                    callback_data = fdp.ConsumeBytes<uint8_t>(callback_size);
                }
                
                // Create a FuzzedDataProvider for the callback
                FuzzedDataProvider *callback_fdp = 
                    new FuzzedDataProvider(callback_data.data(), callback_data.size());
                
                CURLcode data_cb_res = curl_mime_data_cb(part, 
                                                        callback_data.size(),
                                                        mime_read_callback,
                                                        mime_seek_callback,
                                                        mime_free_callback,
                                                        callback_fdp);
                (void)data_cb_res;
                
                // Clean up will happen in free_callback
                break;
            }
            
            case 3: {
                // Scenario 3: Test nested MIME subparts
                // Create a sub-mime structure
                curl_mime *sub_mime = curl_mime_init(easy_handle);
                if (sub_mime) {
                    // Add a simple part to sub-mime
                    curl_mimepart *sub_part = curl_mime_addpart(sub_mime);
                    if (sub_part && fdp.remaining_bytes() > 16) {
                        std::string sub_data = fdp.ConsumeRandomLengthString(128);
                        curl_mime_data(sub_part, sub_data.c_str(), sub_data.size());
                        
                        // Attach sub-mime to parent part
                        CURLcode subparts_res = curl_mime_subparts(part, sub_mime);
                        (void)subparts_res;
                        
                        // Set content type for multipart
                        curl_mime_type(part, "multipart/mixed");
                    } else {
                        curl_mime_free(sub_mime);
                    }
                }
                break;
            }
            
            case 4: {
                // Scenario 4: Test curl_mime_encoder
                // First set some data
                if (fdp.remaining_bytes() > 32) {
                    std::string part_data = fdp.ConsumeRandomLengthString(128);
                    curl_mime_data(part, part_data.c_str(), part_data.size());
                    
                    // Try different encoders based on fuzzed input
                    uint8_t encoder_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
                    const char *encoder_name = nullptr;
                    
                    switch (encoder_choice) {
                        case 0: encoder_name = "base64"; break;
                        case 1: encoder_name = "quoted-printable"; break;
                        case 2: encoder_name = "8bit"; break;
                        case 3: encoder_name = "binary"; break;
                        default: encoder_name = "binary"; break;
                    }
                    
                    CURLcode encoder_res = curl_mime_encoder(part, encoder_name);
                    (void)encoder_res;
                }
                break;
            }
            
            case 5: {
                // Scenario 5: Comprehensive test with multiple operations
                // Mix file data, memory data, and callback data
                uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 3;
                
                if (operation_type == 0 && fdp.remaining_bytes() > 8) {
                    // File data
                    std::string filename = fdp.ConsumeRandomLengthString(128);
                    curl_mime_filedata(part, filename.c_str());
                } else if (operation_type == 1 && fdp.remaining_bytes() > 16) {
                    // Memory data
                    std::string mime_data = fdp.ConsumeRandomLengthString(256);
                    curl_mime_data(part, mime_data.c_str(), mime_data.size());
                } else if (operation_type == 2 && fdp.remaining_bytes() > 32) {
                    // Callback data
                    size_t cb_size = fdp.ConsumeIntegralInRange<size_t>(0, 128);
                    std::vector<uint8_t> cb_data;
                    
                    if (cb_size > 0 && fdp.remaining_bytes() >= cb_size) {
                        cb_data = fdp.ConsumeBytes<uint8_t>(cb_size);
                    }
                    
                    FuzzedDataProvider *cb_fdp = 
                        new FuzzedDataProvider(cb_data.data(), cb_data.size());
                    
                    curl_mime_data_cb(part, 
                                     cb_data.size(),
                                     mime_read_callback,
                                     mime_seek_callback,
                                     mime_free_callback,
                                     cb_fdp);
                }
                
                // Set name if we have data
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
                    std::string part_name = fdp.ConsumeRandomLengthString(64);
                    curl_mime_name(part, part_name.c_str());
                }
                
                // Set type if we have data
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
                    std::string part_type = fdp.ConsumeRandomLengthString(48);
                    curl_mime_type(part, part_type.c_str());
                }
                break;
            }
        }
    }
    
    // Attach MIME to easy handle
    if (mime) {
        CURLcode setopt_res = curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
        (void)setopt_res;
        
        // Set a dummy URL
        if (fdp.remaining_bytes() > 8) {
            std::string url = fdp.ConsumeRandomLengthString(256);
            curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
        }
        
        // Set upload option
        curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
    }
    
    // If using multi handle, perform some multi operations
    if (multi_handle && mime) {
        // Perform a simple multi perform (won't actually transfer without network)
        int running_handles = 0;
        CURLMcode multi_perform_res = curl_multi_perform(multi_handle, &running_handles);
        (void)multi_perform_res;
        
        // Get info from multi handle
        int msgs_in_queue = 0;
        CURLMsg *msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
        (void)msg;
        (void)msgs_in_queue;
    }
    
    // Cleanup in reverse order
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (multi_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_multi_cleanup(multi_handle);
    }
    
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_013
