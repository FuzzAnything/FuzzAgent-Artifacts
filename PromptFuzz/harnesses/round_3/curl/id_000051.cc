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
//<ID> 888
//<Prompt> []
/*<Combination>: [
*/
//<score> 114, nr_unique_branch: 4
//<Quality> {"density":38,"unique_branches":{"cwfds_add_sock":[[451,32,451,38,0,0,4,0],[452,10,452,35,0,0,4,1]],"Curl_waitfds_add_ps":[[477,8,477,37,0,0,4,1],[479,8,479,38,0,0,4,0]]},"library_calls":["curl_global_init","curl_share_init","curl_global_cleanup","curl_multi_init","curl_share_cleanup","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_slist_append","curl_mime_headers","curl_mime_name","curl_mime_data_cb","curl_formadd","curl_formfree","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_waitfds","curl_multi_poll","curl_multi_socket_action","curl_multi_info_read","curl_multi_remove_handle","curl_formget","curl_easy_nextheader","curl_unescape","curl_free","curl_share_strerror","curl_formfree","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_mime_headers","curl_mime_name","curl_mime_data_cb","curl_formadd","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_waitfds","curl_multi_poll","curl_multi_socket_action","curl_multi_info_read","curl_multi_remove_handle","curl_formget","curl_easy_nextheader","curl_unescape","curl_free","curl_share_strerror","curl_formfree","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":0}
/*To create a comprehensive fuzz driver that utilizes all specified curl APIs, I'll implement a scenario where we perform HTTP form submission with MIME data using libcurl's multi interface. Here's the complete implementation:

*/


// Include necessary curl headers
extern "C" {
}

// Custom callback structures
struct ReadCallbackData {
    const uint8_t* data;
    size_t size;
    size_t position;
    FILE* in_file;
};

struct WriteCallbackData {
    FILE* out_file;
    size_t total_written;
};

// Callback functions
static size_t  id_000051_read_callback(char *buffer, size_t size, size_t nitems, void *userdata) {
    ReadCallbackData* data = static_cast<ReadCallbackData*>(userdata);
    size_t total_size = size * nitems;
    size_t remaining = data->size - data->position;
    size_t to_copy = (total_size < remaining) ? total_size : remaining;
    
    if (to_copy == 0) {
        return 0;
    }
    
    memcpy(buffer, data->data + data->position, to_copy);
    data->position += to_copy;
    return to_copy;
}

static int  id_000051_seek_callback(void *userdata, curl_off_t offset, int origin) {
    ReadCallbackData* data = static_cast<ReadCallbackData*>(userdata);
    
    if (origin == SEEK_SET) {
        if (offset >= 0 && offset <= static_cast<curl_off_t>(data->size)) {
            data->position = static_cast<size_t>(offset);
            return CURL_SEEKFUNC_OK;
        }
    } else if (origin == SEEK_CUR) {
        curl_off_t new_pos = static_cast<curl_off_t>(data->position) + offset;
        if (new_pos >= 0 && new_pos <= static_cast<curl_off_t>(data->size)) {
            data->position = static_cast<size_t>(new_pos);
            return CURL_SEEKFUNC_OK;
        }
    } else if (origin == SEEK_END) {
        curl_off_t new_pos = static_cast<curl_off_t>(data->size) + offset;
        if (new_pos >= 0 && new_pos <= static_cast<curl_off_t>(data->size)) {
            data->position = static_cast<size_t>(new_pos);
            return CURL_SEEKFUNC_OK;
        }
    }
    
    return CURL_SEEKFUNC_FAIL;
}

static void  id_000051_free_callback(void *userdata) {
    ReadCallbackData* data = static_cast<ReadCallbackData*>(userdata);
    if (data->in_file) {
        fclose(data->in_file);
    }
    delete data;
}

static size_t  id_000051_formget_callback(void *arg, const char *buf, size_t len) {
    WriteCallbackData* data = static_cast<WriteCallbackData*>(arg);
    size_t written = fwrite(buf, 1, len, data->out_file);
    data->total_written += written;
    return written;
}

static size_t  id_000051_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    WriteCallbackData* data = static_cast<WriteCallbackData*>(userdata);
    size_t written = fwrite(ptr, 1, size * nmemb, data->out_file);
    data->total_written += written;
    return written;
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<41) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}




    if (size < 10) {
        return 0;
    }
    
    // Initialize global curl state
    CURLcode global_ret = curl_global_init(CURL_GLOBAL_ALL);
    if (global_ret != CURLE_OK) {
        return 0;
    }
    
    // Create share handle
    CURLSH* share = curl_share_init();
    if (!share) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create multi handle
    CURLM* multi = curl_multi_init();
    if (!multi) {
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Create easy handle
    CURL* easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Create MIME structure
    curl_mime* mime = curl_mime_init(easy);
    if (!mime) {
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Add MIME part
    curl_mimepart* mime_part = curl_mime_addpart(mime);
    if (!mime_part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // Create headers list
    struct curl_slist* headers = nullptr;
    char header_buf[256];
    
    // Add some headers using a portion of the input data
    size_t header_len = (size > 100) ? 100 : size;
    std::string header_data(reinterpret_cast<const char*>(data), header_len);
    snprintf(header_buf, sizeof(header_buf), "X-Fuzz-Data: %s", header_data.c_str());
    headers = curl_slist_append(headers, header_buf);
    
    // Set MIME headers
    curl_mime_headers(mime_part, headers, 1);
    
    // Set MIME name
    curl_mime_name(mime_part, fuzz_str_1);
    
    // Prepare callback data for MIME data
    ReadCallbackData* cb_data = new ReadCallbackData();
    cb_data->data = data;
    cb_data->size = size;
    cb_data->position = 0;
    cb_data->in_file = fmemopen((void*)data, size, "rb");
    
    // Set MIME data with callback
    curl_mime_data_cb(mime_part, static_cast<curl_off_t>(size),
                      id_000051_read_callback,  id_000051_seek_callback,  id_000051_free_callback, cb_data);
    
    // Create form data
    struct curl_httppost* formpost = nullptr;
    struct curl_httppost* lastptr = nullptr;
    
    // Add form fields using portions of input data
    size_t form_data_len = (size > 50) ? 50 : size;
    std::string form_field(reinterpret_cast<const char*>(data), form_data_len);
    
    curl_formadd(&formpost, &lastptr,
                CURLFORM_COPYNAME, "fuzz_form_field",
                CURLFORM_COPYCONTENTS, form_field.c_str(),
                CURLFORM_CONTENTSLENGTH, form_field.length(),
                CURLFORM_END);
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        curl_formfree(formpost);
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi);
        curl_share_cleanup(share);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Set easy handle options
    curl_easy_setopt(easy, CURLOPT_URL, "http://localhost/test");
    curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy, CURLOPT_HTTPPOST, formpost);
    curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION,  id_000051_write_callback);
    
    WriteCallbackData write_data;
    write_data.out_file = out_file;
    write_data.total_written = 0;
    curl_easy_setopt(easy, CURLOPT_WRITEDATA, &write_data);
    
    // Set multi handle options
    curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, 10L);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);
    
    // Perform multi operations
    int running_handles = 0;
    curl_multi_perform(multi, &running_handles);
    
    // Wait for file descriptors
    struct curl_waitfd ufds[10];
    unsigned int fd_count = 0;
    curl_multi_waitfds(multi, ufds, 10, &fd_count);
    
    // Poll for activity
    int ret = 0;
    curl_multi_poll(multi, nullptr, 0, fuzz_int32_t_2, &ret);
    
    // Simulate socket action (using stdin as fake socket)
    curl_socket_t fake_socket = fuzz_fileno(stdin);
    curl_multi_socket_action(multi, fake_socket, fuzz_int32_t_3, &running_handles);
    
    // Read multi info
    int msgs_in_queue = 0;
    CURLMsg* msg = curl_multi_info_read(multi, &msgs_in_queue);
    
    // Remove handle from multi
    curl_multi_remove_handle(multi, easy);
    
    // Get form data using formget callback
    fseek(out_file, 0, SEEK_SET);
    WriteCallbackData formget_data;
    formget_data.out_file = out_file;
    formget_data.total_written = 0;
    curl_formget(formpost, &formget_data,  id_000051_formget_callback);
    
    // Try to get next header (even though we might not have any)
    struct curl_header* header = curl_easy_nextheader(easy, CURLH_HEADER, fuzz_int32_t_4, nullptr);
    
    // Unescape a sample string
    const char* escaped_string = "test%20string";
    char* unescaped = curl_unescape(fuzz_str_5, static_cast<int>(fuzz_str_sz_5));
    if (unescaped) {
        curl_free(unescaped);
    }
    
    // Get share error string (just to call the function)
    const char* share_error = curl_share_strerror(CURLSHE_OK);
    
    // Cleanup everything
    curl_formfree(formpost);
    curl_mime_free(mime);
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    curl_share_cleanup(share);
    assert_file_closed(&out_file);;
    
    // Remove the output file
    remove("output_file");
    
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fake_socket);
	return 0;
}