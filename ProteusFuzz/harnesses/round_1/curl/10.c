// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_url at urlapi.c:1288:8 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_dup at urlapi.c:1310:8 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/urlapi.h>
#include <curl/typecheck-gcc.h>

/*
 * Fuzz driver for curl URL API functions:
 * - curl_url
 * - curl_url_cleanup
 * - curl_url_get
 * - curl_url_dup
 */

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Write input to dummy file for potential side effects if needed */
    write_dummy_file(Data, Size);

    /* Create a new CURLU handle */
    CURLU *handle = curl_url();
    if (!handle) {
        return 0;
    }

    /* Interpret input data as a potential URL string (null-terminated) */
    char url_buf[4096];
    size_t copy_len = (Size < sizeof(url_buf) - 1) ? Size : sizeof(url_buf) - 1;
    memcpy(url_buf, Data, copy_len);
    url_buf[copy_len] = '\0';

    /* Set the URL string into the handle */
    curl_url_set(handle, CURLUPART_URL, url_buf, CURLU_DEFAULT_SCHEME);

    /* Fuzz curl_url_get with various parts */
    char *part = NULL;
    CURLUPart parts[] = {
        CURLUPART_SCHEME,
        CURLUPART_USER,
        CURLUPART_PASSWORD,
        CURLUPART_HOST,
        CURLUPART_PORT,
        CURLUPART_PATH,
        CURLUPART_QUERY,
        CURLUPART_FRAGMENT
    };
    unsigned int num_parts = sizeof(parts) / sizeof(parts[0]);

    /* Valid flags for curl_url_get. 
       Using only standard flags known to exist in urlapi.h to avoid compilation errors.
    */
    unsigned int flags[] = { 0, CURLU_DEFAULT_SCHEME, CURLU_GUESS_SCHEME, CURLU_URLENCODE };
    unsigned int num_flags = sizeof(flags) / sizeof(flags[0]);

    for (unsigned int i = 0; i < num_parts; i++) {
        for (unsigned int j = 0; j < num_flags; j++) {
            if (curl_url_get(handle, parts[i], &part, flags[j]) == CURLUE_OK && part) {
                /* Use the string to ensure it's valid */
                (void)strlen(part);
                curl_free(part);
                part = NULL;
            }
        }
    }

    /* Fuzz curl_url_dup */
    CURLU *dup_handle = curl_url_dup(handle);
    if (dup_handle) {
        /* Try to get a part from the duplicated handle */
        if (curl_url_get(dup_handle, CURLUPART_HOST, &part, 0) == CURLUE_OK && part) {
            (void)strlen(part);
            curl_free(part);
            part = NULL;
        }
        curl_url_cleanup(dup_handle);
    }

    /* Cleanup original handle */
    curl_url_cleanup(handle);

    return 0;
}