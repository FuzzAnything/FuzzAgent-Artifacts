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
//<ID> 79
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":28,"unique_branches":{},"library_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_socket_action","curl_easy_reset","curl_easy_setopt","curl_easy_setopt","curl_easy_header","curl_easy_getinfo","curl_unescape","curl_free","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_socket_action","curl_easy_reset","curl_easy_setopt","curl_easy_setopt","curl_easy_header","curl_easy_getinfo","curl_unescape","curl_free","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<31) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);

    // Create multi handle and easy handle
    CURLM *multi_handle = curl_multi_init();
    CURL *easy_handle = curl_easy_init();
    if (!multi_handle || !easy_handle) {
        if (easy_handle) curl_easy_cleanup(easy_handle);
        if (multi_handle) curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create MIME structure for form data
    curl_mime *mime = curl_mime_init(easy_handle);
    curl_mimepart *part = curl_mime_addpart(mime);

    // Create subparts MIME structure
    curl_mime *submime = curl_mime_init(easy_handle);
    curl_mimepart *subpart = curl_mime_addpart(submime);

    // Set up subpart with data from input
    size_t data_size = size > 100 ? 100 : size;
    curl_mime_data(subpart, (const char*)data, data_size);
    
    // Attach subparts to main part
    curl_mime_subparts(part, submime);

    // Set up easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file:///dev/null");
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);

    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    int running_handles = 0;
    // Perform multi handle operations
    curl_multi_perform(multi_handle, &running_handles);

    // Use socket action API (simulate with timeout socket)
    curl_multi_socket_action(multi_handle, CURL_SOCKET_TIMEOUT, fuzz_int32_t_1, &running_handles);

    // Reset the easy handle
    curl_easy_reset(easy_handle);

    // Set up easy handle again
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file:///dev/null");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);

    // Try to get header information (will likely fail for file protocol)
    struct curl_header *header = NULL;
    curl_easy_header(easy_handle, fuzz_str_2, fuzz_uint64_t_3, CURLH_HEADER, fuzz_int32_t_4, &header);

    // Get easy handle information
    char *effective_url = NULL;
    curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &effective_url);

    // Unescape some data
    size_t unescape_len = size > 50 ? 50 : size;
    char *unescaped = curl_unescape((const char*)data, unescape_len);
    if (unescaped) {
        curl_free(unescaped);
    }

    // Clean up
    curl_mime_free(mime);
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();
    
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}