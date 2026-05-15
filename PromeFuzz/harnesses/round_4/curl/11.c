// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a dummy mime structure and part
    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) {
        return 0;
    }
    
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        return 0;
    }

    // Use first byte to determine which functions to call
    uint8_t selector = Data[0];
    size_t offset = 1;

    // Prepare a null-terminated string from input data for name/filename/encoding
    char *str_param = NULL;
    if (offset < Size) {
        size_t str_len = Size - offset;
        if (str_len > 256) str_len = 256; // Limit string length
        str_param = (char *)malloc(str_len + 1);
        if (str_param) {
            memcpy(str_param, Data + offset, str_len);
            str_param[str_len] = '\0';
        }
    }

    // Test curl_mime_name
    if (selector & 0x01) {
        curl_mime_name(part, str_param ? str_param : "test_name");
    }

    // Test curl_mime_filedata - create a dummy file
    if (selector & 0x02) {
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            if (Size > 0) {
                fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
            }
            fclose(fp);
            
            curl_mime_filedata(part, "./dummy_file");
            
            // Clear unwanted filename as mentioned in documentation
            curl_mime_filename(part, NULL);
            
            // Remove the dummy file
            remove("./dummy_file");
        }
    }

    // Test curl_slist_append and curl_mime_headers
    if (selector & 0x04) {
        struct curl_slist *headers = NULL;
        
        // Append some headers using fuzzed data
        if (str_param) {
            headers = curl_slist_append(headers, str_param);
        }
        headers = curl_slist_append(headers, "Content-Type: text/plain");
        headers = curl_slist_append(headers, "X-Fuzzed: yes");
        
        // Test with different ownership values
        int take_ownership = (selector & 0x08) ? 1 : 0;
        curl_mime_headers(part, headers, take_ownership);
        
        // If we didn't transfer ownership, free the list ourselves
        if (!take_ownership) {
            curl_slist_free_all(headers);
        }
    }

    // Test curl_mime_encoder
    if (selector & 0x10) {
        const char *encoders[] = {"base64", "8bit", "7bit", "binary", "quoted-printable", NULL};
        int encoder_idx = selector % 6;
        
        if (encoder_idx < 5) {
            curl_mime_encoder(part, encoders[encoder_idx]);
        } else {
            // Test with NULL to remove encoder
            curl_mime_encoder(part, NULL);
        }
    }

    // Test curl_slist_free_all with NULL (should be safe)
    if (selector & 0x20) {
        curl_slist_free_all(NULL);
    }

    // Cleanup
    if (str_param) {
        free(str_param);
    }
    
    // Free the mime structure (will clean up the part)
    curl_mime_free(mime);

    return 0;
}