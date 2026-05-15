// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_getdate at parsedate.c:561:8 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_getdate at parsedate.c:561:8 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
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

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    // Ensure we have at least 1 byte for null-termination if needed
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated string for functions that need it
    char *null_term_str = (char *)malloc(Size + 1);
    if (!null_term_str) {
        return 0;
    }
    memcpy(null_term_str, Data, Size);
    null_term_str[Size] = '\0';

    // Test curl_getdate
    time_t date_result = curl_getdate(null_term_str, NULL);
    (void)date_result; // Use result to avoid unused variable warning

    // Test curl_escape (deprecated)
    char *escaped = curl_escape(null_term_str, (int)Size);
    if (escaped) {
        curl_free(escaped);
    }

    // Test curl_unescape (deprecated)
    char *unescaped = curl_unescape(null_term_str, (int)Size);
    if (unescaped) {
        curl_free(unescaped);
    }

    // Test curl_getenv (deprecated)
    char *env_result = curl_getenv(null_term_str);
    if (env_result) {
        curl_free(env_result);
    }

    // Test curl_easy_escape - requires CURL handle
    CURL *curl = curl_easy_init();
    if (curl) {
        char *easy_escaped = curl_easy_escape(curl, null_term_str, (int)Size);
        if (easy_escaped) {
            curl_free(easy_escaped);
        }
        curl_easy_cleanup(curl);
    }

    // Test with file operations
    write_dummy_file(Data, Size);
    
    // Re-read file for additional testing
    FILE *fp = fopen("./dummy_file", "rb");
    if (fp) {
        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        if (file_size > 0) {
            char *file_data = (char *)malloc(file_size + 1);
            if (file_data) {
                fread(file_data, 1, file_size, fp);
                file_data[file_size] = '\0';
                
                // Test with file data
                time_t file_date = curl_getdate(file_data, NULL);
                (void)file_date;
                
                free(file_data);
            }
        }
        fclose(fp);
    }
    
    remove_dummy_file();

    // Test with different length variations
    if (Size > 1) {
        // Test with half length
        int half_len = (int)Size / 2;
        char *half_escaped = curl_escape(null_term_str, half_len);
        if (half_escaped) {
            curl_free(half_escaped);
        }

        // Test with zero length (auto-detect)
        char *auto_escaped = curl_escape(null_term_str, 0);
        if (auto_escaped) {
            curl_free(auto_escaped);
        }
    }

    // Test curl_free with NULL (should be safe)
    curl_free(NULL);

    free(null_term_str);
    return 0;
}