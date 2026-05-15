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
//<ID> 671
//<Prompt> []
/*<Combination>: [
*/
//<score> 261.33334, nr_unique_branch: 109
//<Quality> {"density":28,"unique_branches":{"multi_done_locked":[[327,4,327,40,18,0,4,1],[327,4,327,40,20,0,4,1],[319,14,319,33,23,0,4,0],[320,5,320,24,23,0,4,0],[319,14,319,33,27,0,4,0],[320,5,320,24,27,0,4,0]],"curl_multi_add_handle":[[327,4,327,40,16,0,4,1],[319,14,319,33,17,0,4,0],[320,5,320,24,17,0,4,0]],"Curl_infof":[[319,4,319,10,2,0,4,0],[319,14,319,33,2,0,4,0],[320,5,320,24,2,0,4,0]],"async_thrdd_query":[[327,4,327,40,4,0,4,1],[319,14,319,33,5,0,4,0],[320,5,320,24,5,0,4,0]],"Curl_conn_connect":[[324,4,324,8,18,0,4,0],[324,12,324,53,18,0,4,1],[319,14,319,33,24,0,4,0],[320,5,320,24,24,0,4,0]],"trc_write":[[51,6,51,23,0,0,4,0],[52,8,52,24,0,0,4,1],[63,7,63,25,0,0,4,0],[64,7,64,31,0,0,4,1],[65,7,65,30,0,0,4,1],[69,7,69,14,0,0,4,1]],"multi_warn_debug":[[319,14,319,33,4,0,4,0],[320,5,320,24,4,0,4,0],[319,14,319,33,5,0,4,0],[320,5,320,24,5,0,4,0]],"mstate":[[327,4,327,40,8,0,4,1],[319,14,319,33,9,0,4,0],[320,5,320,24,9,0,4,0]],"Curl_conn_terminate":[[145,24,145,35,10,0,4,0],[319,14,319,33,16,0,4,0],[320,5,320,24,16,0,4,0]],"Curl_multi_pollset":[[327,4,327,40,12,0,4,1],[319,14,319,33,18,0,4,0],[320,5,320,24,18,0,4,0]],"Curl_async_shutdown":[[327,4,327,40,4,0,4,1],[319,14,319,33,5,0,4,0],[320,5,320,24,5,0,4,0]],"cf_dns_start":[[324,4,324,8,10,0,4,0],[324,12,324,53,10,0,4,1],[319,14,319,33,13,0,4,0],[320,5,320,24,13,0,4,0]],"Curl_async_destroy":[[327,4,327,40,6,0,4,1],[319,14,319,33,7,0,4,0],[320,5,320,24,7,0,4,0]],"cf_ip_ballers_run":[[324,4,324,8,30,0,4,0],[324,12,324,53,30,0,4,1],[324,4,324,8,31,0,4,0],[324,12,324,53,31,0,4,1],[324,4,324,8,32,0,4,0],[324,12,324,53,32,0,4,1],[319,14,319,33,47,0,4,0],[320,5,320,24,47,0,4,0],[319,14,319,33,49,0,4,0],[320,5,320,24,49,0,4,0],[319,14,319,33,51,0,4,0],[320,5,320,24,51,0,4,0]],"cf_ip_happy_init":[[324,4,324,8,3,0,4,0],[324,12,324,53,3,0,4,1],[319,14,319,33,4,0,4,0],[320,5,320,24,4,0,4,0]],"trc_infof":[[251,6,251,10,0,0,4,1],[253,6,253,12,0,0,4,1],[90,3,90,51,1,0,4,1],[319,4,319,10,9,0,4,0],[319,14,319,33,9,0,4,0],[320,5,320,24,9,0,4,0]],"hostip_resolv":[[327,4,327,40,20,0,4,1],[319,14,319,33,24,0,4,0],[320,5,320,24,24,0,4,0]],"Curl_cw_out_done":[[327,4,327,40,3,0,4,1],[319,14,319,33,4,0,4,0],[320,5,320,24,4,0,4,0]],"trc_end_buf":[[113,6,113,39,0,0,4,1],[113,24,113,29,0,0,4,0],[120,11,120,16,0,0,4,0]],"Curl_thrdq_trace":[[327,4,327,40,1,0,4,1],[319,14,319,33,4,0,4,0],[320,5,320,24,4,0,4,0]],"Curl_connect":[[327,4,327,40,12,0,4,1],[319,14,319,33,13,0,4,0],[320,5,320,24,13,0,4,0]],"curl_multi_remove_handle":[[327,4,327,40,21,0,4,1],[319,14,319,33,22,0,4,0],[320,5,320,24,22,0,4,0]],"Curl_cpool_add":[[327,4,327,40,13,0,4,1],[319,14,319,33,14,0,4,0],[320,5,320,24,14,0,4,0]],"multi_done":[[327,4,327,40,9,0,4,1],[319,14,319,33,10,0,4,0],[320,5,320,24,10,0,4,0]],"Curl_conn_cf_add":[[324,4,324,8,5,0,4,0],[324,12,324,53,5,0,4,1],[319,14,319,33,6,0,4,0],[320,5,320,24,6,0,4,0]],"cf_dns_connect":[[324,4,324,8,12,0,4,0],[324,12,324,53,12,0,4,1],[324,4,324,8,14,0,4,0],[324,12,324,53,14,0,4,1],[319,14,319,33,15,0,4,0],[320,5,320,24,15,0,4,0],[319,14,319,33,19,0,4,0],[320,5,320,24,19,0,4,0]],"Curl_client_reset":[[327,4,327,40,4,0,4,1],[319,14,319,33,7,0,4,0],[320,5,320,24,7,0,4,0]]},"library_calls":["curl_global_init","curl_global_trace","curl_version","curl_mprintf","curl_global_cleanup","curl_global_cleanup","curl_multi_init","curl_global_cleanup","curl_multi_notify_disable","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_data_cb","curl_share_strerror","curl_url_strerror","curl_mime_filedata","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_multi_fdset","curl_easy_upkeep","curl_multi_socket_all","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_version","curl_mprintf","curl_multi_init","curl_multi_notify_disable","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_share_strerror","curl_url_strerror","curl_mime_filedata","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_fdset","curl_easy_upkeep","curl_multi_socket_all","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":2}
/*To create a fuzz driver that uses the specified libcurl APIs to process curl output data, I'll design a program that simulates a multi-part form upload with callback-based data reading, while utilizing both easy and multi interfaces.

*/


extern "C" {
}

// Custom callback functions for curl_mime_data_cb
static size_t  id_000043_read_callback(char *buffer, size_t size, size_t nitems, void *instream) {
    FILE *in_file = static_cast<FILE *>(instream);
    if (in_file) {
        return fread(buffer, size, nitems, in_file);
    }
    return 0;
}

static int  id_000043_seek_callback(void *instream, curl_off_t offset, int origin) {
    FILE *in_file = static_cast<FILE *>(instream);
    if (in_file) {
        return fseek(in_file, static_cast<long>(offset), origin);
    }
    return CURL_SEEKFUNC_CANTSEEK;
}

static void  id_000043_free_callback(void *instream) {
    FILE *in_file = static_cast<FILE *>(instream);
    if (in_file) {
        assert_file_closed(&in_file);;
    }
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<19) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 1) {
        return 0;
    }

    // Initialize global curl environment
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // Enable global tracing
    curl_global_trace(fuzz_str_1);

    // Get curl version and print it
    char *version = curl_version();
    if (version) {
        curl_mprintf("CURL Version: %s\n", version);
    }

    // Create a memory file from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Disable notifications (using arbitrary notification ID 0)
    curl_multi_notify_disable(multi_handle, fuzz_uint32_t_2);

    // Initialize easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Add mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up data callback for the mime part using input file
    CURLcode res = curl_mime_data_cb(part, 
                                     static_cast<curl_off_t>(size),
                                      id_000043_read_callback,
                                      id_000043_seek_callback,
                                      id_000043_free_callback,
                                     in_file);
    
    if (res != CURLE_OK) {
        // Error string for share (not used but called for completeness)
        const char *share_err = curl_share_strerror(CURLSHE_OK);
        // Error string for URL (not used but called for completeness)
        const char *url_err = curl_url_strerror(CURLUE_OK);
    }

    // Also set file data (using dummy filename)
    curl_mime_filedata(part, "input_file");

    // Set output file descriptor for easy handle
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com/upload");

    // Add easy handle to multi handle
    CURLMcode mres = curl_multi_add_handle(multi_handle, easy_handle);
    if (mres != CURLM_OK) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // Prepare fd_set structures for multi_fdset
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    FD_ZERO(&exc_fd_set);
    int max_fd = -1;

    // Get file descriptors for multi handle
    mres = curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);
    
    // Perform upkeep on easy handle
    curl_easy_upkeep(easy_handle);

    int running_handles = 0;
    
    // Perform socket operations (deprecated but required by API list)
    mres = curl_multi_socket_all(multi_handle, &running_handles);

    // Remove easy handle from multi handle
    mres = curl_multi_remove_handle(multi_handle, easy_handle);

    // Clean up resources in reverse order
    curl_mime_free(mime);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    // Close files (in_file is closed by  id_000043_free_callback when mime is freed)
    assert_file_closed(&out_file);;
    
    // Clean up global curl environment
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}