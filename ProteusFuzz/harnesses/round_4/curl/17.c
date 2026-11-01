// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_url at urlapi.c:1288:8 in urlapi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/urlapi.h>
#include <curl/typecheck-gcc.h>

/*
 * Fuzz driver for curl URL API functions.
 * Targets: curl_url_get, curl_url_set, curl_url_strerror.
 * Note: Wcurl_* macros are compile-time warnings and cannot be invoked at runtime.
 */

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl global environment */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Create a CURLU handle */
    CURLU *handle = curl_url();
    if (!handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Use the fuzz input as a URL string, ensuring it is null-terminated */
    char *url_str = NULL;
    if (Size > 0) {
        url_str = malloc(Size + 1);
        if (url_str) {
            memcpy(url_str, Data, Size);
            url_str[Size] = '\0';
        }
    }

    if (url_str) {
        /* Set the URL in the handle */
        curl_url_set(handle, CURLUPART_URL, url_str, 0);
        free(url_str);
    } else {
        /* If malloc failed, try with an empty string or skip */
        curl_url_set(handle, CURLUPART_URL, "", 0);
    }

    /* Fuzz curl_url_set with various parts and flags */
    CURLUPart parts[] = {
        CURLUPART_SCHEME, CURLUPART_USER, CURLUPART_PASSWORD,
        CURLUPART_HOST, CURLUPART_PORT, CURLUPART_PATH,
        CURLUPART_QUERY, CURLUPART_FRAGMENT, CURLUPART_ZONEID
    };
    int num_parts = sizeof(parts) / sizeof(parts[0]);

    /* Valid flags for curl_url_set */
    unsigned int flags[] = {
        0,
        CURLU_URLENCODE,
        CURLU_NON_SUPPORT_SCHEME,
        CURLU_DEFAULT_SCHEME,
        CURLU_NO_GUESS_SCHEME
    };
    int num_flags = sizeof(flags) / sizeof(flags[0]);

    /* Create a buffer for setting parts */
    char *set_buffer = NULL;
    size_t set_size = 0;

    if (Size > 0) {
        set_buffer = malloc(Size + 1);
        if (set_buffer) {
            memcpy(set_buffer, Data, Size);
            set_buffer[Size] = '\0';
            set_size = Size;
        }
    }

    for (int i = 0; i < num_parts; i++) {
        for (int j = 0; j < num_flags; j++) {
            /* Try setting with the buffer */
            if (set_buffer) {
                curl_url_set(handle, parts[i], set_buffer, flags[j]);
            }
            /* Try setting with NULL to clear */
            curl_url_set(handle, parts[i], NULL, flags[j]);
        }
    }

    /* Fuzz curl_url_get with various parts and flags */
    char *part_result = NULL;
    for (int i = 0; i < num_parts; i++) {
        for (int j = 0; j < num_flags; j++) {
            curl_url_get(handle, parts[i], &part_result, flags[j]);
            if (part_result) {
                /* Free the returned string as required by the API */
                curl_free(part_result);
                part_result = NULL;
            }
        }
    }

    /* Fuzz curl_url_strerror with various error codes */
    /* We iterate through a range of possible error codes */
    for (int i = 0; i < 100; i++) {
        CURLUcode code = (CURLUcode)i;
        const char *str = curl_url_strerror(code);
        (void)str; /* Use the result to avoid unused variable warnings */
    }

    /* Cleanup */
    if (set_buffer) {
        free(set_buffer);
    }

    curl_url_cleanup(handle);
    curl_global_cleanup();

    return 0;
}