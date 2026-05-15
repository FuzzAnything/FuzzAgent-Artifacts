// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "/promefuzz/database/curl/latest/code/include/curl/curl.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static char *create_null_terminated_string(const uint8_t *data, size_t size, size_t max_len) {
    if (size == 0) return NULL;
    size_t len = size > max_len ? max_len : size;
    char *str = malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize curl
    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    // Create mime structure and part
    curl_mime *mime = curl_mime_init(curl);
    if (!mime) {
        curl_easy_cleanup(curl);
        return 0;
    }
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(curl);
        return 0;
    }

    // Use first byte to determine which functions to call
    uint8_t selector = Data[0];
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;

    // Test curl_mime_filedata
    if (selector & 0x01) {
        write_dummy_file(remaining_data, remaining_size);
        CURLcode res = curl_mime_filedata(part, "./dummy_file");
        if (res == CURLE_OK) {
            curl_mime_filename(part, NULL);  // Clear unwanted filename as per documentation
        }
    }

    // Test curl_slist_append and curl_slist_free_all
    if (selector & 0x02) {
        char *str1 = create_null_terminated_string(remaining_data, remaining_size, 100);
        if (str1) {
            struct curl_slist *slist = NULL;
            slist = curl_slist_append(slist, str1);
            
            // Append another string if we have enough data
            if (remaining_size > 50) {
                char *str2 = create_null_terminated_string(remaining_data + 50, 
                                                          remaining_size - 50, 100);
                if (str2) {
                    slist = curl_slist_append(slist, str2);
                    free(str2);
                }
            }
            
            curl_slist_free_all(slist);
            free(str1);
        }
    }

    // Test curl_mime_headers
    if (selector & 0x04) {
        char *header_str = create_null_terminated_string(remaining_data, remaining_size, 200);
        if (header_str) {
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, header_str);
            
            // Use second byte (if available) to determine take_ownership
            int take_ownership = (remaining_size > 1) ? (remaining_data[1] & 1) : 0;
            CURLcode res = curl_mime_headers(part, headers, take_ownership);
            
            // If we didn't transfer ownership, free the list ourselves
            if (res != CURLE_OK || !take_ownership) {
                curl_slist_free_all(headers);
            }
            free(header_str);
        }
    }

    // Test curl_mime_encoder
    if (selector & 0x08) {
        char *encoding = create_null_terminated_string(remaining_data, remaining_size, 50);
        if (encoding) {
            curl_mime_encoder(part, encoding);
            
            // Also test removing encoder
            if (remaining_size > 10 && (remaining_data[0] & 1)) {
                curl_mime_encoder(part, NULL);
            }
            free(encoding);
        }
    }

    // Test curl_mime_type
    if (selector & 0x10) {
        char *mimetype = create_null_terminated_string(remaining_data, remaining_size, 100);
        if (mimetype) {
            curl_mime_type(part, mimetype);
            
            // Also test clearing mimetype
            if (remaining_size > 20 && (remaining_data[1] & 1)) {
                curl_mime_type(part, NULL);
            }
            free(mimetype);
        }
    }

    // Test multiple functions in sequence to explore more states
    if (selector & 0x20) {
        // Create a slist and use it for headers with ownership
        char *multi_str = create_null_terminated_string(remaining_data, remaining_size, 150);
        if (multi_str) {
            struct curl_slist *multi_headers = NULL;
            multi_headers = curl_slist_append(multi_headers, multi_str);
            
            // Set headers with ownership
            curl_mime_headers(part, multi_headers, 1);
            
            // Set encoder
            curl_mime_encoder(part, "base64");
            
            // Set mimetype
            curl_mime_type(part, "application/octet-stream");
            
            free(multi_str);
        }
    }

    // Cleanup
    curl_mime_free(mime);
    curl_easy_cleanup(curl);
    
    return 0;
}