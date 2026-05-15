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
//<ID> 455
//<Prompt> []
/*<Combination>: [
*/
//<score> 33, nr_unique_branch: 2
//<Quality> {"density":22,"unique_branches":{"setopt_cptr":[[2012,3,2012,26,0,0,4,0]],"curl_easy_nextheader":[[133,6,133,36,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_mime_filedata","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_add_handle","curl_multi_timeout","curl_multi_poll","curl_easy_recv","curl_easy_ssls_export","curl_multi_remove_handle","curl_easy_nextheader","curl_free","curl_easy_reset","curl_multi_add_handle","curl_multi_poll","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_mime_filedata","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_add_handle","curl_multi_timeout","curl_multi_poll","curl_easy_recv","curl_easy_ssls_export","curl_multi_remove_handle","curl_easy_nextheader","curl_free","curl_easy_reset","curl_multi_add_handle","curl_multi_poll","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/**/


// Simple callback function for SSL session export (does nothing)
CURLcode  id_000036_ssls_export_callback(CURL *handle, void *arg, const char *session_key,
                              const unsigned char *shmac, size_t shmac_len,
                              const unsigned char *sdata, size_t sdata_len,
                              curl_off_t sdata_off, int sdata_flags,
                              const char *context, size_t context_len) {
    return CURLE_OK;
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Initialize global curl
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create a file from input data for curl_mime_filedata
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file for potential responses
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    CURL *easy_handle = NULL;
    CURLM *multi_handle = NULL;
    curl_mime *mime = NULL;
    curl_mimepart *part = NULL;
    struct curl_waitfd extra_fds;
    int ret = 0;
    long timeout_ms = 0;
    char buffer[1024];
    size_t n = 0;
    struct curl_header *header = NULL;

    // Create easy handle
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        goto cleanup;
    }
    
    // Create mime structure
    mime = curl_mime_init(easy_handle);
    if (!mime) {
        goto cleanup;
    }
    
    // Add mime part
    part = curl_mime_addpart(mime);
    if (!part) {
        goto cleanup;
    }
    
    // Set file data for mime part
    fseek(in_file, 0, SEEK_SET);
    if (curl_mime_filedata(part, "input_file") != CURLE_OK) {
        // Write data to file for curl_mime_filedata to use
        assert_file_closed(&in_file);;
        in_file = fopen("input_file", "wb");
        if (in_file) {
            fwrite(data, 1, size, in_file);
            assert_file_closed(&in_file);;
            in_file = fopen("input_file", "rb");
        }
        // Try again with the file
        if (in_file) {
            curl_mime_filedata(part, "input_file");
        }
    }
    
    // Set various curl options
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com/upload");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
    curl_easy_setopt(easy_handle, CURLOPT_HEADERDATA, out_file);
    
    // Create multi handle
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        goto cleanup;
    }
    
    // Add easy handle to multi handle (first call)
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        goto cleanup;
    }
    
    // Get timeout from multi handle
    curl_multi_timeout(multi_handle, &timeout_ms);
    if (timeout_ms < 0) timeout_ms = 1000;
    
    // Setup extra file descriptor for polling (using output file)
    extra_fds.fd = fuzz_fileno(out_file);
    extra_fds.events = CURL_WAIT_POLLOUT;
    extra_fds.revents = 0;
    
    // Poll on multi handle
    curl_multi_poll(multi_handle, &extra_fds, 1, fuzz_int32_t_1, &ret);
    
    // Try to receive data (though connection may not be established)
    if (curl_easy_recv(easy_handle, buffer, sizeof(buffer), &n) == CURLE_OK) {
        // If we got data, write it to output
        if (n > 0) {
            fwrite(buffer, 1, n, out_file);
        }
    }
    
    // Try to export SSL session (will likely fail but we call it)
    curl_easy_ssls_export(easy_handle,  id_000036_ssls_export_callback, NULL);
    
    // Remove handle from multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // Get next header (try both request and response origins)
    header = curl_easy_nextheader(easy_handle, CURLH_HEADER, fuzz_int32_t_2, NULL);
    if (header) {
        // Free the header if we got one
        curl_free(header);
    }
    
    // Reset easy handle
    curl_easy_reset(easy_handle);
    
    // Add easy handle again (second call to meet requirement)
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Poll again briefly
    curl_multi_poll(multi_handle, NULL, 0, fuzz_int32_t_3, &ret);
    
    // Remove handle again
    curl_multi_remove_handle(multi_handle, easy_handle);

cleanup:
    // Cleanup in reverse order
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
    
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
        remove("input_file");
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
        remove("output_file");
    }
    
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}