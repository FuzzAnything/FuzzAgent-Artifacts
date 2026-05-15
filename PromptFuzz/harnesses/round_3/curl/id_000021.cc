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
//<ID> 249
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 18
//<Quality> {"density":18,"unique_branches":{"out_number":[[735,11,735,28,0,0,4,0],[737,14,737,35,0,0,4,1],[771,3,771,10,0,0,4,0],[772,11,772,18,0,0,4,0],[772,11,772,18,0,0,4,1]],"curl_multi_get_offt":[[3828,6,3828,24,0,0,4,1],[3830,6,3830,13,0,0,4,1],[3834,3,3834,31,0,0,4,1],[3840,3,3840,31,0,0,4,1],[3846,3,3846,31,0,0,4,1],[3849,3,3849,28,0,0,4,1],[3852,3,3852,29,0,0,4,1],[3855,3,3855,10,0,0,4,0],[83,5,83,8,1,0,4,0],[83,12,83,43,1,0,4,0]],"parsefmt":[[417,7,417,15,0,0,4,0],[418,12,418,34,0,0,4,1],[420,17,420,35,0,0,4,1]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_global_cleanup","curl_mime_filedata","curl_share_init","curl_share_setopt","curl_share_strerror","curl_share_cleanup","curl_slist_append","curl_mime_headers","curl_multi_init","curl_multi_add_handle","curl_multi_get_offt","curl_multi_remove_handle","curl_multi_cleanup","curl_msprintf","curl_mime_addpart","curl_mime_data","curl_easy_setopt","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_share_init","curl_share_setopt","curl_share_strerror","curl_share_cleanup","curl_slist_append","curl_mime_headers","curl_multi_init","curl_multi_add_handle","curl_multi_get_offt","curl_multi_remove_handle","curl_multi_cleanup","curl_msprintf","curl_mime_addpart","curl_mime_data","curl_easy_setopt","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified curl APIs to simulate a scenario where curl processes multipart form data with file upload, handles share errors, and logs information:

*/


// Include necessary curl headers

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create temporary input file with fuzzed data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create mime structure
    curl_mime *mime = curl_mime_init(easy);
    if (!mime) {
        curl_easy_cleanup(easy);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // API 1: Set file data for mime part using the input file
    // First, write data to a temporary file for curl_mime_filedata
    FILE *temp_file = fopen("temp_input_file", "wb");
    if (temp_file) {
        fwrite(data, 1, size, temp_file);
        assert_file_closed(&temp_file);;
        
        CURLcode res = curl_mime_filedata(part, fuzz_str_1);
        // Result intentionally ignored for fuzzing
    }

    // API 2: Create share handle and get error string
    CURLSH *share = curl_share_init();
    if (share) {
        // Set an invalid option to generate error
        CURLSHcode share_res = curl_share_setopt(share, (CURLSHoption)9999, NULL);
        const char *error_str = curl_share_strerror(share_res);
        // Use the error string (prevent unused variable warning)
        if (error_str) {
            // Do nothing, just reference it
        }
        curl_share_cleanup(share);
    }

    // API 3: Create headers and set them for mime part
    struct curl_slist *headers = NULL;
    // Create header from first part of input data
    size_t header_len = size > 100 ? 100 : size;
    char *header_data = (char *)malloc(header_len + 20);
    if (header_data) {
        snprintf(header_data, header_len + 20, "X-Fuzz-Header: ");
        memcpy(header_data + 15, data, header_len);
        header_data[15 + header_len] = '\0';
        
        headers = curl_slist_append(headers, header_data);
        free(header_data);
        
        if (headers) {
            // Take ownership of headers
            CURLcode header_res = curl_mime_headers(part, headers, 1);
            // Result intentionally ignored for fuzzing
        }
    }

    // API 4: Create multi handle and get offset value
    CURLM *multi = curl_multi_init();
    if (multi) {
        // Add easy handle to multi
        CURLMcode multi_res = curl_multi_add_handle(multi, easy);
        
        if (multi_res == CURLM_OK) {
            curl_off_t offset_value = 0;
            // Try to get offset value (CURLMINFO_OFFT_SENT is typically 0x300000 + 4)
            CURLMcode off_res = curl_multi_get_offt(multi, (CURLMinfo_offt)0x300004, &offset_value);
            // Result intentionally ignored for fuzzing
        }
        
        // Clean up multi
        curl_multi_remove_handle(multi, easy);
        curl_multi_cleanup(multi);
    }

    // API 5: Use curl_msprintf to format a string
    char buffer[256];
    int printed = curl_msprintf(buffer, "Fuzzed %zu bytes, first byte: 0x%02x", 
                                size, data[0]);
    // Use the printed value (prevent unused variable warning)
    if (printed > 0) {
        // Do nothing, just reference it
    }

    // Consume input data by setting it as mime data if file wasn't set
    if (size > 0) {
        // Create another mime part with raw data
        curl_mimepart *data_part = curl_mime_addpart(mime);
        if (data_part) {
            // Use a safe copy of the data
            char *safe_data = (char *)malloc(size);
            if (safe_data) {
                memcpy(safe_data, data, size);
                curl_mime_data(data_part, safe_data, size);
                // Note: curl_mime_data makes its own copy, so we need to free ours
                free(safe_data);
            }
        }
    }

    // Set the mime data to easy handle
    curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);

    // Clean up
    curl_mime_free(mime);
    curl_easy_cleanup(easy);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("temp_input_file");
    remove("output_file");
    
    // Cleanup curl
    curl_global_cleanup();

    assert_file_closed(&temp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}