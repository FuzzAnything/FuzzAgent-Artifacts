#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_003 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0; // Need minimum input for meaningful MIME testing
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize curl globally
    CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // 1. curl_easy_init
    CURL* curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // 2. curl_mime_init
    curl_mime* mime = curl_mime_init(curl);
    if (!mime) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    // Determine number of MIME parts to create (1-4 parts)
    size_t num_parts = fdp.ConsumeIntegralInRange<size_t>(1, 4);

    // 3. curl_mime_addpart (multiple parts)
    for (size_t i = 0; i < num_parts; i++) {
        curl_mimepart* part = curl_mime_addpart(mime);
        if (!part) {
            continue;
        }

        // Determine which MIME functions to apply to this part
        uint8_t part_config = fdp.ConsumeIntegral<uint8_t>();

        // For each part, use combinations of:
        // curl_mime_data, curl_mime_filedata, curl_mime_name, 
        // curl_mime_filename, curl_mime_type, curl_mime_encoder

        // 4. curl_mime_name - set part name if bit 0 is set
        if ((part_config & 0x01) && fdp.remaining_bytes() > 4) {
            std::string part_name = fdp.ConsumeRandomLengthString(50);
            curl_mime_name(part, part_name.c_str());
        }

        // 5. curl_mime_filename - set filename if bit 1 is set
        if ((part_config & 0x02) && fdp.remaining_bytes() > 4) {
            std::string filename = fdp.ConsumeRandomLengthString(50);
            curl_mime_filename(part, filename.c_str());
        }

        // 6. curl_mime_type - set MIME type if bit 2 is set
        if ((part_config & 0x04) && fdp.remaining_bytes() > 4) {
            std::string mime_type = fdp.ConsumeRandomLengthString(50);
            curl_mime_type(part, mime_type.c_str());
        }

        // 7. curl_mime_encoder - set encoder if bit 3 is set
        if ((part_config & 0x08) && fdp.remaining_bytes() > 4) {
            std::string encoder = fdp.ConsumeRandomLengthString(20);
            curl_mime_encoder(part, encoder.c_str());
        }

        // 8. curl_mime_data or curl_mime_filedata - based on bit 4
        if ((part_config & 0x10) && fdp.remaining_bytes() > 4) {
            // Use curl_mime_filedata with a dummy filename
            // Since we can't create actual files during fuzzing,
            // we'll use a dummy filename and expect it to fail gracefully
            std::string dummy_file = fdp.ConsumeRandomLengthString(30);
            curl_mime_filedata(part, dummy_file.c_str());
        } else if (fdp.remaining_bytes() > 4) {
            // Use curl_mime_data with fuzzed content
            size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(1024, fdp.remaining_bytes()));
            std::vector<uint8_t> mime_data = fdp.ConsumeBytes<uint8_t>(data_size);
            
            // Convert to string for curl_mime_data
            std::string data_str(reinterpret_cast<const char*>(mime_data.data()), mime_data.size());
            curl_mime_data(part, data_str.c_str(), data_str.size());
        }
    }

    // 5. curl_easy_setopt with CURLOPT_MIMEPOST
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

    // Set some safe options to prevent network operations
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10L);
    
    // Disable actual network operations by setting a dummy URL
    // that won't be used since we're not performing the transfer
    curl_easy_setopt(curl, CURLOPT_URL, "file:///dev/null");

    // 6. curl_mime_free
    curl_mime_free(mime);

    // 7. curl_easy_cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}

} // namespace harness_003
