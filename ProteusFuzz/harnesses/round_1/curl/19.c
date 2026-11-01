// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_url at urlapi.c:1288:8 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_dup at urlapi.c:1310:8 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/urlapi.h>

/*
 * Fuzz driver for curl URL API functions.
 * Entry point: LLVMFuzzerTestOneInput
 */
int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure the input data is null-terminated for string operations */
    /* Allocate a buffer with an extra byte for the null terminator */
    char *safe_input = (char *)malloc(Size + 1);
    if (!safe_input) {
        return 0;
    }
    memcpy(safe_input, Data, Size);
    safe_input[Size] = '\0';

    /* Initialize a CURLU handle */
    CURLU *handle = curl_url();
    if (!handle) {
        free(safe_input);
        return 0;
    }

    /* Use input data to set various URL components */
    /* Set scheme */
    curl_url_set(handle, CURLUPART_SCHEME, safe_input, 0);
    /* Set host */
    curl_url_set(handle, CURLUPART_HOST, safe_input, 0);
    /* Set port */
    curl_url_set(handle, CURLUPART_PORT, safe_input, 0);
    /* Set path */
    curl_url_set(handle, CURLUPART_PATH, safe_input, 0);
    /* Set query */
    curl_url_set(handle, CURLUPART_QUERY, safe_input, 0);
    /* Set fragment */
    curl_url_set(handle, CURLUPART_FRAGMENT, safe_input, 0);
    /* Set user */
    curl_url_set(handle, CURLUPART_USER, safe_input, 0);
    /* Set password */
    curl_url_set(handle, CURLUPART_PASSWORD, safe_input, 0);

    /* Try setting with URL encoding flag */
    curl_url_set(handle, CURLUPART_PATH, safe_input, CURLU_URLENCODE);
    curl_url_set(handle, CURLUPART_QUERY, safe_input, CURLU_URLENCODE);

    /* Clear some parts by setting NULL */
    curl_url_set(handle, CURLUPART_USER, NULL, 0);
    curl_url_set(handle, CURLUPART_PASSWORD, NULL, 0);

    /* Duplicate the handle */
    CURLU *dup_handle = curl_url_dup(handle);
    if (dup_handle) {
        /* Try setting on duplicated handle */
        curl_url_set(dup_handle, CURLUPART_SCHEME, safe_input, 0);
        curl_url_set(dup_handle, CURLUPART_HOST, safe_input, 0);

        /* Get components from duplicated handle */
        char *part = NULL;
        curl_url_get(dup_handle, CURLUPART_SCHEME, &part, 0);
        if (part) {
            curl_free(part);
        }
        curl_url_get(dup_handle, CURLUPART_HOST, &part, 0);
        if (part) {
            curl_free(part);
        }
        curl_url_get(dup_handle, CURLUPART_PATH, &part, 0);
        if (part) {
            curl_free(part);
        }
        curl_url_get(dup_handle, CURLUPART_QUERY, &part, 0);
        if (part) {
            curl_free(part);
        }
        curl_url_get(dup_handle, CURLUPART_FRAGMENT, &part, 0);
        if (part) {
            curl_free(part);
        }
        curl_url_get(dup_handle, CURLUPART_USER, &part, 0);
        if (part) {
            curl_free(part);
        }
        curl_url_get(dup_handle, CURLUPART_PASSWORD, &part, 0);
        if (part) {
            curl_free(part);
        }

        /* Cleanup duplicated handle */
        curl_url_cleanup(dup_handle);
    }

    /* Get components from original handle */
    char *part = NULL;
    curl_url_get(handle, CURLUPART_SCHEME, &part, 0);
    if (part) {
        curl_free(part);
    }
    curl_url_get(handle, CURLUPART_HOST, &part, 0);
    if (part) {
        curl_free(part);
    }
    curl_url_get(handle, CURLUPART_PATH, &part, 0);
    if (part) {
        curl_free(part);
    }
    curl_url_get(handle, CURLUPART_QUERY, &part, 0);
    if (part) {
        curl_free(part);
    }
    curl_url_get(handle, CURLUPART_FRAGMENT, &part, 0);
    if (part) {
        curl_free(part);
    }
    curl_url_get(handle, CURLUPART_USER, &part, 0);
    if (part) {
        curl_free(part);
    }
    curl_url_get(handle, CURLUPART_PASSWORD, &part, 0);
    if (part) {
        curl_free(part);
    }

    /* Test error handling by calling with NULL handle (should be safe) */
    curl_url_cleanup(NULL);

    /* Test error string conversion */
    const char *err_str = curl_url_strerror(CURLUE_OK);
    (void)err_str; /* Suppress unused warning */
    err_str = curl_url_strerror(CURLUE_BAD_HANDLE);
    (void)err_str;
    err_str = curl_url_strerror(CURLUE_MALFORMED_INPUT);
    (void)err_str;

    /* Cleanup original handle */
    curl_url_cleanup(handle);

    free(safe_input);

    return 0;
}