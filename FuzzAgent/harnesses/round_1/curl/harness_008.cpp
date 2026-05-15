#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_008 {


// Callback function for curl_formget
static size_t formget_callback(void *arg, const char *buf, size_t len) {
    // Simply accumulate the length (or could write to buffer)
    size_t* total = static_cast<size_t*>(arg);
    if (total) {
        *total += len;
    }
    return len; // Return the length we processed
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0; // Need minimum input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize curl globally (though not strictly needed for form APIs)
    CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // Step 1: Create curl easy handle
    CURL* curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Step 2: Initialize formpost structure
    struct curl_httppost* formpost = NULL;
    struct curl_httppost* last_post = NULL;
    CURLFORMcode form_result;
    size_t callback_total = 0;

    // Consume data for form configuration
    // Determine how many form fields to create (1-5)
    int num_fields = fdp.ConsumeIntegralInRange<int>(1, 5);

    for (int i = 0; i < num_fields && fdp.remaining_bytes() > 10; i++) {
        // Consume field name
        std::string field_name = fdp.ConsumeRandomLengthString(50);
        
        // Consume field type (0-3 to decide what kind of field to create)
        uint8_t field_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (field_type) {
            case 0: {
                // Regular text field
                std::string field_content = fdp.ConsumeRandomLengthString(100);
                form_result = curl_formadd(&formpost, &last_post,
                    CURLFORM_COPYNAME, field_name.c_str(),
                    CURLFORM_COPYCONTENTS, field_content.c_str(),
                    CURLFORM_END);
                break;
            }
            case 1: {
                // Field with content type
                std::string field_content = fdp.ConsumeRandomLengthString(100);
                std::string content_type = fdp.ConsumeRandomLengthString(50);
                form_result = curl_formadd(&formpost, &last_post,
                    CURLFORM_COPYNAME, field_name.c_str(),
                    CURLFORM_COPYCONTENTS, field_content.c_str(),
                    CURLFORM_CONTENTTYPE, content_type.c_str(),
                    CURLFORM_END);
                break;
            }
            case 2: {
                // Field with pointer contents (don't copy)
                if (fdp.remaining_bytes() > 20) {
                    std::string ptr_content = fdp.ConsumeRandomLengthString(50);
                    // Keep the string alive in a vector
                    static std::vector<std::string> kept_strings;
                    kept_strings.push_back(ptr_content);
                    form_result = curl_formadd(&formpost, &last_post,
                        CURLFORM_COPYNAME, field_name.c_str(),
                        CURLFORM_PTRCONTENTS, kept_strings.back().c_str(),
                        CURLFORM_CONTENTSLENGTH, kept_strings.back().length(),
                        CURLFORM_END);
                } else {
                    // Fallback to regular field if not enough data
                    std::string field_content = fdp.ConsumeRandomLengthString(50);
                    form_result = curl_formadd(&formpost, &last_post,
                        CURLFORM_COPYNAME, field_name.c_str(),
                        CURLFORM_COPYCONTENTS, field_content.c_str(),
                        CURLFORM_END);
                }
                break;
            }
            case 3: {
                // File upload field (simulated with buffer)
                std::string filename = fdp.ConsumeRandomLengthString(50);
                if (fdp.remaining_bytes() > 50) {
                    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(10, 200);
                    std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
                    // Keep buffer alive in a static container until curl_formfree is called
                    static std::vector<std::vector<uint8_t>> kept_buffers;
                    kept_buffers.push_back(buffer); // Copy the buffer
                    form_result = curl_formadd(&formpost, &last_post,
                        CURLFORM_COPYNAME, field_name.c_str(),
                        CURLFORM_BUFFER, filename.c_str(),
                        CURLFORM_BUFFERPTR, kept_buffers.back().data(),
                        CURLFORM_BUFFERLENGTH, kept_buffers.back().size(),
                        CURLFORM_END);
                } else {
                    // Fallback
                    std::string field_content = fdp.ConsumeRandomLengthString(50);
                    form_result = curl_formadd(&formpost, &last_post,
                        CURLFORM_COPYNAME, field_name.c_str(),
                        CURLFORM_COPYCONTENTS, field_content.c_str(),
                        CURLFORM_END);
                }
                break;
            }
            default:
                // Should not happen
                continue;
        }
        
        // Check form add result (but continue even if some fail)
        (void)form_result;
    }

    // Step 3: Only proceed if we successfully created a form
    if (formpost != NULL) {
        // Target API: curl_formget with callback
        int formget_result = curl_formget(formpost, &callback_total, formget_callback);
        (void)formget_result; // Result checking not critical for fuzzing
        
        // Step 4: Clean up formpost structure
        curl_formfree(formpost);
    }

    // Step 5: Clean up curl handle
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}

} // namespace harness_008
