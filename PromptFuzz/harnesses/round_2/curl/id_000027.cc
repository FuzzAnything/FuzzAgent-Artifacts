#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <multi.h>
}
//<ID> 326
//<Prompt> []
/*<Combination>: [
*/
//<score> 126, nr_unique_branch: 22
//<Quality> {"density":21,"unique_branches":{"Curl_mntfy_disable":[[158,6,158,34,0,0,4,1]],"curl_multi_strerror":[[329,10,329,15,0,0,4,1],[330,3,330,32,0,0,4,1],[333,3,333,16,0,0,4,0],[336,3,336,24,0,0,4,1],[339,3,339,29,0,0,4,1],[342,3,342,27,0,0,4,1],[345,3,345,28,0,0,4,1],[348,3,348,24,0,0,4,1],[351,3,351,28,0,0,4,1],[354,3,354,27,0,0,4,1],[357,3,357,32,0,0,4,1],[360,3,360,28,0,0,4,1],[363,3,363,35,0,0,4,1],[366,3,366,33,0,0,4,1],[369,3,369,32,0,0,4,1],[372,3,372,18,0,0,4,1]],"Curl_cpool_upkeep":[[752,6,752,12,0,0,4,0]],"cpool_get_instance":[[263,13,263,24,0,0,4,1]],"curl_multi_notify_disable":[[4069,6,4069,24,0,0,4,1],[83,5,83,8,1,0,4,0],[83,12,83,43,1,0,4,0]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_multi_setopt","curl_getenv","curl_strequal","curl_free","curl_easy_upkeep","curl_multi_notify_disable","curl_multi_timeout","curl_multi_waitfds","curl_multi_strerror","curl_multi_strerror","curl_multi_strerror","curl_multi_strerror","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_multi_setopt","curl_getenv","curl_strequal","curl_free","curl_easy_upkeep","curl_multi_notify_disable","curl_multi_timeout","curl_multi_waitfds","curl_multi_strerror","curl_multi_strerror","curl_multi_strerror","curl_multi_strerror","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified curl APIs to handle curl multi operations and environment variable processing:

*/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 2. Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // 3. Create an easy handle for upkeep
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // 4. Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // 5. Get file descriptor for the input file
    int input_fd = fuzz_fileno(in_file);
    if (input_fd < 0) {
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_fd_closed(input_fd);
	return 0;
    }

    // 6. Set multi handle options
    CURLMcode mcode = curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)10);
    
    // Create environment variable name from first part of input
    char *env_name = nullptr;
    if (size > 0) {
        // Use first 16 bytes or less for env var name
        size_t name_len = size > 16 ? 16 : size;
        env_name = (char *)malloc(name_len + 1);
        if (env_name) {
            memcpy(env_name, data, name_len);
            env_name[name_len] = '\0';
            
            // 7. Get environment variable
            char *env_value = curl_getenv(env_name);
            if (env_value) {
                // 8. Use curl_strequal to compare strings
                int are_equal = curl_strequal(env_name, env_value);
                (void)are_equal; // Prevent unused warning
                curl_free(env_value);
            }
            free(env_name);
        }
    }

    // 9. Call easy handle upkeep
    CURLcode upcode = curl_easy_upkeep(easy_handle);

    // 10. Disable notifications
    CURLMcode notify_code = curl_multi_notify_disable(multi_handle, 0);

    // 11. Get timeout from multi handle
    long timeout = 0;
    CURLMcode timeout_code = curl_multi_timeout(multi_handle, &timeout);

    // 12. Prepare for waitfds call
    struct curl_waitfd wait_fds[1];
    wait_fds[0].fd = input_fd;
    wait_fds[0].events = CURL_WAIT_POLLIN;
    wait_fds[0].revents = 0;
    
    unsigned int fd_count = 0;
    unsigned int max_fds = 1;
    
    // 13. Get file descriptors to wait on
    CURLMcode waitfds_code = curl_multi_waitfds(multi_handle, wait_fds, max_fds, &fd_count);

    // 14. Process errors using curl_multi_strerror
    const char *error_str = curl_multi_strerror(mcode);
    (void)error_str; // Prevent unused warning
    error_str = curl_multi_strerror(notify_code);
    (void)error_str;
    error_str = curl_multi_strerror(timeout_code);
    (void)error_str;
    error_str = curl_multi_strerror(waitfds_code);
    (void)error_str;

    // 15. Try to read a small amount from the input file to consume data
    if (in_file) {
        char buffer[128];
        size_t bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
        (void)bytes_read; // Prevent unused warning
    }

    // 16. Add easy handle to multi handle and perform
    mcode = curl_multi_add_handle(multi_handle, easy_handle);
    
    int running_handles = 0;
    if (mcode == CURLM_OK) {
        // Perform the multi handle
        mcode = curl_multi_perform(multi_handle, &running_handles);
        
        // Remove the easy handle
        curl_multi_remove_handle(multi_handle, easy_handle);
    }

    // 17. Cleanup
    assert_file_closed(&in_file);;
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    assert_file_closed(&in_file);
	assert_fd_closed(input_fd);
	return 0;
}