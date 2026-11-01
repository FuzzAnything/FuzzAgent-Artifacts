// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_subparts at mime.c:1489:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_mime(curl_mime *mime) {
    if (mime) {
        curl_mime_free(mime);
    }
}

static void cleanup_handle(CURL *handle) {
    if (handle) {
        curl_easy_cleanup(handle);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    curl_mime *mime = curl_mime_init(curl);
    if (!mime) {
        cleanup_handle(curl);
        return 0;
    }

    // Ensure null termination for string operations
    char *safe_data = (char *)malloc(Size + 1);
    if (!safe_data) {
        cleanup_mime(mime);
        cleanup_handle(curl);
        return 0;
    }
    memcpy(safe_data, Data, Size);
    safe_data[Size] = '\0';

    // Create a primary part
    curl_mimepart *part1 = curl_mime_addpart(mime);
    if (part1) {
        // Fuzz name
        curl_mime_name(part1, safe_data);

        // Fuzz encoder with various strings from input
        if (Size > 0) {
            // Try setting encoder with data from input (ensure null termination)
            char *enc_str = (char *)malloc(Size + 1);
            if (enc_str) {
                memcpy(enc_str, safe_data, Size);
                enc_str[Size] = '\0';
                curl_mime_encoder(part1, enc_str);
                free(enc_str);
            }
        }
        
        // Try setting encoder to NULL
        curl_mime_encoder(part1, NULL);
    }

    // Create a sub-mime context for subparts fuzzing
    curl_mime *sub_mime = curl_mime_init(curl);
    if (sub_mime) {
        curl_mimepart *sub_part = curl_mime_addpart(sub_mime);
        if (sub_part) {
            curl_mime_name(sub_part, safe_data);
        }

        // Add another part to main mime to attach subparts to
        curl_mimepart *part2 = curl_mime_addpart(mime);
        if (part2) {
            // Attach subparts
            curl_mime_subparts(part2, sub_mime);
            
            // Try setting name on part2
            curl_mime_name(part2, safe_data);
            
            // Try setting encoder on part2
            if (Size > 0) {
                char *enc_str = (char *)malloc(Size + 1);
                if (enc_str) {
                    memcpy(enc_str, safe_data, Size);
                    enc_str[Size] = '\0';
                    curl_mime_encoder(part2, enc_str);
                    free(enc_str);
                }
            }
        }
        
        cleanup_mime(sub_mime);
    }

    free(safe_data);
    cleanup_mime(mime);
    cleanup_handle(curl);

    return 0;
}