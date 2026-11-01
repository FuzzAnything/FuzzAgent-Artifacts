// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_url at urlapi.c:1288:8 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/urlapi.h>
#include <curl/multi.h>

static void cleanup_url(CURLU *handle) {
    if (handle) {
        curl_url_cleanup(handle);
    }
}

static void cleanup_multi(CURLM *multi) {
    if (multi) {
        curl_multi_cleanup(multi);
    }
}

static void cleanup_handles(CURL **handles) {
    if (handles) {
        curl_free(handles);
    }
}

static void cleanup_string(char *str) {
    if (str) {
        curl_free(str);
    }
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize curl global
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Test curl_version (no args, static string)
    const char *version = curl_version();
    (void)version; // Suppress unused warning

    // Test curl_getenv
    char *env_var = curl_getenv("PATH");
    cleanup_string(env_var);

    // Test curl_url functions
    CURLU *url_handle = curl_url();
    if (url_handle) {
        // Create a safe, null-terminated URL string from input
        char *safe_url = NULL;
        if (Size > 0) {
            safe_url = malloc(Size + 1);
            if (safe_url) {
                // Copy data and ensure null termination
                memcpy(safe_url, Data, Size);
                safe_url[Size] = '\0';
                
                // Replace any embedded nulls with a safe character to prevent premature termination
                // while keeping the string valid for strlen
                for (size_t i = 0; i < Size; i++) {
                    if (safe_url[i] == 0) {
                        safe_url[i] = 'a';
                    }
                }
                
                // Set the full URL string
                curl_url_set(url_handle, CURLUPART_URL, safe_url, 0);
                free(safe_url);
            }
        }

        // Fuzz curl_url_set with various parts and flags
        // We use a separate buffer for parts to ensure null-termination
        char part_buffer[256];
        size_t part_len = (Size < sizeof(part_buffer) - 1) ? Size : sizeof(part_buffer) - 1;
        memcpy(part_buffer, Data, part_len);
        part_buffer[part_len] = '\0';
        
        // Ensure no embedded nulls in the part buffer
        for (size_t i = 0; i < part_len; i++) {
            if (part_buffer[i] == 0) {
                part_buffer[i] = 'a';
            }
        }

        CURLUPart parts[] = {
            CURLUPART_SCHEME, CURLUPART_USER, CURLUPART_PASSWORD,
            CURLUPART_HOST, CURLUPART_PORT, CURLUPART_PATH,
            CURLUPART_QUERY, CURLUPART_FRAGMENT
        };
        int num_parts = sizeof(parts) / sizeof(parts[0]);

        for (int i = 0; i < num_parts; i++) {
            // Set a part with data (using the safe buffer)
            unsigned int flags = (Size > i) ? Data[i] & 0x3 : 0; 
            curl_url_set(url_handle, parts[i], part_buffer, flags);
            
            // Set a part to NULL to clear it
            curl_url_set(url_handle, parts[i], NULL, flags);
        }

        // Fuzz curl_url_get
        char *part_str = NULL;
        for (int i = 0; i < num_parts; i++) {
            unsigned int flags = (Size > i + num_parts) ? Data[i + num_parts] & 0x3 : 0;
            curl_url_get(url_handle, parts[i], &part_str, flags);
            cleanup_string(part_str);
        }

        // Test curl_url_strerror with various codes
        for (int i = 0; i < 20; i++) {
            CURLUcode code = (CURLUcode)(i);
            const char *err_str = curl_url_strerror(code);
            (void)err_str; // Suppress unused warning
        }

        cleanup_url(url_handle);
    }

    // Test curl_multi functions
    CURLM *multi_handle = curl_multi_init();
    if (multi_handle) {
        // Add a dummy easy handle to multi
        CURL *easy_handle = curl_easy_init();
        if (easy_handle) {
            curl_multi_add_handle(multi_handle, easy_handle);
        }

        // Get handles array
        CURL **handles = curl_multi_get_handles(multi_handle);
        if (handles) {
            // Just iterate to ensure it's valid, but don't dereference blindly
            int count = 0;
            while (handles[count] != NULL) {
                count++;
                if (count > 1000) break; // Safety limit
            }
            cleanup_handles(handles);
        }

        if (easy_handle) {
            curl_easy_cleanup(easy_handle);
        }

        cleanup_multi(multi_handle);
    }

    // Cleanup global
    curl_global_cleanup();

    return 0;
}