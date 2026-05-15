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
#include <mprintf.h>
}
//<ID> 290
//<Prompt> []
/*<Combination>: [
*/
//<score> 50, nr_unique_branch: 25
//<Quality> {"density":10,"unique_branches":{"cpool_get_instance":[[261,13,261,29,0,0,4,0]],"easy_transfer":[[688,9,688,14,0,0,4,0],[688,9,688,14,0,0,4,1],[688,18,688,26,0,0,4,0],[693,8,693,16,0,0,4,0],[697,8,697,16,0,0,4,0],[697,20,697,34,0,0,4,0],[697,20,697,34,0,0,4,1],[700,10,700,13,0,0,4,0],[708,6,708,13,0,0,4,1]],"curl_multi_remove_handle":[[856,32,856,49,0,0,4,1]],"easy_connection":[[1203,6,1203,29,0,0,4,1],[1210,6,1210,28,0,0,4,0]],"easy_perform":[[751,6,751,17,0,0,4,1],[758,6,758,16,0,0,4,1],[769,6,769,22,0,0,4,1],[775,8,775,14,0,0,4,1],[779,6,779,24,0,0,4,1],[788,6,788,13,0,0,4,1],[802,12,802,18,0,0,4,1]],"multi_xfers_add":[[1289,24,1289,33,1,0,4,1]],"Curl_close":[[227,33,227,49,0,0,4,0]],"Curl_multi_handle":[[314,6,314,44,0,0,4,0],[315,8,315,52,0,0,4,1]],"curl_multi_setopt":[[3341,3,3341,27,0,0,4,0]]},"library_calls":["curl_global_init","curl_version_info","curl_easy_init","curl_free","curl_global_cleanup","curl_easy_cleanup","curl_free","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_easy_getinfo","curl_easy_getinfo","curl_easy_recv","curl_easy_strerror","curl_easy_cleanup","curl_free","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version_info","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_easy_getinfo","curl_easy_getinfo","curl_easy_recv","curl_easy_strerror","curl_easy_cleanup","curl_free","curl_global_cleanup"],"visited":1}
/**/


// Helper functions to use curl_mvsnprintf and curl_mvaprintf
static int  id_000025_my_curl_mvsnprintf(char *buffer, size_t maxlength, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvsnprintf(buffer, maxlength, format, args);
    va_end(args);
    return result;
}

static char*  id_000025_my_curl_mvaprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char* result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 1. Get version information using curl_version_info
    curl_version_info_data *version_info = curl_version_info(CURLVERSION_NOW);
    
    // 2. Use curl_mvaprintf to format version info
    char *version_str =  id_000025_my_curl_mvaprintf(
        "CURL Version: %s\nProtocols: %s\nFeatures: %d\nSSL Version: %s",
        version_info->version,
        (version_info->protocols && version_info->protocols[0]) ? version_info->protocols[0] : "none",
        version_info->features,
        version_info->ssl_version
    );
    
    // 3. Use curl_mvsnprintf to format into fixed buffer
    char buffer[1024];
    int printed =  id_000025_my_curl_mvsnprintf(
        buffer, 
        sizeof(buffer),
        "Input size: %zu\nVersion: %s\n",
        size,
        version_info->version
    );
    
    // 4. Create a CURL handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_free(version_str);
        curl_global_cleanup();
        return 0;
    }
    
    // 5. Use the input data to set up a memory buffer for receiving
    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(curl);
        curl_free(version_str);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Get file descriptor for the input file
    int fd = fuzz_fileno(in_file);
    (void)fd; // Use fd to avoid unused variable warning
    
    // 7. Set up CURL options
    // We'll use a dummy URL since we're just testing the APIs
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L); // Connect only, no transfer
    
    // 8. Try to perform the connection (will likely fail, but that's OK for fuzzing)
    CURLcode res = curl_easy_perform(curl);
    
    // 9. Use curl_easy_getinfo to get information about the handle
    char *effective_url = nullptr;
    long response_code = 0;
    
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    
    // 10. Use curl_easy_recv to try to receive data
    // Since we're in CONNECT_ONLY mode and likely not connected, this will fail
    // but we call it anyway to exercise the API
    char recv_buffer[256];
    size_t n = 0;
    CURLcode recv_res = curl_easy_recv(curl, recv_buffer, sizeof(recv_buffer), &n);
    
    // 11. Create an output file to write some debug info
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write the formatted strings
        if (version_str) {
            fwrite(version_str, 1, strlen(version_str), out_file);
        }
        fwrite(buffer, 1, printed > 0 ? printed : 0, out_file);
        
        // Write some info about the curl operation
        const char *error_str = curl_easy_strerror(res);
        fwrite(error_str, 1, strlen(error_str), out_file);
        fwrite("\n", 1, 1, out_file);
        
        assert_file_closed(&out_file);;
    }
    
    // 12. Cleanup
    assert_file_closed(&in_file);;
    curl_easy_cleanup(curl);
    curl_free(version_str);
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}