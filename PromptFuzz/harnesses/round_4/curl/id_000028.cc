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
//<ID> 422
//<Prompt> []
/*<Combination>: [
*/
//<score> 154, nr_unique_branch: 36
//<Quality> {"density":22,"unique_branches":{"formatf":[[1019,5,1019,24,0,0,4,0]],"curl_ws_start_frame":[[1887,6,1887,17,0,0,4,1],[1893,6,1893,9,0,0,4,0]],"parsefmt":[[310,14,310,32,0,0,4,0],[392,12,392,34,0,0,4,0],[562,5,562,24,0,0,4,0]],"Curl_xfer_pause_recv":[[904,6,904,13,0,0,4,1]],"Curl_conn_cf_cntrl":[[913,8,913,22,0,0,4,0],[913,26,913,32,0,0,4,1]],"mstate_connecting_pollset":[[1032,6,1032,36,0,0,4,0]],"curl_easy_pause":[[1142,33,1142,44,0,0,4,1],[1146,6,1146,31,0,0,4,1],[1154,6,1154,38,0,0,4,1],[1155,6,1155,55,0,0,4,1],[1161,6,1161,36,0,0,4,0],[1168,6,1168,33,0,0,4,0],[1171,8,1171,19,0,0,4,0],[1174,10,1174,17,0,0,4,0],[1175,12,1175,42,0,0,4,1],[1181,6,1181,13,0,0,4,0],[1181,17,1181,24,0,0,4,0],[1181,28,1181,45,0,0,4,0],[1181,49,1181,60,0,0,4,0],[1183,8,1183,52,0,0,4,1],[1186,6,1186,15,0,0,4,1]],"easy_perform":[[751,6,751,17,0,0,4,0]],"Curl_xfer_is_blocked":[[872,6,872,16,0,0,4,0],[873,12,873,21,0,0,4,1]],"Curl_pgrsRecvPause":[[229,6,229,13,0,0,4,1]],"Curl_creader_is_paused":[[1449,9,1449,15,0,0,4,1]],"Curl_update_timer":[[3509,11,3509,37,0,0,4,1],[3513,11,3513,65,0,0,4,0],[319,4,319,10,21,0,4,0],[319,14,319,33,21,0,4,1]],"Curl_rlimit_block":[[280,6,280,30,0,0,4,1],[285,6,285,17,0,0,4,1]]},"library_calls":["curl_getenv","curl_free","curl_global_init","curl_easy_init","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_multi_setopt","curl_multi_setopt","curl_multi_add_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_socket_action","curl_ws_start_frame","curl_easy_perform","curl_ws_meta","curl_easy_pause","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_getenv","curl_free","curl_global_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_setopt","curl_multi_setopt","curl_multi_add_handle","curl_multi_socket_action","curl_ws_start_frame","curl_easy_perform","curl_ws_meta","curl_easy_pause","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified curl APIs to simulate a WebSocket client scenario:

*/


extern "C" {
}

// Helper function for curl_mvsnprintf
static int  id_000028_my_mvsnprintf(char *buffer, size_t maxlength, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvsnprintf(buffer, maxlength, format, args);
    va_end(args);
    return result;
}

// Helper function for curl_mvfprintf
static int  id_000028_my_mvfprintf(FILE *fd, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvfprintf(fd, format, args);
    va_end(args);
    return result;
}

// Callback for curl_multi_socket
static int  id_000028_socket_callback(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {
    return 0;
}

// Callback for curl_multi_setopt
static int  id_000028_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<19) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Get environment variable using curl_getenv
    char *home_env = curl_getenv(fuzz_str_1);
    if (home_env) {
        // Use curl_mvfprintf with the environment variable
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
             id_000028_my_mvfprintf(out_file, "HOME environment: %s\n", home_env);
            assert_file_closed(&out_file);;
        }
        curl_free(home_env);
    }
    
    // 2. Initialize curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // 3. Create easy handle for WebSocket
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }
    
    // 4. Set WebSocket options
    curl_easy_setopt(easy, CURLOPT_URL, "ws://example.com/ws");
    curl_easy_setopt(easy, CURLOPT_CONNECT_ONLY, 2L); // CONNECT_ONLY for WS
    
    // 5. Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }
    
    // 6. Use curl_multi_setopt with callbacks
    curl_multi_setopt(multi, CURLMOPT_SOCKETFUNCTION,  id_000028_socket_callback);
    curl_multi_setopt(multi, CURLMOPT_TIMERFUNCTION,  id_000028_timer_callback);
    
    // 7. Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);
    
    // 8. Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_multi_remove_handle(multi, easy);
        curl_multi_cleanup(multi);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 9. Get file descriptor for socket operations
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        curl_multi_remove_handle(multi, easy);
        curl_multi_cleanup(multi);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 10. Use curl_multi_socket to simulate socket activity
    int running_handles = 0;
    curl_multi_socket(multi, fd, &running_handles);
    
    // 11. Start WebSocket frame using input data
    // Use first 4 bytes as flags, next 8 bytes as frame length
    unsigned int flags = 0;
    curl_off_t frame_len = 0;
    
    if (size >= 12) {
        memcpy(&flags, data, 4);
        memcpy(&frame_len, data + 4, 8);
        // Ensure frame_len doesn't exceed remaining data
        if (frame_len > size - 12) {
            frame_len = size - 12;
        }
    } else {
        flags = CURLWS_TEXT;
        frame_len = size;
    }
    
    // 12. Start WebSocket frame
    curl_ws_start_frame(easy, fuzz_uint32_t_2, fuzz_int64_t_3);
    
    // 13. Use curl_mvsnprintf to format a message
    char buffer[256];
     id_000028_my_mvsnprintf(buffer, sizeof(buffer), 
                  "WebSocket frame: flags=%u, length=%lld, total_input=%zu",
                  flags, (long long)frame_len, size);
    
    // Write formatted message to output
    FILE *log_file = fopen("output_file", "ab");
    if (log_file) {
        fwrite(buffer, 1, strlen(buffer), log_file);
        fwrite("\n", 1, 1, log_file);
        assert_file_closed(&log_file);;
    }
    
    // 14. Perform easy operation (will fail since no real connection, but we call it)
    curl_easy_perform(easy);
    
    // 15. Get WebSocket metadata
    const struct curl_ws_frame *meta = curl_ws_meta(easy);
    if (meta) {
        // Log metadata if available
        FILE *meta_file = fopen("output_file", "ab");
        if (meta_file) {
             id_000028_my_mvfprintf(meta_file, 
                        "WS Meta: age=%d, flags=%d, offset=%lld, bytesleft=%lld, len=%zu\n",
                        meta->age, meta->flags, 
                        (long long)meta->offset, 
                        (long long)meta->bytesleft, 
                        meta->len);
            assert_file_closed(&meta_file);;
        }
    }
    
    // 16. Pause the easy handle
    curl_easy_pause(easy, CURLPAUSE_RECV);
    
    // 17. Clean up
    assert_file_closed(&in_file);;
    curl_multi_remove_handle(multi, easy);
    curl_multi_cleanup(multi);
    curl_easy_cleanup(easy);
    curl_global_cleanup();
    
    assert_file_closed(&log_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}