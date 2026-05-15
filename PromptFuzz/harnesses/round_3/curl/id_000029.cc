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
//<ID> 465
//<Prompt> []
/*<Combination>: [
*/
//<score> 96, nr_unique_branch: 18
//<Quality> {"density":16,"unique_branches":{"Curl_getformdata":[[739,9,739,16,0,0,4,1],[758,22,758,29,0,0,4,1],[803,14,803,21,0,0,4,1],[803,25,803,58,0,0,4,1],[829,10,829,17,0,0,4,1],[836,6,836,12,0,0,4,0]],"FormAddCheck":[[232,9,232,42,0,0,4,0],[233,9,233,45,0,0,4,1],[244,9,244,42,0,0,4,0],[247,39,247,70,0,0,4,1]],"read_part_content":[[719,10,719,18,0,0,4,0],[719,22,719,36,0,0,4,0]],"FormAdd":[[434,5,434,23,0,0,4,0],[436,10,436,39,0,0,4,1],[458,12,458,18,0,0,4,0],[459,14,459,71,0,0,4,1],[311,26,311,31,16,0,4,1]],"curl_formget":[[638,6,638,13,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_sslset","curl_multi_init","curl_global_cleanup","curl_multi_setopt","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_easy_pause","curl_easy_pause","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_filename","curl_mime_filedata","curl_slist_append","curl_mime_headers","curl_mime_free","curl_formadd","curl_formadd","curl_formget","curl_formfree","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_sslset","curl_multi_init","curl_multi_setopt","curl_easy_init","curl_multi_add_handle","curl_easy_pause","curl_easy_pause","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_filename","curl_mime_filedata","curl_slist_append","curl_mime_headers","curl_mime_free","curl_formadd","curl_formadd","curl_formget","curl_formfree","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include necessary curl headers
extern "C" {
}

// Formget callback function
static size_t  id_000029_formget_callback(void *arg, const char *data, size_t size) {
    FILE *out_file = reinterpret_cast<FILE*>(arg);
    if (out_file && data) {
        return fwrite(data, 1, size, out_file);
    }
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<54) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Initialize curl global state
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Initialize SSL backend (use first available)
    const curl_ssl_backend ***avail = nullptr;
    curl_sslbackend id = CURLSSLBACKEND_NONE;
    curl_global_sslset(id, nullptr, avail);
    
    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Set a multi handle option
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, 5L);
    
    // Create an easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Pause and resume the easy handle
    curl_easy_pause(easy_handle, CURLPAUSE_RECV);
    curl_easy_pause(easy_handle, CURLPAUSE_CONT);
    
    // Create a mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (mime) {
        // Create a mime part
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use first part of data for mime name (ensure null termination)
            size_t name_len = size > 0 ? (size % 100) + 1 : 1;
            std::vector<char> name_buf(name_len);
            if (size > 0) {
                size_t copy_len = (name_len - 1 < size) ? name_len - 1 : size;
                memcpy(name_buf.data(), data, copy_len);
            }
            name_buf[name_len - 1] = '\0';
            curl_mime_name(part, name_buf.data());
            
            // Use next part for filename
            size_t filename_offset = name_len - 1;
            size_t filename_len = (size > filename_offset) ? 
                ((size - filename_offset) % 100) + 1 : 1;
            std::vector<char> filename_buf(filename_len);
            if (size > filename_offset) {
                size_t copy_len = (filename_len - 1 < size - filename_offset) ? 
                    filename_len - 1 : size - filename_offset;
                memcpy(filename_buf.data(), data + filename_offset, copy_len);
            }
            filename_buf[filename_len - 1] = '\0';
            curl_mime_filename(part, filename_buf.data());
            
            // Create a temporary file with input data
            FILE *in_file = fmemopen((void *)data, size, "rb");
            if (in_file) {
                // Write data to a temporary file for mime_filedata
                FILE *tmp_file = fopen("input_file", "wb");
                if (tmp_file) {
                    if (size > 0) {
                        fwrite(data, 1, size, tmp_file);
                    }
                    assert_file_closed(&tmp_file);;
                    
                    // Use the file for mime_filedata
                    curl_mime_filedata(part, fuzz_str_1);
                }
                assert_file_closed(&in_file);;
            }
            
            // Create headers for mime part
            struct curl_slist *headers = nullptr;
            headers = curl_slist_append(headers, fuzz_str_2);
            if (headers) {
                curl_mime_headers(part, headers, 1); // Take ownership
            }
        }
        
        // Clean up mime
        curl_mime_free(mime);
    }
    
    // Create HTTP POST form data using legacy API
    struct curl_httppost *formpost = nullptr;
    struct curl_httppost *lastptr = nullptr;
    
    // Add regular field
    size_t fieldname_len = size > 0 ? (size % 50) + 1 : 1;
    std::vector<char> fieldname_buf(fieldname_len);
    if (size > 0) {
        size_t copy_len = (fieldname_len - 1 < size) ? fieldname_len - 1 : size;
        memcpy(fieldname_buf.data(), data, copy_len);
    }
    fieldname_buf[fieldname_len - 1] = '\0';
    
    size_t fieldvalue_offset = fieldname_len - 1;
    size_t fieldvalue_len = (size > fieldvalue_offset) ? 
        ((size - fieldvalue_offset) % 50) + 1 : 1;
    std::vector<char> fieldvalue_buf(fieldvalue_len);
    if (size > fieldvalue_offset) {
        size_t copy_len = (fieldvalue_len - 1 < size - fieldvalue_offset) ? 
            fieldvalue_len - 1 : size - fieldvalue_offset;
        memcpy(fieldvalue_buf.data(), data + fieldvalue_offset, copy_len);
    }
    fieldvalue_buf[fieldvalue_len - 1] = '\0';
    
    curl_formadd(&formpost, &lastptr,
                 CURLFORM_COPYNAME, fieldname_buf.data(),
                 CURLFORM_COPYCONTENTS, fieldvalue_buf.data(),
                 CURLFORM_END);
    
    // Add file field
    curl_formadd(&formpost, &lastptr,
                 CURLFORM_COPYNAME, "filefield",
                 CURLFORM_FILE, "input_file",
                 CURLFORM_END);
    
    // Get form data using formget
    if (formpost) {
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            curl_formget(formpost, out_file,  id_000029_formget_callback);
            assert_file_closed(&out_file);;
        }
        
        // Free form data
        curl_formfree(formpost);
    }
    
    // Clean up temporary files
    remove("input_file");
    remove("output_file");
    
    // Remove easy handle from multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}