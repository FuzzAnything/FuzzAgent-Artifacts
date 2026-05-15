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
//<ID> 12
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 1
//<Quality> {"density":24,"unique_branches":{"curl_mime_headers":[[1409,17,1409,31,0,0,4,1]]},"library_calls":["curl_global_sslset","curl_easy_init","curl_mime_init","curl_easy_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_mime_free","curl_easy_cleanup","curl_slist_append","curl_slist_append","curl_mime_headers","curl_strnequal","curl_mime_encoder","curl_strnequal","curl_mime_encoder","curl_mime_encoder","curl_mime_encoder","curl_mime_data","curl_easy_option_by_name","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_pause","curl_easy_pause","curl_slist_free_all","curl_mime_free","curl_easy_cleanup"],"critical_calls":["curl_global_sslset","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_slist_append","curl_mime_headers","curl_strnequal","curl_strnequal","curl_mime_encoder","curl_mime_data","curl_easy_option_by_name","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_pause","curl_easy_pause","curl_slist_free_all","curl_mime_free","curl_easy_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<73) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
	//fuzzer shim end}




    if (size < 10) {
        return 0;
    }

    // Initialize global SSL backend first
    curl_global_sslset(CURLSSLBACKEND_NONE, nullptr, nullptr);

    // Create a MIME structure for email/multipart handling
    CURL *easy = curl_easy_init();
    if (!easy) {
        return 0;
    }

    curl_mime *mime = curl_mime_init(easy);
    if (!mime) {
        curl_easy_cleanup(easy);
        return 0;
    }

    // Add a MIME part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        return 0;
    }

    // Create headers list for the MIME part
    struct curl_slist *headers = nullptr;
    
    // Use first 20 bytes for header data if available
    size_t header_len = (size > 20) ? 20 : size / 2;
    char *header_data = static_cast<char*>(malloc(header_len + 1));
    if (!header_data) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        return 0;
    }
    
    memcpy(header_data, data, header_len);
    header_data[header_len] = '\0';
    
    // Add multiple headers
    char header1[256], header2[256];
    snprintf(header1, sizeof(header1), "Content-Type: %s", header_data);
    snprintf(header2, sizeof(header2), "Content-Disposition: attachment");
    
    headers = curl_slist_append(headers, header1);
    headers = curl_slist_append(headers, header2);
    
    // Set headers with take_ownership = 0 (we'll free them ourselves)
    curl_mime_headers(part, headers, 0);

    // Set encoder for the MIME part
    // Use a portion of input to determine encoding type
    size_t encoder_offset = header_len % (size - header_len);
    if (encoder_offset + 10 < size) {
        char encoding[11];
        memcpy(encoding, data + encoder_offset, 10);
        encoding[10] = '\0';
        
        // Check if encoding matches known types using curl_strnequal
        if (curl_strnequal(encoding, fuzz_str_1, static_cast<size_t>(fuzz_str_sz_1))) {
            curl_mime_encoder(part, fuzz_str_2);
        } else if (curl_strnequal(encoding, fuzz_str_3, static_cast<size_t>(fuzz_str_sz_3))) {
            curl_mime_encoder(part, fuzz_str_4);
        } else {
            curl_mime_encoder(part, fuzz_str_5);
        }
    } else {
        curl_mime_encoder(part, fuzz_str_6);
    }

    // Set MIME data from remaining input
    size_t data_offset = (header_len + encoder_offset) % size;
    size_t data_size = size - data_offset;
    if (data_size > 0) {
        char *mime_data = static_cast<char*>(malloc(data_size));
        if (mime_data) {
            memcpy(mime_data, data + data_offset, data_size);
            curl_mime_data(part, mime_data, data_size);
            free(mime_data);
        }
    }

    // Look up an easy option by name
    const struct curl_easyoption *opt = curl_easy_option_by_name(fuzz_str_7);
    if (opt) {
        curl_easy_setopt(easy, opt->id, mime);
    }

    // Create a FILE for output
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Set output to file
        curl_easy_setopt(easy, CURLOPT_WRITEDATA, out_file);
        
        // Set a dummy URL
        curl_easy_setopt(easy, CURLOPT_URL, "http://example.com/upload");
        
        // Perform with pause/resume
        curl_easy_pause(easy, CURLPAUSE_RECV);
        curl_easy_pause(easy, CURLPAUSE_CONT);
        
        // Clean up file
        assert_file_closed(&out_file);;
    }

    // Cleanup
    curl_slist_free_all(headers);
    curl_mime_free(mime);
    curl_easy_cleanup(easy);
    
    free(header_data);
    
    assert_file_closed(&out_file);
	return 0;
}