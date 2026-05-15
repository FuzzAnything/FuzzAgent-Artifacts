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
//<ID> 459
//<Prompt> []
/*<Combination>: [
*/
//<score> 189, nr_unique_branch: 11
//<Quality> {"density":27,"unique_branches":{"pgrsupdate":[[634,6,634,26,0,0,4,0],[635,8,635,27,0,0,4,0],[645,10,645,46,0,0,4,0],[646,12,646,18,0,0,4,1]],"setopt_func":[[2526,3,2526,32,0,0,4,0],[2531,8,2531,20,0,0,4,0]],"Curl_pgrsDone":[[201,6,201,26,0,0,4,0],[201,30,201,54,0,0,4,1]],"dnscache_lock":[[156,21,156,55,0,0,4,0]],"dnscache_get":[[146,21,146,71,0,0,4,0]],"dnscache_unlock":[[163,21,163,55,0,0,4,0]]},"library_calls":["curl_global_init","curl_getenv","curl_free","curl_share_init","curl_share_setopt","curl_share_setopt","curl_multi_init","curl_share_cleanup","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_escape","curl_free","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_all","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_getenv","curl_free","curl_share_init","curl_share_setopt","curl_share_setopt","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_escape","curl_free","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_all","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Callback function for reading data from FILE*
static size_t  id_000029_read_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    FILE *in_file = (FILE *)userdata;
    return fread(ptr, size, nmemb, in_file);
}

// Callback function for writing data to FILE*
static size_t  id_000029_write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    FILE *out_file = (FILE *)userdata;
    return fwrite(ptr, size, nmemb, out_file);
}

// Progress callback
static int  id_000029_progress_callback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    return 0; // Return 0 to continue, 1 to abort
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize global curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // Get environment variable
    char *env_var = curl_getenv("HOME");
    if (env_var) {
        curl_free(env_var);
    }

    // Initialize share object
    CURLSH *share = curl_share_init();
    if (share) {
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
    }

    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        if (share) curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // Initialize easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        if (share) curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        if (share) curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        if (share) curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_SHARE, share);
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION,  id_000029_read_callback);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION,  id_000029_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(easy_handle, CURLOPT_XFERINFOFUNCTION,  id_000029_progress_callback);
    
    // Use a portion of data for URL escaping (first 100 bytes or less)
    size_t escape_len = size > 100 ? 100 : size;
    char *escaped = curl_easy_escape(easy_handle, (const char *)data, escape_len);
    if (escaped) {
        curl_free(escaped);
    }

    // Set URL (using a dummy URL since we're fuzzing)
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file:///dev/null");

    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Perform multi socket operations
    int running_handles = 0;
    curl_multi_socket_all(multi_handle, &running_handles);

    // Remove easy handle from multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);

    // Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    if (share) curl_share_cleanup(share);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Cleanup global curl
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}