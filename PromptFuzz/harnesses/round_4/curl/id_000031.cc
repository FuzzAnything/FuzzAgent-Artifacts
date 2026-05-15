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
//<ID> 492
//<Prompt> []
/*<Combination>: [
*/
//<score> 51, nr_unique_branch: 53
//<Quality> {"density":17,"unique_branches":{"Curl_getinfo":[[652,3,652,23,0,0,4,0],[654,8,654,21,0,0,4,0]],"formatf":[[1003,13,1003,33,0,0,4,0],[1035,5,1035,22,0,0,4,0],[1036,10,1036,68,0,0,4,1]],"parsefmt":[[252,14,252,25,0,0,4,1],[272,16,272,22,0,0,4,1],[274,16,274,53,0,0,4,1],[277,16,277,22,0,0,4,1],[439,7,439,15,0,0,4,0],[584,5,584,22,0,0,4,0]],"out_double":[[618,6,618,24,0,0,4,1],[620,6,620,28,0,0,4,1],[622,6,622,25,0,0,4,1],[624,6,624,23,0,0,4,1],[629,6,629,16,0,0,4,0],[631,8,631,25,0,0,4,1],[638,6,638,15,0,0,4,0],[644,8,644,22,0,0,4,1],[646,8,646,17,0,0,4,1],[648,11,648,22,0,0,4,1],[653,8,653,22,0,0,4,1],[655,8,655,16,0,0,4,1],[661,6,661,24,0,0,4,1],[664,6,664,26,0,0,4,1],[666,11,666,31,0,0,4,1],[691,9,691,14,0,0,4,0],[691,9,691,14,0,0,4,1],[692,8,692,30,0,0,4,1]],"getinfo_double":[[495,3,495,27,0,0,4,1],[496,3,496,32,0,0,4,1],[497,3,497,29,0,0,4,1],[498,3,498,32,0,0,4,1],[499,3,499,33,0,0,4,1],[500,3,500,35,0,0,4,1],[501,3,501,30,0,0,4,1],[502,3,502,31,0,0,4,1],[503,3,503,29,0,0,4,1],[512,3,512,10,0,0,4,0],[517,3,517,27,0,0,4,1],[520,3,520,32,0,0,4,1],[523,3,523,29,0,0,4,1],[526,3,526,32,0,0,4,1],[529,3,529,33,0,0,4,1],[532,3,532,35,0,0,4,1],[535,3,535,28,0,0,4,0],[538,3,538,30,0,0,4,1],[541,3,541,31,0,0,4,1],[544,3,544,29,0,0,4,1],[547,3,547,40,0,0,4,1],[551,3,551,38,0,0,4,1],[555,3,555,30,0,0,4,1],[559,3,559,10,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_poll","curl_multi_wakeup","curl_multi_perform","curl_easy_getinfo","curl_easy_upkeep","curl_msnprintf","curl_mfprintf","curl_mvprintf","curl_mvaprintf","curl_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_poll","curl_multi_wakeup","curl_multi_perform","curl_easy_getinfo","curl_easy_upkeep","curl_msnprintf","curl_mfprintf","curl_mvprintf","curl_mvaprintf","curl_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

// Helper function to create va_list for testing
static void  id_000031_build_va_list(va_list* args, const char* fmt, ...) {
    va_start(*args, fmt);
    // We'll just initialize it; actual usage would need proper formatting
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    // Initialize global curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create input file from fuzzer data
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
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set up easy handle options
    curl_easy_setopt(easy, CURLOPT_URL, "file://input_file");
    curl_easy_setopt(easy, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy, CURLOPT_WRITEDATA, out_file);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);
    
    int running_handles = 0;
    int ret_val = 0;
    
    // Set up poll structure
    struct curl_waitfd extra_fds[2];
    extra_fds[0].fd = in_fd;
    extra_fds[0].events = CURL_WAIT_POLLIN;
    extra_fds[0].revents = 0;
    
    extra_fds[1].fd = out_fd;
    extra_fds[1].events = CURL_WAIT_POLLOUT;
    extra_fds[1].revents = 0;
    
    // Use curl_multi_poll - This is the main event
    CURLMcode poll_res = curl_multi_poll(multi, extra_fds, 2, fuzz_int32_t_1, &ret_val);
    
    // Wake up multi handle
    curl_multi_wakeup(multi);
    
    // Perform the transfer
    curl_multi_perform(multi, &running_handles);
    
    // Get info from easy handle
    double upload_size = 0.0;
    curl_easy_getinfo(easy, CURLINFO_SIZE_UPLOAD, &upload_size);
    
    // Use curl_easy_upkeep
    curl_easy_upkeep(easy);
    
    // Create buffers for formatted output
    char buffer1[256];
    char buffer2[256];
    
    // Use curl_msnprintf
    int n1 = curl_msnprintf(buffer1, sizeof(buffer1), 
                           "Processed %zu bytes, upload: %.2f", 
                           size, upload_size);
    
    // Use curl_mfprintf
    if (n1 > 0) {
        curl_mfprintf(out_file, "%s\n", buffer1);
    }
    
    // Prepare va_list for mvprintf functions
    va_list args1, args2;
     id_000031_build_va_list(&args1, "Test format %zu %f", size, upload_size);
     id_000031_build_va_list(&args2, "Another test %zu", size);
    
    // Use curl_mvprintf (output to stdout)
    int n2 = curl_mvprintf("Fuzz test: size=%zu\n", args1);
    
    // Use curl_mvaprintf
    char *dynamic_str = curl_mvaprintf("Dynamic string: processed %zu bytes\n", args2);
    if (dynamic_str) {
        fwrite(dynamic_str, strlen(dynamic_str), 1, out_file);
        curl_free(dynamic_str);
    }
    
    // Clean up va_lists
    va_end(args1);
    va_end(args2);
    
    // Clean up handles
    curl_multi_remove_handle(multi, easy);
    curl_multi_cleanup(multi);
    curl_easy_cleanup(easy);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up global curl
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}