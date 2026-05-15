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
//<ID> 319
//<Prompt> []
/*<Combination>: [
*/
//<score> 6.5, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_unescape","curl_free","curl_easy_init","curl_easy_setopt","curl_ws_start_frame","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_mime_data","curl_mime_free","curl_easy_recv","curl_easy_reset","curl_easy_cleanup","curl_multi_init","curl_multi_wait","curl_multi_cleanup","curl_maprintf","curl_free","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_unescape","curl_free","curl_easy_init","curl_easy_setopt","curl_ws_start_frame","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_mime_data","curl_mime_free","curl_easy_recv","curl_easy_reset","curl_easy_cleanup","curl_multi_init","curl_multi_wait","curl_multi_cleanup","curl_maprintf","curl_free","curl_global_cleanup"],"visited":1}
/**/


// Include necessary curl headers

// Helper function for curl_mvsprintf
static int  id_000024_my_curl_mvsprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvsprintf(buffer, format, args);
    va_end(args);
    return result;
}

// Helper function for curl_mvfprintf
static int  id_000024_my_curl_mvfprintf(FILE *fd, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvfprintf(fd, format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<25) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Initialize curl
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Use curl_unescape on the input data (treat as escaped string)
    char *unescaped = curl_unescape((const char *)data, (int)(size > 100 ? 100 : size));
    if (unescaped) {
        // Use the unescaped string in various ways
        // ... could be used in format strings or other places
        curl_free(unescaped);
    }

    // 2. Create easy handle and set up for WebSocket
    CURL *easy = curl_easy_init();
    if (easy) {
        // Set up a simple URL (won't actually connect in fuzzing)
        curl_easy_setopt(easy, CURLOPT_URL, "ws://localhost:8080");
        
        // 3. Start a WebSocket frame (simulated)
        // Use first 4 bytes for flags (if available)
        unsigned int ws_flags = 0;
        curl_off_t frame_len = 0;
        if (size >= 8) {
            ws_flags = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
            frame_len = data[4] | (data[5] << 8) | (data[6] << 16) | (data[7] << 24);
        }
        curl_ws_start_frame(easy, fuzz_uint32_t_1, fuzz_int64_t_2);
        
        // 4. Create MIME structure and set encoder
        curl_mime *mime = curl_mime_init(easy);
        if (mime) {
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part) {
                // Use a sample encoding (base64)
                curl_mime_encoder(part, fuzz_str_3);
                
                // Set some data to the mime part
                if (size > 0) {
                    curl_mime_data(part, (const char *)data, size > 1024 ? 1024 : size);
                }
            }
            curl_mime_free(mime);
        }
        
        // 5. Use curl_easy_recv (simulated - won't actually receive in fuzzing)
        char recv_buffer[256];
        size_t n = 0;
        curl_easy_recv(easy, recv_buffer, sizeof(recv_buffer), &n);
        
        // 6. Reset the easy handle
        curl_easy_reset(easy);
        
        curl_easy_cleanup(easy);
    }

    // 7. Create multi handle and use curl_multi_wait
    CURLM *multi = curl_multi_init();
    if (multi) {
        // Create a waitfd from the input file descriptor
        struct curl_waitfd wait_fd;
        wait_fd.fd = fuzz_fileno(in_file);
        wait_fd.events = CURL_WAIT_POLLIN;
        wait_fd.revents = 0;
        
        int ret = 0;
        // Wait with 0 timeout (non-blocking)
        curl_multi_wait(multi, &wait_fd, 1, fuzz_int32_t_4, &ret);
        
        curl_multi_cleanup(multi);
    }

    // 8. Use curl_maprintf with a format string
    char *formatted_str = nullptr;
    if (size > 0) {
        // Create a format string using the input data
        char format_buffer[256];
        size_t format_len = size > 255 ? 255 : size;
        memcpy(format_buffer, data, format_len);
        format_buffer[format_len] = '\0';
        
        formatted_str = curl_maprintf("Formatted: %s", format_buffer);
        if (formatted_str) {
            curl_free(formatted_str);
        }
    }

    // 9. Use curl_mvsprintf
    char vsprintf_buffer[256];
    if (size > 0) {
        // Create a simple format string
        const char *format_str = "Received %zu bytes";
         id_000024_my_curl_mvsprintf(vsprintf_buffer, format_str, size);
    }

    // 10. Use curl_mvfprintf to write to output file
    if (size > 0) {
        const char *output_format = "Fuzzed data size: %zu\n";
         id_000024_my_curl_mvfprintf(out_file, output_format, size);
        
        // Write some of the data to the file
        if (size > 0) {
            size_t write_size = size > 100 ? 100 : size;
            fwrite(data, 1, write_size, out_file);
        }
    }

    // Cleanup
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    curl_global_cleanup();
    
    // Remove the output file created during fuzzing
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}