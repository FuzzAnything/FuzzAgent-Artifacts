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
//<ID> 243
//<Prompt> []
/*<Combination>: [
*/
//<score> 86.666664, nr_unique_branch: 59
//<Quality> {"density":20,"unique_branches":{"Curl_cookie_init":[[1064,6,1064,9,0,0,4,1],[1069,14,1069,34,0,0,4,0],[1069,14,1069,34,0,0,4,1]],"getinfo_long":[[205,3,205,27,0,0,4,1],[214,3,214,28,0,0,4,1],[215,3,215,29,0,0,4,1],[224,3,224,10,0,0,4,0],[230,3,230,30,0,0,4,0],[233,3,233,33,0,0,4,1],[236,3,236,25,0,0,4,1],[246,3,246,28,0,0,4,1],[249,3,249,29,0,0,4,1],[252,3,252,33,0,0,4,1],[255,3,255,39,0,0,4,1],[262,3,262,31,0,0,4,1],[265,3,265,31,0,0,4,1],[269,3,269,32,0,0,4,1],[273,3,273,30,0,0,4,1],[277,3,277,31,0,0,4,1],[281,3,281,25,0,0,4,1],[284,3,284,29,0,0,4,1],[292,3,292,27,0,0,4,1],[304,3,304,29,0,0,4,1],[311,3,311,27,0,0,4,1],[318,3,318,28,0,0,4,1],[321,3,321,32,0,0,4,1],[329,3,329,33,0,0,4,1],[332,3,332,33,0,0,4,1],[335,3,335,31,0,0,4,1],[345,3,345,29,0,0,4,1],[364,3,364,25,0,0,4,1],[367,3,367,27,0,0,4,1],[376,3,376,10,0,0,4,1]],"Curl_share_easy_link":[[448,8,448,22,0,0,4,0]],"Curl_cookie_cleanup":[[1422,6,1422,8,0,0,4,0]],"curl_share_setopt":[[221,5,221,31,0,0,4,0],[223,10,223,25,0,0,4,0],[225,12,225,27,0,0,4,1],[261,5,261,32,0,0,4,1]],"Curl_share_unlock":[[395,6,395,50,0,0,4,0],[396,8,396,25,0,0,4,1]],"Curl_cookie_clearall":[[1371,6,1371,8,0,0,4,0],[1373,16,1373,36,0,0,4,0],[1373,16,1373,36,0,0,4,1],[1375,52,1375,53,0,0,4,1]],"Curl_flush_cookies":[[1620,6,1620,19,0,0,4,0],[1621,8,1621,39,0,0,4,1],[1630,8,1630,15,0,0,4,0],[1630,20,1630,32,0,0,4,1],[1630,36,1630,75,0,0,4,1]],"Curl_getinfo":[[642,3,642,23,0,0,4,1],[647,3,647,21,0,0,4,0],[649,8,649,19,0,0,4,0]],"curl_easy_strerror":[[50,3,50,26,0,0,4,0]],"Curl_share_lock":[[379,6,379,50,0,0,4,0],[380,8,380,23,0,0,4,1]],"getinfo_char":[[92,3,92,30,0,0,4,1],[94,20,94,21,0,0,4,0],[127,3,127,29,0,0,4,0]]},"library_calls":["curl_global_init","curl_share_init","curl_global_cleanup","curl_share_setopt","curl_easy_init","curl_share_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_getinfo","curl_easy_getinfo","curl_easy_getinfo","curl_easy_header","curl_easy_ssls_export","curl_easy_strerror","curl_easy_strerror","curl_share_strerror","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_share_setopt","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_getinfo","curl_easy_getinfo","curl_easy_getinfo","curl_easy_header","curl_easy_ssls_export","curl_easy_strerror","curl_easy_strerror","curl_share_strerror","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":2}
/*Here's a fuzzer that uses the specified curl APIs to process HTTP response data:

*/


// Custom callback for curl_easy_ssls_export
static CURLcode  id_000020_export_callback(CURL *handle, void *arg, 
                               const char *session_key, 
                               const unsigned char *shmac, size_t shmac_len,
                               const unsigned char *sdata, size_t sdata_len,
                               curl_off_t timestamp, int expire_after,
                               const char *extra_info, size_t extra_info_len) {
    FILE *out_file = (FILE *)arg;
    if (out_file) {
        fprintf(out_file, "SSL session exported: key=%s, shmac_len=%zu, sdata_len=%zu\n",
                session_key ? session_key : "(null)", shmac_len, sdata_len);
    }
    return CURLE_OK;
}

// Wrapper for curl_mvfprintf to make it usable
static int  id_000020_custom_vfprintf(FILE *fd, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvfprintf(fd, format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize curl global environment
    CURLcode global_init_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_result != CURLE_OK) {
        return 0;
    }
    
    // Create shared object
    CURLSH *share = curl_share_init();
    if (!share) {
        curl_global_cleanup();
        return 0;
    }
    
    // Set share options to cause potential errors for testing
    CURLSHcode share_error = curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    
    // Initialize easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy);
        curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy);
        curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);
    
    // Configure easy handle with input file as response data
    curl_easy_setopt(easy, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(easy, CURLOPT_URL, "file:///dev/null");
    
    // Use share object
    curl_easy_setopt(easy, CURLOPT_SHARE, share);
    
    // Try to get various types of info from the easy handle
    // This simulates processing response data
    
    // 1. Get HTTP response code (will likely fail since we're not actually making a request)
    long http_code = 0;
    CURLcode info_result = curl_easy_getinfo(easy, CURLINFO_RESPONSE_CODE, &http_code);
    
    // 2. Get effective URL (will likely be empty)
    char *effective_url = nullptr;
    curl_easy_getinfo(easy, CURLINFO_EFFECTIVE_URL, &effective_url);
    
    // 3. Get content type if available
    char *content_type = nullptr;
    curl_easy_getinfo(easy, CURLINFO_CONTENT_TYPE, &content_type);
    
    // Try to get header information (though we have no real response)
    // This will likely fail but we still call the API
    struct curl_header *hout = nullptr;
    CURLHcode header_result = curl_easy_header(easy, "Content-Type", 0, 
                                               CURLH_HEADER, 1, &hout);
    
    // Try to export SSL session data (will likely fail but we call it)
    CURLcode export_result = curl_easy_ssls_export(easy,  id_000020_export_callback, out_file);
    
    // Use curl_mvfprintf through wrapper to log information
     id_000020_custom_vfprintf(out_file, "=== CURL FUZZ TEST ===\n");
     id_000020_custom_vfprintf(out_file, "Input size: %zu bytes\n", size);
     id_000020_custom_vfprintf(out_file, "HTTP response code getinfo result: %s\n", 
                    curl_easy_strerror(info_result));
     id_000020_custom_vfprintf(out_file, "Header get result: %d\n", header_result);
     id_000020_custom_vfprintf(out_file, "SSL export result: %s\n", 
                    curl_easy_strerror(export_result));
    
    // Write share error using curl_share_strerror
    if (share_error != CURLSHE_OK) {
        const char *share_err_str = curl_share_strerror(share_error);
         id_000020_custom_vfprintf(out_file, "Share error: %s\n", 
                        share_err_str ? share_err_str : "Unknown error");
    }
    
    // Write some of the input data to output for debugging
    if (size > 0) {
        size_t to_write = size > 100 ? 100 : size;
         id_000020_custom_vfprintf(out_file, "\nFirst %zu bytes of input (hex):\n", to_write);
        for (size_t i = 0; i < to_write; i++) {
             id_000020_custom_vfprintf(out_file, "%02x ", data[i]);
            if ((i + 1) % 16 == 0)  id_000020_custom_vfprintf(out_file, "\n");
        }
         id_000020_custom_vfprintf(out_file, "\n");
    }
    
    // Cleanup
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    curl_easy_cleanup(easy);
    curl_share_cleanup(share);
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}