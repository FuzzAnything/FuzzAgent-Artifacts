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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 182, nr_unique_branch: 86
//<Quality> {"density":13,"unique_branches":{"Curl_hash_clean":[[280,11,280,19,0,0,4,1]],"AddHttpPost":[[64,6,64,17,0,0,4,0],[64,21,64,48,0,0,4,0],[66,6,66,36,0,0,4,1],[66,40,66,63,0,0,4,1],[70,6,70,10,0,0,4,0],[86,6,86,17,0,0,4,1],[95,8,95,18,0,0,4,1]],"curl_formfree":[[668,6,668,11,0,0,4,0],[668,6,668,11,0,0,4,1],[678,8,678,41,0,0,4,0],[680,8,681,71,0,0,4,0],[687,11,687,15,0,0,4,1]],"FormAddCheck":[[226,26,226,38,0,0,4,0],[226,26,226,38,0,0,4,1],[229,10,229,15,0,0,4,1],[229,19,229,49,0,0,4,1],[230,9,230,29,0,0,4,1],[232,9,232,42,0,0,4,1],[235,9,235,22,0,0,4,0],[236,9,236,40,0,0,4,1],[239,9,239,42,0,0,4,1],[244,9,244,42,0,0,4,1],[245,9,245,40,0,0,4,1],[259,8,259,12,0,0,4,0],[259,16,259,32,0,0,4,1],[263,8,263,41,0,0,4,0],[266,10,266,58,0,0,4,1],[269,8,271,44,0,0,4,0],[272,10,272,71,0,0,4,1],[277,8,277,13,0,0,4,1],[280,8,280,43,0,0,4,1]],"curl_easy_reset":[[1087,6,1087,23,0,0,4,1],[134,5,134,8,1,0,4,0],[134,12,134,49,1,0,4,0]],"mime_subparts_unbind":[[1061,14,1061,26,0,0,4,1]],"FormInfoCopyField":[[126,6,126,11,0,0,4,0],[127,8,127,12,0,0,4,0]],"curl_mime_type":[[1351,6,1351,11,0,0,4,1],[1356,6,1356,14,0,0,4,0],[1358,8,1358,23,0,0,4,1]],"Curl_bufref_memdup0":[[130,6,130,9,0,0,4,0],[132,8,132,12,0,0,4,1]],"FormAdd":[[322,6,322,17,0,0,4,1],[330,9,330,34,0,0,4,0],[333,8,333,13,0,0,4,1],[350,10,350,32,0,0,4,0],[350,10,350,32,0,0,4,1],[355,5,355,24,0,0,4,1],[369,5,369,26,0,0,4,1],[372,5,372,27,0,0,4,0],[372,5,372,27,0,0,4,1],[373,10,373,38,0,0,4,1],[377,12,377,18,0,0,4,0],[383,5,383,29,0,0,4,1],[393,5,393,30,0,0,4,1],[396,5,396,31,0,0,4,0],[396,5,396,31,0,0,4,1],[397,10,397,39,0,0,4,1],[401,12,401,18,0,0,4,0],[407,5,407,33,0,0,4,1],[411,5,411,29,0,0,4,1],[417,5,417,30,0,0,4,1],[434,5,434,23,0,0,4,1],[469,5,469,28,0,0,4,1],[485,5,485,31,0,0,4,1],[492,5,492,25,0,0,4,1],[510,5,510,30,0,0,4,1],[541,5,541,32,0,0,4,1],[553,5,553,27,0,0,4,1],[554,5,554,25,0,0,4,1],[562,5,562,12,0,0,4,1],[568,6,568,13,0,0,4,0],[571,6,571,12,0,0,4,1],[579,9,579,19,0,0,4,0],[579,9,579,19,0,0,4,1],[585,6,585,13,0,0,4,0],[587,8,587,18,0,0,4,1],[311,26,311,31,4,0,4,1],[311,26,311,31,8,0,4,1]],"Curl_mime_cleanpart":[[1073,8,1073,44,0,0,4,0]],"curl_mime_headers":[[1401,6,1401,11,0,0,4,1],[1404,6,1404,42,0,0,4,1],[1410,6,1410,13,0,0,4,0],[1410,17,1410,31,0,0,4,0]],"NewFormInfo":[[110,6,110,15,0,0,4,0]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_type","curl_slist_append","curl_slist_append","curl_mime_headers","curl_mime_data","curl_easy_setopt","curl_formadd","curl_easy_reset","curl_mime_free","curl_formfree","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_type","curl_slist_append","curl_slist_append","curl_mime_headers","curl_mime_data","curl_easy_setopt","curl_formadd","curl_easy_reset","curl_mime_free","curl_formfree","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0; // Not enough data to process
    }

    // Initialize curl global state
    curl_global_init(CURL_GLOBAL_ALL);

    // Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }

    // Create multipart form data structure
    struct curl_httppost *formpost = nullptr;
    struct curl_httppost *lastptr = nullptr;
    
    // Add form field using the first part of input data as field name
    // Split input into parts for different purposes
    size_t name_len = size / 3;
    size_t type_len = size / 3;
    size_t data_len = size - name_len - type_len;
    
    if (name_len == 0 || type_len == 0 || data_len == 0) {
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }

    // Create mime structure for multipart data
    curl_mime *mime = curl_mime_init(easy);
    curl_mimepart *part = curl_mime_addpart(mime);
    
    if (part && mime) {
        // Set mime part name using first segment of data
        char *name_buf = new char[name_len + 1];
        memcpy(name_buf, data, name_len);
        name_buf[name_len] = '\0';
        curl_mime_name(part, name_buf);
        delete[] name_buf;

        // Set mime type using second segment of data
        char *type_buf = new char[type_len + 1];
        memcpy(type_buf, data + name_len, type_len);
        type_buf[type_len] = '\0';
        curl_mime_type(part, type_buf);
        delete[] type_buf;

        // Create headers list for the mime part
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "X-Fuzzed-Header: value");
        headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
        
        // Set headers on mime part (taking ownership)
        curl_mime_headers(part, headers, 1); // take_ownership = 1

        // Set mime data using remaining input
        curl_mime_data(part, reinterpret_cast<const char*>(data + name_len + type_len), data_len);
        
        // Set mime data to easy handle
        curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
    }

    // Create traditional form data as well
    char *form_name = new char[10];
    char *form_content = new char[10];
    memcpy(form_name, "field", 5);
    memcpy(form_content, "value", 5);
    form_name[5] = '\0';
    form_content[5] = '\0';
    
    curl_formadd(&formpost, &lastptr,
                 CURLFORM_COPYNAME, form_name,
                 CURLFORM_COPYCONTENTS, form_content,
                 CURLFORM_END);
    
    delete[] form_name;
    delete[] form_content;

    // Reset easy handle (clears all options but keeps mime structure)
    curl_easy_reset(easy);

    // Free resources
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (formpost) {
        curl_formfree(formpost);
    }
    
    curl_easy_cleanup(easy);
    curl_global_cleanup();

    return 0;
}