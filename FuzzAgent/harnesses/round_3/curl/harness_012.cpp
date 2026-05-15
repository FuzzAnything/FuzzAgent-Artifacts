#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_012 {
/*
 * Fuzzing harness for libcurl's multi interface socket assignment operations
 * Targets: curl_multi_assign (completely uncovered, 4 undiscovered branches)
 * Focus on multi interface socket assignment which is 0% covered
 * Test scenarios: socket assignment with various socket values and user data pointers
 */


// Structure to hold user data for socket assignment
struct socket_user_data {
    uint32_t id;
    uint8_t type;
    bool active;
    char name[32];
};

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum input for meaningful fuzzing
    // We need: socket value (int), user data id (uint32_t), type (uint8_t), 
    // active (bool), name (string), URL (string)
    if (size < 80) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    int socket_value = fdp.ConsumeIntegral<int>();
    uint32_t user_data_id = fdp.ConsumeIntegral<uint32_t>();
    uint8_t user_data_type = fdp.ConsumeIntegral<uint8_t>();
    bool user_data_active = fdp.ConsumeBool();
    
    // Consume strings for user data name and URL
    std::string user_data_name = fdp.ConsumeRandomLengthString(31);
    std::string test_url = fdp.ConsumeRandomLengthString(256);
    
    // Variables for handles and results
    CURL *easy_handle = NULL;
    CURLM *multi_handle = NULL;
    CURLcode easy_result = CURLE_OK;
    CURLMcode multi_result = CURLM_OK;
    
    // Step 1: Call curl_global_init to initialize global state
    easy_result = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (easy_result != CURLE_OK) {
        return 0;
    }
    
    // Step 2: Call curl_multi_init to create a multi handle
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Step 3: Call curl_easy_init to create an easy handle
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Set URL option on the easy handle using fuzzer input
    curl_easy_setopt(easy_handle, CURLOPT_URL, test_url.c_str());
    
    // Step 4: Call curl_multi_add_handle to add the easy handle to multi
    multi_result = curl_multi_add_handle(multi_handle, easy_handle);
    if (multi_result != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Step 5: Create socket user data structure
    struct socket_user_data *user_data = 
        (struct socket_user_data *)malloc(sizeof(struct socket_user_data));
    if (user_data) {
        user_data->id = user_data_id;
        user_data->type = user_data_type;
        user_data->active = user_data_active;
        strncpy(user_data->name, user_data_name.c_str(), sizeof(user_data->name) - 1);
        user_data->name[sizeof(user_data->name) - 1] = '\0';
    }
    
    // Step 6: Call curl_multi_assign with the multi handle, socket, and user data pointer
    // This is the primary target API with 4 undiscovered branches
    curl_socket_t socket_fd = (curl_socket_t)socket_value;
    
    // Test various scenarios for curl_multi_assign:
    // 1. Assign with valid user data pointer
    multi_result = curl_multi_assign(multi_handle, socket_fd, (void *)user_data);
    
    // 2. Test with CURL_SOCKET_BAD (invalid socket)
    if (fdp.ConsumeBool()) {
        multi_result = curl_multi_assign(multi_handle, CURL_SOCKET_BAD, (void *)user_data);
    }
    
    // 3. Test with NULL user data (clearing assignment)
    if (fdp.ConsumeBool()) {
        multi_result = curl_multi_assign(multi_handle, socket_fd, NULL);
    }
    
    // 4. Test with different socket values
    int alt_socket_value = fdp.ConsumeIntegral<int>();
    if (fdp.ConsumeBool()) {
        curl_socket_t alt_socket_fd = (curl_socket_t)alt_socket_value;
        multi_result = curl_multi_assign(multi_handle, alt_socket_fd, (void *)user_data);
    }
    
    // Step 7: Call curl_multi_remove_handle to remove easy handle
    multi_result = curl_multi_remove_handle(multi_handle, easy_handle);
    
    // Step 8: Call curl_easy_cleanup to cleanup easy handle
    curl_easy_cleanup(easy_handle);
    
    // Clean up user data if allocated
    if (user_data) {
        free(user_data);
    }
    
    // Step 9: Call curl_multi_cleanup to cleanup multi handle
    curl_multi_cleanup(multi_handle);
    
    // Step 10: Call curl_global_cleanup to cleanup global state
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_012
