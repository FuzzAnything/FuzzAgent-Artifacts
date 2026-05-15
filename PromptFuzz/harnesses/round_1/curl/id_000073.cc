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
//<ID> 1427
//<Prompt> []
/*<Combination>: [
*/
//<score> 120, nr_unique_branch: 6
//<Quality> {"density":24,"unique_branches":{"cr_in_read":[[667,22,667,26,0,0,4,1],[676,33,676,65,0,0,4,1]],"do_init_reader_stack":[[1113,6,1113,10,0,0,4,1]],"curl_multi_setopt":[[3266,8,3266,24,0,0,4,1]],"curlx_sotouz_range":[[1290,25,1290,26,1,0,4,1],[1290,37,1290,38,1,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_trace","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_share_init","curl_easy_cleanup","curl_global_cleanup","curl_multi_init","curl_share_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_setopt","curl_multi_cleanup","curl_share_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_option_next","curl_easy_duphandle","curl_easy_setopt","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_easy_upkeep","curl_multi_timeout","curl_multi_poll","curl_multi_perform","curl_multi_remove_handle","curl_multi_remove_handle","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_easy_init","curl_share_init","curl_multi_init","curl_multi_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_option_next","curl_easy_duphandle","curl_easy_setopt","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_easy_upkeep","curl_multi_timeout","curl_multi_poll","curl_multi_perform","curl_multi_remove_handle","curl_multi_remove_handle","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Initialize global curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // Set trace configuration using first 100 bytes of input
    size_t trace_len = size > 100 ? 100 : size;
    char *trace_config = (char *)malloc(trace_len + 1);
    if (!trace_config) {
        curl_global_cleanup();
        return 0;
    }
    memcpy(trace_config, data, trace_len);
    trace_config[trace_len] = '\0';
    curl_global_trace(trace_config);
    free(trace_config);
    data += trace_len;
    size -= trace_len;

    if (size < 1) {
        curl_global_cleanup();
        return 0;
    }

    // Initialize easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }

    // Initialize share handle
    CURLSH *share = curl_share_init();
    if (!share) {
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }

    // Initialize multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_share_cleanup(share);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }

    // Set multi handle option using input data
    if (size >= sizeof(long)) {
        long maxconnects = *((long *)data);
        curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, maxconnects);
        data += sizeof(long);
        size -= sizeof(long);
    }

    // Create FILE* from remaining input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Set easy handle options using input data
    curl_easy_setopt(easy, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(easy, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy, CURLOPT_INFILESIZE_LARGE, (curl_off_t)size);
    curl_easy_setopt(easy, CURLOPT_URL, "file:///dev/null");

    // Iterate through easy options
    const struct curl_easyoption *opt = NULL;
    int option_count = 0;
    while ((opt = curl_easy_option_next(opt)) != NULL && option_count < 5) {
        option_count++;
    }

    // Duplicate easy handle
    CURL *easy_dup = curl_easy_duphandle(easy);
    if (easy_dup) {
        curl_easy_setopt(easy_dup, CURLOPT_URL, "file:///dev/null");
    }

    // Add handles to multi
    curl_multi_add_handle(multi, easy);
    if (easy_dup) {
        curl_multi_add_handle(multi, easy_dup);
    }

    // Perform multi transfers
    int running_handles = 0;
    curl_multi_perform(multi, &running_handles);

    // Call upkeep on easy handle
    curl_easy_upkeep(easy);

    // Check timeout
    long timeout = 0;
    curl_multi_timeout(multi, &timeout);

    // Setup poll structure
    struct curl_waitfd pollfd;
    pollfd.fd = fuzz_fileno(in_file);
    pollfd.events = CURL_WAIT_POLLIN;
    pollfd.revents = 0;
    
    int numfds = 0;
    curl_multi_poll(multi, &pollfd, 1, fuzz_int32_t_1, &numfds);

    // Perform again after poll
    if (running_handles) {
        curl_multi_perform(multi, &running_handles);
    }

    // Remove handles from multi
    curl_multi_remove_handle(multi, easy);
    if (easy_dup) {
        curl_multi_remove_handle(multi, easy_dup);
    }

    // Cleanup
    assert_file_closed(&in_file);;
    if (easy_dup) {
        curl_easy_cleanup(easy_dup);
    }
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    curl_share_cleanup(share);
    curl_global_cleanup();

    assert_file_closed(&in_file);
	return 0;
}