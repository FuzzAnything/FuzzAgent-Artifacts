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
//<ID> 110
//<Prompt> []
/*<Combination>: [
*/
//<score> 19.333334, nr_unique_branch: 1
//<Quality> {"density":29,"unique_branches":{"Curl_mntfy_enable":[[150,6,150,34,0,0,4,1]]},"library_calls":["curl_global_init","curl_version_info","curl_global_cleanup","curl_multi_init","curl_global_cleanup","curl_multi_notify_enable","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_name","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_type","curl_mime_data","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_timeout","curl_multi_socket_action","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version_info","curl_multi_init","curl_multi_notify_enable","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_type","curl_mime_data","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_timeout","curl_multi_socket_action","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":2}
/**/


// Include necessary headers for curl
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    if (size < 10) {
        return 0; // Not enough data to do anything meaningful
    }

    // Initialize libcurl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // Get curl version info
    curl_version_info_data *version_info = curl_version_info(CURLVERSION_NOW);
    if (!version_info) {
        curl_global_cleanup();
        return 0;
    }

    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Enable notifications on the multi handle
    // Use the first byte of data to determine notification flags
    unsigned int notification_flags = static_cast<unsigned int>(data[0]);
    CURLMcode notify_res = curl_multi_notify_enable(multi_handle, notification_flags);
    (void)notify_res; // Result intentionally ignored for fuzzing

    // Create an easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Create a mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Create a mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Use input data for mime name (use first half)
    size_t name_len = size / 2;
    char *name_buf = static_cast<char*>(malloc(name_len + 1));
    if (!name_buf) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    memcpy(name_buf, data, name_len);
    name_buf[name_len] = '\0';
    
    CURLcode name_res = curl_mime_name(part, name_buf);
    free(name_buf);
    (void)name_res; // Result intentionally ignored for fuzzing

    // Use input data for mime type (use second half)
    size_t type_len = size - name_len;
    char *type_buf = static_cast<char*>(malloc(type_len + 1));
    if (!type_buf) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    memcpy(type_buf, data + name_len, type_len);
    type_buf[type_len] = '\0';
    
    CURLcode type_res = curl_mime_type(part, type_buf);
    free(type_buf);
    (void)type_res; // Result intentionally ignored for fuzzing

    // Set mime data (use the entire input data)
    CURLcode data_res = curl_mime_data(part, reinterpret_cast<const char*>(data), size);
    (void)data_res; // Result intentionally ignored for fuzzing

    // Set up the easy handle with the mime data
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file:///dev/null"); // Dummy URL

    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Get timeout from multi handle (use second byte of data for timeout hint)
    long timeout_ms = 0;
    data++; size--; // Skip first byte already used
    if (size > 0) {
        timeout_ms = static_cast<long>(data[0]) * 10; // Scale for realistic timeout
        curl_multi_timeout(multi_handle, &timeout_ms);
    }

    // Perform socket action with a dummy socket
    int running_handles = 0;
    curl_socket_t dummy_socket = CURL_SOCKET_BAD;
    // Use third byte of data for event bitmask if available
    int ev_bitmask = 0;
    if (size > 1) {
        ev_bitmask = static_cast<int>(data[1]) & 0x03; // Mask to valid event bits
    }
    curl_multi_socket_action(multi_handle, dummy_socket, fuzz_int32_t_1, &running_handles);

    // Clean up in reverse order
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_mime_free(mime);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}