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
//<ID> 963
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 15
//<Quality> {"density":15,"unique_branches":{"curl_url_strerror":[[427,3,427,25,0,0,4,0],[430,3,430,30,0,0,4,0],[439,3,439,33,0,0,4,0],[442,3,442,24,0,0,4,0],[445,3,445,28,0,0,4,0],[448,3,448,31,0,0,4,0],[451,3,451,27,0,0,4,0],[454,3,454,24,0,0,4,0],[457,3,457,22,0,0,4,0],[460,3,460,26,0,0,4,0],[463,3,463,25,0,0,4,0],[469,3,469,22,0,0,4,0],[472,3,472,23,0,0,4,0],[475,3,475,26,0,0,4,0],[478,3,478,24,0,0,4,0]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_multi_get_handles","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_mime_free","curl_free","curl_multi_socket_action","curl_multi_notify_disable","curl_url_strerror","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_multi_add_handle","curl_multi_get_handles","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_mime_free","curl_free","curl_multi_socket_action","curl_multi_notify_disable","curl_url_strerror","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<13) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
        return 0;
    }

    // Initialize global curl
    curl_global_init(CURL_GLOBAL_ALL);

    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Create an easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Get handles from multi handle
    CURL **handles = curl_multi_get_handles(multi_handle);
    if (handles) {
        // Use the first handle if available
        CURL *first_handle = handles[0];
        if (first_handle) {
            // Create a mime structure
            curl_mime *mime = curl_mime_init(first_handle);
            if (mime) {
                // Create a mime part
                curl_mimepart *part = curl_mime_addpart(mime);
                if (part) {
                    // Write input data to a temporary file
                    FILE *temp_file = fopen("input_file", "wb");
                    if (temp_file) {
                        fwrite(data, 1, size, temp_file);
                        assert_file_closed(&temp_file);;
                        
                        // Set filedata for the mime part
                        curl_mime_filedata(part, fuzz_str_1);
                        
                        // Clean up the temporary file
                        remove("input_file");
                    }
                }
                curl_mime_free(mime);
            }
        }
        // Free the handles array
        curl_free(handles);
    }

    // Create a socket from input data
    if (size >= sizeof(curl_socket_t)) {
        curl_socket_t socket_fd;
        memcpy(&socket_fd, data, sizeof(curl_socket_t));
        
        // Use the socket with multi handle
        int running_handles = 0;
        curl_multi_socket(multi_handle, socket_fd, &running_handles);
    }

    // Disable notifications on multi handle
    // Use first byte of data as notification ID (modulo to keep within reasonable range)
    unsigned int notification_id = data[0] % 10;
    curl_multi_notify_disable(multi_handle, notification_id);

    // Get URL error string for a sample error code
    // Use second byte of data as error code (modulo to keep within CURLUcode range)
    CURLUcode error_code = static_cast<CURLUcode>(data[1 % size] % 20);
    const char *error_str = curl_url_strerror(error_code);
    (void)error_str; // Use the result to avoid unused variable warning

    // Clean up
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}