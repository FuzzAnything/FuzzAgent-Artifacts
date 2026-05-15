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
//<ID> 227
//<Prompt> []
/*<Combination>: [
*/
//<score> 121, nr_unique_branch: 184
//<Quality> {"density":11,"unique_branches":{"checktz":[[243,6,243,13,0,0,4,0],[243,6,243,13,0,0,4,1],[246,14,246,32,0,0,4,0],[246,14,246,32,0,0,4,1],[248,8,248,21,0,0,4,0],[248,8,248,21,0,0,4,1],[249,8,249,46,0,0,4,0],[249,8,249,46,0,0,4,1]],"checkday":[[205,6,205,13,0,0,4,0],[205,6,205,13,0,0,4,1],[207,11,207,19,0,0,4,0],[207,11,207,19,0,0,4,1],[211,14,211,19,0,0,4,0],[211,14,211,19,0,0,4,1],[213,8,213,21,0,0,4,0],[213,8,213,21,0,0,4,1],[214,8,214,43,0,0,4,0],[214,8,214,43,0,0,4,1]],"oneortwodigit":[[44,22,44,34,1,0,4,0],[44,22,44,34,1,0,4,1],[44,38,44,50,1,0,4,0],[44,38,44,50,1,0,4,1]],"curl_getenv":[[67,11,67,14,0,0,4,0],[67,18,67,24,0,0,4,0]],"parsedate":[[363,9,363,14,0,0,4,0],[363,9,363,14,0,0,4,1],[363,18,363,28,0,0,4,0],[363,18,363,28,0,0,4,1],[372,28,372,44,0,0,4,0],[372,28,372,44,0,0,4,1],[377,10,377,25,0,0,4,0],[377,10,377,25,0,0,4,1],[378,12,378,25,0,0,4,0],[378,12,378,25,0,0,4,1],[380,14,380,27,0,0,4,0],[380,14,380,27,0,0,4,1],[383,12,383,18,0,0,4,0],[383,12,383,18,0,0,4,1],[383,22,383,36,0,0,4,0],[383,22,383,36,0,0,4,1],[385,14,385,26,0,0,4,0],[385,14,385,26,0,0,4,1],[389,12,389,18,0,0,4,0],[389,12,389,18,0,0,4,1],[389,22,389,35,0,0,4,0],[389,22,389,35,0,0,4,1],[392,14,392,25,0,0,4,0],[392,14,392,25,0,0,4,1],[396,10,396,16,0,0,4,0],[396,10,396,16,0,0,4,1],[405,10,405,24,0,0,4,0],[405,10,405,24,0,0,4,1],[406,10,406,60,0,0,4,0],[406,10,406,60,0,0,4,1],[414,12,414,49,0,0,4,0],[414,12,414,49,0,0,4,1],[421,12,421,25,0,0,4,0],[421,12,421,25,0,0,4,1],[422,12,422,29,0,0,4,0],[422,12,422,29,0,0,4,1],[423,12,423,25,0,0,4,0],[423,12,423,25,0,0,4,1],[424,12,424,27,0,0,4,0],[424,12,424,27,0,0,4,1],[425,13,425,28,0,0,4,0],[425,13,425,28,0,0,4,1],[425,32,425,47,0,0,4,0],[425,32,425,47,0,0,4,1],[439,19,439,34,0,0,4,0],[439,19,439,34,0,0,4,1],[442,17,442,34,0,0,4,0],[442,17,442,34,0,0,4,1],[443,17,443,32,0,0,4,0],[443,17,443,32,0,0,4,1],[444,17,444,31,0,0,4,0],[444,17,444,31,0,0,4,1],[445,17,445,32,0,0,4,0],[445,17,445,32,0,0,4,1],[453,12,453,18,0,0,4,0],[453,12,453,18,0,0,4,1],[453,22,453,44,0,0,4,0],[453,22,453,44,0,0,4,1],[453,48,453,63,0,0,4,0],[453,48,453,63,0,0,4,1],[454,14,454,23,0,0,4,0],[454,14,454,23,0,0,4,1],[454,27,454,37,0,0,4,0],[454,27,454,37,0,0,4,1],[461,12,461,18,0,0,4,0],[461,12,461,18,0,0,4,1],[461,22,461,44,0,0,4,0],[461,22,461,44,0,0,4,1],[461,48,461,63,0,0,4,0],[461,48,461,63,0,0,4,1],[464,14,464,27,0,0,4,0],[464,14,464,27,0,0,4,1],[465,16,465,28,0,0,4,0],[465,16,465,28,0,0,4,1],[470,14,470,27,0,0,4,0],[470,14,470,27,0,0,4,1],[474,12,474,18,0,0,4,0],[474,12,474,18,0,0,4,1],[484,6,484,18,0,0,4,0],[484,6,484,18,0,0,4,1],[487,6,487,21,0,0,4,0],[487,6,487,21,0,0,4,1],[488,6,488,20,0,0,4,0],[488,6,488,20,0,0,4,1],[489,6,489,21,0,0,4,0],[489,6,489,21,0,0,4,1],[523,6,523,20,0,0,4,0],[523,6,523,20,0,0,4,1],[527,6,527,20,0,0,4,0],[527,6,527,20,0,0,4,1],[527,24,527,37,0,0,4,0],[527,24,527,37,0,0,4,1],[528,6,528,20,0,0,4,1],[528,24,528,37,0,0,4,1],[528,41,528,54,0,0,4,1],[538,6,538,17,0,0,4,0],[538,6,538,17,0,0,4,1],[541,6,541,17,0,0,4,1],[44,22,44,34,10,0,4,0],[44,22,44,34,10,0,4,1],[44,38,44,50,10,0,4,0],[43,22,43,34,24,0,4,0],[43,22,43,34,24,0,4,1],[43,38,43,50,24,0,4,0],[42,22,42,34,25,0,4,0],[42,22,42,34,25,0,4,1],[42,38,42,50,25,0,4,0],[43,22,43,34,26,0,4,0],[43,22,43,34,26,0,4,1],[43,38,43,50,26,0,4,0],[43,38,43,50,26,0,4,1],[42,22,42,34,27,0,4,0],[42,22,42,34,27,0,4,1],[42,38,42,50,27,0,4,0],[42,38,42,50,27,0,4,1]],"curl_getdate":[[568,6,568,24,0,0,4,0],[568,6,568,24,0,0,4,1],[569,8,569,28,0,0,4,0],[569,8,569,28,0,0,4,1]],"match_time":[[309,6,309,15,0,0,4,0],[309,6,309,15,0,0,4,1],[309,19,309,30,0,0,4,0],[309,19,309,30,0,0,4,1],[311,8,311,15,0,0,4,0],[311,8,311,15,0,0,4,1],[312,10,312,21,0,0,4,0],[312,10,312,21,0,0,4,1],[314,12,314,20,0,0,4,0],[314,12,314,20,0,0,4,1],[44,22,44,34,1,0,4,0],[44,22,44,34,1,0,4,1],[44,38,44,50,1,0,4,0],[44,38,44,50,1,0,4,1],[44,22,44,34,2,0,4,0],[44,22,44,34,2,0,4,1],[44,38,44,50,2,0,4,0],[44,38,44,50,2,0,4,1]],"checkmonth":[[225,6,225,14,0,0,4,0],[225,6,225,14,0,0,4,1],[228,14,228,20,0,0,4,0],[228,14,228,20,0,0,4,1],[229,8,229,41,0,0,4,0],[229,8,229,41,0,0,4,1]],"skip":[[259,9,259,15,0,0,4,0],[259,9,259,15,0,0,4,1],[44,22,44,34,2,0,4,0],[44,22,44,34,2,0,4,1],[44,38,44,50,2,0,4,0],[44,38,44,50,2,0,4,1],[43,22,43,34,3,0,4,0],[43,22,43,34,3,0,4,1],[43,38,43,50,3,0,4,0],[43,38,43,50,3,0,4,1],[42,22,42,34,4,0,4,0],[42,22,42,34,4,0,4,1],[42,38,42,50,4,0,4,0],[42,38,42,50,4,0,4,1]],"curl_easy_strerror":[[60,3,60,29,0,0,4,1],[150,3,150,35,0,0,4,0],[177,3,177,24,0,0,4,0]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_getenv","curl_getdate","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_easy_strerror","curl_mime_data_cb","curl_ws_start_frame","curl_ws_send","curl_easy_strerror","curl_easy_strerror","curl_mime_free","curl_free","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_getenv","curl_getdate","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_easy_strerror","curl_mime_data_cb","curl_ws_start_frame","curl_ws_send","curl_easy_strerror","curl_easy_strerror","curl_mime_free","curl_free","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Custom callback functions needed for curl_mime_data_cb
static size_t  id_000019_read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    FILE *f = (FILE*)arg;
    return fread(buffer, size, nitems, f);
}

static int  id_000019_seek_callback(void *arg, curl_off_t offset, int origin) {
    FILE *f = (FILE*)arg;
    return fseek(f, offset, origin);
}

static void  id_000019_free_callback(void *arg) {
    // Nothing to free in this case
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
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
    
    // Create a CURL handle for WebSocket operations
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // 1. Use curl_getenv with environment variable name from input
    // Extract environment variable name from first part of data
    size_t env_name_len = size > 50 ? 50 : size/2;
    char *env_name = (char*)malloc(env_name_len + 1);
    if (!env_name) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }
    
    memcpy(env_name, data, env_name_len);
    env_name[env_name_len] = '\0';
    
    char *env_value = curl_getenv(env_name);
    free(env_name);
    
    // 2. Use curl_getdate with date string from input
    // Extract date string from next part of data
    size_t date_offset = env_name_len;
    size_t date_len = size - date_offset > 50 ? 50 : (size - date_offset)/2;
    if (date_len > 0) {
        char *date_str = (char*)malloc(date_len + 1);
        if (date_str) {
            memcpy(date_str, data + date_offset, date_len);
            date_str[date_len] = '\0';
            time_t parsed_time = curl_getdate(date_str, NULL);
            free(date_str);
        }
    }
    
    // 3. Create a mime part and set encoder
    curl_mime *mime = curl_mime_init(curl);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use part of input data as encoding type
            size_t encoding_len = size > 100 ? 20 : 10;
            char *encoding = (char*)malloc(encoding_len + 1);
            if (encoding) {
                memcpy(encoding, data + 50, encoding_len);
                encoding[encoding_len] = '\0';
                
                // Set encoder for the mime part
                CURLcode encoder_result = curl_mime_encoder(part, encoding);
                free(encoding);
                
                // 4. Use curl_easy_strerror to get error string
                const char *error_str = curl_easy_strerror(encoder_result);
                // Use error_str to avoid unused variable warning
                (void)error_str;
            }
            
            // Add some data to the mime part using callback
            FILE *in_file = fmemopen((void *)data, size, "rb");
            if (in_file) {
                curl_mime_data_cb(part, size,  id_000019_read_callback,  id_000019_seek_callback,  id_000019_free_callback, in_file);
                assert_file_closed(&in_file);;
            }
        }
    }
    
    // 5. WebSocket operations
    // Extract WebSocket frame data
    size_t ws_data_offset = size/3;
    size_t ws_data_len = size - ws_data_offset;
    
    if (ws_data_len > 0) {
        // Start a WebSocket frame
        unsigned int ws_flags = CURLWS_TEXT;
        curl_off_t frame_len = ws_data_len > 1024 ? 1024 : ws_data_len;
        
        CURLcode frame_result = curl_ws_start_frame(curl, ws_flags, frame_len);
        
        // Send WebSocket data
        size_t sent = 0;
        CURLcode send_result = curl_ws_send(curl, 
                                           data + ws_data_offset, 
                                           ws_data_len > 1024 ? 1024 : ws_data_len,
                                           &sent, 
                                           frame_len, 
                                           ws_flags);
        
        // Get error string for send result
        const char *ws_error_str = curl_easy_strerror(send_result);
        (void)ws_error_str;
        
        // Also get error string for frame result
        const char *frame_error_str = curl_easy_strerror(frame_result);
        (void)frame_error_str;
    }
    
    // 6. Clean up
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (env_value) {
        curl_free(env_value);
    }
    
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}