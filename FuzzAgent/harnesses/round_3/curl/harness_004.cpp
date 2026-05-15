#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_004 {
/*
 * Fuzzing harness for libcurl's MIME (multipart form data) operations
 * Targets: curl_mime_init (0% covered, 4 undiscovered branches)
 *          curl_mime_headers (0% covered, 11 undiscovered branches)
 *          curl_mime_filename (0% covered, 8 undiscovered branches)
 *          curl_mime_free (0% covered, 5 undiscovered branches)
 *          curl_mime_data_cb (0% covered, 4 undiscovered branches)
 * Focus on mime.c module which has only 0.15% region coverage
 * Following invocation sequence from coverage guidance:
 * curl_easy_init -> curl_mime_init -> curl_mime_addpart -> curl_mime_headers ->
 * curl_mime_filename -> curl_mime_data_cb -> curl_easy_setopt with CURLOPT_MIMEPOST ->
 * curl_mime_free -> curl_easy_cleanup
 */


// Callback functions for curl_mime_data_cb
static size_t read_callback(char *buffer, size_t size, size_t nitems, void *instream) {
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

static int seek_callback(void *instream, curl_off_t offset, int origin) {
    (void)instream;
    (void)offset;
    (void)origin;
    // For fuzzing purposes, we'll just return CURL_SEEKFUNC_OK
    return CURL_SEEKFUNC_OK;
}

static void free_callback(void *instream) {
    (void)instream;
    // Nothing to free since we're using FuzzedDataProvider
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum input for meaningful fuzzing - enough for basic parameters
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
    
    // Step 2: Create mime structure
    curl_mime *mime = curl_mime_init(easy);
    if (!mime) {
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }
    
    // Step 3: Add mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }
    
    // Step 4: Set mime headers from fuzzed input
    // Create headers list
    struct curl_slist *headers = nullptr;
    size_t num_headers = fdp.ConsumeIntegralInRange<size_t>(0, 5);
    
    for (size_t i = 0; i < num_headers && fdp.remaining_bytes() > 10; i++) {
        std::string header_name = fdp.ConsumeRandomLengthString(32);
        std::string header_value = fdp.ConsumeRandomLengthString(64);
        std::string full_header = header_name + ": " + header_value;
        headers = curl_slist_append(headers, full_header.c_str());
    }
    
    // Test curl_mime_headers with take_ownership = 0 (don't take ownership)
    if (headers != nullptr) {
        CURLcode header_res = curl_mime_headers(part, headers, 0);
        (void)header_res; // Use the result
        // Clean up headers since we didn't transfer ownership
        curl_slist_free_all(headers);
    }
    
    // Also test with a single header and take_ownership = 1
    if (fdp.remaining_bytes() > 20) {
        std::string single_header = fdp.ConsumeRandomLengthString(50);
        struct curl_slist *single_header_list = curl_slist_append(nullptr, single_header.c_str());
        if (single_header_list != nullptr) {
            CURLcode single_header_res = curl_mime_headers(part, single_header_list, 1);
            (void)single_header_res; // Ownership transferred to curl
        }
    }
    
    // Step 5: Set filename from fuzzed input
    if (fdp.remaining_bytes() > 10) {
        std::string filename = fdp.ConsumeRandomLengthString(128);
        CURLcode filename_res = curl_mime_filename(part, filename.c_str());
        (void)filename_res;
    }
    
    // Step 6: Set data callback from fuzzed input
    // Create a separate FuzzedDataProvider for the callback data
    size_t callback_data_size = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
    std::vector<uint8_t> callback_data;
    
    if (callback_data_size > 0 && fdp.remaining_bytes() >= callback_data_size) {
        callback_data = fdp.ConsumeBytes<uint8_t>(callback_data_size);
    }
    
    // Create a FuzzedDataProvider for the callback to use
    FuzzedDataProvider *callback_fdp = new FuzzedDataProvider(callback_data.data(), callback_data.size());
    
    curl_off_t datasize = callback_data.size();
    CURLcode data_cb_res = curl_mime_data_cb(part, datasize,
                                            read_callback,
                                            seek_callback,
                                            free_callback,
                                            callback_fdp);
    (void)data_cb_res;
    
    // Step 7: Attach mime to easy handle
    CURLcode setopt_res = curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
    (void)setopt_res;
    
    // Step 8: Set a dummy URL (required for some operations)
    if (fdp.remaining_bytes() > 10) {
        std::string url = fdp.ConsumeRandomLengthString(256);
        curl_easy_setopt(easy, CURLOPT_URL, url.c_str());
    }
    
    // Set other options that might be relevant for MIME
    curl_easy_setopt(easy, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(easy, CURLOPT_READFUNCTION, nullptr);
    
    // Step 9: Clean up mime structure
    curl_mime_free(mime);
    
    // Clean up callback FuzzedDataProvider
    delete callback_fdp;
    
    // Step 10: Clean up easy handle
    curl_easy_cleanup(easy);
    
    // Global cleanup
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_004
