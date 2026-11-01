// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url at urlapi.c:1288:8 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_dup at urlapi.c:1310:8 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/urlapi.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_url_operations(const uint8_t *data, size_t size) {
    CURLU *handle = curl_url();
    if (!handle) {
        return;
    }

    CURLU *dup_handle = NULL;
    char *part = NULL;
    CURLUcode res;

    // Try to set various URL parts based on input data
    if (size > 0) {
        // Use input data as potential URL parts
        res = curl_url_set(handle, CURLUPART_URL, (const char *)data, size);
        if (res != CURLUE_OK) {
            // Error is expected for malformed input, continue
        }

        // Try setting individual parts
        if (size > 10) {
            res = curl_url_set(handle, CURLUPART_SCHEME, (const char *)data, 10);
            res = curl_url_set(handle, CURLUPART_HOST, (const char *)(data + 10), size - 10);
        }

        if (size > 5) {
            res = curl_url_set(handle, CURLUPART_PORT, (const char *)data, 5);
            res = curl_url_set(handle, CURLUPART_PATH, (const char *)(data + 5), size - 5);
        }
    }

    // Try to duplicate the handle
    dup_handle = curl_url_dup(handle);
    if (dup_handle) {
        // Try to get parts from the duplicated handle
        res = curl_url_get(dup_handle, CURLUPART_SCHEME, &part, 0);
        if (res == CURLUE_OK && part) {
            curl_free(part);
            part = NULL;
        }

        res = curl_url_get(dup_handle, CURLUPART_HOST, &part, 0);
        if (res == CURLUE_OK && part) {
            curl_free(part);
            part = NULL;
        }

        res = curl_url_get(dup_handle, CURLUPART_PATH, &part, 0);
        if (res == CURLUE_OK && part) {
            curl_free(part);
            part = NULL;
        }

        // Try to set parts on the duplicated handle
        if (size > 0) {
            res = curl_url_set(dup_handle, CURLUPART_QUERY, (const char *)data, size);
            res = curl_url_set(dup_handle, CURLUPART_FRAGMENT, (const char *)data, size);
        }
    }

    // Try to get parts from the original handle
    res = curl_url_get(handle, CURLUPART_SCHEME, &part, 0);
    if (res == CURLUE_OK && part) {
        curl_free(part);
        part = NULL;
    }

    res = curl_url_get(handle, CURLUPART_USER, &part, 0);
    if (res == CURLUE_OK && part) {
        curl_free(part);
        part = NULL;
    }

    res = curl_url_get(handle, CURLUPART_PASSWORD, &part, 0);
    if (res == CURLUE_OK && part) {
        curl_free(part);
        part = NULL;
    }

    res = curl_url_get(handle, CURLUPART_HOST, &part, 0);
    if (res == CURLUE_OK && part) {
        curl_free(part);
        part = NULL;
    }

    res = curl_url_get(handle, CURLUPART_PORT, &part, 0);
    if (res == CURLUE_OK && part) {
        curl_free(part);
        part = NULL;
    }

    res = curl_url_get(handle, CURLUPART_PATH, &part, 0);
    if (res == CURLUE_OK && part) {
        curl_free(part);
        part = NULL;
    }

    res = curl_url_get(handle, CURLUPART_QUERY, &part, 0);
    if (res == CURLUE_OK && part) {
        curl_free(part);
        part = NULL;
    }

    res = curl_url_get(handle, CURLUPART_FRAGMENT, &part, 0);
    if (res == CURLUE_OK && part) {
        curl_free(part);
        part = NULL;
    }

    // Test error handling with invalid operations
    res = curl_url_get(NULL, CURLUPART_SCHEME, &part, 0);
    (void)res; // Expected to fail

    res = curl_url_set(NULL, CURLUPART_SCHEME, "http", 0);
    (void)res; // Expected to fail

    // Test error string conversion
    const char *err_str = curl_url_strerror(CURLUE_OK);
    (void)err_str;
    err_str = curl_url_strerror(CURLUE_BAD_HANDLE);
    (void)err_str;
    err_str = curl_url_strerror(CURLUE_MALFORMED_INPUT);
    (void)err_str;

    // Cleanup
    curl_url_cleanup(dup_handle);
    curl_url_cleanup(handle);
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    fuzz_url_operations(Data, Size);

    return 0;
}