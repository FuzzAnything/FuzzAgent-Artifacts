#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_010 {


// Callback functions for FTP read/write operations
static size_t read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    // Get the remaining fuzzer data from userdata
    std::vector<uint8_t>* fuzz_data = static_cast<std::vector<uint8_t>*>(userdata);
    size_t buffer_size = size * nmemb;
    size_t to_copy = std::min(buffer_size, fuzz_data->size());
    
    if (to_copy > 0) {
        memcpy(ptr, fuzz_data->data(), to_copy);
        // Remove copied data
        fuzz_data->erase(fuzz_data->begin(), fuzz_data->begin() + to_copy);
    }
    
    return to_copy;
}

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    // Just discard the data for fuzzing purposes
    return size * nmemb;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // FTP operations require significant input for meaningful testing
    if (size < 32) {
        return 0; // Need minimum input for FTP operations
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Initialize curl globally
    CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // Step 2: Create curl easy handle
    CURL* curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Step 3: Construct FTP URL from fuzzer input
    // Consume parts for FTP URL: protocol, host, port, path, credentials
    std::string ftp_protocol = "ftp://";
    std::string ftp_host = fdp.ConsumeRandomLengthString(64);
    if (ftp_host.empty()) {
        ftp_host = "localhost";
    }
    
    // Consume port (0 means default port)
    uint16_t ftp_port = fdp.ConsumeIntegral<uint16_t>();
    std::string ftp_port_str = "";
    if (ftp_port > 0 && ftp_port != 21) {
        ftp_port_str = ":" + std::to_string(ftp_port);
    }
    
    // Consume username and password
    std::string ftp_user = fdp.ConsumeRandomLengthString(32);
    std::string ftp_pass = fdp.ConsumeRandomLengthString(32);
    std::string ftp_credentials = "";
    if (!ftp_user.empty()) {
        ftp_credentials = ftp_user;
        if (!ftp_pass.empty()) {
            ftp_credentials += ":" + ftp_pass;
        }
        ftp_credentials += "@";
    }
    
    // Consume file path
    std::string ftp_path = fdp.ConsumeRandomLengthString(128);
    if (ftp_path.empty()) {
        ftp_path = "/testfile.txt";
    }
    
    // Build complete FTP URL
    std::string ftp_url = ftp_protocol + ftp_credentials + ftp_host + ftp_port_str + ftp_path;
    
    // Set FTP URL
    curl_easy_setopt(curl, CURLOPT_URL, ftp_url.c_str());

    // Step 4: Configure FTP transfer mode (upload or download)
    // Consume operation type: 0 = upload, 1 = download, 2 = both
    uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Consume data for file content (for upload) or buffer (for download)
    std::vector<uint8_t> file_data;
    if (fdp.remaining_bytes() > 0) {
        size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(4096, fdp.remaining_bytes()));
        file_data = fdp.ConsumeBytes<uint8_t>(data_size);
    }

    // Configure based on operation type
    switch (operation_type) {
        case 0: // UPLOAD
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
            curl_easy_setopt(curl, CURLOPT_READDATA, &file_data);
            if (!file_data.empty()) {
                curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_data.size());
            }
            break;
            
        case 1: // DOWNLOAD
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 0L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, nullptr);
            break;
            
        case 2: // Test both directions (simulate upload then download)
            // First configure as upload
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
            curl_easy_setopt(curl, CURLOPT_READDATA, &file_data);
            if (!file_data.empty()) {
                curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_data.size());
            }
            break;
    }

    // Step 5: Configure FTP connection mode (active vs passive)
    // Consume mode: 0 = passive (PASV), 1 = active (PORT)
    uint8_t ftp_mode = fdp.ConsumeIntegral<uint8_t>() % 2;
    curl_easy_setopt(curl, CURLOPT_FTPPORT, ftp_mode == 0 ? nullptr : "-"); // Active mode
    curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, fdp.ConsumeBool() ? 1L : 0L);

    // Step 6: Set additional FTP options
    curl_easy_setopt(curl, CURLOPT_FTP_FILEMETHOD, fdp.ConsumeIntegral<uint8_t>() % 4);
    curl_easy_setopt(curl, CURLOPT_FTP_RESPONSE_TIMEOUT, fdp.ConsumeIntegral<long>());
    curl_easy_setopt(curl, CURLOPT_FTP_SSL, fdp.ConsumeIntegral<uint8_t>() % 4);
    
    // Consume FTP commands (POSTQUOTE) if we have enough data
    struct curl_slist* ftp_commands = nullptr;
    if (fdp.remaining_bytes() > 20) {
        size_t num_commands = fdp.ConsumeIntegralInRange<size_t>(0, 3);
        for (size_t i = 0; i < num_commands && fdp.remaining_bytes() > 10; i++) {
            std::string cmd = fdp.ConsumeRandomLengthString(64);
            if (!cmd.empty()) {
                ftp_commands = curl_slist_append(ftp_commands, cmd.c_str());
            }
        }
        if (ftp_commands) {
            curl_easy_setopt(curl, CURLOPT_POSTQUOTE, ftp_commands);
        }
    }

    // Step 7: Execute FTP operation (targets ftp_perform and FTP state machine)
    // Note: In fuzzing context without network, this will fail but still exercise
    // the FTP state machine initialization and error handling paths
    CURLcode perform_result = curl_easy_perform(curl);

    // Step 8: Clean up FTP commands list
    if (ftp_commands) {
        curl_slist_free_all(ftp_commands);
    }

    // Step 9: Cleanup curl handle
    curl_easy_cleanup(curl);
    
    // Step 10: Global cleanup
    curl_global_cleanup();

    return 0;
}

} // namespace harness_010
