// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_url at urlapi.c:1288:8 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/urlapi.h>
#include <curl/typecheck-gcc.h>

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    /* Initialize libcurl */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    CURLU *handle = NULL;
    CURLUcode res;
    char *url_str = NULL;
    unsigned int flags = 0;
    CURLUPart part = CURLUPART_SCHEME;

    /* Use first byte to decide operations */
    uint8_t op = Data[0];
    uint8_t part_idx = Data[1] % 10; /* 0-9 for different parts */
    
    /* Map part_idx to CURLUPart */
    switch (part_idx) {
        case 0: part = CURLUPART_SCHEME; break;
        case 1: part = CURLUPART_USER; break;
        case 2: part = CURLUPART_PASSWORD; break;
        case 3: part = CURLUPART_OPTIONS; break;
        case 4: part = CURLUPART_HOST; break;
        case 5: part = CURLUPART_ZONEID; break;
        case 6: part = CURLUPART_PORT; break;
        case 7: part = CURLUPART_PATH; break;
        case 8: part = CURLUPART_QUERY; break;
        case 9: part = CURLUPART_FRAGMENT; break;
        default: part = CURLUPART_SCHEME; break;
    }

    /* Use remaining data as potential URL string or part string */
    const char *input_data = (const char *)(Data + 2);
    size_t input_len = Size - 2;

    /* Create a copy to ensure null termination for string operations */
    if (input_len > 0) {
        url_str = malloc(input_len + 1);
        if (!url_str) return 0;
        memcpy(url_str, input_data, input_len);
        url_str[input_len] = '\0';
    } else {
        url_str = strdup("");
        if (!url_str) return 0;
    }

    /* Test curl_version */
    (void)curl_version();

    /* Test curl_getenv with a dummy variable name derived from input */
    if (input_len > 0) {
        char env_var[64];
        size_t env_len = (input_len < 63) ? input_len : 63;
        memcpy(env_var, input_data, env_len);
        env_var[env_len] = '\0';
        char *env_val = curl_getenv(env_var);
        if (env_val) {
            curl_free(env_val);
        }
    }

    /* Create CURLU handle */
    handle = curl_url();

    if (handle) {
        /* Set the URL string into the handle */
        res = curl_url_set(handle, CURLUPART_URL, url_str, 0);
        
        if (res == CURLUE_OK) {
            /* Test curl_url_set with various parts */
            if (input_len > 0) {
                /* Determine flags based on input byte 0 */
                flags = (op & 0x03); /* 0, 1, 2, 3 */
                res = curl_url_set(handle, part, input_data, flags);
            } else {
                /* Try clearing a part */
                res = curl_url_set(handle, part, NULL, 0);
            }

            /* Test curl_url_get */
            char *retrieved = NULL;
            res = curl_url_get(handle, part, &retrieved, flags);
            if (retrieved) {
                curl_free(retrieved);
            }

            /* Test curl_url_strerror with various error codes */
            (void)curl_url_strerror(CURLUE_OK);
            (void)curl_url_strerror(CURLUE_BAD_HANDLE);
            (void)curl_url_strerror(CURLUE_MALFORMED_INPUT);
            (void)curl_url_strerror(CURLUE_OUT_OF_MEMORY);
            (void)curl_url_strerror(CURLUE_NO_SCHEME);
            (void)curl_url_strerror(CURLUE_NO_HOST);
            (void)curl_url_strerror(CURLUE_NO_PORT);
            (void)curl_url_strerror(CURLUE_NO_USER);
            (void)curl_url_strerror(CURLUE_NO_PASSWORD);
            (void)curl_url_strerror(CURLUE_NO_OPTIONS);
            (void)curl_url_strerror(CURLUE_NO_QUERY);
            (void)curl_url_strerror(CURLUE_NO_FRAGMENT);
            (void)curl_url_strerror(CURLUE_NO_ZONEID);
            (void)curl_url_strerror(CURLUE_TOO_LARGE);

            /* Cleanup handle */
            curl_url_cleanup(handle);
        } else {
            /* If setting URL failed, still cleanup */
            curl_url_cleanup(handle);
        }
    }

    if (url_str) {
        free(url_str);
    }

    curl_global_cleanup();

    return 0;
}