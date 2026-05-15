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
//<ID> 539
//<Prompt> []
/*<Combination>: [
*/
//<score> 85, nr_unique_branch: 29
//<Quality> {"density":17,"unique_branches":{"setopt_long_proto":[[1134,3,1134,28,0,0,4,1],[1141,3,1141,21,0,0,4,1],[1149,3,1149,30,0,0,4,1],[1155,3,1155,27,0,0,4,1],[1161,3,1161,26,0,0,4,1],[1167,3,1167,32,0,0,4,1],[1171,3,1171,39,0,0,4,1],[1177,3,1177,30,0,0,4,1],[1185,3,1185,28,0,0,4,1],[1187,3,1187,32,0,0,4,1],[1192,3,1192,32,0,0,4,1],[1209,3,1209,25,0,0,4,1],[1212,3,1212,31,0,0,4,1],[1216,3,1216,26,0,0,4,0],[1221,3,1221,10,0,0,4,1]],"curl_share_strerror":[[389,3,389,18,0,0,4,0],[392,3,392,26,0,0,4,0],[395,3,395,22,0,0,4,1],[398,3,398,23,0,0,4,0],[401,3,401,21,0,0,4,0],[404,3,404,28,0,0,4,0],[407,3,407,20,0,0,4,0]],"setopt_long_http":[[1073,3,1073,30,0,0,4,1],[1114,3,1114,10,0,0,4,0]],"setopt_long_proxy":[[1036,3,1036,25,0,0,4,1],[1041,3,1041,25,0,0,4,1],[1043,3,1043,25,0,0,4,1],[1048,3,1048,27,0,0,4,1],[1053,3,1053,10,0,0,4,0]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_share_init","curl_easy_unescape","curl_getdate","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_easy_setopt","curl_multi_add_handle","curl_easy_upkeep","curl_multi_socket_action","curl_ws_meta","curl_share_strerror","curl_multi_perform","curl_mime_free","curl_free","curl_multi_remove_handle","curl_multi_cleanup","curl_share_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_share_init","curl_easy_unescape","curl_getdate","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_easy_setopt","curl_multi_add_handle","curl_easy_upkeep","curl_multi_socket_action","curl_ws_meta","curl_share_strerror","curl_multi_perform","curl_mime_free","curl_free","curl_multi_remove_handle","curl_multi_cleanup","curl_share_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzzer that uses the specified curl APIs to simulate a WebSocket client scenario:

*/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful fuzzing
    
    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create easy handle for WebSocket
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create multi handle for socket management
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }
    
    // Create share handle (for error demonstration)
    CURLSH *share = curl_share_init();
    
    // 1. Use curl_easy_unescape to decode URL-encoded data
    int outlength = 0;
    // Use first 50 bytes for unescaping (simulating URL-encoded data)
    size_t unescape_len = size > 50 ? 50 : size;
    char *unescaped = curl_easy_unescape(easy, 
                                         reinterpret_cast<const char*>(data), 
                                         unescape_len, 
                                         &outlength);
    
    // 2. Use unescaped data for curl_getdate (if we got valid string)
    time_t parsed_time = 0;
    if (unescaped && outlength > 0) {
        // Null-terminate the string for curl_getdate
        char *date_str = static_cast<char*>(malloc(outlength + 1));
        if (date_str) {
            memcpy(date_str, unescaped, outlength);
            date_str[outlength] = '\0';
            parsed_time = curl_getdate(date_str, nullptr);
            free(date_str);
        }
    }
    
    // 3. Use curl_mime_name for multipart form data
    curl_mime *mime = curl_mime_init(easy);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use next 20 bytes for mime name
            size_t mime_name_len = (size - unescape_len) > 20 ? 20 : (size - unescape_len);
            if (mime_name_len > 0) {
                char *mime_name = static_cast<char*>(malloc(mime_name_len + 1));
                if (mime_name) {
                    memcpy(mime_name, data + unescape_len, mime_name_len);
                    mime_name[mime_name_len] = '\0';
                    curl_mime_name(part, mime_name);
                    free(mime_name);
                }
            }
        }
    }
    
    // 4. Set up WebSocket options
    curl_easy_setopt(easy, CURLOPT_WS_OPTIONS, CURLWS_RAW_MODE);
    
    // 5. Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);
    
    // 6. Call curl_easy_upkeep (for connection maintenance)
    curl_easy_upkeep(easy);
    
    // 7. Simulate socket activity with curl_multi_socket
    int running_handles = 0;
    // Use a socket value derived from input data
    curl_socket_t dummy_socket = static_cast<curl_socket_t>(
        *reinterpret_cast<const int*>(data + (size > 60 ? 60 : 0)) % 1024);
    curl_multi_socket(multi, dummy_socket, &running_handles);
    
    // 8. Get WebSocket metadata (will be NULL since not in WebSocket mode)
    const struct curl_ws_frame *ws_meta = curl_ws_meta(easy);
    (void)ws_meta;  // Use variable to avoid unused warning
    
    // 9. Use curl_share_strerror for error reporting
    if (share) {
        // Generate a share error code from input data
        size_t error_index = size > 80 ? 80 : size - 1;
        CURLSHcode share_error = static_cast<CURLSHcode>(
            data[error_index] % (CURLSHE_LAST + 1));
        const char *error_str = curl_share_strerror(share_error);
        (void)error_str;  // Use variable to avoid unused warning
    }
    
    // Perform a dummy multi perform
    int still_running = 0;
    curl_multi_perform(multi, &still_running);
    
    // Cleanup in reverse order
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (unescaped) {
        curl_free(unescaped);
    }
    
    curl_multi_remove_handle(multi, easy);
    curl_multi_cleanup(multi);
    
    if (share) {
        curl_share_cleanup(share);
    }
    
    curl_easy_cleanup(easy);
    curl_global_cleanup();
    
    return 0;
}